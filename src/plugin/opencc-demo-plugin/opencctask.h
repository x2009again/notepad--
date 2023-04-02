#ifndef OPENCCTASK_H
#define OPENCCTASK_H

#include <QObject>
#include <QRunnable>

class OpenCCTask : public QObject , public QRunnable
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText  NOTIFY textChanged)
    Q_PROPERTY(bool cn2Tn READ cn2Tn WRITE setCn2Tn NOTIFY cn2TnChanged)

public:
    explicit OpenCCTask(QObject *parent = nullptr, QString text = "");

//    enum TaskType {
//        NormalType = 0
//    };
//    Q_FLAGS(TaskType);
//    Q_DECLARE_FLAGS(TaskTypes, TaskType);

//    void setTaskType(TaskType type = NormalType);


public:
    // QRunnable interface
    void run() override;


signals:
    void complete(const QString s);
    void textChanged();
    void cn2TnChanged();

public:
    QString text() const;
    void setText(const QString &newText);
    bool cn2Tn() const;
    void setCn2Tn(bool newCn2Tn);

private:
    QString m_text;
    bool m_cn2Tn;
};

#endif // OPENCCTASK_H
