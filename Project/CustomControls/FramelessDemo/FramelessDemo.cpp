#include "FramelessDemo.h"

#ifdef Q_OS_WIN  
#include <Dwmapi.h>  // Use system shadow frame
#pragma comment(lib, "Dwmapi.lib")
#endif

FramelessDemo::FramelessDemo(QWidget *parent)
    : FramelessWindow(parent)
{
#ifdef Q_OS_WIN 
	BOOL bEnable = false;
	::DwmIsCompositionEnabled(&bEnable);
	if (bEnable)
	{
		DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
		::DwmSetWindowAttribute((HWND)winId(), DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
		MARGINS margins = { -1 };
		::DwmExtendFrameIntoClientArea((HWND)winId(), &margins);
	}
#endif

    initForm();

    connect(BtnMin, &QPushButton::clicked, this, [this]() {
        this->setWindowState(windowState() & ~Qt::WindowActive | Qt::WindowMinimized);
    });

    connect(BtnMax, &QPushButton::clicked, this, [this]() {
        this->setWindowState(this->windowState().testFlag(Qt::WindowNoState) ? Qt::WindowMaximized : Qt::WindowNoState);
    });

    connect(BtnClose, &QPushButton::clicked, this, [this]() {
        this->close();
    });

    setTitleBarWidgets({ TitleBar, LblIcon, LblTitle });
}

FramelessDemo::~FramelessDemo()
{}

// 对主体界面进行初始化
void FramelessDemo::initForm()
{
    // 设置窗口的大小
    this->setMinimumWidth(1000);
    this->setMinimumHeight(618);

    // 设置窗口整体布局为垂直布局
    QVBoxLayout* VLayout = new QVBoxLayout(this);
    VLayout->setSpacing(0);
    VLayout->setMargin(0);

    // 嵌套一层QWidget，因为最底层的窗口要设置为窗口透明
    FrameWidget = new QWidget(this);
    FrameWidget->setObjectName("FrameWidget");
    FrameWidget->setStyleSheet(R"(
        #FrameWidget {
            background-color: #f0f0f0;
        }
    )");
    VLayout->addWidget(FrameWidget);
    QVBoxLayout* frameVLayout = new QVBoxLayout(FrameWidget);
    frameVLayout->setMargin(0);
    frameVLayout->setSpacing(0);

    initTitleBar();
    MainWidget = new QWidget(FrameWidget);

    frameVLayout->addWidget(TitleBar);
    frameVLayout->addWidget(MainWidget);
}

void FramelessDemo::initTitleBar()
{
	// 设置标题栏和主体窗口
    TitleBar = new QWidget(FrameWidget);
    TitleBar->setFixedHeight(38);
	TitleBar->setObjectName("TitleBar");
	TitleBar->setStyleSheet(R"(
        #TitleBar {
            background-color: rgb(255, 255, 255);
        }
    )");
	QHBoxLayout* TitleBarLayout = new QHBoxLayout(TitleBar);
    TitleBarLayout->setAlignment(Qt::AlignVCenter);
	TitleBarLayout->setSpacing(0);
	TitleBarLayout->setContentsMargins(5, 1, 1, 0);

    // 添加图标
    LblIcon = new QLabel(TitleBar);
    LblIcon->setFixedSize(28, 28);
    LblIcon->setStyleSheet(R"(
        border-image:url(:/resources/icon.svg);
        border: none;
    )");
    TitleBarLayout->addWidget(LblIcon);

    TitleBarLayout->addSpacing(10);
    LblTitle = new QLabel(TitleBar);
    LblTitle->setText(u8"无边框窗口");
    LblTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    LblTitle->setStyleSheet(R"(
        background-color: white
        color: black;
        border: none;
    )");
    TitleBarLayout->addWidget(LblTitle);

    // 加载图标字体
	int fontId = QFontDatabase::addApplicationFont(":/resources/iconfont_titlebar.ttf");
	QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
	QFont font(fontFamilies.at(0));

    BtnMin = new QPushButton(TitleBar);
    BtnMin->setObjectName("BtnMin");
    BtnMin->setFont(font);
    BtnMin->setText(QChar(0xe7bf));
    BtnMin->setStyleSheet(R"(
        #BtnMin {
            background-color: white;
            color: black;
            border: none;
        }
        #BtnMin:hover{
            background-color: #E5E5E5;
        }
        #BtnMin:pressed{
            background-color: #CACACB;
        }
    )");
    BtnMin->setFixedSize(60, 37);
    TitleBarLayout->addWidget(BtnMin);

    BtnMax = new QPushButton(TitleBar);
    BtnMax->setObjectName("BtnMax");
    BtnMax->setFont(font);
    BtnMax->setText(QChar(0xe7c1));
	BtnMax->setStyleSheet(R"(
        #BtnMax {
            background-color: white;
            color: black;
            border: none;
        }
        #BtnMax:hover{
            background-color: #E5E5E5;
        }
        #BtnMax:pressed{
            background-color: #CACACB;
        }
    )");
    BtnMax->setFixedSize(59, 37);
    TitleBarLayout->addWidget(BtnMax);

    BtnClose = new QPushButton(TitleBar);
    BtnClose->setObjectName("BtnClose");
    BtnClose->setFont(font);
    BtnClose->setText(QChar(0xe7b4));
	BtnClose->setStyleSheet(R"(
        #BtnClose {
            background-color: white;
            color: black;
            border: none;
        }
        #BtnClose:hover{
            background-color: #E81123;
            color: white;
        }
        #BtnClose:pressed{
            background-color: #F1707A;
            color: white;
        }
    )");
    BtnClose->setFixedSize(60, 37);
    TitleBarLayout->addWidget(BtnClose);
}

void FramelessDemo::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange)
    {
        if (this->windowState().testFlag(Qt::WindowMaximized))
        {
            BtnMax->setText(QChar(0xe7c0));
        }
        else if (this->windowState().testFlag(Qt::WindowNoState))
        {
            BtnMax->setText(QChar(0xe7c1));
        }
    }
}
