#include <calendar.h>

Calendar::EventEditor::EntryEditor::EntryEditor(QWidget* parent, LocalEvent& event, bool newEntry) : QDialog(parent), editorEvent(event) {
    ui.setupUi(this);
    if (newEntry)
        setWindowTitle("New entry");

    editorEvent = event;
    eventIsNew = newEntry;

    findChild<QTimeEdit*>("timeEdit")->setTime(event.time);
    findChild<QLineEdit*>("descriptionEdit")->setText(event.description);

    QDialogButtonBox* buttonBox = findChild<QDialogButtonBox*>("buttonBox");
    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &Calendar::EventEditor::EntryEditor::saveEvent);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &Calendar::EventEditor::EntryEditor::close);
}

void Calendar::EventEditor::EntryEditor::saveEvent() {
    editorEvent.time = findChild<QTimeEdit*>("timeEdit")->time();
    editorEvent.description = findChild<QLineEdit*>("descriptionEdit")->text();
    editorEvent.deleted = false;

    close();
}
