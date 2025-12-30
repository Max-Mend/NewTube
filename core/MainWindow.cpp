#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QStyle>
#include <QMenu>

CustomWebPage::CustomWebPage(QObject *parent) : QWebEnginePage(parent) {}

bool CustomWebPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) {
    QString urlString = url.toString();
    if (urlString.contains("youtube.com/watch?v=") || urlString.contains("youtu.be/")) {
        emit videoUrlDetected(urlString);
        return false;
    }
    return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
}

void CustomWebPage::javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString &message,
                                              int lineNumber, const QString &sourceID) {
    if (message.startsWith("MPV_VIDEO_URL:")) {
        emit videoUrlDetected(message.mid(14));
        return;
    }

    if (message.contains("generate_204") ||
        message.contains("requestStorageAccessFor") ||
        message.contains("preloaded using link preload") ||
        message.contains("LegacyDataMixin") ||
        message.contains("ResizeObserver")) {
        return;
    }

    QWebEnginePage::javaScriptConsoleMessage(level, message, lineNumber, sourceID);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mpvProcess(nullptr) {
    setWindowTitle("NewTube");
    resize(1200, 800);

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *navLayout = new QHBoxLayout();
    navLayout->setSpacing(5);
    navLayout->setContentsMargins(5, 5, 5, 5);

    backButton = new QPushButton();
    backButton->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    backButton->setMaximumWidth(40);
    backButton->setToolTip("Back");

    forwardButton = new QPushButton();
    forwardButton->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    forwardButton->setMaximumWidth(40);
    forwardButton->setToolTip("Forward");

    refreshButton = new QPushButton();
    refreshButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    refreshButton->setMaximumWidth(40);
    refreshButton->setToolTip("Refresh");

    urlEdit = new QLineEdit();
    urlEdit->setText("https://www.youtube.com");
    urlEdit->setPlaceholderText("Enter URL...");

    menuButton = new QPushButton();
    menuButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    menuButton->setMaximumWidth(40);
    menuButton->setToolTip("Menu");

    navLayout->addWidget(backButton);
    navLayout->addWidget(forwardButton);
    navLayout->addWidget(refreshButton);
    navLayout->addWidget(urlEdit, 1);
    navLayout->addWidget(menuButton);

    mainLayout->addLayout(navLayout);

    webView = new QWebEngineView();
    customPage = new CustomWebPage(this);
    webView->setPage(customPage);

    QWebEngineProfile *profile = customPage->profile();
    profile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);

    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/newtube";
    QDir().mkpath(dataPath);
    profile->setPersistentStoragePath(dataPath);
    profile->setCachePath(dataPath + "/cache");

    QWebEngineSettings *settings = customPage->settings();
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);

    QWebEngineScript script;
    script.setName("VideoInterceptor");
    script.setSourceCode(R"(
        (function() {
            document.addEventListener('click', function(e) {
                let el = e.target;
                for (let i = 0; i < 10 && el; i++) {
                    if (el.href && (el.href.includes('/watch?v=') || el.href.includes('youtu.be/'))) {
                        e.preventDefault();
                        e.stopPropagation();
                        console.log('MPV_VIDEO_URL:' + el.href);
                        return false;
                    }
                    el = el.parentElement;
                }
            }, true);
        })();
    )");
    script.setInjectionPoint(QWebEngineScript::DocumentReady);
    script.setWorldId(QWebEngineScript::MainWorld);
    script.setRunsOnSubFrames(false);
    profile->scripts()->insert(script);

    mainLayout->addWidget(webView);
    setCentralWidget(central);

    connect(menuButton, &QPushButton::clicked, this, [this]() {
        QMenu menu(this);
        menu.addAction("Home", this, &MainWindow::onOpenHome);
        menu.addAction("Subscriptions", this, &MainWindow::onOpenSubscriptions);
        menu.addAction("History", this, &MainWindow::onOpenHistory);
        menu.addAction("Trending", this, &MainWindow::onOpenTrending);
        menu.addSeparator();
        menu.addAction("Clear Cache", this, &MainWindow::onClearCache);
        menu.addAction("About", this, &MainWindow::onAbout);
        menu.exec(menuButton->mapToGlobal(QPoint(0, menuButton->height())));
    });

    connect(backButton, &QPushButton::clicked, webView, &QWebEngineView::back);
    connect(forwardButton, &QPushButton::clicked, webView, &QWebEngineView::forward);
    connect(refreshButton, &QPushButton::clicked, webView, &QWebEngineView::reload);
    connect(urlEdit, &QLineEdit::returnPressed, this, &MainWindow::onGoClicked);
    connect(webView, &QWebEngineView::urlChanged, this, &MainWindow::onUrlChanged);
    connect(customPage, &CustomWebPage::videoUrlDetected, this, &MainWindow::onVideoDetected);

    mpvProcess = new QProcess(this);

    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/newtube";
    QDir().mkpath(configDir);

    QFile configFile(configDir + "/mpv.conf");
    if (configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&configFile);
        out << "ytdl=yes\n"
            << "ytdl-format=bestvideo[height<=?1080]+bestaudio/best\n"
            << "hwdec=auto\n"
            << "keep-open=yes\n"
            << "osc=yes\n"
            << "volume=50\n";
        configFile.close();
    }

    webView->load(QUrl("https://www.youtube.com"));
}

MainWindow::~MainWindow() {
    if (mpvProcess && mpvProcess->state() == QProcess::Running) {
        mpvProcess->terminate();
        mpvProcess->waitForFinished(3000);
    }
}

void MainWindow::onGoClicked() {
    QString url = urlEdit->text().trimmed();
    if (!url.isEmpty()) {
        if (!url.startsWith("http://") && !url.startsWith("https://")) {
            url = "https://" + url;
        }
        webView->load(QUrl(url));
    }
}

void MainWindow::onUrlChanged(const QUrl &url) {
    urlEdit->setText(url.toString());
}

void MainWindow::onVideoDetected(const QString &url) {
    playInMpv(url);
}

void MainWindow::playInMpv(const QString &url) {
    if (mpvProcess->state() == QProcess::Running) {
        mpvProcess->terminate();
        mpvProcess->waitForFinished(2000);
    }

    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/newtube";
    QStringList args;
    args << "--config-dir=" + configDir
         << "--title=YouTube MPV Player"
         << "--ytdl=yes"
         << "--ytdl-format=bestvideo[height<=?1080]+bestaudio/best"
         << url;

    mpvProcess->start("mpv", args);
}

void MainWindow::onOpenHome() {
    webView->load(QUrl("https://www.youtube.com"));
}

void MainWindow::onOpenSubscriptions() {
    webView->load(QUrl("https://www.youtube.com/feed/subscriptions"));
}

void MainWindow::onOpenHistory() {
    webView->load(QUrl("https://www.youtube.com/feed/history"));
}

void MainWindow::onOpenTrending() {
    webView->load(QUrl("https://www.youtube.com/feed/trending"));
}

void MainWindow::onClearCache() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Clear Cache");
    msgBox.setText("This will delete all cookies, history and login data.");
    msgBox.setInformativeText("Continue?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QMessageBox::Yes) {
        customPage->profile()->clearHttpCache();
        QMessageBox::information(this, "Done", "Cache cleared!");
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "About",
    "<h3>NewTube</h3>"
    "<p>Version 1.0</p>"
    "<p>With YouTube automatic video playback in MPV.</p>"
    "<br>"
    "<p><b>Features:</b></p>"
    "<ul>"
    "<li>Full-featured YouTube</li>"
    "<li>Automatic video opening in MPV</li>"
    "<li>Session and cookies saving</li>"
    "</ul>"
    "<br>"
    "<p>Built with Qt6 and MPV</p>");
}