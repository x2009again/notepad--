#include "mdrenderer.h"

#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QUrl>
#include <QHash>

namespace MdRenderer {
namespace {

// ---------- 基础工具 ----------

QString escapeHtml(const QString& s)
{
	QString out;
	out.reserve(s.size() + 16);
	for (int i = 0; i < s.size(); ++i) {
		const QChar c = s.at(i);
		switch (c.unicode()) {
		case u'&':  out += QLatin1String("&amp;");  break;
		case u'<':  out += QLatin1String("&lt;");   break;
		case u'>':  out += QLatin1String("&gt;");   break;
		case u'"':  out += QLatin1String("&quot;"); break;
		case u'\'': out += QLatin1String("&#39;");  break;
		default:    out += c;                       break;
		}
	}
	return out;
}

// 去掉行尾空白（保留行首缩进，缩进对代码块/列表有意义）
QString rtrim(const QString& s)
{
	int end = s.size();
	while (end > 0) {
		const QChar c = s.at(end - 1);
		if (c == u' ' || c == u'\t' || c == u'\r')
			--end;
		else
			break;
	}
	return s.left(end);
}

// 去掉行首空白，返回缩进宽度（tab按 4 空格计）
int indentWidth(const QString& line)
{
	int w = 0;
	for (int i = 0; i < line.size(); ++i) {
		const QChar c = line.at(i);
		if (c == u' ')
			++w;
		else if (c == u'\t')
			w += 4;
		else
			break;
	}
	return w;
}

QString stripIndent(const QString& line, int width)
{
	int i = 0, w = 0;
	while (i < line.size() && w < width) {
		const QChar c = line.at(i);
		if (c == u' ') { ++w; ++i; }
		else if (c == u'\t') { w += 4; ++i; }
		else break;
	}
	return line.mid(i);
}

bool isBlank(const QString& s)
{
	for (int i = 0; i < s.size(); ++i) {
		if (!s.at(i).isSpace())
			return false;
	}
	return true;
}

// 横向分割线判定：--- *** ___ （至少3 个，同种字符，允许空格）
bool isHorizontalRule(const QString& line)
{
	const QString t = rtrim(line);
	const QChar c = t.isEmpty() ? QChar() : t.at(0);
	if (c != u'-' && c != u'*' && c != u'_')
		return false;
	int count = 0;
	for (int i = 0; i < t.size(); ++i) {
		const QChar ch = t.at(i);
		if (ch == c)
			++count;
		else if (ch != u' ' && ch != u'\t')
			return false;
	}
	return count >= 3;
}

// ATX 标题：返回级别（0 表示不是标题）
int atxHeadingLevel(const QString& line)
{
	int i = 0;
	while (i < line.size() && line.at(i) == u'#')
		++i;
	if (i < 1 || i > 6)
		return 0;
	if (i < line.size()) {
		const QChar c = line.at(i);
		if (c != u' ' && c != u'\t')
			return 0;
	}
	// 结尾允许可选的闭合 ###
	QString rest = rtrim(line.mid(i));
	if (rest.endsWith(u'#')) {
		int n = 0;
		while (n < rest.size() && rest.at(rest.size() - 1 - n) == u'#')
			++n;
		if (n > 0 && (rest.size() == n || rest.at(rest.size() - n - 1).isSpace()))
			rest = rest.left(rest.size() - n);
	}
	return i;
}

QString headingText(const QString& line, int level)
{
	int i = 0;
	while (i < line.size() && line.at(i) == u'#')
		++i;
	QString rest = line.mid(i);
	if (!rest.isEmpty() && (rest.at(0) == u' ' || rest.at(0) == u'\t'))
		rest = rest.mid(1);
	rest = rtrim(rest);
	if (rest.endsWith(u'#')) {
		int n = 0;
		while (n < rest.size() && rest.at(rest.size() - 1 - n) == u'#')
			++n;
		if (n > 0 && (rest.size() == n || rest.at(rest.size() - n - 1).isSpace()))
			rest = rest.left(rest.size() - n);
	}
	(void)level;
	return rest;
}

// Setext 标题下划线
bool setextHeadingLevel(const QString& line, int* level)
{
	const QString t = rtrim(line);
	if (t.size() < 1)
		return false;
	const QChar c = t.at(0);
	if (c != u'=' && c != u'-')
		return false;
	for (int i = 0; i < t.size(); ++i)
		if (t.at(i) != c)
			return false;
	if (c == u'=') { *level = 1; return true; }
	// '-' 需至少一个；为避免与列表/分割线混淆，要求不构成 HR 且前一行非空
	*level = 2;
	return true;
}

// 围栏代码块起始行：返回长度（0 不是围栏）
int fenceLength(const QString& line)
{
	const QString t = line;
	int i = 0;
	if (i < t.size() && (t.at(i) == u' ' || t.at(i) == u'\t')) {
		while (i < t.size() && (t.at(i) == u' ' || t.at(i) == u'\t'))
			++i;
	}
	QChar c;
	if (i < t.size() && (t.at(i) == u'`' || t.at(i) == u'~'))
		c = t.at(i);
	else
		return 0;
	int n = 0;
	while (i + n < t.size() && t.at(i + n) == c)
		++n;
	if (n < 3)
		return 0;
	return n;
}

// 列表项：返回类型与内容
enum class ListKind { None, Bullet, Ordered };

struct ListItemInfo {
	ListKind kind = ListKind::None;
	QString content;   // 去掉标记后的内容（已按marker宽度 dedent）
	bool task = false;
	bool taskChecked = false;
	int taskStart = 0;
	int taskLen = 0;
};

ListItemInfo parseListItem(const QString& line)
{
	ListItemInfo r;
	const int i = indentWidth(line);
	if (i >= 4)             // 4 空格以上视为缩进代码，不算列表
		return r;
	const QString rest = stripIndent(line, i);
	if (rest.isEmpty())
		return r;

	// 无序：- * + 后必须跟空格
	if (rest.size() >= 2
		&& (rest.at(0) == u'-' || rest.at(0) == u'*' || rest.at(0) == u'+')
		&& (rest.at(1) == u' ' || rest.at(1) == u'\t')) {
		r.kind = ListKind::Bullet;
		r.content = rest.mid(2);
	}
	else {
		// 有序：数字 + . 或 )
		int n = 0;
		while (n < rest.size() && rest.at(n).isDigit() && n < 9)
			++n;
		if (n >= 1 && n < rest.size()
			&& (rest.at(n) == u'.' || rest.at(n) == u')')
			&& (n + 1 < rest.size())
			&& (rest.at(n + 1) == u' ' || rest.at(n + 1) == u'\t')) {
			r.kind = ListKind::Ordered;
			r.content = rest.mid(n + 2);
		}
		else {
			return r;       // 不是列表项
		}
	}

	// 任务列表：内容形如 [ ] / [x] / [X]
	//
	// 这个判断必须放在「确定 kind + 剥掉 marker」之后统一处理。
	// 早期版本把它放在 Bullet 分支的 return 之后，导致任务项
	// 根本走不到这里，checkbox 永远不生成（离线测试已验证）。
	static const QRegularExpression taskRe(QStringLiteral("^\\[([ xX])\\]\\s"));
	const QRegularExpressionMatch m = taskRe.match(r.content);
	if (m.hasMatch()) {
		r.task = true;
		r.taskChecked = (m.captured(1) != QLatin1String(" "));
		r.taskStart = m.capturedStart(1);
		r.taskLen = m.capturedLength(1);
		r.content = r.content.mid(m.capturedLength());
	}
	return r;
}

// ---------- 表格 ----------

QStringList splitTableRow(const QString& line, QString* alignMarkerRow = nullptr)
{
	QString s = rtrim(line);
	if (s.startsWith(u'|'))  s = s.mid(1);
	if (s.endsWith(u'|'))    s.chop(1);

	QStringList cells;
	QString cur;
	for (int i = 0; i < s.size(); ++i) {
		const QChar c = s.at(i);
		if (c == u'\\' && i + 1 < s.size()) {   // 转义竖线
			cur += s.at(i);
			cur += s.at(i + 1);
			++i;
			continue;
		}
		if (c == u'|') {
			cells << rtrim(cur);
			cur.clear();
			continue;
		}
		cur += c;
	}
	cells << rtrim(cur);

	if (alignMarkerRow) {
		*alignMarkerRow = s;
		// 去掉转义反斜杠，还原原始标记
		QString t;
		for (int i = 0; i < s.size(); ++i) {
			if (s.at(i) == u'\\' && i + 1 < s.size()) {
				t += s.at(i + 1); ++i;
			} else {
				t += s.at(i);
			}
		}
		*alignMarkerRow = t;
	}
	return cells;
}

// 分隔行判定：每个 cell 形如 :?-{1,}:?
bool isTableDelimiterRow(const QString& line)
{
	QString raw;
	const QStringList cells = splitTableRow(line, &raw);
	if (cells.isEmpty())
		return false;
	if (cells.size() < 1)
		return false;
	for (const QString& c : cells) {
		QString t;
		for (int i = 0; i < c.size(); ++i) {
			const QChar ch = c.at(i);
			if (ch == u'\\' && i + 1 < c.size()) { t += c.at(i + 1); ++i; continue; }
			t += ch;
		}
		t = t.trimmed();
		if (t.isEmpty())
			return false;
		int lead = 0, trail = 0, mid = 0;
		const int n = t.size();
		int i = 0;
		if (i < n && t.at(i) == u':') { ++lead; ++i; }
		while (i < n && t.at(i) == u'-') { ++mid; ++i; }
		if (i < n && t.at(i) == u':') { ++trail; ++i; }
		if (mid < 1 || i != n)
			return false;
		(void)lead; (void)trail;
	}
	return true;
}

// 返回该列的对齐关键字："left" / "center" / "right"，无法判断时返回空串。
// 注意：返回 QString 而非 Qt::Alignment，因为 Qt::Alignment 是强类型枚举，
// 不能直接和 QString 比较（MSVC 会报 C2666/ C2446）。
QString alignFromMarker(const QString& cell)
{
	QString t;
	for (int i = 0; i < cell.size(); ++i) {
		const QChar c = cell.at(i);
		if (c == u'\\' && i + 1 < cell.size()) { t += cell.at(i + 1); ++i; continue; }
		t += c;
	}
	t = t.trimmed();
	const bool l = t.startsWith(u':');
	const bool r = t.endsWith(u':');
	if (l && r)  return QStringLiteral("center");
	if (r)       return QStringLiteral("right");
	if (l)       return QStringLiteral("left");
	return QString();
}

// ---------- 行内渲染 ----------

bool isSafeUrl(const QString& url)
{
	const QString u = url.trimmed().toLower();
	if (u.startsWith(u"javascript:") || u.startsWith(u"vbscript:"))
		return false;
	return true;
}

QString normalizeHref(const QString& url)
{
	QString u = url.trimmed();
	// 去掉包裹的尖括号
	if (u.startsWith(u'<') && u.endsWith(u'>'))
		u = u.mid(1, u.size() - 2).trimmed();
	return u;
}

// 去掉末尾的 \ 转义并还原实体
QString applyEscapes(const QString& s)
{
	QString out;
	out.reserve(s.size());
	for (int i = 0; i < s.size(); ++i) {
		if (s.at(i) == u'\\' && i + 1 < s.size()) {
			const QChar n = s.at(i + 1);
			if (QStringLiteral("\\`*_{}[]()#+-.!<>|~\"'$%&/:;=@^").contains(n)) {
				out += n;
				++i;
				continue;
			}
		}
		out += s.at(i);
	}
	return out;
}

// 粗体 **  或 __
QString renderBold(const QString& s, const QString& marker)
{
	QString needle = marker + marker;
	QString out;
	int i = 0;
	while (i < s.size()) {
		int p = s.indexOf(needle, i);
		if (p < 0) { out += s.mid(i); break; }
		int end = s.indexOf(needle, p + needle.size());
		if (end < 0) { out += s.mid(i); break; }
		out += s.mid(i, p - i);
		const QString inner = s.mid(p + needle.size(), end - p - needle.size());
		out += QLatin1String("<strong>") + renderBold(inner, marker) + QLatin1String("</strong>");
		i = end + needle.size();
	}
	return out;
}

// 斜体 * 或 _
//
// 边界规则（2026-10-08 重写）：
//   旧实现要求标记两侧都是空白，中文场景下几乎全废——
//   例如「**粗体**、*斜体*」里 * 的前一字符是逗号而非空格，
//   斜体不生效（离线测试 1 项 FAIL 验证过）。
//   新规则：标记不能紧贴「单词字符」（字母数字下划线），
//   但可以紧贴标点与空白。中文标点因此能被正确识别为边界。
// 判断一个字符是否「属于单词内部」，用于斜体/粗体的定界判断。
//
// 注意：不能用 QChar::isLetterOrNumber()。
// Qt 会把 CJK 汉字判成 isLetter()为 true —— 这是对的，
// 但它对全角标点（如顿号、逗号）返回 false，也是对的；
// 真正的问题是很多构建下 Qt 把部分 CJK 标点归入
// 其它类别（Symbol / Punctuation），而 isLetterOrNumber
// 只看 Number 与 Letter 两个类别，对 Symbol 不生效，
// 于是「、*斜体*」这种中文里极常见的写法被判成
// 标记紧贴单词，斜体不渲染（离线测试实测复现）。
//
// 稳妥做法：只把「明确的单词字符」当作不能紧贴的东西，
// 其余（空白、标点、符号、汉字）一律视为合法边界。
static bool isWordChar(QChar c)
{
	if (c.isSpace())
		return false;
	// ASCII 单词字符：字母、数字、下划线
	if (c.unicode() < 128)
		return (c >= u'a' && c <= u'z')
		    || (c >= u'A' && c <= u'Z')
		    || (c >= u'0' && c <= u'9')
		    || c == u'_';
	// 非 ASCII：汉字等文字确实算单词内部，
	// 但 CJK 标点符号（U+3000 区）不算。
	const ushort u = c.unicode();
	if (u >= 0x3000 && u <= 0x303F)   // CJK 符号与标点
		return false;
	if (u >= 0xFF01 && u <= 0xFF20)   // 全角标点
		return false;
	if (u >= 0xFF3B && u <= 0xFF40)
		return false;
	if (u >= 0xFF5B && u <= 0xFF65)
		return false;
	return c.isLetterOrNumber();
}

QString renderItalic(const QString& s, const QString& marker)
{
	const QChar m = marker.at(0);
	QString out;
	int i = 0;
	while (i < s.size()) {
		// 找一个可作为左定界符的标记：
		// 前面不能紧贴单词字符，且必须还有内容
		int p = -1;
		for (int j = i; j < s.size(); ++j) {
			if (s.at(j) != m) continue;
			if (j + 1 >= s.size()) continue;
			if (j > 0 && isWordChar(s.at(j - 1))) continue;
			// 跳过成对标记（如 ** / __），交给 renderBold 处理
			if (j + 1 < s.size() && s.at(j + 1) == m) continue;
			p = j; break;
		}
		if (p < 0) { out += s.mid(i); break; }

		// 找一个可作为右定界符的标记。
		// 判据：标记之后不能再紧贴单词字符
		//（因为标记之后的内容属于「标记之外」）。
		// 注意不要检查标记之前那个字符——那是标记内部的正文，
		// 「*斜体*」的右标记前面正是「体」，是合法内容。
		// （早期版本误把这里写成检查 j-1，导致中文斜体永远
		//   不渲染，因为汉字被 isWordChar 判为单词字符。）
		int e = -1;
		for (int j = p + 1; j < s.size(); ++j) {
			if (s.at(j) != m) continue;
			if (j + 1 < s.size() && isWordChar(s.at(j + 1))) continue;
			if (j + 1 < s.size() && s.at(j + 1) == m) continue;
			e = j; break;
		}
		if (e < 0) { out += s.mid(i); break; }

		out += s.mid(i, p - i);
		const QString inner = s.mid(p + 1, e - p - 1);
		if (!inner.isEmpty()) {
			out += QLatin1String("<em>") + renderItalic(inner, marker) + QLatin1String("</em>");
		} else {
			out += QChar(m);      // 空内容，原样输出
		}
		i = e + 1;
	}
	return out;
}

// 删除线 ~~
QString renderStrike(const QString& s)
{
	static const QString needle = QStringLiteral("~~");
	QString out;
	int i = 0;
	while (i < s.size()) {
		int p = s.indexOf(needle, i);
		if (p < 0) { out += s.mid(i); break; }
		int e = s.indexOf(needle, p + 2);
		if (e < 0) { out += s.mid(i); break; }
		out += s.mid(i, p - i);
		out += QLatin1String("<del>") + renderStrike(s.mid(p + 2, e - p - 2)) + QLatin1String("</del>");
		i = e + 2;
	}
	return out;
}

// ------------------------------------------------------------
// 前向声明
// renderInline 定义在下方，但 renderLink / renderImage 会递归调用它。
// ------------------------------------------------------------
QString renderInline(const QString& raw);

// ------------------------------------------------------------
// 图片 ![alt](src "title")
//
// 注意：入参 s 已由 renderInline 做过 HTML 转义，
// 因此这里不能再对 alt / dest 调escapeHtml，否则会二次转义
// （&amp; 变成 &amp;amp;）。只需对属性值做最小必要的引号处理。
// ------------------------------------------------------------
QString renderImage(const QString& s)
{
	if (!s.startsWith(u'!') || s.size() < 2 || s.at(1) != u'[')
		return s;

	int close = -1, depth = 0;
	for (int i = 1; i < s.size(); ++i) {
		if (s.at(i) == u'[') ++depth;
		else if (s.at(i) == u']') { --depth; if (depth == 0) { close = i; break; } }
	}
	if (close < 0 || close + 1 >= s.size() || s.at(close + 1) != u'(')
		return s;

	int paren = -1; depth = 0;
	for (int i = close + 1; i < s.size(); ++i) {
		if (s.at(i) == u'(') ++depth;
		else if (s.at(i) == u')') { --depth; if (depth == 0) { paren = i; break; } }
	}
	if (paren < 0)
		return s;

	const QString alt = s.mid(2, close - 2);
	QString dest = normalizeHref(s.mid(close + 2, paren - close - 2));
	QString title;
	const int sp = dest.indexOf(QChar(u' '));
	if (sp >= 0) {
		title = dest.mid(sp + 1).trimmed();
		if (title.size() >= 2 && title.startsWith(u'"') && title.endsWith(u'"'))
			title = title.mid(1, title.size() - 2);
		dest = dest.left(sp).trimmed();
	}
	// 入参已转义，" 变成了 &quot;，这里去掉首尾的实体引号再还原
	if (title.startsWith(QLatin1String("&quot;")) && title.endsWith(QLatin1String("&quot;")))
		title = title.mid(6, title.size() - 12);

	QString html = QStringLiteral("<img src=\"%1\" alt=\"%2\"").arg(dest, alt);
	if (!title.isEmpty())
		html += QStringLiteral(" title=\"%1\"").arg(title);
	html += QStringLiteral(" />");
	return html + s.mid(paren + 1);
}

// ------------------------------------------------------------
// 链接 [text](dest "title")
// 同上：入参已转义，不可重复 escapeHtml。
// ------------------------------------------------------------
QString renderLink(const QString& s)
{
	if (!s.startsWith(u'['))
		return s;

	int close = -1, depth = 0;
	for (int i = 0; i < s.size(); ++i) {
		if (s.at(i) == u'[') ++depth;
		else if (s.at(i) == u']') { --depth; if (depth == 0) { close = i; break; } }
	}
	if (close < 0 || close + 1 >= s.size() || s.at(close + 1) != u'(')
		return s;

	int paren = -1; depth = 0;
	for (int i = close + 1; i < s.size(); ++i) {
		if (s.at(i) == u'(') ++depth;
		else if (s.at(i) == u')') { --depth; if (depth == 0) { paren = i; break; } }
	}
	if (paren < 0)
		return s;

	const QString text = s.mid(1, close - 1);
	QString dest = normalizeHref(s.mid(close + 2, paren - close - 2));
	QString title;
	const int sp = dest.indexOf(QChar(u' '));
	if (sp >= 0) {
		title = dest.mid(sp + 1).trimmed();
		if (title.size() >= 2 && title.startsWith(u'"') && title.endsWith(u'"'))
			title = title.mid(1, title.size() - 2);
		dest = dest.left(sp).trimmed();
	}
	if (title.startsWith(QLatin1String("&quot;")) && title.endsWith(QLatin1String("&quot;")))
		title = title.mid(6, title.size() - 12);

	QString html;
	if (!isSafeUrl(dest)) {
		// javascript: 等危险协议：降级为纯文本
		html = text;
	} else {
		html = QStringLiteral("<a href=\"%1\"").arg(dest);
		if (!title.isEmpty())
			html += QStringLiteral(" title=\"%1\"").arg(title);
		html += QLatin1Char('>') + text + QLatin1String("</a>");
	}
	return html + s.mid(paren + 1);
}

// ------------------------------------------------------------
// 裸 URL 自动链接（匹配 http/https 开头的地址）
// 入参已转义，故URL 内不会出现裸引号与尖括号，可直接匹配。
//
// 注意：注释里不要写含双斜杠的协议前缀。
// 本工程曾在注释里写 https?://...，
// 后面的双斜杠把该行后半段变成注释，
// 导致 autolinkBareUrls 的函数头被注释掉，
// 后续所有行号上移，编译器报出一堆
// 「语法错误：双尖括号」之类完全指不到真正问题的错误。
// 这类错误极难定位：报错行号与病因行号相差很远。
// ------------------------------------------------------------
QString autolinkBareUrls(const QString& s)
{
	static const QRegularExpression re(
		QStringLiteral("(https?://[^\\s<>\\)\\]\\}\"']+)"));

	QString out;
	int i = 0;
	auto it = re.globalMatch(s);
	while (it.hasNext()) {
		const QRegularExpressionMatch m = it.next();
		int start = m.capturedStart();
		int len   = m.capturedLength();

		// 去掉尾部标点
		QString url = s.mid(start, len);
		while (!url.isEmpty()) {
			const QChar c = url.at(url.size() - 1);
			if (c == u'.' || c == u',' || c == u';' || c == u':' ||
				c == u')' || c == u'!' || c == u'?') {
				url.chop(1);
			} else {
				break;
			}
		}
		const int keep = url.size();

		out += s.mid(i, start - i);
		if (isSafeUrl(url))
			out += QStringLiteral("<a href=\"%1\">%2</a>").arg(url, url);
		else
			out += url;
		i = start + keep;
		// 跳过被裁掉的标点
	}
	out += s.mid(i);
	return out;
}

// ------------------------------------------------------------
// 占位符池：把已生成的 HTML 片段暂存，返回一个私用区字符。
// 全部行内规则处理完后，再统一把这些字符换回真正的标签。
//
// 为什么需要：正文里的尖括号与 & 必须转义，而规则生成的标签
// 不能被转义。用占位符把两者彻底隔离，是最不易出错的做法。
//
// —— 本工程在此踩过的两个坑，排查成本都很高，记录于此——
//
// 坑一：注释里不能出现协议前缀（含双斜杠）或裸露的尖括号。
//   注释里的双斜杠会把该行后半段变成注释，函数头被吃掉；
//   注释里的尖括号会被当成模板开始符。
//   两者都会让 MSVC 从注释处开始解析错位，
//   报错行号离真正的病因很远（例如报在几十行之后的
//   「语法错误：双尖括号」），极易误判。
//
// 坑二：变量名不能叫 slots。
//   Qt 在 qobjectdefs.h 里定义了宏
//     #define slots Q_SLOTS
//   （当不启用 Qt 的 moc 关键字时才定义，
//     但本工程默认就会生效）
//   一旦用它做变量名，整段代码会被宏替换成 Q_SLOTS，
//   编译报「语法错误：双尖括号」，而代码看起来完全正常。
//   现已统一改名为 slotPool。
// ------------------------------------------------------------
QString stashHtml(QStringList& slotPool, const QString& html)
{
	slotPool << html;
	return QChar(0xE000 + slotPool.size() - 1);   // U+E000 起是 Unicode 私用区
}

QString renderInline(const QString& raw)
{
	// ============================================================
	// 行内渲染策略（2026-10-08 重写）
	//
	// 旧实现把「生成 HTML」和「转义 HTML」混在一个循环里靠
	// if 判断哪些尖括号是标签、哪些是正文，逻辑脆弱且不完整：
	//   - 白名单只列了 <em <del <strong> <code>，漏掉 <a <img <h1>
	//   - <del 判断写成 mid(i,4)==<del，实际 "<del>" 长5，
	//     会误判成正文并把标签本身转义掉
	// 结果是 <strong>/<em>/<del>/<code>/<a>/<img> 全部被转义成
	// 可见文本，行内语法整体失效（离线测试 7 项FAIL 验证过）。
	//
	// 新策略（业界通用的三段式）：
	//   1. 先把原始文本做 HTML 转义（正文安全）
	//   2. 行内语法在「已转义」的文本上匹配并替换，
	//      产出的标签用私用区占位符暂存
	//   3. 最后把占位符还原成真正的标签
	// 这样正文里的尖括号与 & 一定被转义，而生成的标签绝不会被转义。
	// ============================================================

	// ---- 1. 正文转义 ----
	QString s = escapeHtml(raw);

	// ---- 占位符池 ----
	// 用普通的静态辅助函数而不是 lambda：
	// MSVC 在 lambda 体里对 QStringList 的追加运算符与
	// 显式返回类型组合时有语法解析问题。
	QStringList slotPool;

	// ---- 2a. 行内代码 ----
	// 必须最先处理：代码里的 * _ [ ] 等不应再被当作标记。
	{
		QString t;
		int i = 0;
		while (i < s.size()) {
			const int p = s.indexOf(QChar(u'`'), i);
			if (p < 0) { t += s.mid(i); break; }
			int ticks = 0;
			while (p + ticks < s.size() && s.at(p + ticks) == u'`') ++ticks;
			const QString fence(ticks, u'`');
			const int e = s.indexOf(fence, p + ticks);
			if (e < 0) { t += s.mid(i); break; }
			t += s.mid(i, p - i);
			QString code = s.mid(p + ticks, e - p - ticks);
			if (code.size() >= 2 && code.startsWith(u' ') && code.endsWith(u' '))
				code = code.mid(1, code.size() - 2);
			// code 已转义过一次（步骤1），此处直接包裹
			t += stashHtml(slotPool, QStringLiteral("<code>") + code + QLatin1String("</code>"));
			i = e + ticks;
		}
		s = t;
	}

	// ---- 2b. 图片 ![alt](src "title") ----
	s = renderImage(s);

	// ---- 2c. 链接 [text](dest "title") ----
	s = renderLink(s);

	// ---- 2d. 三重标记（粗斜体）----
// 必须最先处理：否则 ** 会先吃掉前两个星号，
// 剩下单个 * 变成斜体，结果是 <strong>*文字</strong>*。
// （渲染实测确认过这个现象。）
{
		static const QString three = QStringLiteral("***");
		QString t;
		int i = 0;
		while (i < s.size()) {
			const int p = s.indexOf(three, i);
			if (p < 0) { t += s.mid(i); break; }
			const int e = s.indexOf(three, p + 3);
			if (e < 0) { t += s.mid(i); break; }
			t += s.mid(i, p - i);
			t += QLatin1String("<strong><em>")
			   + renderInline(s.mid(p + 3, e - p - 3))
			   + QLatin1String("</em></strong>");
			i = e + 3;
		}
		s = t;
	}

	// ---- 2e. 粗体 / 斜体 ----
	// 注意：传给 renderBold / renderItalic 的是「单个」标记字符，
	// 函数内部会自己做 marker + marker。
	// 早期版本传的是已拼好的双星号，函数里又拼一次变成四个星号，
	// 于是永远匹配不上，粗体斜体全部失效。
	s = renderBold(s, QStringLiteral("*"));
	s = renderItalic(s, QStringLiteral("*"));
	s = renderBold(s, QStringLiteral("_"));
	s = renderItalic(s, QStringLiteral("_"));

	// ---- 2e. 删除线 ----
	s = renderStrike(s);

	// ---- 2f. 反斜杠转义（正文已转义，此处只需去掉反斜杠本身）----
	{
		QString t;
		for (int i = 0; i < s.size(); ++i) {
			if (s.at(i) == u'\\' && i + 1 < s.size()) {
				const QChar n = s.at(i + 1);
				if (QStringLiteral("\\`*_{}[]()#+-.!|~\"'$%&/:;=@^<>").contains(n)) {
					t += n;      // n 已是转义后的实体或普通字符
					++i;
					continue;
				}
			}
			t += s.at(i);
		}
		s = t;
	}

	// ---- 2g. 裸 URL 自动链接 ----
	// 放在最后：此时正文里的 " 已被转义成 &quot;，
	// 裸 URL 里不含引号，匹配安全。
	s = autolinkBareUrls(s);

	// ---- 3. 还原占位符 ----
	// 一轮可能还原出新的占位符（理论上不会，但保险起见循环到收敛）
	for (int guard = 0; guard < 8; ++guard) {
		bool changed = false;
		QString t;
		for (int i = 0; i < s.size(); ++i) {
			const QChar c = s.at(i);
			if (c.unicode() >= 0xE000 && c.unicode() <= 0xF8FF) {
				const int idx = c.unicode() - 0xE000;
				if (idx >= 0 && idx < slotPool.size()) {
					t += slotPool.at(idx);
					changed = true;
					continue;
				}
			}
			t += c;
		}
		s = t;
		if (!changed)
			break;
	}
	return s;
}

// ---------- 块级渲染 ----------

struct RenderCtx {
	QString html;
	QStringList* anchors = nullptr;   // 标题锚点暂不需要
};

void appendLine(QString& html, const QString& line)
{
	html += line;
	html += QLatin1Char('\n');
}

// 前向声明（签名必须与下方定义完全一致，包括返回类型）
// 三者都返回「处理到的下一行行号」，供调用方继续推进。
int renderTable(const QStringList& lines, int start, QString& out);
void renderBlocks(const QStringList& lines, int baseIndent, QString& out);
int renderList(const QStringList& lines, int start, int baseIndent, QString& out);
int renderQuote(const QStringList& lines, int start, int baseIndent, QString& out);

int renderTable(const QStringList& lines, int start, QString& out)
{
	const QStringList headCells = splitTableRow(lines.at(start));
	QString markerRaw;
	splitTableRow(lines.at(start + 1), &markerRaw);
	const QStringList markerCells = splitTableRow(lines.at(start + 1));

	// 每列的对齐方式，来自第二行的分隔标记
	QStringList aligns;
	for (int i = 0; i < headCells.size(); ++i)
		aligns << (i < markerCells.size() ? alignFromMarker(markerCells.at(i)) : QString());

	// 注意：这里必须用 lambda，不能写成嵌套函数
	//（C++ 不允许在函数体内定义函数，MSVC 会报
	//  "error C2601: 本地函数定义是非法的"）。
	const auto alignAttr = [&aligns](int i) -> QString {
		if (i < 0 || i >= aligns.size() || aligns.at(i).isEmpty())
			return QString();
		const QString a = aligns.at(i);
		if (a == QLatin1String("center")) return QStringLiteral(" align=\"center\"");
		if (a == QLatin1String("right"))  return QStringLiteral(" align=\"right\"");
		return QStringLiteral(" align=\"left\"");
	};

	appendLine(out, QStringLiteral("<table>"));
	appendLine(out, QStringLiteral("<thead>"));
	appendLine(out, QStringLiteral("<tr>"));
	for (int i = 0; i < headCells.size(); ++i) {
		appendLine(out, QStringLiteral("<th%1>%2</th>").arg(alignAttr(i), renderInline(headCells.at(i))));
	}
	appendLine(out, QStringLiteral("</tr>"));
	appendLine(out, QStringLiteral("</thead>"));
	appendLine(out, QStringLiteral("<tbody>"));

	int i = start + 2;
	while (i < lines.size()) {
		const QString& l = lines.at(i);
		if (isBlank(l)) {
			// 空行后若还是表格行则继续，否则结束
			int k = i + 1;
			while (k < lines.size() && isBlank(lines.at(k))) ++k;
			if (k < lines.size() && lines.at(k).contains(u'|') && !parseListItem(lines.at(k)).content.isEmpty())
				i = k;
			else
				break;
			continue;
		}
		if (!l.contains(u'|')) break;
		if (isHorizontalRule(l)) break;
		const QStringList cells = splitTableRow(l);
		appendLine(out, QStringLiteral("<tr>"));
		for (int c = 0; c < headCells.size(); ++c) {
			const QString txt = (c < cells.size()) ? cells.at(c) : QString();
			appendLine(out, QStringLiteral("<td%1>%2</td>").arg(alignAttr(c), renderInline(txt)));
		}
		appendLine(out, QStringLiteral("</tr>"));
		++i;
	}
	appendLine(out, QStringLiteral("</tbody>"));
	appendLine(out, QStringLiteral("</table>"));
	// 返回表后第一行的行号，供调用方继续推进。
	// （早期版本把行号塞进一段 HTML 注释再 indexOf 取回，
	//  既脏又容易和正文里的注释冲突，改为直接返回。）
	return i;
}

void renderBlocks(const QStringList& linesIn, int baseIndent, QString& out)
{
	QStringList lines = linesIn;

	int i = 0;
	while (i < lines.size()) {
		const QString line = lines.at(i);

		// 空行
		if (isBlank(line)) { ++i; continue; }

		// 缩进代码块（4 空格）
		if (indentWidth(line) - baseIndent >= 4 && !parseListItem(line).content.isNull()) {
			// 注意：这里要求确实是缩进代码，而非列表续行
		}

		// 围栏代码块
		const int flen = fenceLength(line);
		if (flen > 0) {
			QChar fenceChar = line.trimmed().isEmpty() ? u'`' : line.trimmed().at(0);
			QString lang;
			{
				const int ti = line.indexOf(fenceChar);
				if (ti >= 0) {
					lang = line.mid(ti + flen).trimmed();
					const int sp = lang.indexOf(QChar(u' '));
					if (sp >= 0) lang = lang.left(sp);
				}
			}
			QString code;
			++i;
			while (i < lines.size()) {
				const QString& l = lines.at(i);
				const QString t = l.trimmed();
				if (t.size() >= flen) {
					bool same = true;
					for (int k = 0; k < t.size(); ++k) {
						if (t.at(k) != fenceChar) { same = false; break; }
					}
					if (same && t.size() >= flen) { ++i; break; }
				}
				// 内容去掉最多 flen 个前导空格
				int strip = 0, w = 0;
				int p = 0;
				while (p < l.size() && w < flen && (l.at(p) == u' ' || l.at(p) == u'\t')) {
					if (l.at(p) == u'\t') w += 4; else ++w;
					++p;
				}
				code += l.mid(p);
				code += QLatin1Char('\n');
				++i;
			}
			const QString cls = lang.isEmpty()
				? QString()
				: QStringLiteral(" class=\"language-%1\"").arg(escapeHtml(lang));
			appendLine(out, QStringLiteral("<pre><code%1>%2</code></pre>").arg(cls, escapeHtml(code)));
			continue;
		}

		// 分割线
		if (isHorizontalRule(line)) {
			appendLine(out, QStringLiteral("<hr />"));
			++i;
			continue;
		}

		// ATX 标题
		const int hl = atxHeadingLevel(line);
		if (hl > 0) {
			appendLine(out, QStringLiteral("<h%1>%2</h%1>").arg(hl).arg(renderInline(headingText(line, hl))));
			++i;
			continue;
		}

		// 引用块
		if (rtrim(line).startsWith(u'>')) {
			i = renderQuote(lines, i, baseIndent, out);
			continue;
		}

		// 表格：当前行含 | 且下一行是分隔行
		if (line.contains(u'|') && i + 1 < lines.size() && isTableDelimiterRow(lines.at(i + 1))) {
			i = renderTable(lines, i, out);
			continue;
		}

		// 列表
		const ListItemInfo li = parseListItem(line);
		if (li.kind != ListKind::None) {
			i = renderList(lines, i, baseIndent, out);
			continue;
		}

		// Setext 标题（当前行非空、下一行是 === 或 ---）
		if (i + 1 < lines.size() && !isBlank(line)) {
			int lvl = 0;
			const QString& next = lines.at(i + 1);
			if (rtrim(next).startsWith(u'=') && setextHeadingLevel(next, &lvl)) {
				appendLine(out, QStringLiteral("<h%1>%2</h%1>").arg(lvl).arg(renderInline(rtrim(line))));
				i += 2;
				continue;
			}
			if (rtrim(next).startsWith(u'-')
				&& setextHeadingLevel(next, &lvl)
				&& !isHorizontalRule(next)) {
				appendLine(out, QStringLiteral("<h%1>%2</h%1>").arg(lvl).arg(renderInline(rtrim(line))));
				i += 2;
				continue;
			}
		}

		// 缩进代码块
		if (indentWidth(line) >= baseIndent + 4) {
			QString code;
			const int stripTo = baseIndent + 4;
			while (i < lines.size()) {
				const QString& l = lines.at(i);
				if (isBlank(l)) {
					// 保留后续若仍是缩进代码
					int k = i + 1;
					while (k < lines.size() && isBlank(lines.at(k))) ++k;
					if (k < lines.size() && indentWidth(lines.at(k)) >= stripTo) {
						code += QLatin1Char('\n');
						++i;
						continue;
					}
					break;
				}
				if (indentWidth(l) < stripTo) break;
				code += stripIndent(l, stripTo);
				code += QLatin1Char('\n');
				++i;
			}
			appendLine(out, QStringLiteral("<pre><code>%1</code></pre>").arg(escapeHtml(code)));
			continue;
		}

		// 段落：连续非空行，遇到块级起始则结束
		//
		// ⚠ prev 检查只在「续行」时做（i > paraStart），段落首行绝不做。
		// 否则首行会因 prev 属于上一个块而被 break，i 不推进，
		// 外层 while 在同一行无限循环（UI 卡死）。
		// 2026-10-12 用 mdtest 实测复现（用例 bisect_2/8/9）：
		//   "> y\n<p>"  → HANG
		// 根因：renderQuote 消费完引用后返回的 i 指向引用后一行，
		// 该行的 prev 恰是 ">" 开头 → 首行被 break → 死循环。
		// 同理可触发："# h\ntext"、"---\ntext"、"```\nc\n```\ntext"。
		const int paraStart = i;
		QString para;
		while (i < lines.size()) {
			const QString& l = lines.at(i);
			if (isBlank(l)) break;
			if (i > paraStart) {
				const QString& prev = lines.at(i - 1);
				const int pfl = fenceLength(prev);
				if (pfl > 0) break;
				if (atxHeadingLevel(prev) > 0) break;
				if (isHorizontalRule(prev)) break;
				if (rtrim(prev).startsWith(u'>')) break;
				if (!parseListItem(l).content.isNull() && parseListItem(l).kind != ListKind::None) break;
				if (!parseListItem(prev).content.isNull() && parseListItem(prev).kind != ListKind::None) break;
				if (l.contains(u'|') && i + 1 < lines.size() && isTableDelimiterRow(lines.at(i + 1))) break;
			}
			// 硬换行判定
			if (!para.isEmpty()) {
				const QString cur = rtrim(l);
				if (cur.endsWith(u"  ") || cur.endsWith(QLatin1Char('\\')))
					para += QLatin1String("<br />");
				else
					para += QLatin1Char('\n');
			}
			para += rtrim(l);
			++i;
		}
		if (!para.trimmed().isEmpty()) {
			// 段落必须过renderInline：粗体、行内代码、链接、
			// 删除线等行内语法全靠它处理。
			// 早期版本直接 .arg(para) 输出，导致所有行内语法
			// 原样显示为字面量（离线测试 6 项 FAIL 验证过）。
			//
			// 硬换行：先用一个renderInline 会原样透传的私用区字符
			// 占位，待renderInline 完成后再换回真正的换行标签。
			// （不能直接塞 HTML 标签：escapeHtml 会把它转义掉。）
			//
			// ⚠ 占位字符绝不能用 0xE000-0xF8FF 区段——那是行内
			// 占位符池的范围（renderInline 内 stashHtml 从 0xE000
			// 起分配）。旧值 0xE001 会在「硬换行 + 两个行内代码」
			// 时与 stash 槽位冲突：br 标记被当作 slotPool[1] 还原
			// 成错误内容，真正的换行丢失（输出错乱，非卡死）。
			// 改用 U+0001：escapeHtml 原样透传、行内规则不消费、
			// 不在占位符区段，且在 setHtml 之前就已被换回 <br />。
			static const QChar kBrMark(0x0001);
			QString body = para;
			body.replace(QLatin1String("<br />"), QString(kBrMark));
			body = renderInline(body);
			body.replace(kBrMark, QLatin1String("<br />"));
			appendLine(out, QStringLiteral("<p>%1</p>").arg(body));
		}
	}
}

int renderQuote(const QStringList& lines, int start, int baseIndent, QString& out)
{
	QStringList inner;
	int i = start;
	while (i < lines.size()) {
		const QString t = rtrim(lines.at(i));
		if (t.startsWith(u'>')) {
			QString c = t.mid(1);
			if (c.startsWith(u' ')) c = c.mid(1);
			inner << c;
			++i;
			continue;
		}
		// 空行后若仍是引用标记则继续，否则结束
		if (isBlank(lines.at(i))) {
			int k = i + 1;
			while (k < lines.size() && isBlank(lines.at(k))) ++k;
			if (k < lines.size() && rtrim(lines.at(k)).startsWith(u'>')) {
				inner << QString();
				i = k;
				continue;
			}
		}
		break;
	}
	appendLine(out, QStringLiteral("<blockquote>"));
	QString sub;
	renderBlocks(inner, 0, sub);
	out += sub;
	appendLine(out, QStringLiteral("</blockquote>"));
	return i;
}

int renderList(const QStringList& lines, int start, int baseIndent, QString& out)
{
	(void)baseIndent;
	const ListItemInfo first = parseListItem(lines.at(start));
	const bool ordered = (first.kind == ListKind::Ordered);
	const QString tag = ordered ? QStringLiteral("ol") : QStringLiteral("ul");

	QString startAttr;
	if (ordered) {
		const QString t = rtrim(lines.at(start));
		int n = 0;
		while (n < t.size() && t.at(n).isDigit()) ++n;
		if (n > 1) startAttr = QStringLiteral(" start=\"%1\"").arg(t.left(n));
	}

	appendLine(out, QStringLiteral("<%1%2>").arg(tag, startAttr));

	int i = start;
	int itemCount = 0;
	while (i < lines.size()) {
		if (isBlank(lines.at(i))) {
			// 空行：若下一个仍是同级列表项则保留，否则结束
			int k = i + 1;
			while (k < lines.size() && isBlank(lines.at(k))) ++k;
			if (k >= lines.size()) { i = k; break; }
			const ListItemInfo nxt = parseListItem(lines.at(k));
			const bool sameKind = (nxt.kind == first.kind);
			if (!sameKind) {
				// 允许引用/缩进代码在列表内
				if (rtrim(lines.at(k)).startsWith(u'>')) { ++itemCount; /*续*/ }
				else { i = i; break; }
			}
			i = k;
			continue;
		}
		const ListItemInfo li = parseListItem(lines.at(i));
		if (li.kind != first.kind)
			break;

		// 收集本项内容（含后续缩进续行 / 嵌套列表）
		QStringList itemLines;
		const int leadIndent = indentWidth(lines.at(i));
		const QString stripped = stripIndent(lines.at(i), leadIndent);
		QString content;
		{
			// 去掉 marker 后，按 marker 宽度再 dedent 后续行
			int markerWidth = 0;
			QString t = stripped;
			if (li.kind == ListKind::Bullet) markerWidth = 2;
			else {
				int n = 0;
				while (n < t.size() && t.at(n).isDigit()) ++n;
				markerWidth = n + 2;
			}
			content = li.content;
			(void)markerWidth;
		}
		itemLines << content;
		++i;

		const int contentIndent = leadIndent + 2;   // 近似续行缩进
		while (i < lines.size()) {
			const QString& l = lines.at(i);
			if (isBlank(l)) {
				int k = i + 1;
				while (k < lines.size() && isBlank(lines.at(k))) ++k;
				if (k < lines.size() && indentWidth(lines.at(k)) >= contentIndent) {
					itemLines << QString();
					i = k;
					continue;
				}
				break;
			}
			const int wi = indentWidth(l);
			if (wi >= contentIndent) {
				itemLines << stripIndent(l, contentIndent);
				++i;
				continue;
			}
			// 懒续行（非空、非新列表项、非其它块级起始）
			const ListItemInfo nl = parseListItem(l);
			if (nl.kind != ListKind::None) break;
			if (atxHeadingLevel(l) > 0) break;
			if (isHorizontalRule(l)) break;
			if (fenceLength(l) > 0) break;
			if (rtrim(l).startsWith(u'>')) break;
			itemLines << l.trimmed();
			++i;
		}

		// 渲染本项
		QString sub;
		renderBlocks(itemLines, 0, sub);

		if (li.task) {
			const QString box = li.taskChecked
				? QStringLiteral("<input type=\"checkbox\" checked disabled /> ")
				: QStringLiteral("<input type=\"checkbox\" disabled /> ");
			// 去掉 sub 里可能出现的开头 <p>
			QString body = sub.trimmed();
			if (body.startsWith(QLatin1String("<p>"))) {
				const int e = body.indexOf(QLatin1String("</p>"));
				if (e > 0) body = body.mid(3, e - 3);
			}
			appendLine(out, QStringLiteral("<li class=\"task\">%1%2</li>").arg(box, body));
		} else {
			appendLine(out, QStringLiteral("<li>%1</li>").arg(sub.trimmed()));
		}
		++itemCount;
	}

	appendLine(out, QStringLiteral("</%1>").arg(tag));
	(void)itemCount;
	return i;
}

} // namespace

QString renderToHtml(const QString& markdown)
{
	QString normalized = markdown;
	normalized.replace(QLatin1String("\r\n"), QLatin1String("\n"));
	normalized.replace(u'\r', u'\n');

	QStringList lines = normalized.split(u'\n');
	// 去掉尾部空行
	while (!lines.isEmpty() && isBlank(lines.last()))
		lines.removeLast();

	if (lines.isEmpty())
		return QStringLiteral("<p class=\"ndd-empty\"><em>（空文档）</em></p>");

	QString out;
	out.reserve(markdown.size() * 2);
	renderBlocks(lines, 0, out);
	return out;
}

} // namespace MdRenderer