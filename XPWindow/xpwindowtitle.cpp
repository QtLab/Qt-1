#include <cassert>
#include <QEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QStyleOption>
#include "xpwindowtitle.h"

XPWindowTitle::XPWindowTitle(QWidget *window)
    : StyleSheetWidget(window)
    , m_pParentWindow(window)
    , m_pTitleLayout(NULL)
    , m_pMinSizeBtn(NULL)
    , m_pMaxSizeBtn(NULL)
    , m_pRestoreBtn(NULL)
    , m_pCloseBtn(NULL)
{
    m_pTitleLayout = new QHBoxLayout(this);
    m_pTitleLayout->setMargin(0);
    m_pTitleLayout->setSpacing(0);
    m_pTitleLayout->setContentsMargins(0, 0, 0, 0);
    m_pTitleLayout->addStretch(1);
    this->setLayout(m_pTitleLayout);
    this->setFixedHeight(32);

    this->setStyleSheet("background-color: rgb(80, 180, 250);");

    m_pParentWindow->installEventFilter(this);

    this->initButton();
}

bool XPWindowTitle::isCaption(int x, int y) const
{
    if (this->rect().contains(x, y))
    {
        if (!this->childAt(x, y))
        {
            return true;
        }
    }
    return false;
}

bool XPWindowTitle::eventFilter(QObject* o, QEvent* e)
{
    if (e->type() == QEvent::WindowStateChange)
    {
        Qt::WindowStates states = m_pParentWindow->windowState();
        if (states & Qt::WindowMaximized)
        {
            m_pMaxSizeBtn->hide();
            m_pRestoreBtn->show();
        }
        else
        {
            m_pMaxSizeBtn->show();
            m_pRestoreBtn->hide();
        }
        return true;
    }

    return QWidget::eventFilter(o, e);
}

void XPWindowTitle::initButton()
{
    assert(!(m_pMinSizeBtn || m_pMaxSizeBtn || m_pRestoreBtn || m_pCloseBtn));

    m_pMinSizeBtn = new QPushButton("", this);
    m_pMinSizeBtn->setObjectName("minSizeBtn");
    m_pMinSizeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    connect(m_pMinSizeBtn, &QPushButton::clicked, [this]() {emit buttonClick(ButtonMin);});
    m_pTitleLayout->addWidget(m_pMinSizeBtn);

    m_pMaxSizeBtn = new QPushButton("", this);
    m_pMaxSizeBtn->setObjectName("maxSizeBtn");
    m_pMaxSizeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    connect(m_pMaxSizeBtn, &QPushButton::clicked, [this](bool) {emit buttonClick(ButtonMax);});
    m_pTitleLayout->addWidget(m_pMaxSizeBtn);

    m_pRestoreBtn = new QPushButton("", this);
    m_pRestoreBtn->setObjectName("restoreBtn");
    m_pRestoreBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_pRestoreBtn->hide();
    connect(m_pRestoreBtn, &QPushButton::clicked, [this]() {emit buttonClick(ButtonRestore);});
    m_pTitleLayout->addWidget(m_pRestoreBtn);

    m_pCloseBtn = new QPushButton("", this);
    m_pCloseBtn->setObjectName("closeBtn");
    m_pCloseBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    connect(m_pCloseBtn, &QPushButton::clicked, [this](){emit buttonClick(ButtonClose);});
    m_pTitleLayout->addWidget(m_pCloseBtn);
}

void StyleSheetWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QStyleOption opt;
    opt.init(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
