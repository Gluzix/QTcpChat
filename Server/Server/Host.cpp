#include "Host.h"

Host::Host() 
{
	m_id = 0;
	m_name = "";
	m_pSocketHandler = nullptr;
	m_channel = 0;
}

Host::Host(const Host& obj)
{
	m_id = obj.m_id;
	m_name = obj.m_name;
	m_pSocketHandler = obj.m_pSocketHandler;
	m_channel = obj.m_channel;
}

Host::Host(QTcpSocket* pSocketHandler, QString name, int id) :
	m_pSocketHandler(pSocketHandler),
	m_name(name),
	m_id(id),
	m_channel(0)
{}

Host::~Host()
{
	m_pSocketHandler = nullptr;
}

QTcpSocket* Host::GetSocketHandler()
{
	return m_pSocketHandler;
}
int Host::GetId()
{
	return m_id;
}
QString Host::GetName()
{
	return m_name;
}
void Host::SetName(QString name)
{
	m_name = name;
}
void Host::SetSocket(QTcpSocket* socket)
{
	m_pSocketHandler = socket;
}
void Host::SetId(int id)
{
	m_id = id;
}
