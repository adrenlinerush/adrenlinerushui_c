#include "lineedit.h"

LineEdit::LineEdit(QWidget* parent) : QLineEdit(parent) {}

void LineEdit::mousePressEvent(QMouseEvent* event) {
    topLevelWidget()->activateWindow();
    setFocus();
    QLineEdit::mousePressEvent(event);
}
