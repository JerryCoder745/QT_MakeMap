#include "widget.h"
#include"TriangleItem.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;


    // 添加自定义图形项


    w.show();
    return a.exec();
}
