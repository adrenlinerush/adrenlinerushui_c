#ifndef MDIAREA_H
#define MDIAREA_H

#include <QMdiArea>
#include <QPixmap>
#include <QPainter>
#include <QResizeEvent>

class MDIArea : public QMdiArea {
    Q_OBJECT

public:
    MDIArea(const QPixmap& backgroundPixmap, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QPixmap backgroundPixmap_;
    QPixmap displayPixmap_;
    bool centered_;
};

#endif // MDIAREA_H
