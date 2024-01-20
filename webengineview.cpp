#include "webengineview.h"
#include <QDebug>

WebEngineView::WebEngineView(QWidget* parent)
    : QWebEngineView(parent)
{
    QApplication::instance()->installEventFilter(this);
}

bool WebEngineView::eventFilter(QObject *object, QEvent *event) {
        if (object->parent() == this && event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
            if (mouseEvent && mouseEvent->button() == Qt::LeftButton) {
                 topLevelWidget()->activateWindow();
                 setFocus();
            }
        }

        return false;
}
