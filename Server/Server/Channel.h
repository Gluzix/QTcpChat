#include <qobject.h>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <vector>
#include "Host.h"

class Channel : public QObject
{
public:
	Channel();
	Channel(int id);
	~Channel();

	void AddHosts(Host *host);
	void AddMessageToConversation(QString name, QString message);

	QVector<QString>& GetMessages();
	bool CheckHost(QTcpSocket *socket, QTcpSocket *socket_2);
	bool CheckHost(QTcpSocket *socket, int id);
	bool CheckHost(Host* host);
	QTcpSocket* GetReceiver(QTcpSocket *sender);
	int GetId();

private:
	int m_conversationId;
	QVector<Host*> m_hostsVector;
	QVector<QString> m_conversation;
};
