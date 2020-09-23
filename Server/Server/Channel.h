#include <qobject.h>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <vector>

class Channel : public QObject
{
public:
	Channel();
	~Channel();
	void SaveConversation();//Save conversation and id
	void ReadConversation();//Read conversation and id

private:
	int ConversationID;
	QTcpSocket* FirstHost;
	QTcpSocket* SecondHost;
	std::vector<std::string> conversation;
	//QTcpServer klasa posiada cos takiego jak SetRead/WriteChannel. By� mo�e dzi�ki temu, b�d� m�g� w �atwiejszy spos�b obs�u�y� kana�y
};
