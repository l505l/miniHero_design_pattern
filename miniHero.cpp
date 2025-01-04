#include"miniHero.h"
#include"const.h"
#include"HeroDaJi.h"
#include"HeroYase.h"
#include"HeroHouYi.h"
#include "SimpleAudioEngine.h"
void miniHero:: initData(Ecamp Camp)
{
	_camp = Camp;
	_money = 20;
	_exp = 0;
	_hpLim = MINIhp;
	_dragable = true;  //dragable即能不能动的标签
	_isDead = false;
	_isHurt = false;
	_lv = 1;
	_maxOnBoard = _lv + 2;
	setDir(-1);
	_spd = MINISPEED;
}

void miniHero::generateGrid(std::vector<mapPoint>& Grid, Vec2 origin, Vec2 range, Vec2 rowcol, Vec2 dir, double rad) {

	double disx = range.x / rowcol.y;
	double disy = range.y / rowcol.x;

	double _rad = std::min(disx / 2, disy / 2);
	if(rowcol.y!=1)
		disx = (range.x - 2 * _rad) / (rowcol.y - 1);
	if (rowcol.x != 1)
		disy = (range.y - 2 * _rad) / (rowcol.x - 1);
	
	origin.x -= dir.x * (disx - _rad);
	origin.y -= dir.y * (disy - _rad);

	if (!rad)
		rad = _rad;
	Grid.clear();
	for (int i = 1; i <= rowcol.x; i++) {
		for (int j = 1; j <= rowcol.y; j++) {
			Grid.push_back(mapPoint(origin.x + dir.x * j * disx, origin.y + dir.y * i * disy, rad));
		}
	}
}

void miniHero::initGrid() {
	generateGrid(Grid_Battle, myBattleOrigin, myBattleRange, myBattleRowcol, myDir);
	generateGrid(Grid_Wait, myWaitOrigin, myWaitRange, myWaitRowcol, myDir);
	generateGrid(opGrid_Battle, opBattleOrigin, opBattleRange, opBattleRowcol, opDir);
}

bool miniHero::onTouchBegan(Touch* t, Event* e)
{
	if (!_dragable)
		return false;
	this->stopAllActions();
	double dis = this->getPosition().getDistance(t->getLocation());
	auto moveto = MoveTo::create(dis / _spd, t->getLocation());
	this->runAction(moveto);
	return true;
}

mapPoint* miniHero::getEmptyWait() {
	for (auto it = Grid_Wait.begin(); it != Grid_Wait.end(); it++) {
		if (it->_phero == nullptr)
			return &(*it);
	}
	return nullptr;
}

mapPoint* miniHero::landOn(Vec2 vec)
{
	//战斗区，Grid_Battle
	for (auto it = Grid_Battle.begin(); it != Grid_Battle.end(); it++) {
		if (vec.getDistance(Vec2(it->_x, it->_y)) < it->_rad) {
			it->tag = BATTLE;
			return &(*it);
		}
	}

	//备战区，Grid_Wait
	for (auto it = Grid_Wait.begin(); it != Grid_Wait.end(); it++) {
		if (vec.getDistance(Vec2(it->_x, it->_y)) < it->_rad) {
			it->tag = WAIT;
			return &(*it);
		}
	}

	//删除区
	if (vec.x > DleteOrigin.x && vec.x< DleteOrigin.x + DleteRange.x &&
		vec.y>DleteOrigin.y && vec.y < DleteOrigin.y + DleteRange.y
		)
	{
		Delete.tag = DELETE;
		return &Delete;
	}

	return nullptr;
}

miniHero* miniHero::create(const std::string & filename, Ecamp camp, twoStateType type)
	{
		miniHero* sprite = new (std::nothrow)miniHero;
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->initData(camp);
			sprite->addTouchListener();
			sprite->setAnchorPoint(Vec2(0.5, 0.5));
			sprite->setScale(0.25);//硬编码
			//血条
			sprite->_hpBar = HP::create(HEALTH, MINIhp, 0,camp);
			//sprite->getHpBar()->setScale(0.1);
			sprite->_energyBar = HP::create(ENERGY, MINIhp, 0, camp);

			std::function<void(float)> upDateMiniHpPos = [sprite](float) {
				if (sprite->getHpBar() != nullptr)
				{
					sprite->getHpBar()->setPosition(Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height + 10));
					sprite->getEnergyBar()->setPosition(Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height + 0));
				}
			};

			sprite->schedule(upDateMiniHpPos, 0.001f, "upDateMiniHpPos");

			//sprite->_hpBar->setScaleY(5);//硬编码
			//sprite->_hpBar->setScaleX(5);//硬编码

			sprite->addChild(sprite->_hpBar);
			sprite->addChild(sprite->_energyBar);
			sprite->_hpBar->setPosition(Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height + 10));
			sprite->Delete._phero = nullptr;

			//英雄格点
			sprite->initGrid();
			sprite->autorelease();

			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

void miniHero::upGrade(Hero* phero) {
	int cnt=0;
	for (auto it = Grid_Wait.begin(); it != Grid_Wait.end(); it++) {
		if (it->_phero != nullptr) {
			if (*(it->_phero) == (*phero))
				cnt++;
		}
	}
	if (cnt >= 3) {
		cnt=0;
		Hero* p;
		for (auto it = Grid_Wait.begin(); it != Grid_Wait.end(); it++) {
			if (it->_phero != nullptr && *(it->_phero) == (*phero)) {
				cnt++;
				if (cnt == 1) {
					if (it->_phero->getCamp() == RED) {
						auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
						audio->playEffect("Upgrade.mp3", false);
						audio->setEffectsVolume(1.0f);
					}
					//升级逻辑
					if (it->_phero->getTag() == DAJIDAJI)
					{
						dynamic_cast<HeroDaJi*>(it->_phero)->upGrade();
						//it->_phero->setLv(it->_phero->getLv() + 1);
						p = it->_phero;
					}
					else if (it->_phero->getTag() == YASEYASE)
					{
						dynamic_cast<HeroYase*>(it->_phero)->upGrade();
						//it->_phero->setLv(it->_phero->getLv() + 1);
						p = it->_phero;
					}
					else if (it->_phero->getTag() == HOUYIHOUYI)
					{
						dynamic_cast<HeroHouYi*>(it->_phero)->upGrade();
						//it->_phero->setLv(it->_phero->getLv() + 1);
						p = it->_phero;
					}
					///yase、houyi同理
				}
				else {
					it->_phero->removeFromParent();
					it->_phero = nullptr;
				}
			}
		}
		upGrade(p);
	}
	else
		return;
}

std::string miniHero::generateMessage()
{
	std::string message;
	int i = 0;
	for (auto it = Grid_Battle.begin(); it != Grid_Battle.end(); it++)
	{
		if (it->_phero)
		{
			message += it->_phero->getTag() + '0';
			message += it->_phero->getLv() + '0';
		}
		else
		{
			message += "00";
		}
	}
	return message;
}

void miniHero:: Flip() {
	this->setScaleX(-this->getScaleX());
	this->getHpBar()->setScaleX(-this->getHpBar()->getScaleX());
//	this->getEnergyBar()->setScaleX(-this->getEnergyBar()->getScaleX());
	//this->label->setScaleX(-this->label->getScaleX());
	_dir = -_dir;
}