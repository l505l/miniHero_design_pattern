#include"HP.h"

HP* HP::create(twoStateType Type, INT32 defaultState, INT32 defaultRecoverRate, Ecamp ccamp)
{
	HP* StateComp = new (std::nothrow) HP;
	if (StateComp && StateComp->init(Type, defaultState, defaultRecoverRate, ccamp))
	{
		//StateComp->autorelease();
		return StateComp;
	}
	CC_SAFE_DELETE(StateComp);
	return nullptr;
}


bool HP::init(twoStateType Type, INT32 defaultState, INT32 defaultRecoverRate, Ecamp ccamp)
{
	if (!LoadingBar::init())
	{
		return false;
	}

	loadTexture("slider2.png", TextureResType::LOCAL);
	setCcamp(ccamp);
	stateType = Type;
	maxState = defaultState;
	currentState = defaultState;
	recoverRate = defaultRecoverRate;
	setPercent(100);

	if (stateType == twoStateType::HEALTH)
	{
		if (getCcamp() == RED)
			setColor(Color3B(81, 205, 22));//血量
		else
			setColor(Color3B(255, 0, 0));
	}
	else if (stateType == twoStateType::ENERGY)
	{
		setColor(Color3B(39, 123, 221));//蓝条
	}

	auto background = Sprite::create("slider1.png");
	auto size = getContentSize();
	background->setPosition(size / 2);
	background->setOpacity(200);
	addChild(background, -1);

	//loadTexture->setZOrder(1);

	this->schedule(CC_SCHEDULE_SELECTOR(HP::energyRecover), 0.5f);

	return true;
}
void HP::energyRecover(float delta)
{
	CCLOG("energyRecover called with delta: %f", delta);
	if (this->stateType == ENERGY && currentState < maxState && currentState >= 0)
	{
		changeStateBy(recoverRate);
	}
}
void HP::changeStateBy(INT32 delta)
{
	if (currentState < 0)
	{
		return;
	}

	currentState += delta;

	if (currentState > maxState)
	{
		currentState = maxState;
	}
	else if (currentState < 0)
	{
		currentState = 0;
	}

	updatePercent();
}
void HP::changeMaxBy(INT32 delta)//更改HP上限;
{
	setMaxState(maxState + delta);
	if (delta > 0)
	{
		setCurrentState(currentState + delta);
	}
	if (maxState < 0)
	{
		maxState = 0;
	}
	if (currentState > maxState)
	{
		currentState = maxState;
	}
	updatePercent();
}
void HP::updatePercent()
{
	setPercent(currentState * 100.0f / maxState);
}

void HP::setFellow(Sprite*& sprite)
{

	this->setPosition(Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height + 10));

	sprite->addChild(this);
}