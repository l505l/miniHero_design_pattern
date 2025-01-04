#ifndef __HERO_H__
#define __HERO_H__

#include"dragableSprite.h"
#include"HP.h"
USING_NS_CC;

class Hero :public dragableSprite
{
	CC_SYNTHESIZE(Label*, label, Lable);
	CC_SYNTHESIZE(Ecamp, _camp, Camp);//阵营;
	CC_SYNTHESIZE(bool, _isDead, IsDead);
	CC_SYNTHESIZE(bool, _isHurt, IsHurt);
	CC_SYNTHESIZE(bool, _isMoving, IsMoving);
	CC_SYNTHESIZE(INT32, _hpLim, HpLim);//健康上限;
	//CC_SYNTHESIZE(INT32, _hpCur, HpCur);
	CC_SYNTHESIZE(float, _spd, Spd);//英雄移动速度;
	CC_SYNTHESIZE(INT32, _lv, Lv);//等级;
	CC_SYNTHESIZE(float, attackScope, AttackScope);//攻击范围;
	CC_SYNTHESIZE(INT32, attackPower, AttackPower);//攻击力;
	CC_SYNTHESIZE(INT32, skillPower, SkillPower);//技能攻击力;
	CC_SYNTHESIZE(INT32, _energyLim, EnergyLim);
	//CC_SYNTHESIZE(INT32, _energyCur, energyCur);
	CC_SYNTHESIZE(bool, _isOnTheStage, IsOnTheStage);

	CC_SYNTHESIZE(INT32, energyRecoverRate, EnergyRecoverRate);
	CC_SYNTHESIZE(Hero*, _attackTarget, AttackTarget);//攻击目标;
	CC_SYNTHESIZE(HP*, _hpBar, HpBar);//健康值;
	CC_SYNTHESIZE(HP*, _energyBar, EnergyBar);//能量条;
	CC_SYNTHESIZE(int, _dir, Dir);//能量条;

	//CC_SYNTHESIZE(Vec2, _latestTargetPos, LatestTargetPos);
	//Sprite* mySprite;
	Sprite* myAttackSprite;
public:
	virtual bool onTouchBegan(Touch* touch, Event* event);

	virtual void onTouchEnded(Touch* touch, Event* event);

	bool operator==(const Hero& hero1);
	//virtual bool init();
	~Hero() {
		;
	}
	void updateAttackTarget();

	virtual void Flip() {
		this->setScaleX(-this->getScaleX());
		this->getHpBar()->setScaleX(-this->getHpBar()->getScaleX());
		this->getEnergyBar()->setScaleX(-this->getEnergyBar()->getScaleX());
		this->label->setScaleX(-this->label->getScaleX());
		_dir = -_dir;
	}
};


#endif