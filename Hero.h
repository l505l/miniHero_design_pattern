/* Refactored by Prototype Pattern */
#ifndef __HERO_H__
#define __HERO_H__

#include "dragableSprite.h"
#include "HP.h"
USING_NS_CC;

// The Hero class serves as the prototype interface
class Hero : public dragableSprite {
protected:
	// 原有属性
	CC_SYNTHESIZE(Label*, label, Lable);
	CC_SYNTHESIZE(Ecamp, _camp, Camp);
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

	// clone作为纯虚函数，由具体英雄类完成完整的克隆实现
    virtual Hero* clone() const = 0;

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

// 原型管理器
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
            // ... (Yase and Houyi initialization)

            // Create and configure red team prototypes
            auto dajiRed = new HeroDaJi();
            dajiRed->setCamp(RED);
            for (int i = 1; i < level; i++) dajiRed->upGrade();
            registerHero(DAJIDAJI, RED, level, dajiRed);
            
            // Similar initialization for other red team heroes
            // ... (Yase and Houyi initialization)
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