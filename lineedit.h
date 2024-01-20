#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include <QMouseEvent>

class LineEdit : public QLineEdit {

protected:
    void mousePressEvent(QMouseEvent* event) override;

public:
    LineEdit(QWidget* parent = nullptr);
};

#endif // LINEEDIT_H
