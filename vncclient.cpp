#include "vncclient.h"

Q_LOGGING_CATEGORY(logVncClient, "VncClient")

VncClient::VncClient(QWidget *parent)
    : QWidget(parent), vnc(nullptr)
{
    try
    {
        QString host = QInputDialog::getText(this, "VNC Connect", "Host:");
        if (host.isEmpty())
            return;

        QString port = QInputDialog::getText(this, "VNC Connect", "Port:");
        if (port.isEmpty())
            return;

        QString password = QInputDialog::getText(this, "VNC Connect", "Password:", QLineEdit::Password);
        if (password.isEmpty())
            return;

        vnc = new QVNCClientWidget(this);
	vnc->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);  // Set the size policy
	vnc->setFixedSize(1024, 768);
	vnc->connectToVncServer(host,password,port.toInt());
        vnc->startFrameBufferUpdate();
    }
    catch (const std::exception &e)
    {
        qCWarning(logVncClient) << "VncClient::VncClient - Error: " << e.what();
    }
}
