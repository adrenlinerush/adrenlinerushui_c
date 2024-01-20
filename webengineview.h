#ifndef WEBENGINEVIEW_H
#define WEBENGINEVIEW_H

#include <QWebEngineView>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>

class WebEngineView : public QWebEngineView
{

public:
    WebEngineView(QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject *object, QEvent *event); 

signals:

private:
};

#endif // WEBENGINEVIEW_H
