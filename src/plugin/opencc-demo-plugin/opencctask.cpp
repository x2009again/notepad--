#include "opencctask.h"

#include "opencc/opencc.h"

OpenCCTask::OpenCCTask(QObject *parent, QString text) : QObject(parent)
  , m_text(text)
{

}

void OpenCCTask::run()
{
    opencc::SimpleConverter converter(cn2Tn()?"s2t.json":"t2s.json");
    std::string s = converter.Convert(m_text.toStdString().data());
    QString result = QString::fromStdString(s);
    emit complete(result);
}

QString OpenCCTask::text() const
{
    return m_text;
}

void OpenCCTask::setText(const QString &newText)
{
    if (m_text == newText)
        return;
    m_text = newText;
    emit textChanged();
}

bool OpenCCTask::cn2Tn() const
{
    return m_cn2Tn;
}

void OpenCCTask::setCn2Tn(bool newCn2Tn)
{
    if (m_cn2Tn == newCn2Tn)
        return;
    m_cn2Tn = newCn2Tn;
    emit cn2TnChanged();
}
