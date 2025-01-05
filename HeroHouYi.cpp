/*Refactored by Prototype Pattern and Bridge Pattern*/
#include"Hero.h"
#include "HeroHouYi.h"
#include<string>
#include "SimpleAudioEngine.h"

// Bridge Pattern: HeroHouYi class as concrete implementation of the abstraction
// Uses StateBar (implementation part) through composition to manage hero states
class HeroHouYi : public Hero {
public:
    static HeroHouYi* create(Ecamp camp, Ref* scene);
    virtual bool init(Ecamp camp, Ref* scene) override;
    // ... other method declarations ...
};

// Bridge Pattern: Factory method to create concrete hero instance
HeroHouYi* HeroHouYi::create(Ecamp camp, Ref* scene)
{
    HeroHouYi* houyi = new (std::nothrow) HeroHouYi;
    if (houyi && houyi->init(camp, scene))
    {
        houyi->autorelease();
        return houyi;
    }
    CC_SAFE_DELETE(houyi);
    return nullptr;
}

bool HeroHouYi::init(Ecamp camp, Ref* scene)
{
    this->addTouchListener();
    this->initWithFile("HouYiright1.png");
    myAttackSprite = Sprite::create("HouYiNormal.png");
    setPresentScene(scene);

    float scaleFactorX = scalingWidth / this->getContentSize().width;
    float scaleFactorY = scalingHeight / this->getContentSize().height;
    this->setScaleX(scaleFactorX);
    this->setScaleY(scaleFactorY);

    this->setDir(1);
    this->setIsDead(false);
    this->setIsHurt(false);
    this->setIsMoving(false);
    this->setCamp(camp);
    this->setDragable(1);

    this->setHpLim(shooterHpLim);
    this->setEnergyLim(energyLim);
    this->setSpd(shooterSpeed);
    this->setLv(beginLevel);
    this->setAttackScope(shooterAttackScope);
    this->setAttackPower(shooterAttackPower);
    this->setSkillPower(shooterSkillPower);
    this->setIsOnTheStage(false);
    this->setTag(HOUYIHOUYI);
    this->setEnergyRecoverRate(shooterEnergyRecoverRate);

    auto healthBar = HealthBar::create(shooterHpLim, 0, camp);
    auto energyBar = EnergyBar::create(energyLim, shooterEnergyRecoverRate, camp);
    
    this->addStateBar(healthBar);
    this->addStateBar(energyBar);

    displayHeroLevel(this->getLv());

    this->setAttackTarget(nullptr);

    std::function<void(float)> upDateHpAndEnergyPos = [this](float) {
        auto healthBar = dynamic_cast<StateBar*>(this->getStateBar("health"));
        auto energyBar = dynamic_cast<StateBar*>(this->getStateBar("energy"));
        if (healthBar != nullptr && energyBar != nullptr)
        {
            healthBar->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 0));
            energyBar->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height - 10));
            label->setPosition(Vec2(Vec2(this->getContentSize().width / 2,
                this->getContentSize().height - 10 - label->getContentSize().height)));
        }
    };

    this->schedule(upDateHpAndEnergyPos, 0.001f, "upDateHpAndEnergyPos");

    this->updateAttackTarget();
    upDateMoving();
    return true;
}

void HeroHouYi::displayHeroLevel(int level) {
    // Create a Label to display level
    this->label = Label::createWithTTF("lv." + std::to_string(level), "fonts/Marker Felt.ttf", 24);

    // Set Label position
    label->setPosition(Vec2(Vec2(this->getContentSize().width / 2,
        this->getContentSize().height + 30 - label->getContentSize().height)));

    // Set text color
    label->setColor(Color3B::WHITE);

    // Add Label as child
    this->addChild(label, 1);
}

void HeroHouYi::upDateMoving()
{
    std::function<void(float)> upDateMove = [this](float) {
        this->updateAttackTarget();

        if (_attackTarget == nullptr)
        {
            this->unschedule("upDateMoving");
        }
        else if (this->getIsOnTheStage())
        {
            auto heroPosition = this->getPosition();
            auto enemyPosition = _attackTarget->getPosition();

            float deltaX = enemyPosition.x - heroPosition.x;
            int curDir = deltaX > 0 ? 1 : -1;

            if (curDir != this->getDir())
                Flip();

            initWalkingAnimation((_attackTarget)->getPosition());
            if (this && this->getPosition().distance((_attackTarget)->getPosition()) < attackScope)
            {
                this->unschedule("upDateMoving");
                this->stopAllActions();
                this->_isMoving = false;
                performAttack();
            }
            //decideToAttack();+
        }
    };
    this->schedule(upDateMove, 1.0f, "upDateMoving");
}

void HeroHouYi::initWalkingAnimation(Vec2 destination) {
    Vector<SpriteFrame*> animFrames; // Animation frames
    animFrames.pushBack(SpriteFrame::create("HouYiright1.png", Rect(0, 0, 430, 430))); // Frame 1
    animFrames.pushBack(SpriteFrame::create("HouYiright2.png", Rect(0, 0, 430, 430))); // Frame 2
    animFrames.pushBack(SpriteFrame::create("HouYiright3.png", Rect(0, 0, 430, 430))); // Frame 3
    animFrames.pushBack(SpriteFrame::create("HouYiright4.png", Rect(0, 0, 430, 430))); // Frame 4
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.225f);
    auto animate = Animate::create(animation);
    auto repeatForever = RepeatForever::create(animate);
    this->stopAllActions();
    this->runAction(repeatForever);  // Start animation

    auto Time = (this->getPosition().distance((_attackTarget)->getPosition())) / _spd;
    auto move = MoveTo::create(Time, destination);
    this->runAction(move);

}


void HeroHouYi::performAttack() {
    Vector<SpriteFrame*> animAttackFrames;
    animAttackFrames.pushBack(SpriteFrame::create("HouYiAttackright1.png", Rect(0, 0, 400, 400))); // Frame 1
    animAttackFrames.pushBack(SpriteFrame::create("HouYiAttackright2.png", Rect(0, 0, 400, 400))); // Frame 2
    animAttackFrames.pushBack(SpriteFrame::create("HouYiAttackright3.png", Rect(0, 0, 400, 400))); // Frame 3
    //animAttackFrames.pushBack(SpriteFrame::create("DaJiAttackright1.png", Rect(0, 0, 400, 400)));
    auto animationAttack = Animation::createWithSpriteFrames(animAttackFrames, 0.5f); // 0.1f per frame
    auto animateAttack = Animate::create(animationAttack);
    auto ATTACK = Spawn::create(animateAttack, nullptr, nullptr);

    

    auto attackSequence = Spawn::create(ATTACK, this->fireArrow(), nullptr);

    auto repeatAttackAndFire = RepeatForever::create(attackSequence);
    this->runAction(repeatAttackAndFire);

    // Bridge Pattern: Use state checking to control attack behavior
    std::function<void(float)> stop = [this, repeatAttackAndFire](float) {
        this->updateAttackTarget();
        if (this && _attackTarget && 
            !(this->getPosition().distance((_attackTarget)->getPosition()) < attackScope))
        {
            this->unschedule("stop");
            this->stopAction(repeatAttackAndFire);
            upDateMoving();
        }
    };
    this->schedule(stop, 0.001f, "stop");
}

// Bridge Pattern: Handle skill effects through state bar implementation
CallFunc* HeroHouYi::fireArrow() {
    auto fireArrow = CallFunc::create([this]() {
        std::string path;
        INT32 size = 0;
        INT32 damageDelta = 0;
        
        // Bridge Pattern: Check energy state using state bar interface
        // Shows how implementation part (StateBar) affects abstract part (Hero) behavior
        if (auto energyBar = this->getStateBar("energy")) {
            if (energyBar->getPercent() == 100) {
                // Skill state
                path = "HouYiSkill3Logo.png";
                size = 8;
                energyBar->setCurrentState(1);  // Reset energy value
                damageDelta = this->getSkillPower();
            } else {
                // Normal attack state
                path = "HouYiNormal.png";
                size = 20;
                damageDelta = this->getAttackPower();
            }
        }

        // ... Arrow creation and launch logic ...

        // Bridge Pattern: Handle damage effects through state bar interface
        auto arrowSprite = Sprite::create(path); // Create arrow
        arrowSprite->setPosition(this->getPosition()); // Initial arrow position

        auto spriteSize = this->getContentSize();
        arrowSprite->setScale(spriteSize.width / (size * arrowSprite->getContentSize().width),
            spriteSize.height / (size * arrowSprite->getContentSize().height));
        if (_attackTarget == nullptr)
        {

            return nullptr;
        }
        auto moveArrow = MoveTo::create(0.5f, _attackTarget->getPosition()); // Move arrow to target
        auto removeArrow = CallFunc::create([arrowSprite, this, damageDelta]() {
            if (arrowSprite && _attackTarget && 
                arrowSprite->getBoundingBox().intersectsRect((_attackTarget->getBoundingBox()))) 
            {
                arrowSprite->removeFromParent();
                // Bridge Pattern: Update target health through state bar interface
                _attackTarget->getStateBar("health")->changeStateBy(-damageDelta);
                
                // Check target state and handle death logic
                if (_attackTarget->getStateBar("health")->getCurrentState() <= 0)
                {
                    _attackTarget->setIsDead(true);
                    _attackTarget->setVisible(false);
                    _attackTarget->stopAllActions();
                    this->stopAllActions();
                    this->unschedule("arrow_collision_check");
                    upDateMoving();
                }
            }
            else
                arrowSprite->removeFromParent();
        });
        auto arrowSequence = Sequence::create(moveArrow, removeArrow, nullptr);
        if (this->isVisible())
        {
            arrowSprite->runAction(arrowSequence); // Execute arrow action
            dynamic_cast<HelloWorld*>(this->getPresentScene())->addChild(arrowSprite);
        }
        //checkArrowCollision(arrowSprite);

    });
    return fireArrow;
}

void HeroHouYi::upGrade()
{
    // Bridge Pattern: Update status bars on level up
    // Remove old status bar implementations
    this->removeStateBar("health");
    this->removeStateBar("energy");

    // Update attribute values
    this->setHpLim(getHpLim() * 2);
    this->setEnergyLim(getEnergyLim() / 2);

    // Bridge Pattern: Create and add new status bar implementations
    auto newHealthBar = HealthBar::create(this->getHpLim(), 0, getCamp());
    auto newEnergyBar = EnergyBar::create(this->getEnergyLim(), 
        getEnergyRecoverRate() * 2, getCamp());
    
    this->addStateBar(newHealthBar);
    this->addStateBar(newEnergyBar);
}

Hero* HeroHouYi::clone() const {
    // 1. Create new HouYi instance
    auto newHero = new HeroHouYi();

    // 2. Copy basic attributes
    newHero->setLv(this->getLv());
    newHero->setCamp(this->getCamp());
    newHero->setIsDead(this->getIsDead());
    newHero->setIsHurt(this->getIsHurt());
    newHero->setIsMoving(this->getIsMoving());
    newHero->setIsOnTheStage(this->getIsOnTheStage());
    
    // 3. Copy combat attributes
    newHero->setHpLim(this->getHpLim());
    newHero->setSpd(this->getSpd());
    newHero->setAttackScope(this->getAttackScope());
    newHero->setAttackPower(this->getAttackPower());
    newHero->setSkillPower(this->getSkillPower());
    newHero->setEnergyLim(this->getEnergyLim());
    newHero->setEnergyRecoverRate(this->getEnergyRecoverRate());
    newHero->setDir(this->getDir());

    // 4. Copy UI labels
    if (this->getLable()) {
        auto newLabel = Label::createWithTTF(
            this->getLable()->getString(),
            this->getLable()->getTTFConfig().fontFilePath,
            this->getLable()->getTTFConfig().fontSize
        );
        newLabel->setPosition(this->getLable()->getPosition());
        newLabel->setScale(this->getLable()->getScale());
        newHero->setLable(newLabel);
    }

    // 5. Copy health bar
    if (this->getHpBar()) {
        auto newHpBar = HP::create();
        newHpBar->setMaxValue(this->getHpBar()->getMaxValue());
        newHpBar->setCurrentState(this->getHpBar()->getCurrentState());
        newHpBar->setPosition(this->getHpBar()->getPosition());
        newHpBar->setScale(this->getHpBar()->getScale());
        newHero->setHpBar(newHpBar);
    }

    // 6. Copy energy bar
    if (this->getEnergyBar()) {
        auto newEnergyBar = HP::create();
        newEnergyBar->setMaxValue(this->getEnergyBar()->getMaxValue());
        newEnergyBar->setCurrentState(this->getEnergyBar()->getCurrentState());
        newEnergyBar->setPosition(this->getEnergyBar()->getPosition());
        newEnergyBar->setScale(this->getEnergyBar()->getScale());
        newHero->setEnergyBar(newEnergyBar);
    }

    // 7. Copy sprite attributes
    newHero->setScale(this->getScale());
    newHero->setPosition(this->getPosition());
    newHero->setRotation(this->getRotation());
    newHero->setVisible(this->isVisible());
    newHero->setOpacity(this->getOpacity());

    // 8. Copy dragableSprite related attributes
    newHero->setDragable(this->isDragable());
    
    // Note: Don't copy _attackTarget as it's a dynamic target during battle
    newHero->setAttackTarget(nullptr);

    return newHero;
}