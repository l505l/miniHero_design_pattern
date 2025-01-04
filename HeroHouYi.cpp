#include"Hero.h"
#include "HeroHouYi.h"
#include<string>
#include "SimpleAudioEngine.h"
HeroHouYi* HeroHouYi::create(Ecamp camp, Ref* scene)
{
    HeroHouYi* Daji = new (std::nothrow) HeroHouYi;
    if (Daji && Daji->init(camp, scene))
    {

        //Daji->autorelease();
        return Daji;
    }
    CC_SAFE_DELETE(Daji);
    return nullptr;
}
bool HeroHouYi::init(Ecamp camp, Ref* scene)
{
    this->addTouchListener();
    this->initWithFile("HouYiright1.png");
    myAttackSprite = Sprite::create("HouYiNormal.png");
    setPresentScene(scene);
    //mySprite->setScale(150);
    //float _attackTargetWidth = 30.0f; // 目标宽度
    //float _attackTargetHeight = 50.0f; // 目标高度
    float scaleFactorX = scalingWidth / this->getContentSize().width;
    float scaleFactorY = scalingHeight / this->getContentSize().height;
    this->setScaleX(scaleFactorX);
    this->setScaleY(scaleFactorY);

    //this->setPosition();
    this->setDir(1);
    this->setIsDead(false);
    this->setIsHurt(false);
    this->setIsMoving(false);
    this->setHpLim(shooterHpLim);
    this->setEnergyLim(energyLim);
    this->setSpd(shooterSpeed);
    this->setLv(beginLevel);
    this->setCamp(camp);
    this->setAttackScope(shooterAttackScope);
    this->setAttackPower(shooterAttackPower);
    this->setSkillPower(shooterSkillPower);
    this->setIsOnTheStage(false);

    this->setTag(HOUYIHOUYI);

    this->setEnergyRecoverRate(shooterEnergyRecoverRate);
    this->setHpBar(HP::create(HEALTH, this->getHpLim(), 0, getCamp()));
    this->setEnergyBar(HP::create(ENERGY, this->getEnergyLim(), getEnergyRecoverRate(), getCamp()));

    this->addChild(this->getHpBar());
    this->getHpBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 0));

    this->addChild(this->getEnergyBar());
    this->getEnergyBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height - 10));

    displayHeroLevel(this->getLv());
    this->setDragable(1);
    this->setAttackTarget(nullptr);

    std::function<void(float)> upDateHpAndEnergyPos = [this](float) {
        if (getHpBar() != nullptr && getEnergyBar() != nullptr)
        {
            this->getHpBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 0));
            this->getEnergyBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height - 10));
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
void HeroHouYi::displayHeroLevel(int level) {
    // 创建一个 Label 用于显示等级
    this->label = Label::createWithTTF("lv." + std::to_string(level), "fonts/Marker Felt.ttf", 24);

    // 设置 Label 的位置
    label->setPosition(Vec2(Vec2(this->getContentSize().width / 2,
        this->getContentSize().height + 30 - label->getContentSize().height)));

    // 设置文本颜色
    label->setColor(Color3B::WHITE);

    // 将 Label 添加到场景
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
    Vector<SpriteFrame*> animFrames; // 动画帧集合
    animFrames.pushBack(SpriteFrame::create("HouYiright1.png", Rect(0, 0, 430, 430))); // 添加帧
    animFrames.pushBack(SpriteFrame::create("HouYiright2.png", Rect(0, 0, 430, 430))); // 添加帧
    animFrames.pushBack(SpriteFrame::create("HouYiright3.png", Rect(0, 0, 430, 430))); // 添加帧
    animFrames.pushBack(SpriteFrame::create("HouYiright4.png", Rect(0, 0, 430, 430))); // 添加帧
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.225f);
    auto animate = Animate::create(animation);
    auto repeatForever = RepeatForever::create(animate);
    this->stopAllActions();
    this->runAction(repeatForever);  // 开始播放行走动画

    auto Time = (this->getPosition().distance((_attackTarget)->getPosition())) / _spd;
    auto move = MoveTo::create(Time, destination);
    this->runAction(move);

}
//void HeroHouYi::decideToAttack() {
    //std::function<void(float)> decisionCallback = [this](float) {
    //    // 这里是决定是否执行 ATTACK 的逻辑
    //    // 例如，可以根据游戏的状态、精灵的属性或其他条件来决定
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
    animAttackFrames.pushBack(SpriteFrame::create("HouYiAttackright1.png", Rect(0, 0, 400, 400))); // 添加帧
    animAttackFrames.pushBack(SpriteFrame::create("HouYiAttackright2.png", Rect(0, 0, 400, 400))); // 添加帧
    animAttackFrames.pushBack(SpriteFrame::create("HouYiAttackright3.png", Rect(0, 0, 400, 400))); // 添加帧
    //animAttackFrames.pushBack(SpriteFrame::create("DaJiAttackright1.png", Rect(0, 0, 400, 400)));
    auto animationAttack = Animation::createWithSpriteFrames(animAttackFrames, 0.5f); // 0.1f为每帧持续时间
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
            
          

            auto arrowSprite = Sprite::create(path); // 创建箭矢精灵
            arrowSprite->setPosition(this->getPosition()); // 设置箭矢的初始位置

            auto spriteSize = this->getContentSize();
            arrowSprite->setScale(spriteSize.width / (size * arrowSprite->getContentSize().width),
                spriteSize.height / (size * arrowSprite->getContentSize().height));
            if (_attackTarget == nullptr)
            {

                return nullptr;
            }
            auto moveArrow = MoveTo::create(0.5f, _attackTarget->getPosition()); // 箭矢移动到目标位置
            auto removeArrow = CallFunc::create([arrowSprite, this, damageDelta]() {
                //arrowSprite->removeFromParent(); // 移动完成后移除箭矢
                if (arrowSprite && _attackTarget && arrowSprite->getBoundingBox().intersectsRect((_attackTarget->getBoundingBox()))) {
                    arrowSprite->removeFromParent(); // 移除箭矢
                    //arrowSprite = nullptr; // 清空箭矢引用
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
                arrowSprite->runAction(arrowSequence); // 执行箭矢动作
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