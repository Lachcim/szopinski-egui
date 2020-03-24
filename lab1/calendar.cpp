#include <calendar.h>
#include <QFile>
#include <QTextStream>

Calendar::Calendar(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);
}

void Calendar::readData() {
    QFile file("calendar.txt");
    if (!file.open(QIODevice::ReadOnly))
        return;

    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString year = stream.readLine();
        QString month = stream.readLine();
        QString day = stream.readLine();
        QString time = stream.readLine();
        QString description = stream.readLine();

        Event event;
        event.date = QDate::fromString(year + "-" + month + "-" + day, "yyyy-MM-dd");
        event.time = time;
        event.description = description;

        events += event;
        findChild<EventCalendarWidget*>("mainCalendar")->addEvent(event.date);
    }

    file.close();
}
