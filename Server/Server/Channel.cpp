#include "Channel.h"

Channel::Channel() 
{
	m_conversationId = 0;
}

Channel::Channel(int id): m_conversationId(id)
{

}

Channel::~Channel()
{
	for (QVector<Host*>::iterator it = m_hostsVector.begin(); it < m_hostsVector.end(); ++it)
	{
		(*it) = nullptr;
	}
}

void Channel::AddHosts(Host *host)
{
	m_hostsVector.push_back(host);
	qInfo() << __FUNCTION__ + QString(" - Host added to Channel %1!").arg(m_conversationId);
}

void Channel::AddMessageToConversation(QString name, QString message)
{
	m_conversation.push_back(name +": "+ message);
	qInfo() << __FUNCTION__ + QString(" - Message added to Channel! %1").arg(m_conversationId);
}

QVector<QString>& Channel::GetMessages()
{
	qInfo() << __FUNCTION__ + QString(" - conversation of the Channel %1 returned!").arg(m_conversationId);
	return m_conversation;
}

bool Channel::CheckHost(QTcpSocket *socket, QTcpSocket *socket_2)
{
	bool firstSocketFound = false;
	bool secondSocketFound = false;
	for (QVector<Host*>::iterator it = m_hostsVector.begin(); it < m_hostsVector.end(); ++it)
	{
		if (socket == (*it)->GetSocketHandler())
		{
			firstSocketFound = true;
		}
		else if (socket_2 == (*it)->GetSocketHandler())
		{
			secondSocketFound = true;
		}
	}
	if (firstSocketFound && secondSocketFound)
	{
		qInfo() << __FUNCTION__ + QString(" - host exists in that Channel %1").arg(m_conversationId);
	}
	else
	{
		qInfo() << __FUNCTION__ + QString(" - host doesn't exists in that Channel %1").arg(m_conversationId);
	}
	return firstSocketFound && secondSocketFound;
}

bool Channel::CheckHost(QTcpSocket *socket, int id)
{
	bool firstSocketFound = false;
	bool secondSocketFound = false;
	for (QVector<Host*>::iterator it = m_hostsVector.begin(); it < m_hostsVector.end(); ++it)
	{
		if (socket == (*it)->GetSocketHandler())
		{
			firstSocketFound = true;
		}
		else if ((*it)->GetId() == id)
		{
			secondSocketFound = true;
		}
	}
	if (firstSocketFound && secondSocketFound)
	{
		qInfo() << __FUNCTION__ + QString(" - host exists in that Channel %1").arg(m_conversationId);
	}
	else
	{
		qInfo() << __FUNCTION__ + QString(" - host doesn't exists in that Channel %1").arg(m_conversationId);
	}
	return firstSocketFound && secondSocketFound;
}

bool Channel::CheckHost(Host* host)
{
	bool bContains = false;
	if (m_hostsVector.contains(host))
	{
		bContains = true;
	}
	return bContains;
}

QTcpSocket* Channel::GetReceiver(QTcpSocket *sender)
{
	QTcpSocket *receiver = nullptr;
	for (QVector<Host*>::iterator it = m_hostsVector.begin(); it < m_hostsVector.end(); ++it)
	{
		if (sender != (*it)->GetSocketHandler())
		{
			qInfo() << __FUNCTION__ + QString(" - Found pointer to receiver, Channel %1").arg(m_conversationId);
			receiver = (*it)->GetSocketHandler();
			break;
		}
	}
	if (receiver == nullptr)
	{
		qInfo() << __FUNCTION__ + QString(" - receiver is nullptr, Channel %1").arg(m_conversationId);
	}
	return receiver;
}

int Channel::GetId()
{
	return m_conversationId;
}