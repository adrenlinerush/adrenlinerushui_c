#include "mdiarea.h"

MDIArea::MDIArea(const QPixmap& backgroundPixmap, QWidget* parent)
    : QMdiArea(parent), backgroundPixmap_(backgroundPixmap), centered_(false) {
}

void MDIArea::paintEvent(QPaintEvent* event) {
    QPainter painter(viewport());
    if (!centered_) {
        painter.drawPixmap(0, 0, width(), height(), backgroundPixmap_);
    } else {
        painter.fillRect(event->rect(), palette().color(QPalette::Window));
        qreal x = (width() - displayPixmap_.width()) / 2.0;
        qreal y = (height() - displayPixmap_.height()) / 2.0;
        painter.drawPixmap(x, y, displayPixmap_);
    }
}

void MDIArea::resizeEvent(QResizeEvent* event) {
    displayPixmap_ = backgroundPixmap_.scaled(event->size(), Qt::KeepAspectRatio);
}

