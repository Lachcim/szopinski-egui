#include <calendar.h>
#include <eventcalendar.h>
#include <QFile>
#include <QTextStream>

Calendar::Calendar(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("Calendar by Szopi\305\204ski");
    setMinimumSize(QSize(480, 360));
    resize(800, 600);

    mainCalendar = new EventCalendarWidget(this);
    mainCalendar->setFirstDayOfWeek(Qt::Monday);
    mainCalendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setCentralWidget(mainCalendar);

    QObject::connect(mainCalendar, &EventCalendarWidget::activated, this, &Calendar::editEvent);
}

void Calendar::readData() {
    QFile file("calendar.txt");
    if (!file.open(QIODevice::ReadOnly))
        return;

    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString date = stream.readLine();
        QString time = stream.readLine();
        QString description = stream.readLine();

        Event event;
        event.date = QDate::fromString(date, "yyyy-MM-dd");
        event.time = QTime::fromString(time, "HH:mm");
        event.description = description;

        events += event;
    }

    file.close();
    updateWidget();
}
void Calendar::writeData() {
    QFile file("calendar.txt");
    if (!file.open(QIODevice::WriteOnly))
        return;

    QTextStream stream(&file);
    for (QVector<Event>::const_iterator it = events.cbegin(); it != events.cend(); ++it) {
        stream << it->date.toString("yyyy-MM-dd") << endl;
        stream << it->time.toString("HH:mm") << endl;
        stream << it->description << endl;
    }

    file.close();
    updateWidget();
}

void Calendar::editEvent(const QDate& date) {
    EventEditor editor(this, events, date);

    QObject::connect(&editor, &Calendar::EventEditor::dataChanged, this, &Calendar::writeData);

    editor.exec();
}
void Calendar::updateWidget() {
    QSet<QDate> dateSet;

    for (QVector<Event>::const_iterator it = events.cbegin(); it != events.cend(); ++it)
        dateSet += it->date;

    mainCalendar->markEventDates(dateSet);
}
