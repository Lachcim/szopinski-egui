#ifndef EVENTCALENDAR_H
#define EVENTCALENDAR_H

#include <QCalendarWidget>

class EventCalendarWidget : public QCalendarWidget {
    Q_OBJECT

    public:
        EventCalendarWidget(QWidget* parent = nullptr);
};

#endif
