#include <cassert>
#include <QVBoxLayout>
#include <QPainter>
#include <QStyleOption>
#include <windowsx.h>
#include <windows.h>
#include "xpwindow.h"
#include "ui_xpwindow.h"

XPWindow::XPWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::XPWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    //this->setAttribute(Qt::WA_TranslucentBackground, true);

    HWND hwnd = (HWND)this->winId();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);

    StyleSheetWidget* center = new StyleSheetWidget(this);
    this->setCentralWidget(center);

    QVBoxLayout* layout = new QVBoxLayout(center);
    layout->setMargin(0);
    layout->setSpacing(0);
    center->setLayout(layout);

    m_pTitleWindow = new XPWindowTitle(this);

    connect(m_pTitleWindow, SIGNAL(buttonClick(XPWindowTitle::ButtonStatus)), this, SLOT(titleButtonClick(XPWindowTitle::ButtonStatus)));
    layout->addWidget(m_pTitleWindow);

    QWidget* widget = new StyleSheetWidget(this);
    widget->setAutoFillBackground(true);
    QPalette palette = widget->palette();
    palette.setColor(QPalette::Background, Qt::white);
    widget->setPalette(palette);
    layout->addWidget(widget, 1);
}

XPWindow::~XPWindow()
{
    delete ui;
}
void XPWindow::titleButtonClick(XPWindowTitle::ButtonStatus status)
{
    switch (status)
    {
    case XPWindowTitle::ButtonMin:
        this->showMinimized();
        break;

    case XPWindowTitle::ButtonMax:
        this->showMaximized();
        break;

    case XPWindowTitle::ButtonRestore:
        this->showNormal();
        break;

    case XPWindowTitle::ButtonClose:
        this->close();
        break;

    default:
        assert(false);
        break;
    }
}

bool XPWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = (MSG*)message;
    switch (msg->message)
    {
        case WM_NCHITTEST:
        {
            int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
            int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
            if (m_pTitleWindow->isCaption(xPos, yPos))
            {
                *result = HTCAPTION;
                return true;
            }
        }
        break;
        case WM_NCCALCSIZE:
            return true;

        case WM_GETMINMAXINFO:
        {
            if (::IsZoomed(msg->hwnd))
            {
                RECT frame = { 0, 0, 0, 0 };
                AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
                frame.left = abs(frame.left);
                frame.top = abs(frame.bottom);
                this->setContentsMargins(frame.left, frame.top, frame.right, frame.bottom);
            }
            else
            {
                this->setContentsMargins(0, 0, 0, 0);
            }

            *result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
            return true;
        }
        break;
    }

    return QMainWindow::nativeEvent(eventType, message, result);
}
