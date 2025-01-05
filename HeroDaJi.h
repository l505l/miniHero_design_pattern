/* Refactored by Bridge Pattern */
#ifndef __HERODaJi_H__
#define __HERODaJi_H__

#include "Hero.h"
#include "HelloWorldScene.h"

// Bridge Pattern: Concrete Hero Class
// HeroDaJi inherits from Hero class as concrete implementation of abstraction
// Gains bridging capability with status bars (implementation part) through inheritance
class HeroDaJi : public Hero {
public:
	// Bridge Pattern: Factory Method
	// Initializes related status bar implementations when creating hero instance
	static HeroDaJi* create(Ecamp camp, Ref* scene);
	
	// Bridge Pattern: Initialization Method
	// Establishes association with concrete status bar implementations here
	bool init(Ecamp camp, Ref* scene);

	// Hero-specific behavior methods
	// These methods may use bridged status bar implementations
	void initWalkingAnimation(Vec2 destination);
	void decideToAttack();
	void performAttack();
	CallFunc* fireArrow();
	void releaseSkill();
	void upDateMoving();
	void upGrade();
	void displayHeroLevel(int level);
	// Concrete hero class fully implements clone method
	virtual Hero* clone() const override;
};

#endif
