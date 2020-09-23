#include <QDebug>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QAbstractSocket>
#include <QtCore/QRandomGenerator>
#include "Server.h"

Server::Server()
{
	TcpServer.listen(QHostAddress::Any, 4242);
	connect(&TcpServer, SIGNAL(newConnection()), this, SLOT(OnNewConnection()));
}

// I need channel or conversation handler in order to store messages for 2 - hosts.
// I need to handle pending messages, for example when one user will write to another, but
// the receiver has not opened the conversation window .
// for example i could store conversation pointers in a vector for every host. 
// and when opening conversation with some guy, just ask server for this vector.
// Conversation class:
// -Host1 pointer
// -Host2 pointer
// -Vector of conversation

void Server::OnNewConnection()
{
	QTcpSocket *clientSocket = TcpServer.nextPendingConnection();

	connect(clientSocket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
	connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(OnSocketStateChanged(QAbstractSocket::SocketState)));
	int number = 0;
	do
	{
		number = QRandomGenerator::global()->bounded(100000, 200000);
	} while (!CheckIfIdIsAvailable(number));

	Host host(clientSocket, "", number);

	for (QList<Host>::Iterator it = Hosts.begin(); it < Hosts.end(); ++it)
	{
		SendPacket(clientSocket, ID_SEND, it->GetName() + "(" + QString::number(it->GetId()) + ")" );
	}

	Hosts.push_back(host);
}

bool Server::CheckIfIdIsAvailable(int id)
{
	bool retState = true;
	for (QList<Host>::Iterator it = Hosts.begin(); it < Hosts.end(); ++it)
	{
		if (it->GetId() == id)
		{
			retState = false;
		}
	}
	return retState;
}

void Server::SendPacket(QTcpSocket *socket, int code, QString data)
{
	if (socket->isWritable())
	{
		Message msg;
		msg.code = code;
		msg.data = data.toStdString();
		socket->write(reinterpret_cast<char*>(&msg), sizeof(msg));
		socket->waitForBytesWritten();
	}
}

void Server::OnSocketStateChanged(QAbstractSocket::SocketState socketState)
{
	if (socketState == QAbstractSocket::UnconnectedState)
	{
		QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
		for (QList<Host>::Iterator it = Hosts.begin(); it < Hosts.end(); ++it)
		{
			if (it->GetSocketHandler() == sender)
			{
				int dist = it - Hosts.begin();
				Hosts.removeAt(dist);
			}
		}
	}
}

void Server::OnReadyRead()
{
	QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
	QByteArray data = sender->readAll();
	Message *msg = reinterpret_cast<Message*>(data.data());

	if (msg->code == MESSAGE_SEND)
	{
		for (QList<Host>::Iterator it = Hosts.begin(); it < Hosts.end(); ++it)
		{
			QTcpSocket *receiver = it->GetSocketHandler();
			if (receiver != sender)
			{
				SendPacket(receiver, MESSAGE_SEND, data);
			}
		}
	}
	else if (msg->code == NAME_SEND)
	{
		Message msgBack;
		int number = 0;
		for (QList<Host>::Iterator it = Hosts.begin(); it < Hosts.end(); ++it)
		{
			QTcpSocket *receiver = it->GetSocketHandler();
			if (receiver == sender)
			{
				number = it->GetId();
				it->SetName(QString::fromStdString(msg->data));
			}
		}

		if (number != 0)
		{
			for (QList<Host>::Iterator it = Hosts.begin(); it < Hosts.end(); ++it)
			{
				QTcpSocket* receiver = it->GetSocketHandler();
				if (receiver != sender)
				{
					SendPacket(receiver, ID_SEND, QString::fromStdString(msg->data) + "(" + QString::number(number) + ")");
				}
			}
		}
	}

}

Server::~Server()
{
}
