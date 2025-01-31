#include "chatComponent.h"
#include "chatsWidget.h"
#include "addChatDialogComponent.h"
#include "buttons.h"
#include "mainwindow.h"

ChatComponent::ChatComponent(QWidget* parent, ChatsWidget* chatsWidget, Chat* chat)
    : QWidget(parent), m_avatarSize(50), m_theme(DARK), m_chat(chat), m_isClicked(true), m_isSelected(false) {
    setMinimumSize(100, 70);
    setMaximumSize(1000, 70);

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);

    m_nameLabel = new QLabel(this);
    m_lastMessageLabel = new QLabel(this);

    if (m_chat->getIsFriendHasPhoto() == true) {
        const Photo& photo = m_chat->getFriendPhoto();
        m_avatar = QPixmap(QString::fromStdString(photo.getPhotoPath()));
        update();
    }
    else {
        if (m_theme == DARK) {
            m_avatar = QPixmap(":/resources/LoginWidget/lightLoginBackground.jpg");
        }
        else {
            m_avatar = QPixmap(":/resources/LoginWidget/lightLoginBackground.jpg");

        }
        update();
    }

    m_nameLabel->setStyleSheet("font-weight: bold; font-size: 14px; font-family: 'Segoe UI'; ");
    m_lastMessageLabel->setStyleSheet("font-size: 12px; color: rgb(122, 122, 122); font-family: 'Segoe UI'; ");
    m_lastMessageLabel->setText("no messages yet");

    m_contentsVLayout = new QVBoxLayout();
    m_contentsVLayout->addWidget(m_nameLabel);
    m_contentsVLayout->addWidget(m_lastMessageLabel);

    

    m_UnreadDot = new ButtonIcon(this, 50, 50);
    QIcon icon1(":/resources/ChatsWidget/online.png");
    QIcon iconHover1(":/resources/ChatsWidget/online.png");
    m_UnreadDot->uploadIconsDark(icon1, iconHover1);
    QIcon icon2(":/resources/ChatsWidget/online.png");
    QIcon iconHover2(":/resources/ChatsWidget/online.png");
    m_UnreadDot->uploadIconsLight(icon2, iconHover2);
    m_UnreadDot->hide();
    m_UnreadDot->setTheme(m_theme);

    m_statusVLayout = new  QVBoxLayout;
    m_statusVLayout->setAlignment(Qt::AlignBottom);
    m_statusVLayout->addWidget(m_UnreadDot);
    m_statusVLayout->addSpacing(-10);

    m_mainHLayout = new QHBoxLayout();
    m_mainHLayout->setContentsMargins(70, 10, 10, 10);
    m_mainHLayout->addLayout(m_contentsVLayout);
    m_mainHLayout->addSpacing(500);
    m_mainHLayout->addLayout(m_statusVLayout);

    m_hoverColorLight = QColor(214, 214, 214);
    m_hoverColorDark = QColor(56, 56, 56);

    connect(this, &ChatComponent::clicked, this, &ChatComponent::slotToSendChatData);
    connect(this, &ChatComponent::sendChatData, chatsWidget, &ChatsWidget::onSetChatMessagingArea);

    setLayout(m_mainHLayout);
}
void ChatComponent::setSelected(bool isSelected) {
    m_isSelected = isSelected;
    if (isSelected == true) {
        if (m_theme == DARK) {
            m_backColor = QColor(135, 135, 135);
            m_lastMessageLabel->setStyleSheet("font-size: 12px; color: rgb(227, 227, 227); font-family: 'Segoe UI'; ");
            m_currentColor = m_backColor;
        }
        else {
            m_backColor = QColor(176, 208, 255);
            m_currentColor = m_backColor;
        }
        update();
    }
    else {
        if (m_theme == DARK) {
            m_backColor = QColor(25, 25, 25);
            m_lastMessageLabel->setStyleSheet("font-size: 12px; color: rgb(122, 122, 122); font-family: 'Segoe UI'; ");
            m_currentColor = m_backColor;
        }
        else {
            m_backColor = QColor(224, 224, 224);
            m_lastMessageLabel->setStyleSheet("font-size: 12px; color: rgb(122, 122, 122); font-family: 'Segoe UI'; ");
            m_currentColor = m_backColor;
        }
        update();
    }
}

void ChatComponent::setUnreadMessageDot(bool isUnreadMessages) {
    if (isUnreadMessages == true) {
        m_UnreadDot->show();
    }
    else {
        m_UnreadDot->hide();
    }
}

void ChatComponent::setName(const QString& name) {
    m_nameLabel->setText(name);
}

void ChatComponent::setLastMessage(const QString& message) {
    m_lastMessageLabel->setText(message);
}

void ChatComponent::setAvatar(const QPixmap& avatar) {
    m_avatar = avatar.scaled(m_avatarSize, m_avatarSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
}

void ChatComponent::setTheme(Theme theme) {
    m_theme = theme;
    if (m_theme == DARK) {
        m_backColor = QColor(25, 25, 25);
        m_currentColor = m_backColor;
        update();
    }
    else {
        m_backColor = QColor(255, 255, 255);
        m_currentColor = m_backColor;
        m_nameLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: rgb(47, 47, 48);");
        update();
    }
}

void ChatComponent::paintEvent(QPaintEvent* event) {
    QPainter painter(this);


    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(m_currentColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 5, 5);


    if (!m_avatar.isNull()) {
        QPainterPath path;
        QRectF avatarRect(10, (height() - m_avatarSize) / 2, m_avatarSize, m_avatarSize);
        path.addEllipse(avatarRect);
        painter.setClipPath(path);
        painter.drawPixmap(avatarRect.toRect(), m_avatar);
        painter.setClipPath(QPainterPath());
    }
}

bool ChatComponent::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::HoverEnter:
        hoverEnter(static_cast<QHoverEvent*>(event));
        return true;
    case QEvent::HoverLeave:
        hoverLeave(static_cast<QHoverEvent*>(event));
        return true;
    default:
        return QWidget::event(event);
    }
}


void ChatComponent::hoverEnter(QHoverEvent* event)
{
    if (m_theme == LIGHT) {
        if (m_isSelected == true) {
            m_currentColor = QColor(176, 208, 255);
        }
        else {
            m_currentColor = m_hoverColorLight;
        }
        
    }
    else {
        if (m_isSelected == true) {
            m_currentColor = QColor(135, 135, 135);
        }
        else {
            m_currentColor = m_hoverColorDark;
        }
    }
    update();
}

void ChatComponent::hoverLeave(QHoverEvent* event)
{
    m_currentColor = m_backColor;
    update();
}



void ChatComponent::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

void ChatComponent::slotToSendChatData() {
    setUnreadMessageDot(false);
    emit sendChatData(m_chat, this);
    
}