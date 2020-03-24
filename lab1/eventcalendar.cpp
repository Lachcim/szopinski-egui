#include <eventcalendar.h>
#include <QPainter>

EventCalendarWidget::EventCalendarWidget(QWidget* parent) : QCalendarWidget(parent) {

}

void EventCalendarWidget::paintCell(QPainter* painter, const QRect& rect, const QDate& date) const {
    QCalendarWidget::paintCell(painter, rect, date);

    if (eventDates.contains(date))
        painter->fillRect(rect, QColor::fromRgb(128, 128, 255, 64));
}

void EventCalendarWidget::addEvent(const QDate& date) {
    eventDates += date;
}

void EventCalendarWidget::removeEvent(const QDate& date) {
    eventDates.removeOne(date);
}
