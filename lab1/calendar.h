#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include <ui_calendar.h>
#include <QWidget>
#include <QVector>

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

        Ui::Calendar ui;
        QVector<Event> events;

    signals:
        void eventAdded(QDate date);
        void dateCleared(QDate date);
};

#endif
