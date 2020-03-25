#include <calendar.h>
#include <QTableWidget>
#include <QPushButton>

Calendar::EventEditor::EventEditor(QWidget* parent, QVector<Event>& parentEvents, const QDate& date) : QDialog(parent), events(parentEvents)
{
    setWindowTitle(date.toString("yyyy-MM-dd"));
    setMinimumSize(QSize(480, 360));

    table = new QTableWidget(this);
    table->setColumnCount(3);

    QStringList header;
    header << "Time" << "Description" << "Actions";
    table->setHorizontalHeaderLabels(header);
    table->verticalHeader()->hide();
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
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

    editorDate = date;
    events = parentEvents;

    QVector<Event>::const_iterator it = events.cbegin();
    int i = 0;
    while (it != events.cend()) {
        if (it->date == date) {
            localEvents += *it;
            localEventsIndices += i;
        }

        ++it;
        i++;
    }

    populateList();

    QObject::connect(addButton, &QPushButton::clicked, this, &Calendar::EventEditor::addEvent);
    QObject::connect(saveButton, &QPushButton::clicked, this, &Calendar::EventEditor::saveChanges);
}

void Calendar::EventEditor::populateList() {
    table->setRowCount(0);

    for (QVector<Event>::const_iterator it = localEvents.cbegin(); it != localEvents.cend(); ++it) {
        if (it->deleted)
            continue;

        table->insertRow(table->rowCount());
        table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem(it->time));
        table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(it->description));

        QWidget* buttonContainer = new QWidget();
        QHBoxLayout* buttonLayout = new QHBoxLayout(buttonContainer);
        buttonLayout->setContentsMargins(3, 0, 3, 0);

        QPushButton* editButton = new QPushButton(this);
        editButton->setText("Edit");
        QPushButton* deleteButton = new QPushButton(this);
        deleteButton->setText("Delete");

        QObject::connect(deleteButton, &QPushButton::clicked, [=] { deleteEvent(it - localEvents.cbegin()); });

        buttonLayout->addWidget(editButton);
        buttonLayout->addWidget(deleteButton);
        buttonContainer->setLayout(buttonLayout);

        table->setCellWidget(table->rowCount() - 1, 2, buttonContainer);
    }
}

void Calendar::EventEditor::saveChanges() {
    QVector<Event>::const_reverse_iterator evIt = localEvents.crbegin();
    QVector<int>::const_reverse_iterator inIt = localEventsIndices.crbegin();

    while (evIt != localEvents.crend()) {
        if (*inIt == -1) {
            if (!evIt->deleted)
                events += *evIt;
        }
        else {
            if (!evIt->deleted) {
                events[*inIt].time = evIt->time;
                events[*inIt].description = evIt->description;
            }
            else
                events.removeAt(*inIt);
        }

        ++evIt;
        ++inIt;
    }

    close();
}

void Calendar::EventEditor::deleteEvent(int index) {
    localEvents[index].deleted = true;
    populateList();
}

void Calendar::EventEditor::addEvent() {
    Event event;
    event.date = editorDate;
    event.time = "";
    event.description = "";

    localEvents += event;
    localEventsIndices += -1;

    populateList();
}
