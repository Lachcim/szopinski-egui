#include <calendar.h>

Calendar::EventEditor::EntryEditor::EntryEditor(QWidget* parent, Event& event) : QDialog(parent), editorEvent(event) {
    ui.setupUi(this);
}
