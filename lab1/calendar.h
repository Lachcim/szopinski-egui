#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include <eventcalendar.h>
#include <ui_entryeditor.h>
#include <QWidget>
#include <QVector>
#include <QDialog>
#include <QDate>
#include <QMainWindow>
#include <QTableWidget>

//main application widget
//responible for displaying the calendar, storing the data and performing IO operations
//delegates high-level data operations to its private widgets
class Calendar : public QMainWindow {
    Q_OBJECT

    public:
        explicit Calendar(QWidget* parent = nullptr);

    private:
        struct Event {
            QDate date;
            QTime time;
            QString description;
        };
        class EventEditor;

        QVector<Event> events;

        EventCalendarWidget* mainCalendar;

    public slots:
        void readData();
        void writeData();
        void editEvent(const QDate& date);

    private slots:
        void updateWidget();
};

//event editor dialog
//provides data editing functionality for an individual day
//maintains a local event list which is synchronized with the main list when the save button is clicked
//delegates text and time entry to private widget
class Calendar::EventEditor : public QDialog {
    Q_OBJECT

    public:
        explicit EventEditor(QWidget* parent, QVector<Event>& events, const QDate& date);

    private:
        //wrapper structure for external events in the current context
        //inherits from Event to provide extended functionality
        //linked to its global counterpart via the origin field
        //unlinked (new) elements have no origin and are marked as such
        //deleted linked elements are removed from the top list
        //deleted unlinked elements are ignored
        struct LocalEvent : Event {
            LocalEvent() : isNew(true) {};
            LocalEvent(QVector<Event>::iterator iter) : Event(*iter), origin(iter) {};

            QVector<Event>::iterator origin;
            bool isNew = false;
            bool deleted = false;
        };
        class EntryEditor;

        QTableWidget* table;
        QDate editorDate;

        QVector<Event>& events;
        QVector<LocalEvent> localEvents;

    signals:
        void dataChanged();

    public slots:
        void populateList();
        void addEvent();
        void editEvent(LocalEvent& event);
        void deleteEvent(LocalEvent& event);
        void saveChanges();
};

//text and time entry dialog
//changes the specified event when saved
//deletes events when cancelled while adding
class Calendar::EventEditor::EntryEditor : public QDialog {
    Q_OBJECT

    public:
        explicit EntryEditor(QWidget* parent, LocalEvent& event, bool adding = false);

    private:
        LocalEvent& editorEvent;
        bool addingEvent;

        virtual void closeEvent(QCloseEvent* e);

        Ui::EntryEditor ui;

    private slots:
        void saveEvent();
        void discardEvent();
};

#endif
