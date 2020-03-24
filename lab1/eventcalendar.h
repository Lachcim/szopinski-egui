#ifndef EVENTCALENDAR_H
#define EVENTCALENDAR_H

#include <QCalendarWidget>
#include <QSet>

class EventCalendarWidget : public QCalendarWidget {
    Q_OBJECT

    public:
        EventCalendarWidget(QWidget* parent = nullptr);

    protected:
        void paintCell(QPainter* painter, const QRect& rect, const QDate& date) const;

    private:
        QSet<QDate> eventDates;

    public slots:
        void addEvent(const QDate&);
        void removeEvent(const QDate&);
};

#endif
