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

class Calendar : public QMainWindow {
    Q_OBJECT

    public:
        explicit Calendar(QWidget* parent = nullptr);

    private:
        struct Event {
            QDate date;
            QString time;
            QString description;
            bool deleted = false;
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

class Calendar::EventEditor : public QDialog {
    Q_OBJECT

    public:
        explicit EventEditor(QWidget* parent, QVector<Event>& events, const QDate& date);

    private:
        class EntryEditor;

        QTableWidget* table;
        QDate editorDate;

        QVector<Event>& events;
        QVector<Event> localEvents;
        QVector<int> localEventsIndices;

    signals:
        void dataChanged();

    public slots:
        void populateList();
        void addEvent();
        void editEvent(int index);
        void deleteEvent(int index);
        void saveChanges();
};

class Calendar::EventEditor::EntryEditor : public QDialog {
    Q_OBJECT

    public:
        explicit EntryEditor(QWidget* parent, Event& event, bool newEntry = false);

    private:
        bool eventIsNew;
        Event& editorEvent;

        Ui::EntryEditor ui;

    private slots:
        void saveEvent();
};

#endif
