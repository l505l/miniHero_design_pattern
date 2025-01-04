#ifndef __HERO_H__
#define __HERO_H__

#include"dragableSprite.h"
#include"HP.h"
USING_NS_CC;

class Hero :public dragableSprite
{
	CC_SYNTHESIZE(Label*, label, Lable);
	CC_SYNTHESIZE(Ecamp, _camp, Camp);//��Ӫ;
	CC_SYNTHESIZE(bool, _isDead, IsDead);
	CC_SYNTHESIZE(bool, _isHurt, IsHurt);
	CC_SYNTHESIZE(bool, _isMoving, IsMoving);
	CC_SYNTHESIZE(INT32, _hpLim, HpLim);//��������;
	//CC_SYNTHESIZE(INT32, _hpCur, HpCur);
	CC_SYNTHESIZE(float, _spd, Spd);//Ӣ���ƶ��ٶ�;
	CC_SYNTHESIZE(INT32, _lv, Lv);//�ȼ�;
	CC_SYNTHESIZE(float, attackScope, AttackScope);//������Χ;
	CC_SYNTHESIZE(INT32, attackPower, AttackPower);//������;
	CC_SYNTHESIZE(INT32, skillPower, SkillPower);//���ܹ�����;
	CC_SYNTHESIZE(INT32, _energyLim, EnergyLim);
	//CC_SYNTHESIZE(INT32, _energyCur, energyCur);
	CC_SYNTHESIZE(bool, _isOnTheStage, IsOnTheStage);

	CC_SYNTHESIZE(INT32, energyRecoverRate, EnergyRecoverRate);
	CC_SYNTHESIZE(Hero*, _attackTarget, AttackTarget);//����Ŀ��;
	CC_SYNTHESIZE(HP*, _hpBar, HpBar);//����ֵ;
	CC_SYNTHESIZE(HP*, _energyBar, EnergyBar);//������;
	CC_SYNTHESIZE(int, _dir, Dir);//������;

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