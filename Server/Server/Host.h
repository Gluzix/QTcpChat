#include <qobject.h>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <vector>

class Host : public QObject
{
	Q_OBJECT
public:

	Host();
	Host(const Host& obj);
	Host(QTcpSocket* socketHandler, QString name, int id);
	~Host();

	QTcpSocket* GetSocketHandler();
	int GetId();
	QString GetName();

	void SetName(QString name);
	void SetSocket(QTcpSocket* socket);
	void SetId(int id);

private:
	int m_id;
	QString m_name;
	QTcpSocket *m_pSocketHandler;
	int m_channel;
};

