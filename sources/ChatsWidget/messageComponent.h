#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPainterPath>
#include <QPainter>
#include <QScrollBar>

class ButtonIcon;
enum Theme;

struct StyleInnerComponent {
    QString labelStyleDarkMessage = R"(
    QLabel {
        background-color: transparent; 
        color: rgb(240, 240, 240); 
        font-family: 'Segoe UI'; 
        font-size: 14px; 
        font-weight: bold;
        padding: 10px; 
        border: none; 
        border-radius: 20px; 
    }
)";

    QString labelStyleDarkTime = R"(
    QLabel {
        background-color: transparent; 
        color: rgb(219, 219, 219); 
        font-family: 'Segoe UI'; 
        font-size: 12px; 
        padding: 10px; 
        border: none; 
        border-radius: 20px; 
    }
)";

    QString labelStyleLight = R"(
    QLabel {
        background-color: transparent; 
        color: rgb(38, 38, 38); 
        font-family: 'Segoe UI'; 
        font-size: 14px; 
        padding: 10px; 
        border: none; 
        border-radius: 20px; 
    }
)";
};

class InnerComponent : public QWidget {
public:
    InnerComponent(QWidget* parent, const QString& timestamp, const QString& text, Theme theme);
    void setTheme(Theme theme);

    void setText(const QString& text) { m_textLabel->setText(text); }
    void setTimestamp(const QString& text) { m_timestampLabel->setText(text); }
    void setReadStatus(bool read);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    StyleInnerComponent* style;
    Theme m_theme;
    QColor m_backColor;

    QLabel* m_textLabel;          
    QLabel* m_timestampLabel; 
    ButtonIcon* m_readStatusBtn;

    QHBoxLayout* m_main_HLayout; 
    QVBoxLayout* m_time_VLayout; 
    QVBoxLayout* m_isRead_VLayout; 

};


class MessageComponent : public QWidget {
    Q_OBJECT

public:
    explicit MessageComponent(QWidget* parent, const QString& timestamp, const QString& text, Theme theme, int id);
    void setTheme(Theme theme) { m_innerWidget->setTheme(theme); }
    void setMessage(const QString& message) { m_innerWidget->setText(message); }
    void setTimestamp(const QString& timestamp) { m_innerWidget->setTimestamp(timestamp); }
    void setReadStatus(bool isRead) { m_innerWidget->setReadStatus(isRead); }



private:
    Theme m_theme;
    int m_id;
    QHBoxLayout* m_main_HLayout;
    InnerComponent* m_innerWidget;
};