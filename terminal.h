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
#include <QMenu>

class Terminal : public QTermWidget
{
public:
    Terminal(QWidget *parent = nullptr);
    void runCommand(QString command);
    void setBackground(QString pathToImage);

protected:
    void handleMousePress(QMouseEvent *event);
    void handleKeyEvents(QKeyEvent *event);
    bool eventFilter(QObject *object, QEvent *event); 


	
private:
    void contextExecute(QAction* action);
};

#endif // TERMINAL_H

