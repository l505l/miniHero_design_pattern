#pragma once
#ifndef __COMMAND_H__
#define __COMMAND_H__
#include"cocos2d.h"
#include<string>
class Command
{
public:
	void sendHeroMessage(std::string Message);
	void reset();
	void recvHeroMessage();
	std::string HeroMessage;
};

#endif // !__COMMAND_H__
