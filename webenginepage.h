#ifndef WEBENGINEPAGE_H
#define WEBENGINEPAGE_H

#include <QUrl>
#include <QWebEnginePage>
#include <QMessageBox>
#include <QInputDialog>
#include <QAuthenticator>
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QWebEngineDownloadItem>
#include <QWebEngineCertificateError>

class WebEnginePage : public QWebEnginePage {
public:
    explicit WebEnginePage(QObject* parent = nullptr);
    void setBrowser(QObject* tabbedBrowser);

protected:
    bool certificateError(const QWebEngineCertificateError& error) override;
    QWebEnginePage* createWindow(QWebEnginePage::WebWindowType type) override;

private:
    QObject* browser;
};

#endif // WEBENGINEPAGE_H

