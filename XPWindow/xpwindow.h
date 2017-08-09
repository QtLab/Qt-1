#ifndef XPWINDOW_H
#define XPWINDOW_H

#include <QMainWindow>
#include "xpwindowtitle.h"

namespace Ui {
class XPWindow;
}

class XPWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit XPWindow(QWidget *parent = 0);
    ~XPWindow();

public slots:
    void titleButtonClick(XPWindowTitle::ButtonStatus status);

protected:
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
    Ui::XPWindow *ui;
    XPWindowTitle *m_pTitleWindow;
};

#endif // XPWINDOW_H
