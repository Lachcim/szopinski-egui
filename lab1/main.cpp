#include <calendar.h>
#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Calendar calendar;
    calendar.show();

    return app.exec();
}
