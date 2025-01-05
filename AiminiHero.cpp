/* Refactored by Strategy Pattern */
#include "AiminiHero.h"

// Implementation of AggressiveStrategy - A concrete strategy focusing on offensive tactics
void AggressiveStrategy::initialize(AiminiHero* hero) {
    // Initialize hero with aggressive stats - higher money for buying attack units
    hero->setMoney(100);
    hero->setExp(0);
    hero->setMaxOnBoard(5);
    hero->setLevel(1);
    
    // Set up initial hero roster focused on high damage dealers
    std::vector<HeroMessage> attackHeroes = {
        {"Warrior", 100, HeroType::ATTACKER},
        {"Assassin", 80, HeroType::ATTACKER}, 
        {"Berserker", 120, HeroType::ATTACKER}
    };
    hero->setInitialHeroes(attackHeroes);
}

// Implementation of DefensiveStrategy - A concrete strategy focusing on defensive tactics
void DefensiveStrategy::initialize(AiminiHero* hero) {
    // Initialize hero with defensive stats - extra money for buying tank units
    hero->setMoney(120);
    hero->setExp(0);
    hero->setMaxOnBoard(4);
    hero->setLevel(1);
    
    // Set up initial hero roster focused on tanks and supports
    std::vector<HeroMessage> defensiveHeroes = {
        {"Tank", 150, HeroType::DEFENDER},
        {"Guardian", 130, HeroType::DEFENDER},
        {"Support", 90, HeroType::SUPPORT}
    };
    hero->setInitialHeroes(defensiveHeroes);
}

// Implementation of BalancedStrategy - A concrete strategy providing mixed tactics
void BalancedStrategy::initialize(AiminiHero* hero) {
    // Initialize hero with balanced stats
    hero->setMoney(110);
    hero->setExp(0);
    hero->setMaxOnBoard(5);
    hero->setLevel(1);
    
    // Set up initial hero roster with mix of attackers, defenders and supports
    std::vector<HeroMessage> balancedHeroes = {
        {"Warrior", 100, HeroType::ATTACKER},
        {"Tank", 150, HeroType::DEFENDER},
        {"Support", 90, HeroType::SUPPORT}
    };
    hero->setInitialHeroes(balancedHeroes);
}

// AiminiHero class implementation - The context class in Strategy pattern
AiminiHero::AiminiHero(int AiID) 
    : _money(0), _exp(0), _maxOnBoard(0), _level(1) {
    _battleStats = {0, 0, 0, 0.0f};
}

// Strategy pattern method to dynamically change AI behavior
void AiminiHero::setStrategy(std::unique_ptr<IAiBehaviorStrategy> strategy) {
    if (strategy) {
        CCLOG("Changing strategy to: %s", strategy->getStrategyName().c_str());
        _strategy = std::move(strategy);
        // Initialize the hero with new strategy's settings
        _strategy->initialize(this);
    }
}

// Update method delegates behavior to current strategy
void AiminiHero::update(float dt) {
    if (_strategy) {
        bool isAdd = false;
        // Strategy pattern: Delegate pre-stage logic to current strategy
        _strategy->preStageLogic(this, isAdd);
        if (isAdd) {
            CCLOG("Strategy %s made changes in pre-stage", 
                  _strategy->getStrategyName().c_str());
        }
    }
}

// Handle battle results using current strategy
void AiminiHero::onBattleResult(bool isWin) {
    updateBattleStats(isWin);
    if (_strategy) {
        // Strategy pattern: Let strategy handle battle aftermath
        _strategy->onBattleEnd(this, isWin);
        _strategy->adjustStrategy(this, _battleStats);
    }
}

// Update battle statistics for strategy evaluation
void AiminiHero::updateBattleStats(bool isWin) {
    if (isWin) {
        _battleStats.wins++;
        _battleStats.currentStreak = std::max(_battleStats.currentStreak + 1, 0);
    } else {
        _battleStats.losses++;
        _battleStats.currentStreak = std::min(_battleStats.currentStreak - 1, 0);
    }
    
    int totalBattles = _battleStats.wins + _battleStats.losses;
    _battleStats.winRate = totalBattles > 0 ? 
        static_cast<float>(_battleStats.wins) / totalBattles : 0.0f;
}