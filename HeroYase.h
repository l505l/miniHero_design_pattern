#ifndef __HEROHouYi_H__
#define __HEROHouYi_H__

#include "Hero.h"
#include "HelloWorldScene.h"

// Bridge Pattern: 具体英雄类
// HeroHouYi 继承自 Hero 类，作为抽象部分的具体实现
// 通过继承获得了与状态栏（实现部分）的桥接能力
class HeroYase : public Hero {
public:
	// Bridge Pattern: 工厂方法
	// 创建英雄实例时会同时初始化相关的状态栏实现
	static HeroYase* create(Ecamp camp, Ref* scene);
	
	// Bridge Pattern: 初始化方法
	// 在这里建立与具体状态栏实现的关联
	bool init(Ecamp camp, Ref* scene);

	// 英雄特有的行为方法
	// 这些方法可能会使用到桥接的状态栏实现
	void initWalkingAnimation(Vec2 destination);
	void decideToAttack();
	void performAttack();
	CallFunc* fireArrow();
	void releaseSkill();
	void upDateMoving();
	void upGrade();
	void displayHeroLevel(int level);
	CallFunc* hitTarget();
	// 具体英雄类完整实现clone方法
	virtual Hero* clone() const override;
};

#endif
