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
    * @brief ���ñ������ı�
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
    * @brief ��갴���¼���Ӧ
    */
    virtual void mousePressEvent(QMouseEvent* event);

    /*!
    * @brief ����ͷ��¼���Ӧ
    */
    virtual void mouseReleaseEvent(QMouseEvent* event);

    /*!
    * @brief ����ƶ��¼���Ӧ
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
    /// �Ի��������
    QWidget*                    _titleWidget;
    /// �Ի������岼��
    QVBoxLayout*                _vLayout;
    /// ������ͼ��
    QLabel*                     _titleIcon;
    /// �������ı�
    QLabel*                     _titleText;
    /// �رհ�ť
    QPushButton*                _closeBtn;

    /// ����Ƿ���
    bool                                                _mousePress;
    ///��������갴��һ�̵�λ��
    QPoint                                              _pointLastMouse;
};

#endif // __MESSAGEBOX_H__
