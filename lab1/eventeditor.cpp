#include <calendar.h>
#include <QTableWidget>
#include <QPushButton>

Calendar::EventEditor::EventEditor(QWidget* parent, QVector<Event>& parentEvents, const QDate& date) : QDialog(parent), events(parentEvents)
{
    setWindowTitle(date.toString("yyyy-MM-dd"));
    setMinimumSize(QSize(480, 360));

    QTableWidget* table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setRowCount(0);

    QStringList header;
    header << "Time" << "Description" << "Actions";
    table->setHorizontalHeaderLabels(header);
    table->verticalHeader()->hide();
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    QPushButton* addButton = new QPushButton(this);
    addButton->setText("Add");
    QPushButton* saveButton = new QPushButton(this);
    saveButton->setText("Save");

    QHBoxLayout* subLayout = new QHBoxLayout();
    subLayout->addWidget(addButton);
    subLayout->addWidget(saveButton);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(table);
    mainLayout->addLayout(subLayout);
    setLayout(mainLayout);
}
