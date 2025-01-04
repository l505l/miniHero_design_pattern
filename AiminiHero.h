#ifndef __AIMINIHERO_H__
#define __AIMINIHERO_H__

#include "Hero.h"
#include "cocos2d.h"
#include <vector>
#include <memory>
#include <string>

USING_NS_CC;

// 基础数据结构
enum class AiType {
	AGGRESSIVE = 1,
	DEFENSIVE = 2,
	BALANCED = 3
};

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

// 策略接口
class IAiBehaviorStrategy {
public:
	virtual void initialize(class AiminiHero* hero) = 0;
	virtual void preStageLogic(AiminiHero* hero, bool& isAdd) = 0;
	virtual void afterStageLogic(AiminiHero* hero) = 0;
	virtual void onBattleEnd(AiminiHero* hero, bool isWin) = 0;
	virtual void adjustStrategy(AiminiHero* hero, const BattleStats& stats) = 0;
	virtual std::string getStrategyName() const = 0;
	virtual ~IAiBehaviorStrategy() = default;
};

// 具体策略类声明
class AggressiveStrategy : public IAiBehaviorStrategy {
public:
	void initialize(AiminiHero* hero) override;
	void preStageLogic(AiminiHero* hero, bool& isAdd) override;
	void afterStageLogic(AiminiHero* hero) override;
	void onBattleEnd(AiminiHero* hero, bool isWin) override;
	void adjustStrategy(AiminiHero* hero, const BattleStats& stats) override;
	std::string getStrategyName() const override;

private:
	bool shouldUpgrade(AiminiHero* hero);
	void performUpgrade(AiminiHero* hero);
	bool shouldBuyHero(AiminiHero* hero);
	void purchaseAttackHero(AiminiHero* hero);
	void optimizeFormation(AiminiHero* hero);
	void sellWeakHeroes(AiminiHero* hero);
	void adjustTeamComposition(AiminiHero* hero);
	void increaseBattleAggression(AiminiHero* hero);
	void adjustAfterLoss(AiminiHero* hero);
	void reduceAggression(AiminiHero* hero);
	void increaseAggression(AiminiHero* hero);
};

class DefensiveStrategy : public IAiBehaviorStrategy {
public:
	void initialize(AiminiHero* hero) override;
	void preStageLogic(AiminiHero* hero, bool& isAdd) override;
	void afterStageLogic(AiminiHero* hero) override;
	void onBattleEnd(AiminiHero* hero, bool isWin) override;
	void adjustStrategy(AiminiHero* hero, const BattleStats& stats) override;
	std::string getStrategyName() const override;

private:
	// 防守策略特有的私有方法
};

class BalancedStrategy : public IAiBehaviorStrategy {
public:
	void initialize(AiminiHero* hero) override;
	void preStageLogic(AiminiHero* hero, bool& isAdd) override;
	void afterStageLogic(AiminiHero* hero) override;
	void onBattleEnd(AiminiHero* hero, bool isWin) override;
	void adjustStrategy(AiminiHero* hero, const BattleStats& stats) override;
	std::string getStrategyName() const override;

private:
	// 平衡策略特有的私有方法
};

// AI英雄类
class AiminiHero : public Hero {
private:
	std::unique_ptr<IAiBehaviorStrategy> _strategy;
	BattleStats _battleStats;
	std::vector<HeroMessage> _heroesInBattle;
	std::vector<HeroMessage> _heroesOutBattle;
	
	int _money;
	int _exp;
	int _maxOnBoard;
	int _level;

public:
	explicit AiminiHero(int AiID);
	virtual ~AiminiHero() = default;

	// 策略相关方法
	void setStrategy(std::unique_ptr<IAiBehaviorStrategy> strategy);
	void update(float dt);

	// 战斗相关方法
	void onBattleResult(bool isWin);
	void updateBattleStats(bool isWin);

	// Getter/Setter方法
	int getMoney() const { return _money; }
	void setMoney(int value) { _money = value; }
	
	int getExp() const { return _exp; }
	void setExp(int value) { _exp = value; }
	
	int getMaxOnBoard() const { return _maxOnBoard; }
	void setMaxOnBoard(int value) { _maxOnBoard = value; }
	
	int getLevel() const { return _level; }
	void setLevel(int value) { _level = value; }

	const std::vector<HeroMessage>& getHeroesInBattle() const { return _heroesInBattle; }
	void setInitialHeroes(const std::vector<HeroMessage>& heroes) { _heroesInBattle = heroes; }
};

// 策略工厂
class AiStrategyFactory {
public:
	static std::unique_ptr<IAiBehaviorStrategy> createStrategy(AiType type);
};

#endif