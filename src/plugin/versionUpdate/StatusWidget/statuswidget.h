#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QTimer>
#include <QWidget>


namespace Ui {
class StatusWidget;
}
class QPropertyAnimation;
class StatusWidget : public QWidget
{
    Q_OBJECT

public:
    // Entering and leaving
//    enum EnterLeaveDirection {
//    typedef void(*AlgorithDirctionFunc)(QPoint &before, QPoint &after, int direction);
    enum EnterDirection {
        TopIn = 0,
        LeftIn,
        RightIn,
        BottomIn,
    };

    enum LeaveDirection {
        TopOut = 4,
        LeftOut,
        RightOut,
        BottomOut,
    };

    //    AlgorithDirctionFunc algorithDirectionFunc;

//    typedef EnterLeaveDirection EnterDirection;
//    typedef EnterLeaveDirection LeaveDirection;

public:
    explicit StatusWidget(QWidget *parent = nullptr, enum EnterDirection in = RightIn, enum LeaveDirection out = RightOut);
    ~StatusWidget();

    ///
    /// \brief initializeIcon
    ///
    void initializeIcon();

    ///
    /// \brief setIcon
    /// \param iconPixmap
    ///
    void setIcon(QPixmap iconPixmap);
    QPixmap getIcon();

    ///
    /// \brief 消息设置
    /// \param title 标题
    /// \param message 消息
    ///
    void setShowMessage(QString title, QString message);

    // 方位设置与计算
    void setTimeout(int timeoutMsec);
    void setDirection(EnterDirection in, LeaveDirection out);
    void algorithmDirection(QPoint &before, QPoint &after, int direction);

public slots:
    void start(bool autoLeave = true);
    void stop();

private slots:
    void on_close_clicked();
    void onAnimationDone();

private:
    Ui::StatusWidget *ui;
    QWidget *parent;
    QPropertyAnimation *animation;
    QPoint oldPos;
    EnterDirection inDirection;
    LeaveDirection outDirection;
    QTimer showTimeout;


    bool hasMouse;
    bool isLeaveOut;

    // QWidget interface
protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

    // QWidget interface
protected:
//    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // STATUSWIDGET_H
