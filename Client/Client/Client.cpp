#include <QDebug>
#include <QtNetwork/QHostAddress>
#include <QtCore/QTextCodec>
#include "Client.h"

Client::Client()
{
	Socket.connectToHost(QHostAddress("127.0.0.1"), 4242);
	connect(&Socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void Client::onReadyRead()
{
	QByteArray data = Socket.readAll();
	Message *msg = reinterpret_cast<Message*>(data.data());
	if (msg->code == MESSAGE_SEND)
	{
		QString data = QString::fromStdString(msg->data);
		emit PassDataToConversation(data);
	}
	else if (msg->code == ID_SEND)
	{
		QString data = QString::fromStdString(msg->data);
		emit PassIdToHostList(data);
	}
}

void Client::GetMessage(QString message)
{
	QString messageToSend = Name + ": " + message;
	SendPacket(MESSAGE_SEND, messageToSend);
}

void Client::SendPacket(int code, QString data)
{
	if (Socket.isWritable())
	{
		Message msg;
		msg.code = code;
		msg.data = data.toStdString();
		Socket.write(reinterpret_cast<char*>(&msg), sizeof(msg));
	}
}

void Client::SetUserName(QString name)
{
	Name = name;
	SendPacket(NAME_SEND, Name);
}

Client::~Client()
{
}
