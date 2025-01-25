#include "dialogAnimation/qtmaterialjbydialog.h"

#include <QPainter>
#include <QDebug>
#include <QEvent>
#include <QtWidgets/QVBoxLayout>
#define AnimationTime 177
QtMaterialJbyDialog::QtMaterialJbyDialog(QWidget *parent)
{
    installEventFilter(this);
    opacityAnimation = new QPropertyAnimation(this, "opacity",this);
    opacityAnimation->setDuration(AnimationTime);
    opacityAnimation->setEasingCurve(QEasingCurve::InCirc);
}

QtMaterialJbyDialog::~QtMaterialJbyDialog()
{

}

void QtMaterialJbyDialog::overlayAnimation()
{
    //mainWidget->setFixedSize(this->rect().width()*0.57,this->rect().height()*0.4);
    if(!mainWidget)
        return;

    if(showAnimation)
    {
        QRect rect = mainWidget->rect();
        QRect mainRect = this->rect();
        //qDebug() << rect << mainRect;
        if(m_animationDirection == UPTODOWN)
        {
            showAnimation->setStartValue(QRect((mainRect.width()-rect.width())/2, 0,rect.width(), rect.height()));
            showAnimation->setEndValue(QRect((mainRect.width()-rect.width())/2, (mainRect.height()-rect.height())/2,rect.width(), rect.height()));
        }else if(m_animationDirection == DOWNTOUP)
        {
            showAnimation->setStartValue(QRect((mainRect.width()-rect.width())/2, mainRect.height()-rect.height(),rect.width(), rect.height()));
            showAnimation->setEndValue(QRect((mainRect.width()-rect.width())/2, (mainRect.height()-rect.height())/2,rect.width(), rect.height()));
        }else if(m_animationDirection == RIGHTTOLEFT)
        {
            showAnimation->setStartValue(QRect(mainRect.width(), (mainRect.height()-rect.height())/2,rect.width(), rect.height()));
            showAnimation->setEndValue(QRect((mainRect.width()-rect.width()), (mainRect.height()-rect.height())/2,rect.width(), rect.height()));
        }else if(m_animationDirection == LEFTTORIGHT)
        {
            showAnimation->setStartValue(QRect(-rect.width(), (mainRect.height()-rect.height())/2,rect.width(), rect.height()));
            showAnimation->setEndValue(QRect(0, (mainRect.height()-rect.height())/2,rect.width(), rect.height()));
        }
    }
    if(hideAnimation)
    {
        QRect rect = mainWidget->rect();
        QRect mainRect = this->rect();
        //qDebug() << rect << mainRect;
        if(m_animationDirection == UPTODOWN)
        {
            hideAnimation->setStartValue(QRect((mainRect.width()-rect.width())/2, (mainRect.height()-rect.height())/2,rect.width(), rect.height()));
            hideAnimation->setEndValue(QRect((mainRect.width()-rect.width())/2,0,rect.width(), rect.height()));
        }else if(m_animationDirection == DOWNTOUP)
        {
            hideAnimation->setStartValue(QRect((mainRect.width()-rect.width())/2, (mainRect.height()-rect.height())/2,rect.width(), rect.height()));
            hideAnimation->setEndValue(QRect((mainRect.width()-rect.width())/2,mainRect.height()-rect.height(),rect.width(), rect.height()));
        }else if(m_animationDirection == RIGHTTOLEFT)
        {
            hideAnimation->setStartValue(QRect((mainRect.width()-rect.width()), (mainRect.height()-rect.height())/2,rect.width(), rect.height()));
            hideAnimation->setEndValue(QRect(mainRect.width(),(mainRect.height()-rect.height())/2,rect.width(), rect.height()));
        }else if(m_animationDirection == LEFTTORIGHT)
        {
            hideAnimation->setStartValue(QRect(0, (mainRect.height()-rect.height())/2,rect.width(), rect.height()));
            hideAnimation->setEndValue(QRect(-rect.width(),(mainRect.height()-rect.height())/2,rect.width(), rect.height()));
        }
    }
}

void QtMaterialJbyDialog::setMainWidget(QWidget *w)
{
    mainWidget = w;
    if(showAnimation)
        delete showAnimation;
    showAnimation = new QPropertyAnimation(mainWidget, "geometry",this);
    showAnimation->setDuration(AnimationTime);
    showAnimation->setEasingCurve(QEasingCurve::InCirc);

    if(hideAnimation)
        delete hideAnimation;
    hideAnimation = new QPropertyAnimation(mainWidget, "geometry",this);
    hideAnimation->setDuration(AnimationTime);
    hideAnimation->setEasingCurve(QEasingCurve::InCirc);
    connect(hideAnimation, SIGNAL(finished()),this, SLOT(slotHideAnimationFinish()));
}

void QtMaterialJbyDialog::showDialog()
{
    show();
    startShowAnimation();
}

void QtMaterialJbyDialog::closeDialog()
{
    startHideAnimation();
}

void QtMaterialJbyDialog::slotHideAnimationFinish()
{
    if(m_Loop.isRunning())
        m_Loop.exit();
    hide();
}

void QtMaterialJbyDialog::exec()
{
    showDialog();
    m_Loop.exec(); //利用事件循环实现模态
}

void QtMaterialJbyDialog::setOpacity(qreal opacity)
{
    m_opacity = opacity;
    update();
}

void QtMaterialJbyDialog::startShowAnimation()
{
    overlayAnimation();
    opacityAnimation->setStartValue(0);
    opacityAnimation->setEndValue(0.4);
    opacityAnimation->start();
    if(showAnimation)
        showAnimation->start();
}

void QtMaterialJbyDialog::startHideAnimation()
{
    overlayAnimation();
    opacityAnimation->setStartValue(0.4);
    opacityAnimation->setEndValue(0);
    opacityAnimation->start();
    if(hideAnimation)
        hideAnimation->start();
}

//bool QtMaterialJbyDialog::eventFilter(QObject *obj, QEvent *event)
//{
//    bool flag = QtMaterialOverlayWidget::eventFilter(obj, event);
//    //qDebug() << event->type();
//    switch (event->type())
//    {
//    //    case QEvent::ShowToParent:
//    //    {
//    //        qDebug() << "show";
//    //        startShowAnimation();
//    //        break;
//    //    }
//    //    case QEvent::Hide:
//    //    {
//    //        qDebug() << "hide";
//    //        startHideAnimation();
//    //        break;
//    //    }
//    case QEvent::Resize:
//    {
//        //qDebug() << "resize";
//        overlayAnimation();
//        break;
//    }
//    default:
//        break;
//    }
//    return flag;
//}


void QtMaterialJbyDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.setOpacity(m_opacity);        //值越高越不透明
    painter.drawRect(rect());
}
