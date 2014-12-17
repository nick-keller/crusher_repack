#include <QApplication>
#include "../widgets/mainwindow.h"
#include "../dialogs/brushselector.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Keller");
    QCoreApplication::setApplicationName("Crusher");

    MainWindow widget;
    widget.show();

    return app.exec();
}

