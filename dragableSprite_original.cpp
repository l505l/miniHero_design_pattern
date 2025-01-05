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

	//���濪ʼλ��
	setDragStart(this->getPosition());
	// ��ȡ����������ھ������½ǵ�ƫ����
	//MAP::Takeup(touch->getLocation());

	touchOffset = touch->getLocation() - this->getPosition();

	// ����������ھ��������ڣ�����Ϊ�����¼�������
	return _dragable && getBoundingBox().containsPoint(touch->getLocation());
}

void dragableSprite::onTouchMoved(Touch* touch, Event* event) {
	// �ƶ����鵽�µ�λ�ã����Ǵ������ƫ����
	this->setPosition(touch->getLocation() - touchOffset);
}

void dragableSprite::onTouchEnded(Touch* touch, Event* event) {
	// ��������Դ���������ʱ���߼�
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

//�Ѽ���������dispatcher
void dragableSprite::addTouchListener()
{
	//scheduleupdate();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(dragableSprite::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(dragableSprite::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(dragableSprite::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

