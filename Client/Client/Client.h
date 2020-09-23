#include <QDebug>
#include <QtNetwork/QTcpSocket>

const int NAME_SEND = 150;
const int MESSAGE_SEND = 151;
const int ID_SEND = 152;
const int CONFIRM = 180;

#pragma pack(push, 1)
struct Message {
	short code;
	std::string data;
};
#pragma pack(pop)

class Client : public QObject
{
	Q_OBJECT
public:
	Client();
	~Client();

public slots:
	void onReadyRead();
	void GetMessage(QString message);
	void SetUserName(QString name);

signals:
	void PassDataToConversation(QString data);
	void PassIdToHostList(QString host);

private:
	void SendPacket(int code, QString data);

	QTcpSocket  Socket;
	QString		Name;
};

