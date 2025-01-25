#include "ib_login.h"
#include "ui_ib_login.h"

ib_Login::ib_Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ib_Login)
    , m_ptrSqlDao(nullptr)
{
    ui->setupUi(this);

    // 去掉系统边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    // 设置父窗口为透明, 设置父窗口的目的是实现圆角窗口
    this->setAttribute(Qt::WA_TranslucentBackground);

    // 实现无边框窗口的移动
    frame= new FramelessWidget(this);
    frame->setPadding(5);
    frame->setMoveEnable(true);
    frame->setResizeEnable(false);
    frame->setWidget(this);

    QFile f;
    f.setFileName(":/Styles/ib_login.css");
    f.open(QIODevice::ReadOnly);
    QString strQss = f.readAll();
    this->setStyleSheet(strQss);
    f.close();

    ui->lb_head->setFont(loadFont(":/Fonts/fzhzgbjt.TTF", 16));

    ui->lb_subtitle->setFont(loadFont(":/Fonts/hwxk.TTF", 13));

    QFont iconFont = loadFont(":/Fonts/login_iconfont.ttf", 12);
    ui->lb_uname->setFont(iconFont);
    ui->lb_uname->setText(QChar(0xe693));

    ui->lb_password->setFont(iconFont);
    ui->lb_password->setText(QChar(0xe69f));

    m_ptrSqlDao = SqlDao::getinstance();
    m_ptrSqlDao->init();

}

ib_Login::~ib_Login()
{
    delete ui;
}

QFont ib_Login::loadFont(QString name, int fontSize)
{
    //加入字体，并获取字体ID
    int fontId = QFontDatabase::addApplicationFont(name);
    //获取字体名称
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font;
    font.setFamily(fontName);
    font.setPointSize(fontSize);
    return font;
}


void ib_Login::on_btn_close_clicked()
{
    this->close();
}


void ib_Login::on_btn_clear_clicked()
{
    ui->le_uname->clear();
    ui->le_password->clear();
}


void ib_Login::on_btn_login_clicked()
{
#if 0
    emit sendLoginSuccess("admin");
#else
    // 获取登录界面输入框内登录名和密码
    QString loginName = ui->le_uname->text();
    QString password = ui->le_password->text();

    // reg_username 用户名正则表达式，只能输入数字字母下划线，5-18位，字母开头
    static QRegularExpression reg_username("^[A-Za-z][A-Za-z0-9_]{4,17}$");

    // reg_password 密码正则表达式，5-16位非空字符
    static QRegularExpression reg_password("^[\\S]{5,16}$");

    // 分别检测登录名和密码是否满足要求
    QRegularExpressionMatch match_uname = reg_username.match(loginName);
    QRegularExpressionMatch match_password = reg_password.match(password);

    dialog.setPix("提示", ":/Images/error.png");

    if(!match_uname.hasMatch())             // 登录名格式错误
    {
        dialog.setInformation("用户名不能为空或格式错误!字母开头,5-18位字母或数字下划线");
        dialog.setFixedSize(520, 136);
        dialog.exec();
    }
    else if(!match_password.hasMatch())     // 密码格式错误
    {
        dialog.setInformation("密码不能为空或格式错误! !5-16位非空字符");
        dialog.setFixedSize(380, 136);
        dialog.exec();
    }
    else
    {
        if(m_ptrSqlDao->user_isExist(loginName))
        {
            if(m_ptrSqlDao->get_loginName_password(loginName) != password)  // 存在该用户，但密码不正确
            {
                qDebug() << m_ptrSqlDao->get_loginName_password(loginName);
                dialog.setInformation("密码错误，请提供有效的密码。");
                dialog.setFixedSize(287, 136);
                dialog.exec();
            }
            else
                emit sendLoginSuccess(loginName);        // 登录成功
        }
        else                                    // 用户不存在或已被禁用
        {
            dialog.setInformation("登录用户不存在，请提供有效的用户名信息。");
            dialog.setFixedSize(377, 136);
            dialog.exec();
        }
    }
#endif
}
