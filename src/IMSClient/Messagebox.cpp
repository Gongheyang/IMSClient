#include "MessageBox.h"
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QEvent>
#include <ui_Messagebox.h>


CMessageBox::CMessageBox(QWidget* parent)
    : QDialog(parent), ui(new Ui::CMessageBox)
    , _pOkButton(nullptr), _pCancelButton(nullptr), _pYesButton(nullptr)
    , _pNoButton(nullptr), _pContentWnd(nullptr)
{
    //设置标题栏
    setTitleWidget();
    setMainWidget();
    initChildrenWidget();
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowOpacity(0.9);
    _clickedBtn = QMessageBox::StandardButton::Cancel;
}

CMessageBox::~CMessageBox()
{
}
void CMessageBox::initChildrenWidget()
{
    _pOkButton = this->findChild<QPushButton*>("OkButton");
    connect(_pOkButton, SIGNAL(clicked()), this, SLOT(on_OkButton_clicked()));
    _pCancelButton = this->findChild<QPushButton*>("CancelButton");
    connect(_pCancelButton, SIGNAL(clicked()), this, SLOT(on_CancelButton_clicked()));
    _pYesButton = this->findChild<QPushButton*>("YesButton");
    connect(_pYesButton, SIGNAL(clicked()), this, SLOT(on_YesButton_clicked()));
    _pNoButton = this->findChild<QPushButton*>("NoButton");
    connect(_pNoButton, SIGNAL(clicked()), this, SLOT(on_NoButton_clicked()));
    _pContentWnd = this->findChild<QTextEdit*>("textEdit_MSGBOX");
    _pOkButton->hide();
    _pCancelButton->hide();
    _pYesButton->hide();
    _pNoButton->hide();
    _pContentWnd->setAlignment(Qt::AlignCenter);
}
void CMessageBox::on_OkButton_clicked()
{
    _clickedBtn = QMessageBox::StandardButton::Ok;
    QDialog::accept();
}

void CMessageBox::on_CancelButton_clicked()
{
    _clickedBtn = QMessageBox::StandardButton::Cancel;
    QDialog::reject();
}

void CMessageBox::on_YesButton_clicked()
{
    _clickedBtn = QMessageBox::StandardButton::Yes;
    QDialog::accept();
}

void CMessageBox::on_NoButton_clicked()
{
    _clickedBtn = QMessageBox::StandardButton::No;
    QDialog::reject();
}

void CMessageBox::closeWindow()
{
    _clickedBtn = QMessageBox::StandardButton::Close;
    QDialog::reject();
}

void CMessageBox::setTitleWidget()
{
    _titleWidget = new QWidget(this);
    _titleWidget->setObjectName("dialogTitleWidget");
    _titleText = new QLabel(this);
    _titleText->setObjectName("titleText");

    _closeBtn = new QPushButton(this);
    _closeBtn->setObjectName("closeDlgButton");

    //关联关闭按钮信号和槽
    connect(_closeBtn, SIGNAL(clicked()), this, SLOT(close()));

    //设置标题栏背景
    int nTitleWidgetH = 30;
    _titleWidget->setFixedHeight(nTitleWidgetH);

    //设置标题栏文本
    int nTitleTextH = 28;
    _titleText->setFixedHeight(nTitleTextH);

    //设置关闭按钮
    int nCloseBtnW = 28;
    int nCloseBtnH = 28;
    _closeBtn->setFixedSize(nCloseBtnW, nCloseBtnH);

    //标题栏添加布局
    QHBoxLayout* pHLayout = new QHBoxLayout();
    pHLayout->setContentsMargins(7, 0, 7, 0);
    pHLayout->setSpacing(0);
    _titleWidget->setLayout(pHLayout);

    pHLayout->addWidget(_titleText);
    pHLayout->addStretch();
    pHLayout->addWidget(_closeBtn);

    //对话框整体布局
    _vLayout = new QVBoxLayout();
    _vLayout->setContentsMargins(0, 0, 0, 0);
    _vLayout->setSpacing(0);

    //添加标题栏到整体布局
    _vLayout->addWidget(_titleWidget);

    //设置整体布局
    setLayout(_vLayout);
}

void CMessageBox::setMainWidget()
{
    QWidget* pMainWidget = new QWidget(this);
    pMainWidget->setObjectName("CMSGDialog");
    ui->setupUi(pMainWidget);
    _vLayout->addWidget(pMainWidget);
    pMainWidget->setMouseTracking(true);
}

QMessageBox::StandardButton CMessageBox::clickedButton2()
{
    return _clickedBtn;
}
CMessageBox::MessageBoxCode CMessageBox::clickedButton()
{
    switch (_clickedBtn) {
    case QMessageBox::StandardButton::Ok:
        return Ok;
    case QMessageBox::StandardButton::Cancel:
        return Cancel;
    case QMessageBox::StandardButton::Yes:
        return Yes;
    case QMessageBox::StandardButton::No:
        return No;
    default:
        break;
    }
    return No;

}
void CMessageBox::setButtonText(QMessageBox::StandardButton button, QString strText)
{
    switch (button) {
    case QMessageBox::StandardButton::Ok:
        _pOkButton->setText(strText);
        break;
    case QMessageBox::StandardButton::Cancel:
        _pCancelButton->setText(strText);
        break;
    case QMessageBox::StandardButton::Yes:
        _pYesButton->setText(strText);
        break;
    case QMessageBox::StandardButton::No:
        _pNoButton->setText(strText);
        break;
    default:
        break;
    }
}

void CMessageBox::showButton(QMessageBox::StandardButton button)
{
    switch (button) {
    case QMessageBox::StandardButton::Ok:
        _pOkButton->show();
        break;
    case QMessageBox::StandardButton::Cancel:
        _pCancelButton->show();
        break;
    case QMessageBox::StandardButton::Yes:
        _pYesButton->show();
        break;
    case QMessageBox::StandardButton::No:
        _pNoButton->show();
        break;
    default:
        break;
    }
}

void CMessageBox::setContent(QString strText)
{
    _pContentWnd->setText(strText);
}

void CMessageBox::setTitleText(QString text)
{
    _titleText->setText(tr("%1").arg(text));
}

QMessageBox::StandardButton CMessageBox::question(QWidget* parent, QString strTitle, QString strContent, QFlags<QMessageBox::StandardButton> button0, QFlags<QMessageBox::StandardButton> button1)
{
    CMessageBox dlg;
    dlg.resize(300, 200);
    dlg.setWindowTitle(strTitle);
    dlg.setTitleText(strTitle);
    dlg.setContent(strContent);
    dlg.showButton(QMessageBox::StandardButton::Yes);
    dlg.showButton(QMessageBox::StandardButton::No);
    dlg.exec();
    return dlg.clickedButton2();
}

QMessageBox::StandardButton CMessageBox::warning(QWidget* parent, QString strTitle, QString strContent, QFlags<QMessageBox::StandardButton> button0, QFlags<QMessageBox::StandardButton> button1)
{
    CMessageBox dlg;
    dlg.resize(300, 200);
    dlg.setWindowTitle(strTitle);
    dlg.setTitleText(strTitle);
    dlg.setContent(strContent);
    dlg.showButton(QMessageBox::StandardButton::Ok);
    dlg.exec();
    return dlg.clickedButton2();
}

int CMessageBox::information(QWidget* parent, QString strTitle, QString strContent)
{
    CMessageBox dlg;
    dlg.resize(300, 200);
    dlg.setWindowTitle(strTitle);
    dlg.setTitleText(strTitle);
    dlg.setContent(strContent);
    dlg.showButton(QMessageBox::StandardButton::Ok);
    dlg.exec();
    MessageBoxCode button = dlg.clickedButton();
    if (button == Cancel) {
        return Cancel;
    }
    return button;
}

void CMessageBox::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint pt = event->pos();
        QRect rect = _titleWidget->rect();
        if (rect.contains(pt)) {
            _pointLastMouse = event->pos();
            _mousePress = true;
        }
    }
}

void CMessageBox::mouseReleaseEvent(QMouseEvent* event)
{
    //设置鼠标为未被按下
    _mousePress = false;
}

void CMessageBox::mouseMoveEvent(QMouseEvent* event)
{
    if (false == _mousePress) {
        return QWidget::mouseMoveEvent(event);
    }

    if (_mousePress) {
        //如果鼠标不是放在边缘那么说明这是在拖动窗口
        move(event->globalPos() - _pointLastMouse);
    }
    event->ignore();
}





