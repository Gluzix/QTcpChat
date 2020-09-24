#include <qobject.h>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <qlist.h>
#include "Host.h"

const int NAME_SEND = 150;
const int MESSAGE_SEND = 151;
const int ID_SEND = 152;
const int PENDING_MSG = 153;
const int CONFIRM = 180;

template<class type>
struct Message {
	short code;
	type data;
};

//QDataStream & operator << (QDataStream &stream, const PendingMessages& obj);
//QDataStream & operator >> (QDataStream &stream, const PendingMessages& obj);

class Server : public QObject
{
	Q_OBJECT
public:
	Server();
	~Server();
public slots:
	void OnNewConnection();
	void OnSocketStateChanged(QAbstractSocket::SocketState socketState);
	void OnReadyRead();
private:
	bool CheckIfIdIsAvailable(int id);
	void SendPacket(QTcpSocket *socket, int code, QString data);
	void SendPacket(QTcpSocket *socket, int code);

	QTcpServer  TcpServer;
	QList<Host>  Hosts;
};
