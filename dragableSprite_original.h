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
	// Tag currently has no specific meaning, can be inherited and modified
	CC_SYNTHESIZE(int, _tag, Tag);

	// Starting point for dragging
	CC_SYNTHESIZE(Vec2, _dragStart,DragStart);
	// Flag indicating if sprite can be dragged
	CC_SYNTHESIZE(bool,_dragable, Dragable);

	CC_SYNTHESIZE(Ref*, _presentScene, PresentScene);

	// Add touch listener
	void addTouchListener();
protected:
	// Touch event callback functions, currently only basic implementation
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	Vec2 touchOffset;  // Store the offset between touch point and sprite's bottom-left corner

public:
	static dragableSprite* create(const std::string& filename, int tag,Ref* presentScene);
};

#endif // !Actor_h