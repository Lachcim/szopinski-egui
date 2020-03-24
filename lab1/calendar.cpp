#include <calendar.h>
#include <QFile>
#include <QTextStream>

Calendar::Calendar(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    EventCalendarWidget* mainCalendar = findChild<EventCalendarWidget*>("mainCalendar");

    QObject::connect(this, &Calendar::eventAdded, mainCalendar, &EventCalendarWidget::addEvent);
    QObject::connect(this, &Calendar::eventRemoved, mainCalendar, &EventCalendarWidget::removeEvent);
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
        emit eventAdded(event.date);
    }

    file.close();
}
