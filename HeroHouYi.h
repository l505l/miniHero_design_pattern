//#ifndef __HEROHouYi_H__
//#define __HEROHouYi_H__
//
//#include"Hero.h"
//#include"HelloWorldScene.h"
//#include"dragableSprite.h"
//
//class Equipment;
//class ExpComponent;
//class StateComponent;
//class Record;
//
//USING_NS_CC;
//
//class HeroHouYi :public Hero
//{
//	Label* label;
//public:
//	static HeroHouYi* create(Ecamp camp, Ref* scene);//����Ӣ��;
//	bool init(Ecamp camp, Ref* scene);
//	void initWalkingAnimation(Vec2 destination);
//	void decideToAttack();
//	void performAttack();
//	CallFunc* fireArrow();
//	void releaseSkill();
//	void upDateMoving();
//	void upGrade();
//	void displayHeroLevel(int level);
//};
//
//
//#endif
//
#ifndef __HEROHouYi_H__
#define __HEROHouYi_H__

#include"Hero.h"
#include"HelloWorldScene.h"
//#include"dragableSprite.h"

class Equipment;
class ExpComponent;
class StateComponent;
class Record;

USING_NS_CC;

class HeroHouYi :public Hero
{
	//Label* label;
public:
	static HeroHouYi* create(Ecamp camp, Ref* scene);//����Ӣ��;
	bool init(Ecamp camp, Ref* scene);
	void initWalkingAnimation(Vec2 destination);
	void decideToAttack();
	void performAttack();
	CallFunc* fireArrow();
	void releaseSkill();
	void upDateMoving();
	void upGrade();
	void displayHeroLevel(int level);
	// 具体英雄类完整实现clone方法
	virtual Hero* clone() const override;
};


#endif
