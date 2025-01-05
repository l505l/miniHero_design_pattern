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
	//��ǩ��Ŀǰû�о��庬�壬���Լ̳к��޸�
	CC_SYNTHESIZE(int, _tag, Tag);

	//һ����ק�����
	CC_SYNTHESIZE(Vec2, _dragStart,DragStart);
	//�ɷ��϶��ı�ǩ
	CC_SYNTHESIZE(bool,_dragable, Dragable);

	CC_SYNTHESIZE(Ref*, _presentScene, PresentScene);

	//���ü�����
	void addTouchListener();
protected:
	//touch�����������ص�������Ŀǰֻ����
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	Vec2 touchOffset;  // ���ڴ洢����������ھ������½ǵ�ƫ����

public:
	static dragableSprite* create(const std::string& filename, int tag,Ref* presentScene);
};

#endif // !Actor_h