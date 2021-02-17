#include <QtNetwork/QHostAddress>
#include <QtNetwork/QAbstractSocket>
#include <QtCore/QRandomGenerator>
#include <qdatastream.h>
#include "Server.h"

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

Server::Server()
{
	m_tcpServer.listen(QHostAddress::Any, 4242);
	connect(&m_tcpServer, SIGNAL(newConnection()), this, SLOT(OnNewConnection()));
}

Server::~Server()
{
	for (QList<Channel*>::iterator it = m_channels.begin(); it < m_channels.end(); ++it)
	{
		delete* it;
		*it = nullptr;
	}
}

void Server::OnNewConnection()
{
	QTcpSocket *clientSocket = m_tcpServer.nextPendingConnection();

	connect(clientSocket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
	connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(OnSocketStateChanged(QAbstractSocket::SocketState)));
	int number = 0;
	do
	{
		number = QRandomGenerator::global()->bounded(100000, 200000);
	} while (!CheckIfIdIsAvailable(number));

	Host host(clientSocket, "", number);

	SendPacket(clientSocket, CONNECTED_HOSTS); //Sending to new host, all connected m_hosts

	m_hosts.push_back(host);
}

bool Server::CheckIfIdIsAvailable(int id)
{
	bool retState = true;
	for (QList<Host>::Iterator it = m_hosts.begin(); it < m_hosts.end(); ++it)
	{
		if (it->GetId() == id)
		{
			retState = false;
		}
	}
	return retState;
}

bool Server::CheckIfChannelIdIsAvailable(int id)
{
	bool retState = true;
	for (QList<Channel*>::Iterator it = m_channels.begin(); it < m_channels.end(); ++it)
	{
		if ((*it)->GetId() == id)
		{
			retState = false;
		}
	}
	return retState;
}

void Server::SendPacket(QTcpSocket *socket, int code, QString data)
{
	if (socket != nullptr)
	{
		Message<QString> msg;
		msg.code = code;
		msg.data = data;
		QByteArray bytes;
		QDataStream stream(&bytes, QIODevice::WriteOnly);
		stream << msg;
		socket->write(bytes);
	}
}

void Server::SendPacket(QTcpSocket *socket, int code, QVector<QString> &data)
{
	if (socket != nullptr)
	{
		Message<QVector<QString>> msg;
		msg.code = code;
		msg.data = data;
		QByteArray bytes;
		QDataStream stream(&bytes, QIODevice::WriteOnly);
		stream << msg;
		socket->write(bytes);
	}
}

void Server::SendPacket(QTcpSocket *socket, int code)
{
	if (socket != nullptr)
	{
		Message<QVector<QString>> msg;
		msg.code = code;
		for (QList<Host>::iterator it = m_hosts.begin(); it < m_hosts.end(); ++it)
		{
			QString data = it->GetName() + ":" + QString::number(it->GetId());
			msg.data.push_back(data);
		}
		QByteArray bytes;
		QDataStream stream(&bytes, QIODevice::WriteOnly);
		stream << msg;
		socket->write(bytes);
	}
}

void Server::OnSocketStateChanged(QAbstractSocket::SocketState socketState)
{
	if (socketState == QAbstractSocket::UnconnectedState)
	{
		QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
		int id = 0;

		for (QList<Host>::Iterator it = m_hosts.begin(); it < m_hosts.end(); ++it)
		{
			if (it->GetSocketHandler() == sender)
			{
				for (QList<Channel*>::Iterator iter = m_channels.begin(); iter < m_channels.end(); ++iter)
				{
					if ( (*iter)->CheckHost( &(*it) ) )
					{
						delete (*iter);
						(*iter) = nullptr;
						m_channels.removeOne((*iter));
					}

				}
				int dist = it - m_hosts.begin();
				id = it->GetId();
				m_hosts.removeAt(dist);
			}
		}
		for (QList<Host>::Iterator it = m_hosts.begin(); it < m_hosts.end(); ++it)
		{
			QTcpSocket* receiver = it->GetSocketHandler();
			SendPacket(receiver, REMOVE_HOST, QString::number(id));
		}
	}
}

void Server::OnReadyRead()
{
	QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());

	QByteArray data = sender->readAll();
	QDataStream stream(data);
	short code;
	stream >> code;

	if (code == MESSAGE_SEND)
	{
		QVector<QString> vector;
		stream >> vector;
		int id = vector[0].toInt();

		for (QList<Channel*>::iterator it = m_channels.begin(); it < m_channels.end(); ++it)
		{
			if ((*it)->CheckHost(sender, id))
			{
				for (QList<Host>::Iterator it = m_hosts.begin(); it < m_hosts.end(); ++it)
				{
					QTcpSocket *recv = it->GetSocketHandler();
					if (recv == sender)
					{
						vector[0] = QString::number(it->GetId());
					}
				}
				QTcpSocket* receiver = (*it)->GetReceiver(sender);
				SendPacket(receiver, MESSAGE_SEND, vector);
				break;
			}
		}
	}
	else if (code == NAME_SEND)
	{
		QString message;
		stream >> message;
		Message<QString> msgBack;
		int number = 0;
		for (QList<Host>::Iterator it = m_hosts.begin(); it < m_hosts.end(); ++it)
		{
			QTcpSocket *receiver = it->GetSocketHandler();
			if (receiver == sender)
			{
				
				number = it->GetId();
				it->SetName(message);
			}
		}

		if (number != 0)
		{
			for (QList<Host>::Iterator it = m_hosts.begin(); it < m_hosts.end(); ++it)
			{
				QTcpSocket* receiver = it->GetSocketHandler();
				if (receiver != sender)
				{
					SendPacket(receiver, ID_SEND, message + ":" + QString::number(number));
				}
			}
		}
	}
	else if (code == ADD_CHANNEL)
	{
		bool bIfExist = false;
		QString target;
		int id;
		Host *receiver = nullptr;
		Host *m_hostsender = nullptr;
		stream >> target;
		id = target.toInt();

		for (QList<Host>::iterator it = m_hosts.begin(); it < m_hosts.end(); ++it)
		{
			if (id == it->GetId())
			{
				receiver = &(*it);
				break;
			}
		}

		for (QList<Channel*>::iterator it = m_channels.begin(); it < m_channels.end(); ++it)
		{
			if ((*it)->CheckHost(receiver->GetSocketHandler(), sender))
			{
				bIfExist = true;
				break;
			}
		}

		if (!bIfExist)
		{
			int number = 0;
			do
			{
				number = QRandomGenerator::global()->bounded(100000, 200000);
			} while (!CheckIfChannelIdIsAvailable(number));
			Channel *channel = new Channel(number);
			channel->AddHosts(receiver);
			for (QList<Host>::iterator it = m_hosts.begin(); it < m_hosts.end(); ++it)
			{
				if (it->GetSocketHandler() == sender)
				{
					m_hostsender = &(*it);
					break;
				}
			}
			channel->AddHosts(m_hostsender);
			m_channels.push_back(channel);
		}
	}
}

