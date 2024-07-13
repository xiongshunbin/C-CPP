#ifndef QTMATERIALJBYDIALOG_H
#define QTMATERIALJBYDIALOG_H
#include <dialogAnimation/qtmaterialoverlaywidget.h>
#include <QEventLoop>
#include <QPropertyAnimation>
#include <QtWidgets/QMessageBox>

class QtMaterialJbyDialog : public QtMaterialOverlayWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity WRITE setOpacity READ opacity)
public:
    explicit QtMaterialJbyDialog(QWidget *parent = 0);
    ~QtMaterialJbyDialog();

    enum ANIMATIONDIRECTION
    {
        UPTODOWN = 0,
        DOWNTOUP = 1,
        RIGHTTOLEFT = 2,
        LEFTTORIGHT = 3
    };

    void overlayAnimation();
    void setMainWidget(QWidget *w);

    void showDialog();
    void closeDialog();
    void exec();

    void setAnimationDirection(ANIMATIONDIRECTION direction) { m_animationDirection = direction; }
    void setDlgState(int state) { m_dialogState = state; }
    int DlgState() { return m_dialogState; }

    void setOpacity(qreal opacity);
    inline qreal opacity() const;

private:
    void startShowAnimation();
    void startHideAnimation();
private slots:
    void slotHideAnimationFinish();
protected:
    void paintEvent(QPaintEvent *event);
    //bool eventFilter(QObject *obj, QEvent *event);
private:
    QPropertyAnimation *showAnimation = NULL;
    QPropertyAnimation *hideAnimation = NULL;
    QPropertyAnimation *opacityAnimation = NULL;
    QWidget            *mainWidget = NULL;
    int                m_dialogState = QMessageBox::Cancel;
    QEventLoop         m_Loop;
    qreal              m_opacity;
    ANIMATIONDIRECTION m_animationDirection = UPTODOWN;    //运动方向
};

inline qreal QtMaterialJbyDialog::opacity() const
{
    return m_opacity;
}

#endif // QTMATERIALJBYDIALOG_H
