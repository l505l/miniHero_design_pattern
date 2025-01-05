#include"Hero.h"
#include "HeroHouYi.h"
#include<string>
#include "SimpleAudioEngine.h"

// Bridge Pattern: HeroHouYi 类作为抽象部分的具体实现
// 通过组合方式使用 StateBar（实现部分）来管理英雄状态
class HeroHouYi : public Hero {
public:
    static HeroHouYi* create(Ecamp camp, Ref* scene);
    virtual bool init(Ecamp camp, Ref* scene) override;
    // ... 其他方法声明 ...
};

// Bridge Pattern: 工厂方法创建具体的英雄实例
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
    // һ Label ʾȼ
    this->label = Label::createWithTTF("lv." + std::to_string(level), "fonts/Marker Felt.ttf", 24);

    //  Label λ
    label->setPosition(Vec2(Vec2(this->getContentSize().width / 2,
        this->getContentSize().height + 30 - label->getContentSize().height)));

    // ıɫ
    label->setColor(Color3B::WHITE);

    //  Label ӵ
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
    Vector<SpriteFrame*> animFrames; // ֡
    animFrames.pushBack(SpriteFrame::create("HouYiright1.png", Rect(0, 0, 430, 430))); // ֡
    animFrames.pushBack(SpriteFrame::create("HouYiright2.png", Rect(0, 0, 430, 430))); // ֡
    animFrames.pushBack(SpriteFrame::create("HouYiright3.png", Rect(0, 0, 430, 430))); // ֡
    animFrames.pushBack(SpriteFrame::create("HouYiright4.png", Rect(0, 0, 430, 430))); // ֡
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.225f);
    auto animate = Animate::create(animation);
    auto repeatForever = RepeatForever::create(animate);
    this->stopAllActions();
    this->runAction(repeatForever);  // ʼ߶

    auto Time = (this->getPosition().distance((_attackTarget)->getPosition())) / _spd;
    auto move = MoveTo::create(Time, destination);
    this->runAction(move);

}


void HeroHouYi::performAttack() {
    Vector<SpriteFrame*> animAttackFrames;
    animAttackFrames.pushBack(SpriteFrame::create("HouYiAttackright1.png", Rect(0, 0, 400, 400))); // ֡
    animAttackFrames.pushBack(SpriteFrame::create("HouYiAttackright2.png", Rect(0, 0, 400, 400))); // ֡
    animAttackFrames.pushBack(SpriteFrame::create("HouYiAttackright3.png", Rect(0, 0, 400, 400))); // ֡
    //animAttackFrames.pushBack(SpriteFrame::create("DaJiAttackright1.png", Rect(0, 0, 400, 400)));
    auto animationAttack = Animation::createWithSpriteFrames(animAttackFrames, 0.5f); // 0.1fΪÿ֡ʱ
    auto animateAttack = Animate::create(animationAttack);
    auto ATTACK = Spawn::create(animateAttack, nullptr, nullptr);

    

    auto attackSequence = Spawn::create(ATTACK, this->fireArrow(), nullptr);

    auto repeatAttackAndFire = RepeatForever::create(attackSequence);
    this->runAction(repeatAttackAndFire);

    // Bridge Pattern: 使用状态检查来控制攻击行为
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

// Bridge Pattern: 通过状态栏实现来处理技能效果
CallFunc* HeroHouYi::fireArrow() {
    auto fireArrow = CallFunc::create([this]() {
        std::string path;
        INT32 size = 0;
        INT32 damageDelta = 0;
        
        // Bridge Pattern: 使用状态栏接口检查能量状态
        // 展示了实现部分（StateBar）如何影响抽象部分（Hero）的行为
        if (auto energyBar = this->getStateBar("energy")) {
            if (energyBar->getPercent() == 100) {
                // 技能状态
                path = "HouYiSkill3Logo.png";
                size = 8;
                energyBar->setCurrentState(1);  // 重置能量值
                damageDelta = this->getSkillPower();
            } else {
                // 普通攻击状态
                path = "HouYiNormal.png";
                size = 20;
                damageDelta = this->getAttackPower();
            }
        }

        // ... 箭矢创建和发射逻辑 ...

        // Bridge Pattern: 通过状态栏接口处理伤害效果
        auto arrowSprite = Sprite::create(path); // ʸ
        arrowSprite->setPosition(this->getPosition()); // ʸĳʼλ

        auto spriteSize = this->getContentSize();
        arrowSprite->setScale(spriteSize.width / (size * arrowSprite->getContentSize().width),
            spriteSize.height / (size * arrowSprite->getContentSize().height));
        if (_attackTarget == nullptr)
        {

            return nullptr;
        }
        auto moveArrow = MoveTo::create(0.5f, _attackTarget->getPosition()); // ʸƶĿλ
        auto removeArrow = CallFunc::create([arrowSprite, this, damageDelta]() {
            if (arrowSprite && _attackTarget && 
                arrowSprite->getBoundingBox().intersectsRect((_attackTarget->getBoundingBox()))) 
            {
                arrowSprite->removeFromParent();
                // Bridge Pattern: 通过状态栏接口更新目标生命值
                _attackTarget->getStateBar("health")->changeStateBy(-damageDelta);
                
                // 检查目标状态并处理死亡逻辑
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
            arrowSprite->runAction(arrowSequence); // ִмʸ
            dynamic_cast<HelloWorld*>(this->getPresentScene())->addChild(arrowSprite);
        }
        //checkArrowCollision(arrowSprite);

    });
    return fireArrow;
}

void HeroHouYi::upGrade()
{
    this->removeStateBar("health");
    this->removeStateBar("energy");

    this->setHpLim(getHpLim() * 2);
    this->setEnergyLim(getEnergyLim() / 2);
    this->setSpd(getSpd() * 1.5f);
    this->setLv(getLv() + 1);

    this->removeChild(label, true);
    this->displayHeroLevel(getLv());

    this->setAttackScope(getAttackScope() * 1.2f);
    this->setAttackPower(getAttackPower() * 2);
    this->setSkillPower(getSkillPower() * 2);

    auto newHealthBar = HealthBar::create(this->getHpLim(), 0, getCamp());
    auto newEnergyBar = EnergyBar::create(this->getEnergyLim(), 
        getEnergyRecoverRate() * 2, getCamp());
    
    this->addStateBar(newHealthBar);
    this->addStateBar(newEnergyBar);
}