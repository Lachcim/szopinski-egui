#include <calendar.h>
#include <eventcalendar.h>
#include <QFile>
#include <QTextStream>

Calendar::Calendar(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("Calendar by Szopi\305\204ski");
    setMinimumSize(QSize(480, 360));
    resize(800, 600);

    EventCalendarWidget* mainCalendar = new EventCalendarWidget(this);
    mainCalendar->setFirstDayOfWeek(Qt::Monday);
    mainCalendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setCentralWidget(mainCalendar);

    QObject::connect(this, &Calendar::eventAdded, mainCalendar, &EventCalendarWidget::addEvent);
    QObject::connect(this, &Calendar::eventRemoved, mainCalendar, &EventCalendarWidget::removeEvent);
    QObject::connect(mainCalendar, &EventCalendarWidget::clicked, this, &Calendar::editEvent);
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
void Calendar::writeData() {
    QFile file("calendar.txt");
    if (!file.open(QIODevice::WriteOnly))
        return;

    QTextStream stream(&file);
    for (QVector<Event>::const_iterator it = events.cbegin(); it != events.cend(); ++it) {
        stream << it->date.toString("yyyy\nMM\ndd") << endl;
        stream << it->time << endl;
        stream << it->description << endl;
    }

    file.close();
}

void Calendar::editEvent(const QDate& date) {
    EventEditor editor(this, events, date);

    QObject::connect(&editor, &Calendar::EventEditor::dataChanged, this, &Calendar::writeData);

    editor.exec();
}
