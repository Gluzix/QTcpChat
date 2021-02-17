#include <QDebug>
#include <QtNetwork/QHostAddress>
#include <QtCore/QTextCodec>
#include <qdatastream.h>
#include "Client.h"

QDataStream & operator << (QDataStream &stream, const Message<QString>& obj)
{
	return stream << (short)obj.code << (QString)obj.data;
}

QDataStream & operator >> (QDataStream &stream, Message<QString>& obj)
{
	short code;
	stream >> code;
	obj.code = code;
	QString str;
	stream >> str;
	obj.data = str;
	return stream;
}

QDataStream & operator << (QDataStream &stream, const Message<QVector<QString>>& obj)
{
	return stream << static_cast<short>(obj.code) << static_cast<QVector<QString>>(obj.data);
}

QDataStream & operator >> (QDataStream &stream, Message<QVector<QString>>& obj)
{
	short code;
	stream >> code;
	obj.code = code;
	QVector<QString> str;
	stream >> str;
	obj.data = str;
	return stream;
}

Client::Client()
{
	m_socket.connectToHost(QHostAddress("127.0.0.1"), 4242);
	connect(&m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void Client::onReadyRead()
{
	QByteArray data = m_socket.readAll();

	QDataStream stream(data);
	short code;
	stream >> code;

	if (code == MESSAGE_SEND)
	{
		QVector<QString> dataVector;
		stream >> dataVector;
		QString id = dataVector[0];
		QString data = dataVector[1];
		emit PassDataToConversation(data, id);
	}
	else if (code == ID_SEND)
	{
		QString data;
		stream >> data;
		QStringList list;
		list = data.split(":");
		emit PassIdToHostList(list[0] + "(" + list[1] + ")");
	}
	else if (code == CONNECTED_HOSTS)
	{
		QVector<QString> vect;
		stream >> vect;
		for (QVector<QString>::iterator it = vect.begin(); it < vect.end(); ++it)
		{
			QStringList list;
			list = it->split(":");
			emit PassIdToHostList(list[0]+"("+list[1]+")");
		}
	}
	else if (code == REMOVE_HOST)
	{
		QString data;
		stream >> data;
		emit SendIdToRemove(data);
	}
}

void Client::GetMessage(QString message, QString id)
{
	QString messageToSend = m_name + ": " + message;
	QVector<QString> vectorToSend;
	vectorToSend.push_back(id);
	vectorToSend.push_back(messageToSend);
	SendPacket(MESSAGE_SEND, vectorToSend);
}

void Client::SendPacket(int code, QString data)
{
	if (m_socket.isWritable())
	{
		Message<QString> msg;
		msg.code = code;
		msg.data = data;
		QByteArray bytes;
		QDataStream stream(&bytes, QIODevice::WriteOnly);
		stream << msg;
		m_socket.write(bytes);
	}
}

void Client::SendPacket(int code, QVector<QString> data)
{
	if (m_socket.isWritable())
	{
		Message<QVector<QString>> msg;
		msg.code = code;
		msg.data = data;
		QByteArray bytes;
		QDataStream stream(&bytes, QIODevice::WriteOnly);
		stream << msg;
		m_socket.write(bytes);
	}
}

void Client::SetUserName(QString name)
{
	m_name = name;
	SendPacket(NAME_SEND, m_name);
}

void Client::GetIdToSend(QString id)
{
	SendPacket(ADD_CHANNEL, id);
}

Client::~Client()
{
}
