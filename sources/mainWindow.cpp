#include <filesystem>
#include "mainWindow.h"
#include "utility.h"
#include "client.h"
#include "chatsListComponent.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    if (isDarkMode()) {
        m_theme = DARK;
    }
    else {
        m_theme = LIGHT;
    }



    m_chatsWidget = nullptr;
    m_client = new Client;
    m_client->connectTo("192.168.1.49", 8080);
    m_client->run();
    //m_client->setWorkerUI(&workerUI);

    setupLoginWidget();
}


MainWindow::~MainWindow() {
    delete m_chatsWidget;
}


bool MainWindow::isDarkMode() {
    HKEY hKey;
    const TCHAR* subKey = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize");
    const TCHAR* valueNameApps = TEXT("AppsUseLightTheme");
    const TCHAR* valueNameSystem = TEXT("SystemUsesLightTheme");
    DWORD value;
    DWORD size = sizeof(value);

    if (RegOpenKeyEx(HKEY_CURRENT_USER, subKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, valueNameApps, NULL, NULL, (LPBYTE)&value, &size) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return value == 0;
        }
        RegCloseKey(hKey);
    }
    return false;
}


void MainWindow::onLogin() {
    setupChatsWidget();
}


void MainWindow::setupLoginWidget() {
    m_loginWidget = new LoginWidget(this, this, m_client);
    m_loginWidget->setTheme(m_theme);
    setCentralWidget(m_loginWidget);
}


void MainWindow::setupChatsWidget() {
    QString directoryPath = Utility::getSaveDir();

    QDir dir(directoryPath);
    if (!dir.exists()) {
        qWarning() << "The directory was not found:" << directoryPath;
    }
    QStringList fileList = dir.entryList(QDir::Files);

    if (fileList.size() > 0) {
        for (const QString& fileName : fileList) {
            if (fileName.left(fileName.length() - 5) == QString::fromStdString(m_client->getMyLogin())) {
                m_client->load(fileName.toStdString());

            }
        }
    }
    m_chatsWidget = new ChatsWidget(this, m_client, m_theme);
    m_chatsWidget->load();
    m_chatsWidget->setTheme(m_theme);
    m_chatsWidget->setTheme(m_theme);
    setCentralWidget(m_chatsWidget);
}