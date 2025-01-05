#pragma once
#ifndef __dragableSprite_h_
#define __dragableSprite_h_

#include <iostream>
#include <string>
#include <time.h>
#include"cocos2d.h"
USING_NS_CC;

class dragableSprite: public cocos2d::Sprite
{
	//标签，目前没有具体含义，可以继承后修改
	CC_SYNTHESIZE(int, _tag, Tag);

	//一次拖拽的起点
	CC_SYNTHESIZE(Vec2, _dragStart,DragStart);
	//可否拖动的标签
	CC_SYNTHESIZE(bool,_dragable, Dragable);

	CC_SYNTHESIZE(Ref*, _presentScene, PresentScene);

	//放置监听器
	void addTouchListener();
protected:
	//touch动作的三个回调函数，目前只用于
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	Vec2 touchOffset;  // 用于存储触摸点相对于精灵左下角的偏移量

public:
	static dragableSprite* create(const std::string& filename, int tag,Ref* presentScene);
};

#endif // !Actor_h