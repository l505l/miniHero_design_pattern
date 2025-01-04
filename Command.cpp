#include "Command.h"
#include"Net.h"
#include"HelloWorldScene.h"

void Command::sendHeroMessage(std::string Message)
{
	Net::getInstance()->Send(&Message[0], Message.length());
}

void Command::recvHeroMessage()
{
	char buf;
	while (HeroMessage.length()<=55)
	{
		if (Net::getInstance()->Recv(&buf, 1) != -1)
			HeroMessage += buf;
	}
}