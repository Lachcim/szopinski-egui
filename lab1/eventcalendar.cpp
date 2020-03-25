#include <eventcalendar.h>
#include <QPainter>

void EventCalendarWidget::paintCell(QPainter* painter, const QRect& rect, const QDate& date) const {
    QCalendarWidget::paintCell(painter, rect, date);

    if (eventDates.contains(date))
        painter->fillRect(rect, QColor::fromRgb(128, 128, 255, 64));
}

void EventCalendarWidget::markEventDates(const QSet<QDate>& dates) {
    eventDates = dates;
}
