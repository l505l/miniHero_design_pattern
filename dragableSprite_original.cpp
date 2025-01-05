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
	// Set starting position
	setDragStart(this->getPosition());
	
	// Calculate offset between touch point and sprite position
	touchOffset = touch->getLocation() - this->getPosition();

	// Return true if sprite is dragable and touch point is within sprite bounds
	return _dragable && getBoundingBox().containsPoint(touch->getLocation());
}

void dragableSprite::onTouchMoved(Touch* touch, Event* event) {
	// Move sprite to new position while maintaining touch offset
	this->setPosition(touch->getLocation() - touchOffset);
}

void dragableSprite::onTouchEnded(Touch* touch, Event* event) {
	// Handle logic when touch ends
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

// Add touch listener to event dispatcher
void dragableSprite::addTouchListener()
{
	//scheduleupdate();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(dragableSprite::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(dragableSprite::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(dragableSprite::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}
