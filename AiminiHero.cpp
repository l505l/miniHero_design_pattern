#include"AiminiHero.h"
#include<ctime>
#include<vector>
#include<algorithm>
#include<iostream>
#include"const.h" 

// 1. AI行为策略接口
class IAiBehaviorStrategy {
public:
    virtual void initialize(AiminiHero* hero) = 0;
    virtual void preStageLogic(AiminiHero* hero, bool& isAdd) = 0;
    virtual void afterStageLogic(AiminiHero* hero) = 0;
    virtual void onBattleEnd(AiminiHero* hero, bool isWin) = 0;
    virtual void adjustStrategy(AiminiHero* hero, const BattleStats& stats) = 0;
    virtual std::string getStrategyName() const = 0;
    virtual ~IAiBehaviorStrategy() = default;
};

// 2. 具体策略类
class AggressiveStrategy : public IAiBehaviorStrategy {
public:
    void initialize(AiminiHero* hero) override {
        hero->setMoney(100);
        hero->setExp(0);
        hero->setMaxOnBoard(5);
        hero->setLevel(1);
        
        std::vector<HeroMessage> attackHeroes = {
            {"Warrior", 100, HeroType::ATTACKER},
            {"Assassin", 80, HeroType::ATTACKER},
            {"Berserker", 120, HeroType::ATTACKER}
        };
        hero->setInitialHeroes(attackHeroes);
    }

    void preStageLogic(AiminiHero* hero, bool& isAdd) override {
        // 激进策略：优先升级和购买攻击型英雄
        if (shouldUpgrade(hero)) {
            performUpgrade(hero);
            isAdd = true;
        }
        
        if (shouldBuyHero(hero)) {
            purchaseAttackHero(hero);
            isAdd = true;
        }
    }

    void afterStageLogic(AiminiHero* hero) override {
        optimizeFormation(hero);
        sellWeakHeroes(hero);
        adjustTeamComposition(hero);
    }

    void onBattleEnd(AiminiHero* hero, bool isWin) override {
        if (isWin) {
            increaseBattleAggression(hero);
        } else {
            adjustAfterLoss(hero);
        }
    }

    void adjustStrategy(AiminiHero* hero, const BattleStats& stats) override {
        if (stats.winRate < 0.3f) {
            reduceAggression(hero);
        } else if (stats.winRate > 0.7f) {
            increaseAggression(hero);
        }
    }

    std::string getStrategyName() const override {
        return "Aggressive";
    }

private:
    bool shouldUpgrade(AiminiHero* hero) {
        return hero->getMoney() >= 8 && hero->getLevel() < 5;
    }

    void performUpgrade(AiminiHero* hero) {
        hero->setMoney(hero->getMoney() - 4);
        hero->setLevel(hero->getLevel() + 1);
    }

    bool shouldBuyHero(AiminiHero* hero) {
        return hero->getMoney() >= 4 && 
               hero->getHeroesInBattle().size() < hero->getMaxOnBoard();
    }

    // ... 其他辅助方法实现
};

class DefensiveStrategy : public IAiBehaviorStrategy {
    // 类似AggressiveStrategy的实现，但侧重防守
};

class BalancedStrategy : public IAiBehaviorStrategy {
    // 平衡型策略实现
};

// 3. 重构后的AiminiHero类
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
    explicit AiminiHero(int AiID) 
        : _money(0), _exp(0), _maxOnBoard(0), _level(1) {
        _battleStats = {0, 0, 0, 0.0f};
    }

    void setStrategy(std::unique_ptr<IAiBehaviorStrategy> strategy) {
        if (strategy) {
            CCLOG("Changing strategy to: %s", strategy->getStrategyName().c_str());
            _strategy = std::move(strategy);
            _strategy->initialize(this);
        }
    }

    void update(float dt) {
        if (_strategy) {
            bool isAdd = false;
            _strategy->preStageLogic(this, isAdd);
            if (isAdd) {
                CCLOG("Strategy %s made changes in pre-stage", 
                      _strategy->getStrategyName().c_str());
            }
        }
    }

    const std::vector<HeroMessage>& getHeroesInBattle() const {
        return _heroesInBattle;
    }

    // ... 其他必要的getter/setter方法
};

// 4. 策略工厂
class AiStrategyFactory {
public:
    static std::unique_ptr<IAiBehaviorStrategy> createStrategy(AiType type) {
        switch(type) {
            case AiType::AGGRESSIVE:
                return std::make_unique<AggressiveStrategy>();
            case AiType::DEFENSIVE:
                return std::make_unique<DefensiveStrategy>();
            case AiType::BALANCED:
            default:
                return std::make_unique<BalancedStrategy>();
        }
    }
};

// 5. 使用示例
void initializeAI(AiminiHero* hero, int aiType) {
    auto strategy = AiStrategyFactory::createStrategy(aiType);
    hero->setStrategy(std::move(strategy));
}