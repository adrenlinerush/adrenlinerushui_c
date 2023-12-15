#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <tinyexpr.h>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QKeySequence>

class Calculator : public QWidget {

public:
    Calculator(QWidget* parent = nullptr);

private slots:
    void btnClicked();

private:
    QGridLayout* calcLayout;
    QLabel* labelIn;
    QLabel* labelOut;

    double eval(const QString& expression);
};

#endif // CALCULATOR_H

