#include "terminal.h"


Terminal::Terminal(QWidget *parent)
    : QTermWidget(parent)
{
	QFont font = QApplication::font();
	font.setFamily("Terminus");
	font.setPointSize(6);
	setTerminalFont(font);
	setColorScheme("Linux");
	
        setFocusPolicy(Qt::ClickFocus);

        connect(this, &Terminal::termMouseClicked, this, &Terminal::handleMousePress);
        connect(this, &Terminal::termKeyPressed, this, &Terminal::handleKeyEvents);
}

void Terminal::handleMousePress(QMouseEvent *event) {
    qDebug() << "mouse pressed in terminal";
    if (event->button() == Qt::LeftButton) {
        topLevelWidget()->activateWindow();
        setFocus();
    } else if (event->button() == Qt::RightButton) {
        QMenu* context = new QMenu;
        context->addAction("copy");
        context->addAction("paste");
        connect(context, &QMenu::triggered, this, &Terminal::contextExecute);
        context->exec(event->globalPos()); 
    }
}

void Terminal::contextExecute(QAction* action) {
    if (action->text() == "copy") {
        copyClipboard();
    } else if (action->text() == "paste") {
        pasteClipboard();
    }
}

void Terminal::runCommand(QString command) {
    qDebug() << "Running command: " + command;
    sendText(command + "\n");
}

void Terminal::setBackground(QString pathToImage) {
    setTerminalBackgroundImage(pathToImage);
    setTerminalBackgroundMode(1);
}

void Terminal::handleKeyEvents(QKeyEvent *event) {
   qDebug() << "key pressed in terminal";
   if (event->matches(QKeySequence::Copy)) {
        copyClipboard();
   } else if (event->matches(QKeySequence::Paste)) {
        pasteClipboard();
   } /*(else if (event->matches(QKeySequence::Next_Tab)) {
        if(QTabWidget *w = qobject_cast<QTabWidget *>(parentWidget())){
            int i = w->index();
        }
   }*/

}
