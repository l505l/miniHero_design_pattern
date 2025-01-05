
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
	//if (getAttackTarget() != NULL)
	//	setLatestTargetPos(getAttackTarget()->getPosition());//������һ�ε�����
	std::vector<Hero* > AllHeroes = dynamic_cast<HelloWorld*>(_presentScene)->_heroes;
	for (auto it = AllHeroes.begin(); it != AllHeroes.end(); ++it)
	{
		//��Ӫ��ͬ û������ �ڹ�����Χ�� �ɼ�
		if (this->getCamp() != (*it)->getCamp() && !(*it)->getIsDead())
		{
			if ((*it)->getPosition().distance(this->getPosition()) <= MinDistance) //ȡ���������
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
/*void Hero::onTouchEnded(Touch* touch, Event* event) {
	HelloWorld* scene = dynamic_cast<HelloWorld*>(_presentScene);
	if (scene != nullptr) {
		mapPoint* landPoint = scene->myMiniHero->landOn(touch->getLocation());
		mapPoint* startPoint = scene->myMiniHero->landOn(_dragStart);

		//����ĳ���������Ҹõ�Ϊ��
		if (landPoint && landPoint->_phero == nullptr) {
			if (landPoint->tag == BATTLE) {
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
				removeFromParent();
				//setPosition(Vec2(landPoint->_x, landPoint->_y));
			}
		}
		//�����ڣ�����ԭλ��
		else {
			setPosition(Vec2(startPoint->_x, startPoint->_y));
			startPoint->_phero = this;
		}
	}
};
*/

void Hero::onTouchEnded(Touch* touch, Event* event) {
	HelloWorld* scene = dynamic_cast<HelloWorld*>(_presentScene);
	if (scene != nullptr) {
		miniHero* mmHero = scene->myMiniHero;
		std::vector<mapPoint> Battle = mmHero->Grid_Battle;
		mapPoint* landPoint = scene->myMiniHero->landOn(touch->getLocation());
		mapPoint* startPoint = scene->myMiniHero->landOn(_dragStart);

		//����ĳ���������Ҹõ�Ϊ��
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
				//setPosition(Vec2(landPoint->_x, landPoint->_y));
			}
		}
		//�����ڣ�����ԭλ��
		else {
			setPosition(Vec2(startPoint->_x, startPoint->_y));
			startPoint->_phero = this;
		}
	}
};

bool Hero::onTouchBegan(Touch* touch, Event* event)
{
	//�������ק���ھ��������ڣ������߼�
	if (_dragable && getBoundingBox().containsPoint(touch->getLocation())) {
		//���濪ʼλ��
		setDragStart(this->getPosition());
		// ��ȡ����������ھ������½ǵ�ƫ����
		touchOffset = touch->getLocation() - this->getPosition();
		HelloWorld* scene = dynamic_cast<HelloWorld*>(_presentScene);

		if (scene != nullptr) {
			mapPoint* landPoint = scene->myMiniHero->landOn(this->getPosition());
			if (landPoint) {
				landPoint->_phero = nullptr;
			}
		}
	}
	// ����������ھ��������ڣ�����Ϊ�����¼�������
	return _dragable && getBoundingBox().containsPoint(touch->getLocation());
}

bool Hero::operator==(const Hero& hero1)
{
	return _tag == hero1._tag && _lv == hero1._lv;
}