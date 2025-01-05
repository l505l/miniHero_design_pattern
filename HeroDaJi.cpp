#include"Hero.h"
#include "HeroDaJi.h"
#include<string>
#include "SimpleAudioEngine.h"
HeroDaJi* HeroDaJi::create(Ecamp camp, Ref* scene)
{
    HeroDaJi* Daji = new (std::nothrow) HeroDaJi;
    if (Daji && Daji->init(camp, scene))
    {

        Daji->autorelease();
        return Daji;
    }
    CC_SAFE_DELETE(Daji);
    return nullptr;
}
bool HeroDaJi::init(Ecamp camp, Ref* scene)
{
    this->addTouchListener();
    this->initWithFile("DaJiright1.png");
    myAttackSprite = Sprite::create("Normal.png");
    setPresentScene(scene);
    //mySprite->setScale(150);
    //float _attackTargetWidth = 30.0f; // Ŀ����
    //float _attackTargetHeight = 50.0f; // Ŀ߶
    float scaleFactorX = scalingWidth / this->getContentSize().width;
    float scaleFactorY = scalingHeight / this->getContentSize().height;
    this->setScaleX(scaleFactorX);
    this->setScaleY(scaleFactorY);
    /*float scaleY = this->getScaleY();
    this->setScaleY(-scaleY);*/
    //this->setPosition();
    this->setDir(1);
    this->setIsDead(false);
    this->setIsHurt(false);
    this->setIsMoving(false);
    this->setCamp(camp);
    this->setDragable(1);

    this->setHpLim(mageHpLim);
    this->setEnergyLim(energyLim);
    this->setSpd(mageSpeed);
    this->setLv(beginLevel);
    this->setAttackScope(mageAttackScope);
    this->setAttackPower(mageAttackPower);
    this->setSkillPower(mageSkillPower);
    this->setIsOnTheStage(false);

    this->setTag(DAJIDAJI);

    this->setEnergyRecoverRate(mageEnergyRecoverRate);
    auto healthBar = HealthBar::create(mageHpLim, 0, camp);
    auto energyBar = EnergyBar::create(energyLim, mageEnergyRecoverRate, camp);
    
    this->addStateBar(healthBar);
    this->addStateBar(energyBar);

    this->addChild(this->getHpBar());
    this->getHpBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 0));

    this->addChild(this->getEnergyBar());
    this->getEnergyBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height - 10));

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
    //decideToAttack();
    return true;
}
void HeroDaJi::displayHeroLevel(int level) {
    // һ Label ʾȼ
    this->label = Label::createWithTTF("lv." + std::to_string(level), "fonts/Marker Felt.ttf", 24);

    // Label λ
    label->setPosition(Vec2(Vec2(this->getContentSize().width / 2,
        this->getContentSize().height + 30 - label->getContentSize().height)));

    // ıɫ
    label->setColor(Color3B::WHITE);

    // Label ӵ
    this->addChild(label, 1);
}
void HeroDaJi::upDateMoving()
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

            // ĽǶ
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
void HeroDaJi::initWalkingAnimation(Vec2 destination) {
    Vector<SpriteFrame*> animFrames; // ֡
    animFrames.pushBack(SpriteFrame::create("DaJiright1.png", Rect(0, 0, 430, 430))); // ֡
    animFrames.pushBack(SpriteFrame::create("DaJiright2.png", Rect(0, 0, 430, 430))); // ֡
    animFrames.pushBack(SpriteFrame::create("DaJiright3.png", Rect(0, 0, 430, 430))); // ֡
    animFrames.pushBack(SpriteFrame::create("DaJiright4.png", Rect(0, 0, 430, 430))); // ֡
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.225f);
    auto animate = Animate::create(animation);
    auto repeatForever = RepeatForever::create(animate);
    this->stopAllActions();
    this->runAction(repeatForever);  // ʼ߶

    auto Time = (this->getPosition().distance((_attackTarget)->getPosition())) / _spd;
    auto move = MoveTo::create(Time, destination);
    this->runAction(move);

}

void HeroDaJi::performAttack() {
    Vector<SpriteFrame*> animAttackFrames;
    //animAttackFrames.pushBack(SpriteFrame::create("DaJiAttackright1.png", Rect(0, 0, 400, 400))); // ֡
    animAttackFrames.pushBack(SpriteFrame::create("DaJiAttackright2.png", Rect(0, 0, 400, 400))); // ֡
    animAttackFrames.pushBack(SpriteFrame::create("DaJiAttackright3.png", Rect(0, 0, 400, 400))); // ֡
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
CallFunc* HeroDaJi::fireArrow() {
    auto fireArrow = CallFunc::create([this]()
        {
            std::string path;
            INT32 size = 0;
            INT32 damageDelta = 0;
            if (this->getEnergyBar() && (this->getEnergyBar()->getPercent() == 100))
            {
                auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
                audio->playEffect("wanshua.mp3", false);
                audio->setEffectsVolume(1.0f);
                path = "DaJiSkill1.png";
                size = 20;
                this->getEnergyBar()->setCurrentState(1);
                damageDelta = this->getSkillPower();
            }
            else
            {
                path = "Normal.png";
                size = 40;
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
void HeroDaJi::upGrade()
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

    this->addChild(this->getHpBar());
    this->getHpBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 0));

    this->addChild(this->getEnergyBar());
    this->getEnergyBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height - 10));
}
Hero* HeroDaJi::clone() const {
    // 1. 创建新的妲己实例
    auto newHero = new HeroDaJi();

    // 2. 复制基本属性
    newHero->setLv(this->getLv());
    newHero->setCamp(this->getCamp());
    newHero->setIsDead(this->getIsDead());
    newHero->setIsHurt(this->getIsHurt());
    newHero->setIsMoving(this->getIsMoving());
    newHero->setIsOnTheStage(this->getIsOnTheStage());
    
    // 3. 复制战斗属性
    newHero->setHpLim(this->getHpLim());
    newHero->setSpd(this->getSpd());
    newHero->setAttackScope(this->getAttackScope());
    newHero->setAttackPower(this->getAttackPower());
    newHero->setSkillPower(this->getSkillPower());
    newHero->setEnergyLim(this->getEnergyLim());
    newHero->setEnergyRecoverRate(this->getEnergyRecoverRate());
    newHero->setDir(this->getDir());

    // 4. 复制UI标签
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

    // 5. 复制血条
    if (this->getHpBar()) {
        auto newHpBar = HP::create();
        newHpBar->setMaxValue(this->getHpBar()->getMaxValue());
        newHpBar->setCurrentState(this->getHpBar()->getCurrentState());
        newHpBar->setPosition(this->getHpBar()->getPosition());
        newHpBar->setScale(this->getHpBar()->getScale());
        newHero->setHpBar(newHpBar);
    }

    // 6. 复制能量条
    if (this->getEnergyBar()) {
        auto newEnergyBar = HP::create();
        newEnergyBar->setMaxValue(this->getEnergyBar()->getMaxValue());
        newEnergyBar->setCurrentState(this->getEnergyBar()->getCurrentState());
        newEnergyBar->setPosition(this->getEnergyBar()->getPosition());
        newEnergyBar->setScale(this->getEnergyBar()->getScale());
        newHero->setEnergyBar(newEnergyBar);
    }

    // 7. 复制精灵属性
    newHero->setScale(this->getScale());
    newHero->setPosition(this->getPosition());
    newHero->setRotation(this->getRotation());
    newHero->setVisible(this->isVisible());
    newHero->setOpacity(this->getOpacity());

    // 8. 复制dragableSprite相关属性
    newHero->setDragable(this->isDragable());
    
    // 注意：不复制 _attackTarget，因为这是战斗时的动态目标
    newHero->setAttackTarget(nullptr);

    return newHero;
}

