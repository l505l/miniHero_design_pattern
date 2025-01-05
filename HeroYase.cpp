#include"Hero.h"
#include "HeroYase.h"
#include<string>
#include"const.h"
#include "SimpleAudioEngine.h"
HeroYase* HeroYase::create(Ecamp camp, Ref* scene)
{
    HeroYase* Daji = new (std::nothrow) HeroYase;
    if (Daji && Daji->init(camp, scene))
    {

        Daji->autorelease();
        return Daji;
    }
    CC_SAFE_DELETE(Daji);
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
    this->setHpBar(HP::create(HEALTH, this->getHpLim(), 0,getCamp()));
    this->setEnergyBar(HP::create(ENERGY, this->getEnergyLim(), getEnergyRecoverRate(),getCamp()));
    
    this->setTag(YASEYASE);
    this->setAttackTarget(nullptr);
    //this->setLatestTargetPos(Vec2(0, 0));
    //this->addChild(mySprite, 0);
    this->updateAttackTarget();

    displayHeroLevel(this->getLv());
    this->setDragable(1);
    upDateMoving();
    SkillLogic(); 
   
    auto healthBar = HealthBar::create(shooterHpLim, 0, camp);
    auto energyBar = EnergyBar::create(energyLim, shooterEnergyRecoverRate, camp);
    
    this->addStateBar(healthBar);
    this->addStateBar(energyBar);
    
    this->addChild(this->getHpBar());
    this->getHpBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 0));
    this->getHpBar()->setScale(0.7, 1);

    this->addChild(this->getEnergyBar());
    this->getEnergyBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height - 10));
    this->getEnergyBar()->setScale(0.7, 1);



    std::function<void(float)> upDateHpAndEnergyPos = [this](float) {
        if (getHpBar() != nullptr && getEnergyBar() != nullptr)
        {
            this->getHpBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 0));
            this->getEnergyBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height - 10));
            label->setPosition( Vec2(this->getContentSize().width / 2,
                this->getContentSize().height - 10 - label->getContentSize().height));
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
    std::function<void(float)> Skill = [this](float) {
        if (this->getEnergyBar() && ((this->getEnergyBar())->getCurrentState() ==
            (this->getEnergyBar())->getMaxState()))
        {
            double dis = -1;
            //�����߼�
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
                animSkillFrames.pushBack(SpriteFrame::create("MinatoSkill.png", Rect(0, 0, 400, 400))); // ����֡
                animSkillFrames.pushBack(SpriteFrame::create("flash.png", Rect(0, 0, 400, 400))); // ����֡
                animSkillFrames.pushBack(SpriteFrame::create("MinatoSkill.png", Rect(0, 0, 400, 400))); // ����֡


                auto animationSkill = Animation::createWithSpriteFrames(animSkillFrames, 0.15f); // 0.1fΪÿ֡����ʱ��
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
                this->getEnergyBar()->setCurrentState(5);
            }
        }
        if (getIsDead())
            this->unschedule("Skill");
    };
    this->schedule(Skill, 1.0f, "Skill");
}

void HeroYase::initWalkingAnimation(Vec2 destination) {
    Vector<SpriteFrame*> animFrames; // ����֡����

    animFrames.pushBack(SpriteFrame::create("Minatorun1.png", Rect(0, 0, 400, 400))); // ����֡
    animFrames.pushBack(SpriteFrame::create("Minatorun2.png", Rect(0, 0, 400, 400))); // ����֡
    animFrames.pushBack(SpriteFrame::create("Minatorun3.png", Rect(0, 0, 400, 400))); // ����֡
    animFrames.pushBack(SpriteFrame::create("Minatorun4.png", Rect(0, 0, 400, 400))); // ����֡
    animFrames.pushBack(SpriteFrame::create("Minatorun4.png", Rect(0, 0, 400, 400))); // ����֡
    animFrames.pushBack(SpriteFrame::create("Minatorun3.png", Rect(0, 0, 400, 400))); // ����֡
    animFrames.pushBack(SpriteFrame::create("Minatorun2.png", Rect(0, 0, 400, 400))); // ����֡
    animFrames.pushBack(SpriteFrame::create("Minatorun1.png", Rect(0, 0, 400, 400))); // ����֡

  
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.125f);
    auto animate = Animate::create(animation);
    auto repeatForever = RepeatForever::create(animate);
    this->stopAllActions();
    this->runAction(repeatForever);  // ��ʼ�������߶���

    auto Time = (this->getPosition().distance((_attackTarget)->getPosition())) / _spd;
    auto move = MoveTo::create(Time, destination);
    this->runAction(move);
}


//һֱ�򣬸�stop
void HeroYase::performAttack() {
    Vector<SpriteFrame*> animAttackFrames;
    
    animAttackFrames.pushBack(SpriteFrame::create("MinatoAttack1.png", Rect(0, 0, 400, 400))); // ����֡
    animAttackFrames.pushBack(SpriteFrame::create("MinatoAttack2.png", Rect(0, 0, 400, 400))); // ����֡

    auto animationAttack = Animation::createWithSpriteFrames(animAttackFrames, 0.5f); // 0.1fΪÿ֡����ʱ��
    auto animateAttack = Animate::create(animationAttack);
    auto ATTACK = Spawn::create(animateAttack, nullptr, nullptr);

    //�չ���һ������
    auto attackSequence = Spawn::create(ATTACK, this->hitTarget(), nullptr);
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

CallFunc* HeroYase::hitTarget() {
    //�����߼�
    auto fireArrow = CallFunc::create([this]() {
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("Base_Attack_Minato.mp3", false);
        audio->setEffectsVolume(1.0f);
        if (_attackTarget) {
            INT32 damageDelta = attackPower;
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
    });
    return fireArrow;
}



void HeroYase::displayHeroLevel(int level) {
    // ����һ�� Label ������ʾ�ȼ�
    this->label = Label::createWithTTF("lv."+std::to_string(level), "fonts/Marker Felt.ttf", 24);

    // ���� Label ��λ��
    label->setPosition(Vec2(Vec2(this->getContentSize().width / 2,
        this->getContentSize().height + 30 - label->getContentSize().height)));

    // �����ı���ɫ
    label->setColor(Color3B::WHITE);

    // �� Label ���ӵ�����
    this->addChild(label, 1);
}
void HeroYase::upGrade()
{
    this->removeChild(this->getHpBar(), true);
    this->removeChild(this->getEnergyBar(), true);

    this->setHpLim(getHpLim() * 2);
   
    this->setSpd(getSpd() * 1.5f);
    this->setLv(getLv() + 1);

    this->removeChild(label, true);
    this->displayHeroLevel(getLv());

    this->setAttackPower(getAttackPower() * 2);
    this->setSkillPower(getSkillPower() * 2);
    this->setHpBar(HP::create(HEALTH, this->getHpLim(), 0,getCamp()));
    this->setEnergyBar(HP::create(ENERGY, this->getEnergyLim(), getEnergyRecoverRate() * 2,getCamp()));

    this->addChild(this->getHpBar());
    this->getHpBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 0));

    this->addChild(this->getEnergyBar());
    this->getEnergyBar()->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height - 10));
}
