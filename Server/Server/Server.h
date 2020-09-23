#include <qobject.h>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <qlist.h>
#include "Host.h"

const int NAME_SEND = 150;
const int MESSAGE_SEND = 151;
const int ID_SEND = 152;
const int CONFIRM = 180;

#pragma pack(push, 1)
struct Message {
	//Message(int cd, QString d):
	//	code(cd),
	//	data(d){}
	short code;
	std::string data;
};
#pragma pack(pop)

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

	QTcpServer  TcpServer;
	QList<Host>  Hosts;
};
