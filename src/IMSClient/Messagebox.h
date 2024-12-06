#ifndef __MESSAGEBOX_H__
#define __MESSAGEBOX_H__

#include <QMessageBox>
#include <QDialog>

class QPushButton;
class QTextEdit;
class QHBoxLayout;
class QVBoxLayout;

namespace Ui
{
class CMessageBox;
};

class CMessageBox : public QDialog
{
    Q_OBJECT

public:
    enum MessageBoxCode {
        No = 0,
        Yes = 1,
        Ok = 2,
        Cancel = 3,
    };
public:
    CMessageBox(QWidget* parent = 0);
    ~CMessageBox();

    MessageBoxCode clickedButton();
    QMessageBox::StandardButton clickedButton2();
    void setButtonText(QMessageBox::StandardButton button, QString strText);
    void showButton(QMessageBox::StandardButton button);
    void setContent(QString strText);
    /*!
    * @brief 设置标题栏文本
    */
    void setTitleText(QString text);
public:
    static QMessageBox::StandardButton question(QWidget* parent, QString strTitle, QString strContent, QFlags<QMessageBox::StandardButton> button0 = QMessageBox::StandardButton::Yes, QFlags<QMessageBox::StandardButton> button1 = QMessageBox::StandardButton::Yes);
    static QMessageBox::StandardButton warning(QWidget* parent, QString strTitle, QString strContent, QFlags<QMessageBox::StandardButton> button0 = QMessageBox::StandardButton::Ok, QFlags<QMessageBox::StandardButton> button1 = QMessageBox::StandardButton::Ok);
    static int information(QWidget* parent, QString strTitle, QString strContent);

public slots:
    void on_OkButton_clicked();
    void on_CancelButton_clicked();
    void on_YesButton_clicked();
    void on_NoButton_clicked();
protected:
    virtual void closeWindow();
    void setTitleWidget();
    void setMainWidget();
    /*!
    * @brief 鼠标按下事件响应
    */
    virtual void mousePressEvent(QMouseEvent* event);

    /*!
    * @brief 鼠标释放事件响应
    */
    virtual void mouseReleaseEvent(QMouseEvent* event);

    /*!
    * @brief 鼠标移动事件响应
    */
    virtual void mouseMoveEvent(QMouseEvent* event);
private:
    void initChildrenWidget();
private:
    QMessageBox::StandardButton _clickedBtn;
    QPushButton* _pOkButton;
    QPushButton* _pCancelButton;
    QPushButton* _pYesButton;
    QPushButton* _pNoButton;
    QTextEdit*   _pContentWnd;
    Ui::CMessageBox* ui;
    /// 对话框标题栏
    QWidget*                    _titleWidget;
    /// 对话框整体布局
    QVBoxLayout*                _vLayout;
    /// 标题栏图标
    QLabel*                     _titleIcon;
    /// 标题栏文本
    QLabel*                     _titleText;
    /// 关闭按钮
    QPushButton*                _closeBtn;

    /// 鼠标是否按下
    bool                                                _mousePress;
    ///窗口在鼠标按下一刻的位置
    QPoint                                              _pointLastMouse;
};

#endif // __MESSAGEBOX_H__
