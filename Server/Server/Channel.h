#include <qobject.h>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <vector>
#include "Host.h"

class Channel : public QObject
{
public:
	Channel();
	~Channel();
	void ReadPendingMessages(Host *which);
	void SendMessages();

private:
	int ConversationID;
	QVector<Host*> HostsVector;
	std::vector<std::string> conversation;
};
