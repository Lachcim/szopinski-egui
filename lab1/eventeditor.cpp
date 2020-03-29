#include <calendar.h>
#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>

//constructor of event editor
Calendar::EventEditor::EventEditor(QWidget* parent, QVector<Event>& parentEvents, const QDate& date) : QDialog(parent), events(parentEvents) {
    //style window
    setWindowTitle(date.toString("yyyy-MM-dd"));
    setMinimumSize(QSize(480, 360));

    //create event table
    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //create table header
    QStringList header;
    header << "Time" << "Description" << "Actions";
    table->verticalHeader()->hide();
    table->setHorizontalHeaderLabels(header);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    //create add and save buttons
    QPushButton* addButton = new QPushButton(this);
    addButton->setText("Add");
    QPushButton* saveButton = new QPushButton(this);
    saveButton->setText("Save");

    //create layout for buttons and add them
    QHBoxLayout* subLayout = new QHBoxLayout();
    subLayout->addWidget(addButton);
    subLayout->addWidget(saveButton);

    //create main layout and add table with buttons
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(table);
    mainLayout->addLayout(subLayout);
    setLayout(mainLayout);

    //set private members
    editorDate = date;
    events = parentEvents;

    //generate local events list - filter global events by date
    for (QVector<Event>::iterator it = events.begin(); it != events.end(); ++it)
        if (it->date == date)
            localEvents += LocalEvent(it);

    //add local events to table
    populateList();

    //connect buttons to actions
    QObject::connect(addButton, &QPushButton::clicked, this, &Calendar::EventEditor::addEvent);
    QObject::connect(saveButton, &QPushButton::clicked, this, &Calendar::EventEditor::saveChanges);
}

//add rows with widgets to table
void Calendar::EventEditor::populateList() {
    //remove all rows
    table->setRowCount(0);

    //sort local events by time
    std::sort(localEvents.begin(), localEvents.end(), [](const LocalEvent& a, const LocalEvent& b) { return a.time < b.time; });

    //iterate over local event list
    for (QVector<LocalEvent>::iterator it = localEvents.begin(); it != localEvents.end(); ++it) {
        //if the event was marked for deletion, skip it
        if (it->deleted)
            continue;

        //insert row with time and description
        table->insertRow(table->rowCount());
        table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem(it->time.toString("HH:mm")));
        table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(it->description));

        //create button container with layout
        QWidget* buttonContainer = new QWidget();
        QHBoxLayout* buttonLayout = new QHBoxLayout(buttonContainer);
        buttonLayout->setContentsMargins(3, 0, 3, 0);

        //create buttons
        QPushButton* editButton = new QPushButton(this);
        editButton->setText("Edit");
        QPushButton* deleteButton = new QPushButton(this);
        deleteButton->setText("Delete");

        //connect buttons to actions
        QObject::connect(editButton, &QPushButton::clicked, [=] { editEvent(*it); });
        QObject::connect(deleteButton, &QPushButton::clicked, [=] { deleteEvent(*it); });

        //add buttons to layout and set button container layout
        buttonLayout->addWidget(editButton);
        buttonLayout->addWidget(deleteButton);
        buttonContainer->setLayout(buttonLayout);

        //add buttons to row
        table->setCellWidget(table->rowCount() - 1, 2, buttonContainer);
    }
}

//synchronize local events with global events
void Calendar::EventEditor::saveChanges() {
    //sort local events by index in global list to prevent iterator invalidation when erasing
    std::sort(localEvents.begin(), localEvents.end(), [&](const LocalEvent& a, const LocalEvent& b) {
        return b.origin - events.begin() < a.origin - events.begin();
    });

    //iterate over local events
    for (QVector<LocalEvent>::const_iterator it = localEvents.cbegin(); it != localEvents.cend(); ++it)
        if (it->isNew) {
            //if an event is new and not deleted, add it to global events
            if (!it->deleted)
                events += *it;
        }
        else {
            if (!it->deleted) {
                //if an element is pre-existing and not deleted, update time and description
                it->origin->time = it->time;
                it->origin->description = it->description;
            }
            else {
                //if an element has been deleted, remove it from global events
                events.erase(it->origin);
            }
        }

    //notify main widget of data changes and close dialog
    emit dataChanged();
    close();
}

//open entry editor to modify time and description, repopulate table
void Calendar::EventEditor::editEvent(LocalEvent& event) {
    EntryEditor editor(this, event);
    editor.exec();
    populateList();
}
//mark event as deleted, repopulate table
void Calendar::EventEditor::deleteEvent(LocalEvent& event) {
    event.deleted = true;
    populateList();
}
//open entry editor to add new event
void Calendar::EventEditor::addEvent() {
    //create unlinked local event with the relevant date
    LocalEvent event;
    event.date = editorDate;

    //pass event to entry editor in adding mode
    EntryEditor editor(this, event, true);
    editor.exec();

    //if the event was saved, add it to list and repopulate table
    if (!event.deleted) {
        localEvents += event;
        populateList();
    }
}
