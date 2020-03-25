#include <eventcalendar.h>
#include <QPainter>

//paintCell override
//paint the cell as the superclass would and give it a blue tint if it's marked
void EventCalendarWidget::paintCell(QPainter* painter, const QRect& rect, const QDate& date) const {
    QCalendarWidget::paintCell(painter, rect, date);

    if (eventDates.contains(date))
        painter->fillRect(rect, QColor::fromRgb(128, 128, 255, 64));
}

//set the set of marked dates
void EventCalendarWidget::markEventDates(const QSet<QDate>& dates) {
    eventDates = dates;
}
