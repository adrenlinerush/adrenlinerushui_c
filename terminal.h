#ifndef TERMINAL_H
#define TERMINAL_H

#include <QWidget>
#include <QDebug>
#include <qtermwidget5/qtermwidget.h>
#include <QString>
#include <QFont>
#include <QApplication>
#include <QShortcut>
#include <QShortcut>

class Terminal : public QTermWidget
{
public:
    Terminal(QWidget *parent = nullptr);
    void runCommand(QString command);

protected:
    void handleMousePress(QMouseEvent *event);
    void handleKeyEvents(QKeyEvent *event);
	
private:
    QShortcut* shortcut_copy;
    QShortcut* shortcut_paste;

};

#endif // TERMINAL_H

