/*Refactored by Prototype Pattern and Bridge Pattern*/
#include"Hero.h"
#include "HeroYase.h"
#include<string>
#include"const.h"
#include "SimpleAudioEngine.h"

// Bridge Pattern: HeroYase class as concrete implementation of abstract part
// Uses StateBar (implementation part) through composition to manage hero status
class HeroYase : public Hero {
public:
    static HeroYase* create(Ecamp camp, Ref* scene);
    virtual bool init(Ecamp camp, Ref* scene) override;
    // ... other method declarations ...
};

// Bridge Pattern: Factory method to create concrete hero instance
HeroYase* HeroYase::create(Ecamp camp, Ref* scene)
{
    HeroYase* yase = new (std::nothrow) HeroYase;
    if (yase && yase->init(camp, scene))
    {
        yase->autorelease();
        return yase;
    }
    CC_SAFE_DELETE(yase);
    return nullptr;
}

bool HeroYase::init(Ecamp camp, Ref* scene)
{
    this->addTouchListener();
    this->initWithFile("MinatoStand.png");
    //myAttackSprite = Sprite::create("MinatoStand.png");
    setPresentScene(scene);
    //mySprite->setScale(150);
    //float _attackTargetWidth = 150.0f; // Target width
    //float _attackTargetHeight = 0.0f; // Target height
    float scaleFactorX = 120 / this->getContentSize().width;// scalingWidth
    float scaleFactorY = 120 / this->getContentSize().height;// scalingHeight
    this->setScaleX(scaleFactorX);
    this->setScaleY(scaleFactorY);

    //this->setPosition();
    this->setDir(-1);
    this->setIsDead(false);
    this->setIsHurt(false);
    this->setIsMoving(false);
    this->setHpLim(warriorHpLim);
    this->setEnergyLim(energyLim);
    this->setSpd(warriorSpeed);
    this->setLv(beginLevel);
    this->setAttackScope(60);//
    this->setAttackPower(warriorAttackPower);
    this->setSkillPower(warriorSkillPower);
    this->setIsOnTheStage(false);
    this->setCamp(camp);
    this->setEnergyRecoverRate(warriorEnergyRecoverRate);

    // Bridge Pattern: Create concrete state bar implementation
    // Create status bars with specific initial properties for warrior type hero
    auto healthBar = HealthBar::create(shooterHpLim, 0, camp);
    auto energyBar = EnergyBar::create(energyLim, shooterEnergyRecoverRate, camp);
    
    // Bridge Pattern: Add status bars through bridge pattern interface
    this->addStateBar(healthBar);
    this->addStateBar(energyBar);

    this->setTag(YASEYASE);
    this->setAttackTarget(nullptr);
    //this->setLatestTargetPos(Vec2(0, 0));
    //this->addChild(mySprite, 0);
    this->updateAttackTarget();

    displayHeroLevel(this->getLv());
    this->setDragable(1);
    upDateMoving();
    SkillLogic(); 
   
    // Bridge Pattern: Status bar position update callback
    std::function<void(float)> upDateHpAndEnergyPos = [this](float) {
        // Bridge Pattern: Get concrete status bars through unified interface
        if (auto healthBar = this->getStateBar("health")) {
            if (auto energyBar = this->getStateBar("energy")) {
                healthBar->setPosition(Vec2(this->getContentSize().width / 2, 
                    this->getContentSize().height + 0));
                energyBar->setPosition(Vec2(this->getContentSize().width / 2, 
                    this->getContentSize().height - 10));
                label->setPosition(Vec2(this->getContentSize().width / 2,
                    this->getContentSize().height - 10 - label->getContentSize().height));
            }
        }
    };

    this->schedule(upDateHpAndEnergyPos, 0.001f, "upDateHpAndEnergyPos");

    return true;
}
void HeroYase::upDateMoving()
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

            // Calculate angle between positions
            float deltaX = enemyPosition.x - heroPosition.x;
            int curDir = deltaX > 0 ? 1 : -1;

            if (curDir != this->getDir())
                Flip();

                initWalkingAnimation((_attackTarget)->getPosition());
                if (this && this->_attackTarget && this->getPosition().distance((_attackTarget)->getPosition()) < attackScope)
                {
                    this->unschedule("upDateMoving");
                    this->stopAllActions();
                    this->_isMoving = false;
                    performAttack();
                }
        }
    };
    this->schedule(upDateMove, 1.0f, "upDateMoving");
}

void HeroYase::SkillLogic() {
    // Bridge Pattern: Use status bar interface in skill logic
    std::function<void(float)> Skill = [this](float) {
        // Bridge Pattern: Check energy status through status bar interface
        if (auto energyBar = this->getStateBar("energy")) {
            if (energyBar->getCurrentState() == energyBar->getMaxState())
            {
                double dis = -1;
                // Logic
                HelloWorld* Scene = dynamic_cast<HelloWorld*>(this->getPresentScene());
                Hero* farestEnermy = nullptr;
                for (auto it = Scene->_heroes.begin(); it != Scene->_heroes.end(); it++) {

                    if ((*it)->getCamp() != _camp && (*it)->getIsOnTheStage() && getPosition().getDistance((*it)->getPosition()) > dis && !(*it)->getIsDead()) {
                        dis = getPosition().getDistance((*it)->getPosition());
                        farestEnermy = (*it);
                    }
                }
                if (farestEnermy != nullptr && getIsOnTheStage()) {
                    Vector<SpriteFrame*> animSkillFrames;
                    animSkillFrames.pushBack(SpriteFrame::create("MinatoSkill.png", Rect(0, 0, 400, 400))); // Frame
                    animSkillFrames.pushBack(SpriteFrame::create("flash.png", Rect(0, 0, 400, 400))); // Frame
                    animSkillFrames.pushBack(SpriteFrame::create("MinatoSkill.png", Rect(0, 0, 400, 400))); // Frame


                    auto animationSkill = Animation::createWithSpriteFrames(animSkillFrames, 0.15f); // 0.1f per frame
                    auto animateSkill = Animate::create(animationSkill);
                    auto Skill = Spawn::create(animateSkill, nullptr, nullptr);
                    this->runAction(Skill);
                    int diff = 0;
                    if (farestEnermy->getPosition().x < Director::getInstance()->getVisibleSize().width / 2) {
                        if (getDir() != -1)
                            Flip();
                        diff = 50;
                    }
                    else {
                        if (getDir() != 1)
                            Flip();
                        diff = -50;
                    }
                    setPosition(Vec2(farestEnermy->getPosition().x + diff, farestEnermy->getPosition().y));
                    energyBar->setCurrentState(5);
                }
            }
        }
        if (getIsDead())
            this->unschedule("Skill");
    };
    this->schedule(Skill, 1.0f, "Skill");
}

void HeroYase::initWalkingAnimation(Vec2 destination) {
    Vector<SpriteFrame*> animFrames; // Frames

    animFrames.pushBack(SpriteFrame::create("Minatorun1.png", Rect(0, 0, 400, 400))); // Frame
    animFrames.pushBack(SpriteFrame::create("Minatorun2.png", Rect(0, 0, 400, 400))); // Frame
    animFrames.pushBack(SpriteFrame::create("Minatorun3.png", Rect(0, 0, 400, 400))); // Frame
    animFrames.pushBack(SpriteFrame::create("Minatorun4.png", Rect(0, 0, 400, 400))); // Frame
    animFrames.pushBack(SpriteFrame::create("Minatorun4.png", Rect(0, 0, 400, 400))); // Frame
    animFrames.pushBack(SpriteFrame::create("Minatorun3.png", Rect(0, 0, 400, 400))); // Frame
    animFrames.pushBack(SpriteFrame::create("Minatorun2.png", Rect(0, 0, 400, 400))); // Frame
    animFrames.pushBack(SpriteFrame::create("Minatorun1.png", Rect(0, 0, 400, 400))); // Frame

  
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.125f);
    auto animate = Animate::create(animation);
    auto repeatForever = RepeatForever::create(animate);
    this->stopAllActions();
    this->runAction(repeatForever);  // Start animation

    auto Time = (this->getPosition().distance((_attackTarget)->getPosition())) / _spd;
    auto move = MoveTo::create(Time, destination);
    this->runAction(move);
}


// Keep attacking until stopped
void HeroYase::performAttack() {
    Vector<SpriteFrame*> animAttackFrames;
    
    animAttackFrames.pushBack(SpriteFrame::create("MinatoAttack1.png", Rect(0, 0, 400, 400))); // Frame
    animAttackFrames.pushBack(SpriteFrame::create("MinatoAttack2.png", Rect(0, 0, 400, 400))); // Frame

    auto animationAttack = Animation::createWithSpriteFrames(animAttackFrames, 0.5f); // 0.1f per frame
    auto animateAttack = Animate::create(animationAttack);
    auto ATTACK = Spawn::create(animateAttack, nullptr, nullptr);

    // Execute one attack
    auto attackSequence = Spawn::create(ATTACK, this->hitTarget(), nullptr);
    auto repeatAttackAndFire = RepeatForever::create(attackSequence);
    this->runAction(repeatAttackAndFire);

    // Attack status check
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

CallFunc* HeroYase::hitTarget() {
    // Bridge Pattern: Handle damage effects through status bar interface
    auto fireArrow = CallFunc::create([this]() {
        if (_attackTarget) {
            INT32 damageDelta = attackPower;
            
            // Bridge Pattern: Update target health through status bar interface
            if (auto targetHealth = _attackTarget->getStateBar("health")) {
                targetHealth->changeStateBy(-damageDelta);
                
                // Check target status and handle death logic
                if (targetHealth->getCurrentState() <= 0)
                {
                    _attackTarget->setIsDead(true);
                    _attackTarget->setVisible(false);
                    _attackTarget->stopAllActions();
                    this->stopAllActions();
                    this->unschedule("arrow_collision_check");
                    upDateMoving();
                }
            }
        }
    });
    return fireArrow;
}



void HeroYase::displayHeroLevel(int level) {
    // Create a Label to display level
    this->label = Label::createWithTTF("lv."+std::to_string(level), "fonts/Marker Felt.ttf", 24);

    // Set Label position
    label->setPosition(Vec2(Vec2(this->getContentSize().width / 2,
        this->getContentSize().height + 30 - label->getContentSize().height)));

    // Set text color
    label->setColor(Color3B::WHITE);

    // Add Label as child
    this->addChild(label, 1);
}
void HeroYase::upGrade()
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

Hero* HeroYase::clone() const {
    // 1. Create new Yase instance
    auto newHero = new HeroYase();

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

    // 4. Copy UI label
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

    // 8. Copy dragableSprite attributes
    newHero->setDragable(this->isDragable());
    
    // Note: Don't copy _attackTarget as it's a dynamic combat target
    newHero->setAttackTarget(nullptr);

    return newHero;
}