#include <QCoreApplication>
#include "servak.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Servak server("0.0.0.0", 5901);
    return a.exec();
}
