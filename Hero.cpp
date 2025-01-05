/* Refactored by Bridge Pattern */
#include"Hero.h"
#include"HelloWorldScene.h"
std::string msg = "has reached its maximum";
void showmessage(Scene* scene, const std::string& msg) {
	Label* message = Label::createWithSystemFont(msg, "Arial", 28);
	//Label* message = Label::createWithTTF("msg", "fonts/Mark Felt.ttf", 14);
	scene->addChild(message);
	message->setPosition(960 / 2, 300);
	std::function<void(float)> showmessage = [message, scene](float) {
		scene->unschedule("showmsg");
		message->setVisible(false);
	};
	scene->schedule(showmessage, 1.0f, "showmsg");
}

void Hero::updateAttackTarget()
{
	Hero* tmpTarget = NULL;
	float MinDistance = INT_MAX;
	std::vector<Hero* > AllHeroes = dynamic_cast<HelloWorld*>(_presentScene)->_heroes;
	for (auto it = AllHeroes.begin(); it != AllHeroes.end(); ++it)
	{
		// Check if target is: different camp, not dead, in attack range, visible
		if (this->getCamp() != (*it)->getCamp() && !(*it)->getIsDead())
		{
			if ((*it)->getPosition().distance(this->getPosition()) <= MinDistance) // Get closest target
			{
				MinDistance = (*it)->getPosition().distance(this->getPosition());
				tmpTarget = *it;
			}
		}
	}
	if (!tmpTarget)
	{
		CCLOG("Target find error");
	}
	this->setAttackTarget(tmpTarget);
}

void Hero::onTouchEnded(Touch* touch, Event* event) {
	HelloWorld* scene = dynamic_cast<HelloWorld*>(_presentScene);
	if (scene != nullptr) {
		miniHero* mmHero = scene->myMiniHero;
		std::vector<mapPoint> Battle = mmHero->Grid_Battle;
		mapPoint* landPoint = scene->myMiniHero->landOn(touch->getLocation());
		mapPoint* startPoint = scene->myMiniHero->landOn(_dragStart);

		// If landing point exists and is empty
		if (landPoint && landPoint->_phero == nullptr) {
			if (landPoint->tag == BATTLE) {
				if (!this->getIsOnTheStage()) {
					int cnt = 0;
					for (auto it = Battle.begin(); it != Battle.end(); it++) {
						if ((*it)._phero != nullptr)
							cnt++;
					}
					if (cnt >= mmHero->getMaxOnBoard()) {
						setPosition(Vec2(startPoint->_x, startPoint->_y));
						startPoint->_phero = this;
						showmessage(scene, msg);
						return;
					}
				}
				_isOnTheStage = true;
				landPoint->_phero = this;
				setPosition(Vec2(landPoint->_x, landPoint->_y));
			}
			if (landPoint->tag == WAIT) {
				_isOnTheStage = false;
				landPoint->_phero = this;
				setPosition(Vec2(landPoint->_x, landPoint->_y));
				scene->myMiniHero->upGrade(this);
			}
			if (landPoint->tag == DELETE) {
				mmHero->setMoney(mmHero->getMoney() + getLv() * 1);
				removeFromParent();
			}
		}
		// If landing point occupied, return to original position
		else {
			setPosition(Vec2(startPoint->_x, startPoint->_y));
			startPoint->_phero = this;
		}
	}
};

bool Hero::onTouchBegan(Touch* touch, Event* event)
{
	// If draggable and touch point is within bounding box
	if (_dragable && getBoundingBox().containsPoint(touch->getLocation())) {
		// Record starting position
		setDragStart(this->getPosition());
		// Get offset between touch point and sprite position
		touchOffset = touch->getLocation() - this->getPosition();
		HelloWorld* scene = dynamic_cast<HelloWorld*>(_presentScene);

		if (scene != nullptr) {
			mapPoint* landPoint = scene->myMiniHero->landOn(this->getPosition());
			if (landPoint) {
				landPoint->_phero = nullptr;
			}
		}
	}
	// Return true if touch is valid for this sprite
	return _dragable && getBoundingBox().containsPoint(touch->getLocation());
}

bool Hero::operator==(const Hero& hero1)
{
	return _tag == hero1._tag && _lv == hero1._lv;
}

// Bridge Pattern: Implementation of state bar management methods
void Hero::addStateBar(StateBar* stateBar) {
	// Bridge Pattern: Connection point between abstraction and implementation
	// Associates concrete state bar implementation (StateBar) with hero class (Hero) through composition
	if (stateBar) {
		// Store state bar object in map, establishing type-to-instance mapping
		_stateBars[stateBar->getType()] = stateBar;
		
		// Add state bar as child node to hero object
		this->addChild(stateBar);
		
		// Bridge Pattern: Concrete positioning logic in implementation
		// Set different display positions based on state bar type
		// Demonstrates independent variation of implementation part
		stateBar->setPosition(Vec2(this->getContentSize().width / 2, 
			this->getContentSize().height + (stateBar->getType() == "energy" ? -10 : 0)));
	}
}

// Bridge Pattern: Interface method to get specific type of state bar
StateBar* Hero::getStateBar(const std::string& type) {
	// Look up and return corresponding state bar implementation from map
	// This implementation decouples state bar access from concrete implementation
	auto it = _stateBars.find(type);
	return it != _stateBars.end() ? it->second : nullptr;
}