#include <calendar.h>
#include <eventcalendar.h>
#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    //instantiate and show main widget
    Calendar calendar;
    calendar.readData();
    calendar.show();

    return app.exec();
}
