#include "MessageComponent.h"
#include "mainwindow.h"
#include "buttons.h"


InnerComponent::InnerComponent(QWidget* parent, const QString& timestamp, const QString& text, Theme theme, bool isSent) {
    style = new StyleInnerComponent;
    m_isSent = isSent;

    m_main_HLayout = new QHBoxLayout(this);
    m_main_HLayout->setAlignment(Qt::AlignRight); 
    m_main_HLayout->setSpacing(5);
    m_main_HLayout->setContentsMargins(5, 5, 5, 5);


    m_text_VLayout = new QVBoxLayout;
    m_text_VLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_textLabel = new QLabel(this);
    m_textLabel->setStyleSheet(style->labelStyleDarkMessage);
    m_textLabel->setText(text);
    m_textLabel->setWordWrap(true); 
    m_textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_textLabel->setMaximumWidth(400); 
    m_textLabel->adjustSize();
    m_text_VLayout->addWidget(m_textLabel);
    m_text_VLayout->addSpacing(-15);

    m_time_VLayout = new QVBoxLayout;
    m_time_VLayout->setAlignment(Qt::AlignBottom);
    m_timestampLabel = new QLabel(this);
    m_timestampLabel->setStyleSheet(style->labelStyleDarkTime);
    m_timestampLabel->setText(timestamp);
    m_timestampLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_time_VLayout->addWidget(m_timestampLabel);
    m_time_VLayout->addSpacing(-6);

    m_isRead_VLayout = new QVBoxLayout;
    m_isRead_VLayout->setAlignment(Qt::AlignBottom);
    m_readStatusBtn = new ButtonIcon(this, 30, 30);
    m_readStatusBtn->uploadIconsLight(QIcon(":/resources/ChatsWidget/check.png"), QIcon(":/resources/ChatsWidget/check.png"));
    m_readStatusBtn->uploadIconsDark(QIcon(":/resources/ChatsWidget/notCheck.png"), QIcon(":/resources/ChatsWidget/notCheck.png"));
    m_isRead_VLayout->addWidget(m_readStatusBtn);
    m_isRead_VLayout->addSpacing(-2);


    m_main_HLayout->addLayout(m_text_VLayout);
    m_main_HLayout->addLayout(m_time_VLayout);
    m_main_HLayout->addSpacing(-8);

    if (m_isSent == true) {
        m_main_HLayout->addLayout(m_isRead_VLayout);
    }
    else {
        m_readStatusBtn->hide();
    }

    this->setLayout(m_main_HLayout);
    this->setMinimumSize(30, 35);
    
    this->setMaximumSize(600, 800);
    setTheme(m_theme);
}

void InnerComponent::setReadStatus(bool read) {
    
    if (read) {
        m_is_read = true;
        m_readStatusBtn->setTheme(LIGHT);
    }
    else {
        m_is_read = false;
        m_readStatusBtn->setTheme(DARK);
    }
}

void InnerComponent::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    QRect rect = this->rect();
    int radius = 20; // ������ �����������
    path.addRoundedRect(rect, radius, radius);
    painter.fillPath(path, m_backColor); // �������� ���

    QWidget::paintEvent(event); // �������� ������� �����
}


void InnerComponent::setTheme(Theme theme) {
    m_theme = theme;
    if (theme == DARK) {
        if (m_isSent) {
            m_backColor = QColor(102, 102, 102);
            m_textLabel->setStyleSheet(style->labelStyleDarkMessage);
            m_timestampLabel->setStyleSheet(style->labelStyleDarkTime);
        }
        else {
            m_backColor = QColor(71, 71, 71);
            m_textLabel->setStyleSheet(style->labelStyleDarkMessage);
            m_timestampLabel->setStyleSheet(style->labelStyleDarkTime);
        }
        
    }
    else {
        if (m_isSent) {
            m_backColor = QColor(212, 229, 255);
            m_textLabel->setStyleSheet(style->labelStyleLightMessage);
            m_timestampLabel->setStyleSheet(style->labelStyleLightTime);
        }
        else {
            m_backColor = QColor(224, 224, 224);
            m_textLabel->setStyleSheet(style->labelStyleLightMessage);
            m_timestampLabel->setStyleSheet(style->labelStyleLightTime);
        }
    }
    update(); 
}


MessageComponent::MessageComponent(QWidget* parent, const QString& timestamp, 
    const QString& text, Theme theme, double id, bool isSent)
    : QWidget(parent), m_theme(theme), m_id(id), m_isSent(isSent){
    m_innerWidget = new InnerComponent(this, timestamp, text, m_theme, m_isSent);
    this->setStyleSheet("background-color: transparent;");
  
    m_main_HLayout = new QHBoxLayout(this);
    if (m_isSent){
        m_main_HLayout->setAlignment(Qt::AlignRight);
    }
    else {
        m_main_HLayout->setAlignment(Qt::AlignLeft);
    }
    m_main_HLayout->addWidget(m_innerWidget);
    m_main_HLayout->addSpacing(5);
    m_main_HLayout->setContentsMargins(5, 5, 5, 5);

   
    setLayout(m_main_HLayout);
    setReadStatus(false);
    setTheme(theme);
}


QJsonObject MessageComponent::serialize() const {
    QJsonObject jsonObject;
    jsonObject["id"] = m_id;
    jsonObject["isSent"] = m_isSent;
    jsonObject["theme"] = static_cast<int>(m_theme);
    jsonObject["text"] = m_innerWidget->getText();
    jsonObject["timestamp"] = m_innerWidget->getTimestamp();
    jsonObject["isRead"] = m_innerWidget->getIsRead();
    return jsonObject;
}


MessageComponent* MessageComponent::deserialize(const QJsonObject& jsonObject) {

    double id = jsonObject["id"].toInt();
    bool isSent = jsonObject["isSent"].toBool();
    Theme theme = static_cast<Theme>(jsonObject["theme"].toInt());
    const QString text = jsonObject["text"].toString();
    const QString timestamp = jsonObject["timestamp"].toString();
    bool isRead = jsonObject["isRead"].toBool();

    MessageComponent* messageComponent = new MessageComponent(nullptr, timestamp, text, theme, id, isSent);

    return messageComponent;
}

