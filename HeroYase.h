#ifndef __HEROMinato_H__
#define __HEROMinato_H__

#include"Hero.h"
#include"HelloWorldScene.h"
#include"dragableSprite.h"

class Equipment;
class ExpComponent;
class StateComponent;
class Record;

USING_NS_CC;

class HeroYase :public Hero
{
	//Label* label;
public:
	static HeroYase* create(Ecamp camp, Ref* scene);//����Ӣ��;
	bool init(Ecamp camp, Ref* scene);
	void initWalkingAnimation(Vec2 destination);
	//void decideToAttack();
	void performAttack();
	//CallFunc* fireArrow();
	//void releaseSkill();
	void upDateMoving();
	void SkillLogic();
	void upGrade();
	void displayHeroLevel(int level);
	CallFunc* hitTarget();
	// 具体英雄类完整实现clone方法
	virtual Hero* clone() const override;
};



#endif

