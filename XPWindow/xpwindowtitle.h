#ifndef XPWINDOWTITLE_H
#define XPWINDOWTITLE_H

#include <QWidget>

class QHBoxLayout;
class QPushButton;

class StyleSheetWidget : public QWidget
{
public:
    StyleSheetWidget(QWidget* parent = nullptr) : QWidget(parent) {}

    virtual void paintEvent(QPaintEvent *) override;
};

class XPWindowTitle : public StyleSheetWidget
{
    Q_OBJECT
public:
    enum ButtonStatus
    {
        ButtonMin = 0,
        ButtonMax,
        ButtonRestore,
        ButtonClose
    };

    XPWindowTitle(QWidget *window);
    bool isCaption(int x, int y) const;

protected:
    virtual bool eventFilter(QObject* o, QEvent* e) override;
    void initButton();

signals:
    void buttonClick(XPWindowTitle::ButtonStatus status);

private:
    QWidget *m_pParentWindow;
    QHBoxLayout *m_pTitleLayout;
    QPushButton *m_pMinSizeBtn;
    QPushButton *m_pMaxSizeBtn;
    QPushButton *m_pRestoreBtn;
    QPushButton *m_pCloseBtn;
};

#endif // XPWINDOWTITLE_H
