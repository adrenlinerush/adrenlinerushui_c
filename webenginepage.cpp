#include "webenginepage.h"
#include "browser.h"
#include <QMessageBox>
#include <QLoggingCategory>
#include <QUrl>

WebEnginePage::WebEnginePage(QObject* parent) : QWebEnginePage(parent) {}

void WebEnginePage::setBrowser(QWidget* tabbedBrowser) {
	browser = tabbedBrowser;
}

bool WebEnginePage::certificateError(const QWebEngineCertificateError& error) {
    QWebEngineCertificateError& error_ref = const_cast <QWebEngineCertificateError&>(error);
    try {
        qInfo() << error_ref.url();
        
        QMessageBox ignoreCertificate;
        ignoreCertificate.setText("Invalid Certificate");
        ignoreCertificate.setInformativeText(error_ref.url().toString() + " has presented an invalid certificate. Continue to site anyway?");
        ignoreCertificate.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        ignoreCertificate.setDefaultButton(QMessageBox::No);
        int cont = ignoreCertificate.exec();

        if (cont == QMessageBox::Yes) {
            qInfo() << "Attempting to ignore certificate error.";
            error_ref.ignoreCertificateError();
            return true;
        }

        return QWebEnginePage::certificateError(error);
    } catch (const std::exception& e) {
        qCritical() << "WebEnginePage.certificateError: " << e.what();
        return false;
    }
}

QWebEnginePage* WebEnginePage::createWindow(QWebEnginePage::WebWindowType type) {
    try {
        qInfo() << "Create Window Type: " << type;

        if (browser != nullptr) {
            auto tabbedBrowser = dynamic_cast<Browser*>(browser);
            if (tabbedBrowser) {
                return tabbedBrowser->add_tab(QUrl("https://google.com"));
            }
        }

        return nullptr;
    } catch (const std::exception& e) {
        qCritical() << "WebEnginePage.createWindow: " << e.what();
        return nullptr;
    }
}
