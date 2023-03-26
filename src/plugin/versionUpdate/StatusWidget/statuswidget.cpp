#include "statuswidget.h"
#include "ui_statuswidget.h"

#include <QPropertyAnimation>

StatusWidget::StatusWidget(QWidget *parent, StatusWidget::EnterDirection in, StatusWidget::LeaveDirection out) :
    QWidget(parent),
    ui(new Ui::StatusWidget),
    parent(parent),
    animation(new QPropertyAnimation(this, "pos")),
    oldPos(this->pos()),
    inDirection(in),
    outDirection(out),
    hasMouse(false),
    isLeaveOut(true) // 默认为已经离开时
{
    ui->setupUi(this);
    ui->widget->setGeometry(this->geometry());

    // 自身初始化(图标)
    initializeIcon();

//    const QRect parentGeometry = parent->geometry();
//    QRect localGeometry = this->geometry();

//    int parentWidget = parentGeometry.width();
//    int parentHeight = parentGeometry.height();
//    localGeometry.setX(parentGeometry.width() - geometry().width());
//    setGeometry(localGeometry);
    // parent->x();

// 动画事件与初始化
    // 设置使用动画的控件
	animation->setTargetObject(this);
    // 设置动画效果
    animation->setEasingCurve(QEasingCurve::InOutQuart);
    // 设置目标控件的属性名称
    animation->setPropertyName("pos");
    // 设置动画时间
    animation->setDuration(1000);

    // 在动画时进行重绘
    connect(animation, &QPropertyAnimation::valueChanged, this, [=](const QVariant &value){
        update();
    });
    
    connect(animation, SIGNAL(finished()), this, SLOT(onAnimationDone()));
    // connect(animation, SIGNAL(()), this, SLOT(onAnimationDone()));

// 动画事件显示超时与初始化
    setTimeout(1500);

    connect(&showTimeout, &QTimer::timeout, this, [=](){
        // 鼠标存在于此窗口的标志？
        if (hasMouse) return ;
        // 没有鼠标存在于此窗口表示可以移出
        stop();
        showTimeout.stop();
    });
}

StatusWidget::~StatusWidget()
{
    delete ui;
}

void StatusWidget::initializeIcon()
{
    QSize iconSize = this->ui->icon->size();
    QPixmap iconPixmap= QPixmap(":/images/images/icon.png").scaled(iconSize);
    ui->icon->setPixmap(iconPixmap);
}

void StatusWidget::setIcon(QPixmap iconPixmap)
{
    ui->icon->setPixmap(iconPixmap.scaled(ui->icon->size()));
}

QPixmap StatusWidget::getIcon() {
    return ui->icon->pixmap()->copy();
}

void StatusWidget::setShowMessage(QString title, QString message)
{
    ui->title->setText(title);
    ui->message->setText(message);
}

void StatusWidget::setTimeout(int timeoutMsec)
{
    showTimeout.setInterval(timeoutMsec);
}

void StatusWidget::setDirection(StatusWidget::EnterDirection in, LeaveDirection out) {
    this->inDirection = in;
    this->outDirection = out;
}

void StatusWidget::algorithmDirection(QPoint &before, QPoint &after, int direction)
{
    int parentX = parent->x();
    int parentY = parent->y();
    int parentWidth = parent->width();
    int parentHeight = parent->height();

// 预留边缘空间
    int spaceGap = 10;

// before
    int beforeX, beforeY,
// after
        afterX, afterY;

// 方位计算，详细/精确扩展？// 这里是不完全的设计形式，如果你想使用不同的方案请另行设计
    // 上进只有右上，
    if (direction >= TopOut) oldPos = this->pos();
    switch (direction) {
    // 进入？
    case TopIn: // 从上面进入，起始位置为 x(不变)，y(小于父)
        beforeX = afterX = parentWidth - this->width() - spaceGap;
        beforeY = 0 - this->height() - spaceGap;
        afterY = 0 + spaceGap;
        break;
    case LeftIn: // 从左面进入，起始位置为 x(小于父)，y(不变)
        beforeY = afterY = this->y();
        beforeX = parentX - this->x();
        afterX = parentX + this->x() + spaceGap;
        break;
    case RightIn: // 从左面进入，起始位置为 x(大于父)，y(不变)
        beforeY = afterY = (outDirection == TopOut) ? spaceGap : parentHeight - this->height() - spaceGap;
        beforeX = parentWidth + this->width() + spaceGap;
        afterX = parentWidth - this->width() - spaceGap;
        break;
    case BottomIn: // 从下面进入，起始位置为 x(不变)，y(大于父)
        beforeX = afterX = parentWidth - this->width() - spaceGap;
        beforeY = parentHeight + this->height();
        afterY = parentHeight - this->height() - spaceGap;
        break;

    // 离开?
    case TopOut: // 从上面离开，起始位置为 x(不变)，y(小于父)
        beforeX = afterX = this->x();
        beforeY = this->y();
        afterY = 0 - this->height() - spaceGap;
        break;
    case LeftOut: // 从左面离开，起始位置为 x(小于父)，y(不变)
        beforeY = afterY = this->y();
        beforeX = this->x();
        afterX = 0 - this->width() - spaceGap;
        break;
    case RightOut: // 从左面离开，起始位置为 x(大于父)，y(不变)
        beforeY = afterY = this->y();
        beforeX = this->x();
        afterX = parentWidth + spaceGap;
        break;
    case BottomOut: // 从下面离开，起始位置为 x(不变)，y(大于父)
        beforeX = afterX = this->x();
        beforeY = this->y();
        afterY = parentHeight + this->height() + spaceGap;
        break;
    }

    // New ?
//    switch (direction) {
//    case TopIn: // 从上面进入，起始位置为 x(不变)，y(小于父与其自身高)
//        beforeX = afterX = this->x();
//        beforeY = 0 - this->height() - spaceGap;
//        afterY = spaceGap;
//        break;
//    case LeftIn: // 从左面进入，起始位置为 x(小于父与其自身宽)，y(不变)
//        beforeY = afterY = this->y();
//        beforeX = 0 - this->x() - this->width();
//        afterX = parentX + this->x() + spaceGap;
//        break;
//    case RightIn: // 从左面进入，起始位置为 x(大于父与其宽)，y(不变)
//        beforeY = afterY = this->y();
//        beforeX = parentX + parentWidth + this->x() + spaceGap;
//        afterX = parentWidth - this->x() - spaceGap;
//        break;
//    case BottomIn: // 从下面进入，起始位置为 x(不变)，y(大于父与其高)
//        beforeX = afterX = this->x();
//        beforeY = parentHeight + this->height();
//        afterY = parentHeight - this->height() - spaceGap;
//        break;
//    }

    before.setX(beforeX);
    before.setY(beforeY);
    after.setX(afterX);
    after.setY(afterY);
}


// 进入动画
void StatusWidget::start(bool autoLeave) {
    // 计算方向
    QPoint before, after;
    algorithmDirection(before, after, inDirection);

    animation->setStartValue(before);
    animation->setEndValue(after);
    animation->start();

    // 超时定时器启动?
    if (autoLeave) showTimeout.start();
    show(); // 默认情况启动动画时应该是显示的
    // hide(); //默认情况完成动画时并且不在窗口内时应该是隐藏的
}

// 离开动画
void StatusWidget::stop()
{
    QPoint before, after;
    algorithmDirection(before, after, outDirection);

    animation->setStartValue(before);
    animation->setEndValue(after);
    animation->start();

    // 超时定时器停止?
    showTimeout.stop();
}

void StatusWidget::on_close_clicked()
{
    this->stop();
}

void StatusWidget::onAnimationDone()
{
    // 不知道逻辑是否严谨
    // if (x() >= parent->width()) {
    //     isLeaveOut = true;
    // } else {
    //     isLeaveOut = false;
    // }

//    isLeaveOut = x() >= parent->width(); // 如果默认情况在移动未完成时，这个可能不会成立

    // 如果默认情况在移动未完成时，这个可能不会成立
    // 判断是否在右边
    //    isLeaveOut = false;
    isLeaveOut |= (y() <= parent->y() - height() && x() > 0  && x() < parent->width());
    isLeaveOut |= (y() >= parent->height() && x() > 0  && x() < parent->width());
    isLeaveOut |= (x() <= parent->width() - width() && y() > 0 && y() < parent->height());
    isLeaveOut |= (x() >= parent->width() + width() && y() > 0 && y() < parent->height());


    if (!isLeaveOut) {
        hide();
    } else {
        // show();  // ?
    }
}

void StatusWidget::enterEvent(QEvent *event)
{
    hasMouse = true;
}

void StatusWidget::leaveEvent(QEvent *event)
{
    hasMouse = false;
}
