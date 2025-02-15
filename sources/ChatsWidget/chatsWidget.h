#pragma once

#include <QWidget>
#include <vector>
#include <thread>
#include <QLayout>
#include <algorithm>

#include "chat.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

class ChatsListComponent;
class MessagingAreaComponent;
class HelloAreaComponent;
class ChatComponent;
class MainWindow;
class ClientSide;
enum Theme;

class ChatsWidget : public QWidget {
	Q_OBJECT
public:
	ChatsWidget(QWidget* parent, ClientSide* client, Theme theme);
	ChatsWidget() {}
	~ChatsWidget();
	void setTheme(Theme theme);
	const Theme getTheme() const { return m_theme; }
	
	MessagingAreaComponent* getMessagingArea() { return m_current_messagingAreaComponent; }
	ClientSide* getClientSide() { return m_client; }
	ChatsListComponent* getChatsList() { return m_chatsListComponent; }
	std::vector<MessagingAreaComponent*>& getMessagingComponentsCacheVec() { return m_vec_messagingComponents_cache; }
	void setClient(ClientSide* client);

	void serialize();
	static ChatsWidget* deserialize(const QString& fileName, QWidget* parent, ClientSide* client, Theme theme);

public slots:
	void onChangeThemeClicked();
	void onSendMessageData(QString message, const QString& timeStamp, Chat* chat, double id);
	void createMessagingAreaFromClientSide(QString message, QString timeStamp, Chat* chat, double id);
	void onCreateChatButtonClicked(QString login);
	void onSetChatMessagingArea(Chat* chat, ChatComponent* component);
	

private:
	void loadChats() {/*TODO*/}
	void paintEvent(QPaintEvent* event) override;
	void setBackGround(Theme theme);

private:
	Theme					m_theme;
	QPixmap					m_background;
	ClientSide*				m_client;
	ChatsListComponent*		m_chatsListComponent;
	MessagingAreaComponent* m_current_messagingAreaComponent;
	HelloAreaComponent*		m_helloAreaComponent;
	QVBoxLayout*			m_leftVLayout;
	QHBoxLayout*			m_mainHLayout;

	std::vector<MessagingAreaComponent*> m_vec_messagingComponents_cache;
	bool m_isFirstChatSet;
};