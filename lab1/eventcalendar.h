#ifndef EVENTCALENDAR_H
#define EVENTCALENDAR_H

#include <QCalendarWidget>
#include <QSet>

//custom calendar widget
//inherits from QCalendarWidget, provides minor graphical changes
//maintains a set of dates to be marked on the calendar
class EventCalendarWidget : public QCalendarWidget {
    Q_OBJECT

    public:
        EventCalendarWidget(QWidget* parent = nullptr) : QCalendarWidget(parent) {};

    protected:
        void paintCell(QPainter* painter, const QRect& rect, const QDate& date) const;

    private:
        QSet<QDate> eventDates;

    public slots:
        void markEventDates(const QSet<QDate>&);
};

#endif
