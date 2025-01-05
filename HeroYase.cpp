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
   
    //decideToAttack();
    //Ѫ��������
    
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


////�����߼�
//CallFunc* HeroYase::hitTarget() {
//    auto fireArrow = CallFunc::create([this]()
//        {
//            std::string path = "kuwu.png";
//    INT32 size = 0;
//    INT32 damageDelta = 0;
//    if (this->getEnergyBar() && ((this->getEnergyBar())->getCurrentState() ==
//        (this->getEnergyBar())->getMaxState()))
//    {
//        damageDelta = this->getAttackPower();
//        //�����߼�
//        HelloWorld* Scene = dynamic_cast<HelloWorld*>(this->getPresentScene());
//        Hero* farestEnermy;
//        double dis = -1;
//        for (auto it = Scene->_heroes.begin(); it != Scene->_heroes.end(); it++) {
//            if ((*it)->getCamp() != _camp && getPosition().getDistance((*it)->getPosition()) > dis) {
//                dis = getPosition().getDistance((*it)->getPosition());
//                farestEnermy = (*it);
//            }
//        }
//        setPosition(farestEnermy->getPosition());
//
//        path = "kuwu.png";
//        size = 20;
//        this->getEnergyBar()->setCurrentState(5);
//    }
//    else
//    {
//        size = 40;
//        damageDelta = this->getAttackPower();
//    }
//    auto arrowSprite = Sprite::create("kuwu.png"); // ������ʸ����
//    arrowSprite->setPosition(this->getPosition()); // ���ü�ʸ�ĳ�ʼλ��
//
//    auto spriteSize = this->getContentSize();
//    arrowSprite->setScale(spriteSize.width / (size * arrowSprite->getContentSize().width),
//        spriteSize.height / (size * arrowSprite->getContentSize().height));
//    if (_attackTarget == nullptr)
//    {
//
//        return nullptr;
//    }
//    auto moveArrow = MoveTo::create(0.5f, _attackTarget->getPosition()); // ��ʸ�ƶ���Ŀ��λ��
//    auto removeArrow = CallFunc::create([arrowSprite, this, damageDelta]() {
//        //arrowSprite->removeFromParent(); // �ƶ���ɺ��Ƴ���ʸ
//        if (arrowSprite && _attackTarget && arrowSprite->getBoundingBox().intersectsRect((_attackTarget->getBoundingBox()))) {
//            arrowSprite->removeFromParent(); // �Ƴ���ʸ
//            //arrowSprite = nullptr; // ��ռ�ʸ����
//            _attackTarget->getHpBar()->changeStateBy(-damageDelta);
//            if (getCamp() == BLUE)
//            {
//                int a = 0;
//            }
//            if (_attackTarget->getHpBar()->getCurrentState() <= 0)
//            {
//                _attackTarget->setIsDead(true);
//                _attackTarget->setVisible(false);
//                _attackTarget->stopAllActions();
//                //arrowSprite->stopAllActions();
//                //_attackTarget->mySprite->removeFromParent();
//                this->stopAllActions();
//                this->unschedule("arrow_collision_check");
//
//                upDateMoving();
//            }
//        }
//        else
//            arrowSprite->removeFromParent();
//        });
//    auto arrowSequence = Sequence::create(moveArrow, removeArrow, nullptr);
//    if (this->isVisible())
//    {
//        arrowSprite->runAction(arrowSequence); // ִ�м�ʸ����
//        dynamic_cast<HelloWorld*>(this->getPresentScene())->addChild(arrowSprite);
//    }
//        });
//    return fireArrow;
//}
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

Hero* HeroYase::clone() const {
    // 1. 创建新的妲己实例
    auto newHero = new HeroYase();

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