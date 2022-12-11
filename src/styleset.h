#pragma once
#include <QColor>

enum StyleId {
	DEFAULT_SE=0,
	LIGHT_SE,
	THIN_BLUE_SE,
	THIN_YELLOW_SE,
	RICE_YELLOW_SE,
	SILVER_SE,
	LAVENDER_SE,
	MISTYROSE_SE,
	BLACK_SE,
	MAX_SE,
};

class StyleSet 
{
public:
	StyleSet();
	~StyleSet();

	static void setCommonStyle(QColor foldfgColor_, QColor foldbgColor_, QColor marginsBackgroundColor_, QString colorName);

	static void setSkin(int id);
	static QString getCurrentStyle();
	static StyleId getCurrentSytleId()
	{
		return StyleId(m_curStyleId);
	}
	static QColor foldfgColor;
	static QColor foldbgColor;
	static QColor marginsBackgroundColor;
	static QColor bookmarkBkColor;
	static int m_curStyleId;

	static void setDefaultStyle();
	static void setLightStyle();
	static void setThinBlueStyle();
	static void setThinYellowStyle();
	static void setRiceYellowStyle();
	static void setSilverStyle();
	static void setLavenderBlushStyle();
	static void setMistyRoseStyle();
	static void setBlackStyle();
};
