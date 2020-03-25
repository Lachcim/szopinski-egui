#include <calendar.h>

Calendar::EventEditor::EntryEditor::EntryEditor(QWidget* parent, LocalEvent& event) : QDialog(parent), editorEvent(event) {
    ui.setupUi(this);
    if (event.isNew)
        setWindowTitle("New entry");

    editorEvent = event;

    findChild<QTimeEdit*>("timeEdit")->setTime(event.time);
    findChild<QLineEdit*>("descriptionEdit")->setText(event.description);

    QDialogButtonBox* buttonBox = findChild<QDialogButtonBox*>("buttonBox");
    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &Calendar::EventEditor::EntryEditor::saveEvent);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &Calendar::EventEditor::EntryEditor::discardEvent);
}

void Calendar::EventEditor::EntryEditor::saveEvent() {
    editorEvent.time = findChild<QTimeEdit*>("timeEdit")->time();
    editorEvent.description = findChild<QLineEdit*>("descriptionEdit")->text();

    close();
}
void Calendar::EventEditor::EntryEditor::discardEvent() {
    if (editorEvent.isNew)
        editorEvent.deleted = true;

    close();
}
