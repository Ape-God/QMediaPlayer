/*

��������  �� win10/win11 vs2022-Qt5.15.2
VS Build :  x64


ԭ���ߣ��������
*/


#include "QQMediaPlayerCopy.h"
#include <QtWidgets/QApplication>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/resources/logo.png"));
    //qRegisterMetaType<std::string>("std::string");

    QQMediaPlayerCopy w;
    w.show();
    return a.exec();
}
