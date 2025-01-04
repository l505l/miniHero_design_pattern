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

// Bridge Pattern: 状态栏管理相关方法实现
void Hero::addStateBar(StateBar* stateBar) {
	// Bridge Pattern: 实现部分与抽象部分的连接点
	// 通过组合方式将具体的状态栏实现(StateBar)与英雄类(Hero)关联
	if (stateBar) {
		// 将状态栏对象存储在映射表中，建立类型到实例的对应关系
		_stateBars[stateBar->getType()] = stateBar;
		
		// 将状态栏作为子节点添加到英雄对象中
		this->addChild(stateBar);
		
		// Bridge Pattern: 实现部分的具体定位逻辑
		// 根据状态栏类型设置不同的显示位置，展现了实现部分的独立变化
		stateBar->setPosition(Vec2(this->getContentSize().width / 2, 
			this->getContentSize().height + (stateBar->getType() == "energy" ? -10 : 0)));
	}
}

// Bridge Pattern: 获取特定类型状态栏的接口方法
StateBar* Hero::getStateBar(const std::string& type) {
	// 在映射表中查找并返回对应类型的状态栏实现
	// 这种实现方式使得状态栏的访问与具体实现解耦
	auto it = _stateBars.find(type);
	return it != _stateBars.end() ? it->second : nullptr;
}