/* Refactored by Strategy Pattern */
#ifndef __AIMINIHERO_H__
#define __AIMINIHERO_H__

#include "Hero.h"
#include "cocos2d.h"
#include <vector>
#include <memory>
#include <string>

USING_NS_CC;

// basic data structure
struct BattleStats {
	int wins;
	int losses;
	int currentStreak;
	float winRate;
};

struct HeroMessage {
	std::string name;
	int power;
	HeroType type;
};

// Strategy Pattern: Interface defining common behavior for all AI strategies
class IAiBehaviorStrategy {
public:
	// Pure virtual methods to be implemented by concrete strategies
	virtual void initialize(AiminiHero* hero) = 0;
	virtual void preStageLogic(AiminiHero* hero, bool& isAdd) = 0;
	virtual void afterStageLogic(AiminiHero* hero) = 0;
	virtual ~IAiBehaviorStrategy() = default;
};

// Strategy Pattern: Concrete strategy implementing aggressive AI behavior
class AggressiveStrategy : public IAiBehaviorStrategy {
public:
	// Aggressive initialization logic
	void initialize(AiminiHero* hero) override {
		// Set aggressive initial stats and heroes
	}
	
	// Aggressive pre-stage behavior implementation
	void preStageLogic(AiminiHero* hero, bool& isAdd) override {
		// Implement aggressive tactics
	}
};

// Strategy Pattern: Context class that uses strategies
class AiminiHero : public Hero {
private:
	// Strategy reference - allows dynamic strategy switching
	std::unique_ptr<IAiBehaviorStrategy> _strategy;
	
	// Game state variables
	BattleStats _battleStats;
	std::vector<HeroMessage> _heroesInBattle;
	int _money;
	int _exp;
	int _level;

public:
	// Constructor and strategy setter
	explicit AiminiHero(int AiID);
	
	// Strategy Pattern: Method to change behavior at runtime
	void setStrategy(std::unique_ptr<IAiBehaviorStrategy> strategy) {
		if (strategy) {
			_strategy = std::move(strategy);
			// Initialize with new strategy
			_strategy->initialize(this);
		}
	}
	
	// Game update method - delegates to current strategy
	void update(float dt) {
		if (_strategy) {
			bool isAdd = false;
			// Strategy Pattern: Delegate behavior to strategy object
			_strategy->preStageLogic(this, isAdd);
		}
	}

	// Battle result handling - uses current strategy
	void onBattleResult(bool isWin) {
		updateBattleStats(isWin);
		if (_strategy) {
			// Strategy Pattern: Strategy handles battle outcomes
			_strategy->afterStageLogic(this);
		}
	}

	// Getters and setters for game state
	int getMoney() const { return _money; }
	void setMoney(int value) { _money = value; }
};

#endif




