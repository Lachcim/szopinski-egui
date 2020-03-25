#include <calendar.h>
#include <eventcalendar.h>
#include <QFile>
#include <QTextStream>

//constructor of calendar widget
Calendar::Calendar(QWidget* parent) : QMainWindow(parent)
{
    //style window
    setWindowTitle("Calendar by Szopi\305\204ski");
    setMinimumSize(QSize(480, 360));
    resize(800, 600);

    //add custom calendar widget
    mainCalendar = new EventCalendarWidget(this);
    mainCalendar->setFirstDayOfWeek(Qt::Monday);
    mainCalendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setCentralWidget(mainCalendar);

    //show event editor when a date is clicked
    QObject::connect(mainCalendar, &EventCalendarWidget::activated, this, &Calendar::editEvent);
}

//IO - read data from storage file
void Calendar::readData() {
    //open file, return on error
    QFile file("calendar.txt");
    if (!file.open(QIODevice::ReadOnly))
        return;

    //read entries until end is reached
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        //read data
        QString date = stream.readLine();
        QString time = stream.readLine();
        QString description = stream.readLine();

        //construct event
        Event event;
        event.date = QDate::fromString(date, "yyyy-MM-dd");
        event.time = QTime::fromString(time, "HH:mm");
        event.description = description;

        //add event to list
        events += event;
    }

    //close file and mark event dates
    file.close();
    updateWidget();
}
//IO - write data to storage file
void Calendar::writeData() {
    //open and overwrite file, return on error
    QFile file("calendar.txt");
    if (!file.open(QIODevice::WriteOnly))
        return;

    //write entries for each event
    QTextStream stream(&file);
    for (QVector<Event>::const_iterator it = events.cbegin(); it != events.cend(); ++it) {
        stream << it->date.toString("yyyy-MM-dd") << endl;
        stream << it->time.toString("HH:mm") << endl;
        stream << it->description << endl;
    }

    //close file
    file.close();
}

//launch editor to edit individual day
void Calendar::editEvent(const QDate& date) {
    EventEditor editor(this, events, date);

    //if the editor introduced data changes, write events to file and update calendar widget
    QObject::connect(&editor, &Calendar::EventEditor::dataChanged, this, &Calendar::writeData);
    QObject::connect(&editor, &Calendar::EventEditor::dataChanged, this, &Calendar::updateWidget);

    editor.exec();
}
//select days to be marked on the calendar
void Calendar::updateWidget() {
    //constuct a set of unique event dates
    QSet<QDate> dateSet;

    for (QVector<Event>::const_iterator it = events.cbegin(); it != events.cend(); ++it)
        dateSet += it->date;

    //pass set to widget
    mainCalendar->markEventDates(dateSet);
}
