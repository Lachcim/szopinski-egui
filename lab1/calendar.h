#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include <ui_calendar.h>
#include <QWidget>

class Calendar : public QMainWindow {
    Q_OBJECT

    public:
        explicit Calendar(QWidget *parent = nullptr);

    private:
        Ui::Calendar ui;
};

#endif
