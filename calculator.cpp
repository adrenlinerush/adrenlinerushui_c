#include "calculator.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QKeySequence>
#include <QKeyEvent>
#include <stdexcept>

Calculator::Calculator(QWidget* parent) : QWidget(parent) {
    calcLayout = new QGridLayout(this);

    labelIn = new QLabel(this);
    labelOut = new QLabel(this);
    labelIn->setAlignment(Qt::AlignRight);
    labelOut->setAlignment(Qt::AlignRight);
    calcLayout->addWidget(labelIn, 0, 0, 1, 4);
    calcLayout->addWidget(labelOut, 1, 0, 1, 4);

    const QStringList buttonsUI[] = {
        {"(", ")", "DEL", "AC"},
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {".", "0", "^", "+"}
    };

    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < buttonsUI[row].size(); ++col) {
            const QString& val = buttonsUI[row][col];

            QPushButton* btn = new QPushButton(val, this);
            connect(btn, &QPushButton::clicked, this, &Calculator::btnClicked);
            calcLayout->addWidget(btn, row + 2, col, 1, 1);

            if (val.size() == 1) {
                btn->setShortcut(QKeySequence(val));
            } else if (val == "DEL") {
                btn->setShortcut(QKeySequence("Del"));
            } else if (val == "AC") {
                btn->setShortcut(QKeySequence("Backspace"));
            }
        }
    }
}

void Calculator::btnClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn)
        return;

    if (btn->text().size() == 1) {
        labelIn->setText(labelIn->text() + btn->text());
    } else if (btn->text() == "DEL") {
        labelIn->setText(labelIn->text().left(labelIn->text().length() - 1));
    } else if (btn->text() == "AC") {
        labelIn->clear();
    }

    try {
        QString output = QString::number(te_interp(labelIn->text().toStdString().c_str(),0)).left(20);
        labelOut->setText(output);
    } catch (const std::exception& e) {
        //labelOut->setText(' ');
    }
}

double Calculator::eval(const QString& expression) {
    bool ok;
    double result = expression.toDouble(&ok);
    if (!ok) {
        //throw std::runtime_error("Invalid expression");
    }
    return result;
}

