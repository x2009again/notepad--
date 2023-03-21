#include "opencctask.h"

#include "opencc/opencc.h"

OpenCCTask::OpenCCTask(QObject *parent, QString text) : QObject(parent)
  , m_text(text)
{

}

QString OpenCCTask::text() const
{
    return m_text;
}

void OpenCCTask::setText(const QString &text)
{
    m_text = text;
}

void OpenCCTask::run()
{
    opencc::SimpleConverter converter("s2t.json");

    std::string s = converter.Convert(m_text.toStdString().data());
    QString result = QString::fromStdString(s);
    emit complete(result);
}
