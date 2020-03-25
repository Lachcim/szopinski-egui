#include <calendar.h>

//constuctor of entry editor
Calendar::EventEditor::EntryEditor::EntryEditor(QWidget* parent, LocalEvent& event, bool adding) : QDialog(parent), editorEvent(event) {
    //load UI from generated header, set different title if in adding mode
    ui.setupUi(this);
    if (adding)
        setWindowTitle("New entry");

    //set local members
    editorEvent = event;
    addingEvent = adding;

    //fill data entry widgets
    findChild<QTimeEdit*>("timeEdit")->setTime(event.time);
    findChild<QLineEdit*>("descriptionEdit")->setText(event.description);

    //connect buttons to actions
    QDialogButtonBox* buttonBox = findChild<QDialogButtonBox*>("buttonBox");
    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &Calendar::EventEditor::EntryEditor::saveEvent);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &Calendar::EventEditor::EntryEditor::discardEvent);
}

//modify event to match input
void Calendar::EventEditor::EntryEditor::saveEvent() {
    editorEvent.time = findChild<QTimeEdit*>("timeEdit")->time();
    editorEvent.description = findChild<QLineEdit*>("descriptionEdit")->text();

    close();
}
//mark event as deleted if discarded in adding mode
void Calendar::EventEditor::EntryEditor::discardEvent() {
    if (addingEvent)
        editorEvent.deleted = true;

    close();
}
