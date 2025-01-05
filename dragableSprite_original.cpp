#include "dragablesprite.h"

dragableSprite* dragableSprite::create(const std::string& filename, int tag,Ref* presentScenen)
{
	dragableSprite* sprite = new (std::nothrow)dragableSprite();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->setTag(tag);
		sprite->setPresentScene(presentScenen);
		sprite->setDragable(true);
		sprite->addTouchListener();
		sprite->setAnchorPoint(Vec2(0.5, 0.5));
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool dragableSprite::onTouchBegan(Touch* touch, Event* event)
{

	//保存开始位置
	setDragStart(this->getPosition());
	// 获取触摸点相对于精灵左下角的偏移量
	//MAP::Takeup(touch->getLocation());

	touchOffset = touch->getLocation() - this->getPosition();

	// 如果触摸点在精灵区域内，则认为触摸事件被处理
	return _dragable && getBoundingBox().containsPoint(touch->getLocation());
}

void dragableSprite::onTouchMoved(Touch* touch, Event* event) {
	// 移动精灵到新的位置，考虑触摸点的偏移量
	this->setPosition(touch->getLocation() - touchOffset);
}

void dragableSprite::onTouchEnded(Touch* touch, Event* event) {
	// 在这里可以处理触摸结束时的逻辑
	//int index = -1;
	//Vec2 dest=MAP::CanDrop(touch->getLocation(),index);
	//if (dest != Vec2(-1, -1)) {
	//	this->setPosition(dest);
	//  setDragStart(dest);
	//}
	//else {
	//	MAP::CanDrop(getDragStart(),index);
	//	this->setPosition(getDragStart());
	//}
}

//把监听器给到dispatcher
void dragableSprite::addTouchListener()
{
	//scheduleupdate();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(dragableSprite::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(dragableSprite::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(dragableSprite::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

