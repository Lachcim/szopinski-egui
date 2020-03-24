#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include <ui_eventeditor.h>
#include <QWidget>
#include <QVector>
#include <QDialog>
#include <QDate>
#include <QMainWindow>

class Calendar : public QMainWindow {
    Q_OBJECT

    public:
        explicit Calendar(QWidget* parent = nullptr);
        void readData();

    private:
        struct Event {
            QDate date;
            QString time;
            QString description;
        };
        class EventEditor;

        QVector<Event> events;

    public slots:
        void editEvent(const QDate& date);

    signals:
        void eventAdded(QDate date);
        void eventRemoved(QDate date);
};

class Calendar::EventEditor : public QDialog {
    Q_OBJECT

    public:
        explicit EventEditor(QWidget* parent, QVector<Event>& events, const QDate& date);

    private:
        QTableWidget* table;

        QVector<Event>& events;
        QVector<Event> localEvents;
        QVector<int> localEventsIndices;

    public slots:
        void populateList();
        void saveChanges();
};

#endif
