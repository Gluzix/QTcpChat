#include "Channel.h"

Channel::Channel()
{
}


Channel::~Channel()
{
}


void Channel::ReadPendingMessages(Host *which)
{
	for (QVector<Host*>::iterator it = HostsVector.begin(); it < HostsVector.end(); ++it)
	{
		// Send messages to the host!
	}
}

void Channel::SendMessages()
{

}