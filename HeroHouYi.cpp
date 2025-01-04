#include"Hero.h"
#include "HeroHouYi.h"
#include<string>
#include "SimpleAudioEngine.h"

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
//void HeroHouYi::decideToAttack() {
    //std::function<void(float)> decisionCallback = [this](float) {
    //    // ǾǷִ ATTACK ߼
    //    // 磬ԸϷ״̬Ի
    //    if (this&&_attackTarget&& this->getPosition().distance((_attackTarget)->getPosition()) < attackScope)
    //    {
    //        this->unschedule("check_condition_key");
    //        this->stopAllActions();
    //        this->_isMoving = false;
    //        performAttack();
    //    }
    //};
    //this->schedule(decisionCallback, 0.001f, "check_condition_key");
//}

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

    std::function<void(float)> stop = [this, repeatAttackAndFire](float) {
        this->updateAttackTarget();
        if (this && _attackTarget && !(this->getPosition().distance((_attackTarget)->getPosition()) < attackScope))
        {
            this->unschedule("stop");
            this->stopAction(repeatAttackAndFire);
            upDateMoving();
        }
    };
    this->schedule(stop, 0.001f, "stop");
}

CallFunc* HeroHouYi::fireArrow() {
    auto fireArrow = CallFunc::create([this]()
        {
            std::string path;
            INT32 size = 0;
            INT32 damageDelta = 0;
            if (this->getEnergyBar() && (this->getEnergyBar()->getPercent() == 100))
            {
                path = "HouYiSkill3Logo.png";
                size = 8;
                this->getEnergyBar()->setCurrentState(1);
                damageDelta = this->getSkillPower();
            }
            else
            {
                path = "HouYiNormal.png";
                size = 20;
                damageDelta = this->getAttackPower();
            }
            
          

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
                //arrowSprite->removeFromParent(); // ɺƳʸ
                if (arrowSprite && _attackTarget && arrowSprite->getBoundingBox().intersectsRect((_attackTarget->getBoundingBox()))) {
                    arrowSprite->removeFromParent(); // Ƴʸ
                    //arrowSprite = nullptr; // ռʸ
                    _attackTarget->getHpBar()->changeStateBy(-damageDelta);
                    if (getCamp() == BLUE)
                    {
                        int a = 0;
                    }
                    if (_attackTarget->getHpBar()->getCurrentState() <= 0)
                    {
                        _attackTarget->setIsDead(true);
                        _attackTarget->setVisible(false);
                        _attackTarget->stopAllActions();
                        //arrowSprite->stopAllActions();
                        //_attackTarget->mySprite->removeFromParent();
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
    this->removeChild(this->getHpBar(), true);
    this->removeChild(this->getEnergyBar(), true);

    this->setHpLim(getHpLim() * 2);
    this->setEnergyLim(getEnergyLim() / 2);
    this->setSpd(getSpd() * 1.5f);
    this->setLv(getLv() + 1);

    this->removeChild(label, true);
    this->displayHeroLevel(getLv());

    this->setAttackScope(getAttackScope() * 1.2f);
    this->setAttackPower(getAttackPower() * 2);
    this->setSkillPower(getSkillPower() * 2);
    this->setHpBar(HP::create(HEALTH, this->getHpLim(), 0, getCamp()));
    this->setEnergyBar(HP::create(ENERGY, this->getEnergyLim(), getEnergyRecoverRate() * 2, getCamp()));

    this->addChild(this->getHpBar());
    this->getHpBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 0));

    this->addChild(this->getEnergyBar());
    this->getEnergyBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height - 10));
}