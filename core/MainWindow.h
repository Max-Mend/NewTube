#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>

class CustomWebPage : public QWebEnginePage {
    Q_OBJECT
public:
    CustomWebPage(QObject *parent = nullptr);

protected:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override;
    void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString &message,
                                  int lineNumber, const QString &sourceID) override;

    signals:
        void videoUrlDetected(const QString &url);
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGoClicked();
    void onUrlChanged(const QUrl &url);
    void onVideoDetected(const QString &url);
    void onOpenHome();
    void onOpenSubscriptions();
    void onOpenHistory();
    void onOpenTrending();
    void onClearCache();
    void onAbout();

private:
    void playInMpv(const QString &url);

    QWebEngineView *webView;
    CustomWebPage *customPage;
    QLineEdit *urlEdit;
    QPushButton *menuButton;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QPushButton *refreshButton;
    QProcess *mpvProcess;
};

#endif