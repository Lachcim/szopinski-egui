#include <calendar.h>
#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>

Calendar::EventEditor::EventEditor(QWidget* parent, QVector<Event>& parentEvents, const QDate& date) : QDialog(parent), events(parentEvents) {
    setWindowTitle(date.toString("yyyy-MM-dd"));
    setMinimumSize(QSize(480, 360));

    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList header;
    header << "Time" << "Description" << "Actions";
    table->verticalHeader()->hide();
    table->setHorizontalHeaderLabels(header);
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

    for (QVector<Event>::iterator it = events.begin(); it != events.end(); ++it)
        if (it->date == date)
            localEvents += LocalEvent(it);

    populateList();

    QObject::connect(addButton, &QPushButton::clicked, this, &Calendar::EventEditor::addEvent);
    QObject::connect(saveButton, &QPushButton::clicked, this, &Calendar::EventEditor::saveChanges);
}

void Calendar::EventEditor::populateList() {
    table->setRowCount(0);

    std::sort(localEvents.begin(), localEvents.end(), [](const LocalEvent& a, const LocalEvent& b) { return a.time < b.time; });

    for (QVector<LocalEvent>::iterator it = localEvents.begin(); it != localEvents.end(); ++it) {
        if (it->deleted)
            continue;

        table->insertRow(table->rowCount());
        table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem(it->time.toString("HH:mm")));
        table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(it->description));

        QWidget* buttonContainer = new QWidget();
        QHBoxLayout* buttonLayout = new QHBoxLayout(buttonContainer);
        buttonLayout->setContentsMargins(3, 0, 3, 0);

        QPushButton* editButton = new QPushButton(this);
        editButton->setText("Edit");
        QPushButton* deleteButton = new QPushButton(this);
        deleteButton->setText("Delete");

        QObject::connect(editButton, &QPushButton::clicked, [=] { editEvent(*it); });
        QObject::connect(deleteButton, &QPushButton::clicked, [=] { deleteEvent(*it); });

        buttonLayout->addWidget(editButton);
        buttonLayout->addWidget(deleteButton);
        buttonContainer->setLayout(buttonLayout);

        table->setCellWidget(table->rowCount() - 1, 2, buttonContainer);
    }
}

void Calendar::EventEditor::saveChanges() {
    for (QVector<LocalEvent>::const_iterator it = localEvents.cbegin(); it != localEvents.cend(); ++it)
        if (it->isNew) {
            if (!it->deleted)
                events += *it;
        }
        else {
            if (!it->deleted) {
                it->origin->time = it->time;
                it->origin->description = it->description;
            }
            else
                events.erase(it->origin);
        }

    emit dataChanged();
    close();
}

void Calendar::EventEditor::editEvent(LocalEvent& event) {
    EntryEditor editor(this, event);
    editor.exec();
    populateList();
}
void Calendar::EventEditor::deleteEvent(LocalEvent& event) {
    event.deleted = true;
    populateList();
}

void Calendar::EventEditor::addEvent() {
    LocalEvent event;
    event.date = editorDate;

    EntryEditor editor(this, event, true);
    editor.exec();

    if (!event.deleted) {
        localEvents += event;
        populateList();
    }
}
