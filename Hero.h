/* Refactored by Prototype Pattern and Bridge Pattern */
#ifndef __HERO_H__
#define __HERO_H__

#include"dragableSprite.h"
#include"HP.h"
#include"StateBar.h"
USING_NS_CC;

// The Hero class serves as the prototype interface
class Hero : public dragableSprite {
protected:
	// Original attributes
	CC_SYNTHESIZE(Label*, label, Lable);
	CC_SYNTHESIZE(Ecamp, _camp, Camp);
	CC_SYNTHESIZE(bool, _isDead, IsDead);
	CC_SYNTHESIZE(bool, _isHurt, IsHurt);
	CC_SYNTHESIZE(bool, _isMoving, IsMoving);
	CC_SYNTHESIZE(INT32, _hpLim, HpLim);// HP limit
	//CC_SYNTHESIZE(INT32, _hpCur, HpCur);
	CC_SYNTHESIZE(float, _spd, Spd);// Hero movement speed
	CC_SYNTHESIZE(INT32, _lv, Lv);// Level
	CC_SYNTHESIZE(float, attackScope, AttackScope);// Attack range
	CC_SYNTHESIZE(INT32, attackPower, AttackPower);// Attack power
	CC_SYNTHESIZE(INT32, skillPower, SkillPower);// Skill power
	CC_SYNTHESIZE(INT32, _energyLim, EnergyLim);
	//CC_SYNTHESIZE(INT32, _energyCur, energyCur);
	CC_SYNTHESIZE(bool, _isOnTheStage, IsOnTheStage);

	CC_SYNTHESIZE(INT32, energyRecoverRate, EnergyRecoverRate);
	CC_SYNTHESIZE(Hero*, _attackTarget, AttackTarget);// Attack target
	CC_SYNTHESIZE(HP*, _hpBar, HpBar);// Health bar
	CC_SYNTHESIZE(HP*, _energyBar, EnergyBar);// Energy bar
	CC_SYNTHESIZE(int, _dir, Dir);// Direction

	//CC_SYNTHESIZE(Vec2, _latestTargetPos, LatestTargetPos);
	//Sprite* mySprite;
	Sprite* myAttackSprite;

	// Bridge Pattern Implementation:
	// 1. Core Idea: Separate Hero class from StateBar class so they can vary independently
	//
	// 2. Data Structure:
	//    - Using std::map to store state bar objects
	//    - Key: string type, represents state bar type identifier (like "hp", "energy", etc.)
	//    - Value: StateBar pointer, points to concrete state bar object
	//
	// 3. Design Benefits:
	//    - Decoupling: Hero class doesn't need to know StateBar implementation details
	//    - Extensibility: Can easily add new state bar types without modifying Hero class
	//    - Dynamic: Can add, remove or modify state bars at runtime
	//    - Maintainability: State bar changes won't affect Hero class core logic
	//
	// 4. Use Cases:
	//    - Can display different types of status effects (like HP, energy, buffs, etc.)
	//    - Supports dynamic updates and management of these status displays during gameplay
	//
	// 5. Related Methods:
	//    - addStateBar(): Add new state bar
	//    - getStateBar(): Get state bar of specified type
	std::map<std::string, StateBar*> _stateBars;

public:

	// clone as pure virtual function, implemented by concrete hero classes
    virtual Hero* clone() const = 0;

//Bridge Pattern: Add state bar to hero object
	void addStateBar(StateBar* stateBar);
	StateBar* getStateBar(const std::string& type);

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

// Prototype Manager
class HeroPrototypeRegistry {
private:
    // Map to store prototypes using tuple<heroType, camp, level> as key
    // This allows quick access to specific hero configurations
    std::map<std::tuple<int, Ecamp, int>, std::unique_ptr<Hero>> prototypes;
    
    // Private constructor for singleton pattern
    HeroPrototypeRegistry() {
        // Initialize prototypes for all possible hero configurations
        for (int level = 1; level <= MAX_LEVEL; level++) {
            // Create and configure blue team prototypes
            auto dajiBlue = new HeroDaJi();
            dajiBlue->setCamp(BLUE);
            // Level up the prototype to desired level
            for (int i = 1; i < level; i++) dajiBlue->upGrade();
            registerHero(DAJIDAJI, BLUE, level, dajiBlue);
            
            // Similar initialization for other blue team heroes

            // Create and configure red team prototypes
            auto dajiRed = new HeroDaJi();
            dajiRed->setCamp(RED);
            for (int i = 1; i < level; i++) dajiRed->upGrade();
            registerHero(DAJIDAJI, RED, level, dajiRed);
            
            // Similar initialization for other red team heroes
        }
    }

public:
    // Singleton instance accessor
    static HeroPrototypeRegistry& getInstance() {
        static HeroPrototypeRegistry instance;
        return instance;
    }

    // Register a new hero prototype with specified configuration
    void registerHero(int heroType, Ecamp camp, int level, Hero* prototype) {
        prototypes[std::make_tuple(heroType, camp, level)] = std::unique_ptr<Hero>(prototype);
    }

    // Create a new hero by cloning the appropriate prototype
    // Returns nullptr if no matching prototype is found
    Hero* createHero(int heroType, Ecamp camp, int level) const {
        auto key = std::make_tuple(heroType, camp, level);
        auto it = prototypes.find(key);
        if (it != prototypes.end()) {
            return it->second->clone();
        }
        return nullptr;
    }
}; 


#endif