#ifndef MDISUBWINDOW_H
#define MDISUBWINDOW_H

#include <QMdiSubWindow>
#include <QWidget>

class MdiSubWindow : public QMdiSubWindow {
public:
    MdiSubWindow(QWidget *parent = nullptr);

protected:
    bool event(QEvent *event) override;
    void changeEvent(QEvent *event) override;
};

#endif // MDISUBWINDOW_H

