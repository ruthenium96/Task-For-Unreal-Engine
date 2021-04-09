#include "DictionaryWidget.h"

#include <QApplication>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DictionaryWidget wgt;
    wgt.show();

    return app.exec();
}