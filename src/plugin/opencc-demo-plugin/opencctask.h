#ifndef OPENCCTASK_H
#define OPENCCTASK_H

#include <QObject>
#include <QRunnable>

class OpenCCTask : public QObject , public QRunnable
{
    Q_OBJECT
public:
    explicit OpenCCTask(QObject *parent = nullptr, QString text = "");

//    enum TaskType {
//        NormalType = 0
//    };
//    Q_FLAGS(TaskType);
//    Q_DECLARE_FLAGS(TaskTypes, TaskType);

//    void setTaskType(TaskType type = NormalType);

    QString text() const;
    void setText(const QString &text);

signals:
    void complete(const QString s);

    // QRunnable interface
public:
    void run() override;

private:
    QString m_text;
};

#endif // OPENCCTASK_H
