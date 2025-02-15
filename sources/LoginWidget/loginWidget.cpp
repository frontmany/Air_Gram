#include "loginWidget.h"
#include "authorizationComponent.h"
#include "registrationComponent.h"
#include "mainwindow.h"
#include "clientSide.h"

LoginWidget::LoginWidget(QWidget* parent, MainWindow* mw, ClientSide* client)
    : QWidget(parent), m_client(client){

    m_switchState = AUTHORIZATION;
    style = new StyleLoginWidget;

    m_switchToRegisterButton = new QPushButton("register", this);
    m_switchToAuthorizeButton = new QPushButton("login", this);

    m_switchersHLayout = new QHBoxLayout;
    m_switchersHLayout->addSpacing(-300);
    m_switchersHLayout->setAlignment(Qt::AlignCenter);
    m_switchersHLayout->addWidget(m_switchToAuthorizeButton);
    m_switchersHLayout->addWidget(m_switchToRegisterButton);



    m_FormsHLayout = new QHBoxLayout;
    m_FormsHLayout->setAlignment(Qt::AlignCenter);

    m_authorizationWidget = new AuthorizationComponent(this, this);
    m_registrationWidget = new RegistrationComponent(this, this);
    m_FormsHLayout->addWidget(m_authorizationWidget);
    m_FormsHLayout->addWidget(m_registrationWidget);


    
    m_mainVLayout = new QVBoxLayout(this);
    m_mainVLayout->setAlignment(Qt::AlignCenter);
    m_mainVLayout->addLayout(m_switchersHLayout);
    m_registrationWidget->hide();
    m_mainVLayout->addLayout(m_FormsHLayout);

    
    connect(m_switchToRegisterButton, &QPushButton::clicked, this, &LoginWidget::switchToRegister);
    connect(m_switchToAuthorizeButton, &QPushButton::clicked, this, &LoginWidget::switchToAuthorize);

    connect(this, &LoginWidget::sendLoginStatus, mw, &MainWindow::onLogin);
}

void LoginWidget::onAuthorizeButtonClicked(QString& login, QString& password) {
    bool isLog =  m_client->authorizeClient(login.toStdString(), password.toStdString());
    if (isLog) {
        User me;
        me.setLogin(login.toStdString());
        me.setPassword(password.toStdString());
        me.setLastSeen("online");
        m_client->setMyInfo(me);
    }
    emit(sendLoginStatus(isLog));
}

void LoginWidget::onRegisterButtonClicked(QString& login, QString& password, QString& name) {
    bool isLog = m_client->registerClient(login.toStdString(), password.toStdString(), name.toStdString());
    if (isLog) {
        User me;
        me.setLogin(login.toStdString());
        me.setName(name.toStdString());
        me.setPassword(password.toStdString());
        me.setLastSeen("online");
        m_client->setMyInfo(me);
        
    }
    emit(sendLoginStatus(isLog));
}

void LoginWidget::switchToAuthorize() {
    m_registrationWidget->hide();
    m_authorizationWidget->show();
    m_switchState = AUTHORIZATION;
    swapSwitchStyles();
}

void LoginWidget::switchToRegister() {
    m_authorizationWidget->hide();
    m_registrationWidget->show();
    m_switchState = REGISTRATION;
    swapSwitchStyles();
}

void LoginWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawPixmap(this->rect(), m_background); 
    QWidget::paintEvent(event);
}

void LoginWidget::setTheme(Theme theme) {
    swapSwitchStyles();
    m_authorizationWidget->setTheme(theme);
    m_registrationWidget->setTheme(theme);
    setBackGround(theme);
    update();
}

void LoginWidget::setBackGround(Theme theme) {
    if (theme == DARK) {
        if (m_background.load(":/resources/LoginWidget/darkLoginBackground.jpg")) {
        }
    }
    else {
        if (m_background.load(":/resources/LoginWidget/lightLoginBackground.jpg")) { 
        }
    }
}

void LoginWidget::swapSwitchStyles() {
    if (m_switchState == AUTHORIZATION) {
        m_switchToAuthorizeButton->setStyleSheet(style->buttonStyleBlue);
        m_switchToRegisterButton->setStyleSheet(style->buttonStyleGray);
    }
    if (m_switchState == REGISTRATION) {
        m_switchToAuthorizeButton->setStyleSheet(style->buttonStyleGray);
        m_switchToRegisterButton->setStyleSheet(style->buttonStyleBlue);
    }
}