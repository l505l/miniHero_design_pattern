#include"Hero.h"
#include "HeroYase.h"
#include<string>
#include"const.h"
#include "SimpleAudioEngine.h"

// Bridge Pattern: HeroYase 类作为抽象部分的具体实现
// 通过组合方式使用 StateBar（实现部分）来管理英雄状态
class HeroYase : public Hero {
public:
    static HeroYase* create(Ecamp camp, Ref* scene);
    virtual bool init(Ecamp camp, Ref* scene) override;
    // ... 其他方法声明 ...
};

// Bridge Pattern: 工厂方法创建具体的英雄实例
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
    //float _attackTargetWidth = 150.0f; // Ŀ�����
    //float _attackTargetHeight = 0.0f; // Ŀ��߶�
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

    // Bridge Pattern: 创建具体的状态栏实现类
    // 为战士类型英雄创建特定初始属性的状态栏
    auto healthBar = HealthBar::create(shooterHpLim, 0, camp);
    auto energyBar = EnergyBar::create(energyLim, shooterEnergyRecoverRate, camp);
    
    // Bridge Pattern: 通过桥接模式的接口添加状态栏
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
   
    // Bridge Pattern: 状态栏位置更新回调
    std::function<void(float)> upDateHpAndEnergyPos = [this](float) {
        // Bridge Pattern: 通过统一接口获取具体的状态栏实现
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

            // ���������ĽǶ�
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
    // Bridge Pattern: 技能逻辑中使用状态栏接口
    std::function<void(float)> Skill = [this](float) {
        // Bridge Pattern: 通过状态栏接口检查能量状态
        if (auto energyBar = this->getStateBar("energy")) {
            if (energyBar->getCurrentState() == energyBar->getMaxState())
            {
                double dis = -1;
                //߼
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
                    animSkillFrames.pushBack(SpriteFrame::create("MinatoSkill.png", Rect(0, 0, 400, 400))); // ֡
                    animSkillFrames.pushBack(SpriteFrame::create("flash.png", Rect(0, 0, 400, 400))); // ֡
                    animSkillFrames.pushBack(SpriteFrame::create("MinatoSkill.png", Rect(0, 0, 400, 400))); // ֡


                    auto animationSkill = Animation::createWithSpriteFrames(animSkillFrames, 0.15f); // 0.1fΪÿ֡ʱ
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
    Vector<SpriteFrame*> animFrames; // ֡

    animFrames.pushBack(SpriteFrame::create("Minatorun1.png", Rect(0, 0, 400, 400))); // ֡
    animFrames.pushBack(SpriteFrame::create("Minatorun2.png", Rect(0, 0, 400, 400))); // ֡
    animFrames.pushBack(SpriteFrame::create("Minatorun3.png", Rect(0, 0, 400, 400))); // ֡
    animFrames.pushBack(SpriteFrame::create("Minatorun4.png", Rect(0, 0, 400, 400))); // ֡
    animFrames.pushBack(SpriteFrame::create("Minatorun4.png", Rect(0, 0, 400, 400))); // ֡
    animFrames.pushBack(SpriteFrame::create("Minatorun3.png", Rect(0, 0, 400, 400))); // ֡
    animFrames.pushBack(SpriteFrame::create("Minatorun2.png", Rect(0, 0, 400, 400))); // ֡
    animFrames.pushBack(SpriteFrame::create("Minatorun1.png", Rect(0, 0, 400, 400))); // ֡

  
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.125f);
    auto animate = Animate::create(animation);
    auto repeatForever = RepeatForever::create(animate);
    this->stopAllActions();
    this->runAction(repeatForever);  // ʼ߶

    auto Time = (this->getPosition().distance((_attackTarget)->getPosition())) / _spd;
    auto move = MoveTo::create(Time, destination);
    this->runAction(move);
}


//һֱ�򣬸stop
void HeroYase::performAttack() {
    Vector<SpriteFrame*> animAttackFrames;
    
    animAttackFrames.pushBack(SpriteFrame::create("MinatoAttack1.png", Rect(0, 0, 400, 400))); // ֡
    animAttackFrames.pushBack(SpriteFrame::create("MinatoAttack2.png", Rect(0, 0, 400, 400))); // ֡

    auto animationAttack = Animation::createWithSpriteFrames(animAttackFrames, 0.5f); // 0.1fΪÿ֡ʱ
    auto animateAttack = Animate::create(animationAttack);
    auto ATTACK = Spawn::create(animateAttack, nullptr, nullptr);

    //չһ
    auto attackSequence = Spawn::create(ATTACK, this->hitTarget(), nullptr);
    auto repeatAttackAndFire = RepeatForever::create(attackSequence);
    this->runAction(repeatAttackAndFire);

    //攻击状态检查
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
    // Bridge Pattern: 通过状态栏接口处理伤害效果
    auto fireArrow = CallFunc::create([this]() {
        if (_attackTarget) {
            INT32 damageDelta = attackPower;
            
            // Bridge Pattern: 通过状态栏接口更新目标生命值
            if (auto targetHealth = _attackTarget->getStateBar("health")) {
                targetHealth->changeStateBy(-damageDelta);
                
                // 检查目标状态并处理死亡逻辑
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
    // һ Label ʾȼ
    this->label = Label::createWithTTF("lv."+std::to_string(level), "fonts/Marker Felt.ttf", 24);

    //  Label 
    label->setPosition(Vec2(Vec2(this->getContentSize().width / 2,
        this->getContentSize().height + 30 - label->getContentSize().height)));

    // ıɫ
    label->setColor(Color3B::WHITE);

    //  Label 
    this->addChild(label, 1);
}
void HeroYase::upGrade()
{
    // Bridge Pattern: 升级时更新状态栏
    // 移除旧的状态栏实现
    this->removeStateBar("health");
    this->removeStateBar("energy");

    // 更新属性值
    this->setHpLim(getHpLim() * 2);
    this->setEnergyLim(getEnergyLim() / 2);
    // ... 其他属性更新 ...

    // Bridge Pattern: 创建并添加新的状态栏实现
    auto newHealthBar = HealthBar::create(this->getHpLim(), 0, getCamp());
    auto newEnergyBar = EnergyBar::create(this->getEnergyLim(), 
        getEnergyRecoverRate() * 2, getCamp());
    
    this->addStateBar(newHealthBar);
    this->addStateBar(newEnergyBar);
}
