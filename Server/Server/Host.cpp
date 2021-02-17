#include "Host.h"

Host::Host() 
{

}

Host::Host(const Host& obj)
{
	m_id = obj.m_id;
	Name = obj.Name;
	SocketHandler = obj.SocketHandler;
	Channel = obj.Channel;
}

Host::Host(QTcpSocket* socketHandler, QString name, int id) :
	SocketHandler(socketHandler),
	Name(name),
	Id(id),
	Channel(0)
{}

Host::~Host()
{

}

QTcpSocket* Host::GetSocketHandler()
{
	return SocketHandler;
}
int Host::GetId()
{
	return Id;
}
QString Host::GetName()
{
	return Name;
}
void Host::SetName(QString name)
{
	Name = name;
}
void Host::SetSocket(QTcpSocket* socket)
{
	SocketHandler = socket;
}
void Host::SetId(int id)
{
	Id = id;
}
