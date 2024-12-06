#include "IMSClient.h"
#include <QApplication>
#include <QDir>
#include <QFile>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QString qssFile;
    QDir qDir = QDir(QApplication::applicationDirPath());
    qDir.cdUp();
    QString qstrWorkDir = qDir.absolutePath();//exe 的上级目录

    QString strIcon = qstrWorkDir + QString::fromLocal8Bit("/style/logo.ico");
    app.setWindowIcon(QIcon(strIcon));

    qssFile = qstrWorkDir + "/style/ims.qss";
    QFile file(qssFile);
    file.open(QFile::ReadOnly);
    QByteArray byteArr = file.readAll();
    file.close();
    QString qstrQss = QString::fromLocal8Bit(byteArr);
    app.setStyleSheet(qstrQss);

    IMSClient w;
    if (w.showLoginWidget()) {
        return app.exec();
    } else {
        return 0;
    }
}
