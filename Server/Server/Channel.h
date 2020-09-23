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
	//QTcpServer klasa posiada cos takiego jak SetRead/WriteChannel. Byæ mo¿e dziêki temu, bêdê móg³ w ³atwiejszy sposób obs³u¿yæ kana³y
};
