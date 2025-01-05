#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "MusicSettingScene.h"
#include"AiminiHero.h"
#include"const.h"
#include"Hero.h"
#include"HeroDaji.h"
#include"HeroYase.h"
#include"HeroHouYi.h"
#include"Command.h"
#include "SimpleAudioEngine.h"
#include <random>
USING_NS_CC;
// ����ȫ�ֵ�α������������ͷֲ�
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 3);

extern int _player;
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }



    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();



    auto sprite = Sprite::create("map.png");
    this->addChild(sprite, 0);
    sprite->setAnchorPoint(Vec2(0, 0));
    sprite->setPosition(Vec2(0, 0));
    sprite->setScaleX(1 / 2.45f);
    sprite->setScaleY(1 / 1.8f);
    
   

    ////////////////////////
    countdownLabel = Label::createWithTTF("9", "fonts/arial.ttf", 32);
    countdownLabel->setPosition(Vec2(480,580));
    this->addChild(countdownLabel,0);

    // ����ʱ��ʼֵ
    countdownValue = 9;

    // ��ʱ1���ʼ����ʱ
    this->scheduleOnce([this](float dt) {
        // ÿ��1�����һ�ε���ʱ
        this->schedule([this](float dt) {
            countdownValue--;
            if (countdownValue >= 0) {
                // ����Label��ʾ
                countdownLabel->setString(StringUtils::format("%d", countdownValue));
            }
            else {
                // ����ʱ������ȡ��schedule
                this->unschedule("countdown");
            }
            }, 1.0f, "countdown");
        }, 1.0f, "startCountdown");
    /////////////////////////////////////////////////





    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    myMiniHero = miniHero::create("minihero.png", RED, HEALTH);
    this->addChild(myMiniHero);
    myMiniHero->setDragable(0);
    myMiniHero->setPosition(Vec2(120, 120));

    for (int i = 0; i < Ainum; i++)
    {
        AiminiHero* _aiai = AiminiHero::create("minihero.png", RED, HEALTH, dis(gen));
        this->addChild(_aiai);
        _aiai->setDragable(0);
        _aiai->setPosition(Vec2(800, 500));
        _AiMiniHero.push_back(_aiai);
    }

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

  
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
        for (int i = 0; i < 4; i++)
        {
            int _Tagofhero = dis(gen);
            switch (_Tagofhero) {
            case 1:
                menuItemImageBuy[i] = MenuItemImage::create("Dajibuy.png", "Daji1.png", "Daji1.png", CC_CALLBACK_1(HelloWorld::buydaji, this));
                 break;
            case 2:
                menuItemImageBuy[i] = MenuItemImage::create("Minato.png", "Minato1.png", "Minato1.png", CC_CALLBACK_1(HelloWorld::buyyase, this));
                break;
            case 3:
                menuItemImageBuy[i] = MenuItemImage::create("Houyi.png", "Houyi1.png", "Houyi1.png", CC_CALLBACK_1(HelloWorld::buyhouyi, this));
                break;
            }
            menuItemImageBuy[i]->setPosition(Vec2(140, 240+75*i));
            menuItemImageBuy[i]->setScale(0.4);
            menuItemImageBuy[i]->setTag(100+i);
        }
        menuItemImageRefresh = MenuItemImage::create("Refresh.png", "Refresh1.png", "Refresh1.png", CC_CALLBACK_1(HelloWorld::Refresh, this));
        menuItemImageRefresh->setPosition(Vec2(100, 240 + 75 * 4));
        menuItemImageBuyExp = MenuItemImage::create("BuyExp.png", "BuyExp1.png", "BuyExp1.png", CC_CALLBACK_1(HelloWorld::buyExp, this));
        menuItemImageBuyExp->setPosition(Vec2(100, 50));
        menu = Menu::create(closeItem, menuItemImageBuy[0], menuItemImageBuy[1], menuItemImageBuy[2], menuItemImageBuy[3], menuItemImageRefresh, menuItemImageBuyExp,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    //add
    Highlight = Sprite::create("inpos.png");
    this->addChild(Highlight);
    Highlight->setVisible(false);

    Sale = Sprite::create("sale.png");
    this->addChild(Sale);
    Sale->setVisible(false);
    Sale->setScale(0.2f);
    Sale->setPosition(DleteOrigin + Vec2(40, 50));
    mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::OnMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    //ССӢ�۵ȼ�
    lvLabel = Label::createWithSystemFont("lv:" + std::to_string(myMiniHero->getLv()), "Arial", 20);
    lvLabel->setPosition(Vec2(50, 100));
    addChild(lvLabel);
    //add
    /////////////////////////////
    disPlayMoney();
    Waiting();

   
    

    return true;
}
void HelloWorld::disPlayMoney()
{
    auto sell = Sprite::create("gold.png");
    sell->setPosition(800, 570);
    sell->setScale(0.5);
    this->addChild(sell);

    this->label = Label::createWithTTF("money:" + std::to_string(this->myMiniHero->getMoney()), "fonts/Marker Felt.ttf", 30);
    
    // ���� Label ��λ��
    label->setPosition(Vec2(900, 580));

    // �����ı���ɫ
    label->setColor(Color3B::WHITE);

    // �� Label ���ӵ�����
    this->addChild(label, 1);
    std::function<void(float)> screenMoney = [this](float) {
        this->getLable()->setString("money:" + std::to_string(this->myMiniHero->getMoney()));

    };
    this->schedule(screenMoney, 0.001f, "screenMoney");
}
void HelloWorld::Waiting() {
    this->scheduleOnce(CC_CALLBACK_1(HelloWorld::startGame1, this), 10.0f, "startGame1");

    this->schedule(CC_CALLBACK_1(HelloWorld::checkWindow, this), 0.05f, "checkWindow");
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}
void HelloWorld::buydaji(Ref* obj)
{
    int _iCount = 0;
    for (auto it = myMiniHero->Grid_Battle.begin(); it < myMiniHero->Grid_Battle.end(); it++)
    {
        if (it->_phero)
            _iCount++;
    }
    for (auto it = myMiniHero->Grid_Wait.begin(); it < myMiniHero->Grid_Wait.end(); it++)
    {
        if (it->_phero)
            _iCount++;
    }
    if (myMiniHero->getMoney() >= 2 && _iCount < 10)//_iCount < myMiniHero->getMaxOnBoard())
    {
        auto Hero1 = HeroDaJi::create(RED, this);
        //Hero1->setScale(0.5f);
        Vec2 aa = Vec2(this->myMiniHero->getEmptyWait()->_x, this->myMiniHero->getEmptyWait()->_y);
        this->myMiniHero->getEmptyWait()->_phero = Hero1;
        this->addChild(Hero1);
        Hero1->setPosition(aa);
        myMiniHero->upGrade(Hero1);
        myMiniHero->setMoney(myMiniHero->getMoney() - 2);
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("buy_success.mp3", false);
        audio->setEffectsVolume(1.0f);
        auto item = static_cast<MenuItemImage*>(obj);
        valid[item->getTag() - 100] = 0;

        item->removeFromParent();
    }
   
}
void HelloWorld::buyyase(Ref* obj)
{
    int _iCount = 0;
    for (auto it = myMiniHero->Grid_Battle.begin(); it < myMiniHero->Grid_Battle.end(); it++)
    {
        if (it->_phero)
            _iCount++;
    }
    for (auto it = myMiniHero->Grid_Wait.begin(); it < myMiniHero->Grid_Wait.end(); it++)
    {
        if (it->_phero)
            _iCount++;
    }
    if (myMiniHero->getMoney() >= 2 && _iCount < 10)
    {
        auto Hero1 = HeroYase::create(RED, this);
        //Hero1->setScale(0.5);
        Vec2 aa = Vec2(this->myMiniHero->getEmptyWait()->_x, this->myMiniHero->getEmptyWait()->_y);
        this->myMiniHero->getEmptyWait()->_phero = Hero1;
        this->addChild(Hero1);
        Hero1->setPosition(aa);
        myMiniHero->upGrade(Hero1);
        myMiniHero->setMoney(myMiniHero->getMoney() - 2);
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("buy_success.mp3", false);
        audio->setEffectsVolume(1.0f);
        auto item = static_cast<MenuItemImage*>(obj);
        valid[item->getTag() - 100] = 0;
        item->removeFromParent();
    }
    
}
void HelloWorld::buyhouyi(Ref* obj)
{
    int _iCount = 0;
    for (auto it = myMiniHero->Grid_Battle.begin(); it < myMiniHero->Grid_Battle.end(); it++)
    {
        if (it->_phero)
            _iCount++;
    }
    for (auto it = myMiniHero->Grid_Wait.begin(); it < myMiniHero->Grid_Wait.end(); it++)
    {
        if (it->_phero)
            _iCount++;
    }
    if (myMiniHero->getMoney() >= 2 && _iCount <10)
    {
        auto Hero1 = HeroHouYi::create(RED, this);
        //Hero1->setScale(0.5);
        Vec2 aa = Vec2(this->myMiniHero->getEmptyWait()->_x, this->myMiniHero->getEmptyWait()->_y);
        this->myMiniHero->getEmptyWait()->_phero = Hero1;
        this->addChild(Hero1);
        Hero1->setPosition(aa);
        myMiniHero->upGrade(Hero1);
        myMiniHero->setMoney(myMiniHero->getMoney() - 2);
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("buy_success.mp3", false);
        audio->setEffectsVolume(1.0f);
        auto item = static_cast<MenuItemImage*>(obj);
        valid[item->getTag() - 100] = 0;
        item->removeFromParent();
    }
    
}

void HelloWorld::startGame1(float ft) {
    disableMouseEvents();
    menu->setEnabled(false);
    setAlldragable(false);
    myMiniHero->setDragable(true);
    if (_player == 1)
    {
       /* for (int i = 0; i < 28; i++)
        {
            _message_[i * 2] = '1';
            _message_[i * 2+1] = '1';
        }*/
        _AiMiniHero[_order]->setVisible(true);
        _AiMiniHero[_order]->sendAIMessage();
        for (int i = 0; i < 56; i++)
            _message_[i] = _AiMiniHero[_order]->_heroMessage[i];
    }
    else  if (_player == 2) {
        Command *com=new Command;
        com->sendHeroMessage(myMiniHero->generateMessage());
        com->recvHeroMessage();
        for (int i = 0; i < 56; i++)
            _message_[i] = com->HeroMessage[i];
        ////����д���������message��ô�仯
    }

    this->scheduleOnce(CC_CALLBACK_1(HelloWorld::startGame2, this), 2.0, "startGame2");
    
}

void HelloWorld::startGame2(float ft) {
    // Get the singleton instance of prototype registry
    auto& registry = HeroPrototypeRegistry::getInstance();

    // Create opponent (blue team) heroes
    for (int i = 0; i < 28; i++) {
        Hero* newHero = nullptr;
        int heroType;
        // Extract level information from message
        int level = _message_[2 * i + 1] - '0';
        
        // Convert character code to hero type
        switch (_message_[2 * i]) {
        case '1':
            heroType = DAJIDAJI;
            break;
        case '2':
            heroType = YASEYASE;
            break;
        case '3':
            heroType = HOUYIHOUYI;
            break;
        default:
            continue;
        }

        // Clone hero from prototype registry with desired configuration
        newHero = registry.createHero(heroType, BLUE, level);
        
        if (newHero) {
            // Configure the cloned hero
            newHero->setIsOnTheStage(true);
            // Set position based on battle grid
            newHero->setPosition(Vec2(myMiniHero->opGrid_Battle[i]._x, 
                                    myMiniHero->opGrid_Battle[i]._y));
            newHero->setDragable(false);
            
            // Add hero to scene and tracking vector
            this->addChild(newHero);
            _heroes.push_back(newHero);
        }
    }

    // Create player (red team) heroes
    for (auto it = myMiniHero->Grid_Battle.begin(); 
         it < myMiniHero->Grid_Battle.end(); it++) {
        if (it->_phero != nullptr) {
            // Get existing hero's type and level
            int heroType = (it->_phero)->getTag();
            int level = (it->_phero)->getLv();
            // Remove the old hero instance
            it->_phero->removeFromParent();

            // Clone new hero from prototype registry
            Hero* newHero = registry.createHero(heroType, RED, level);

            if (newHero) {
                // Configure the cloned hero
                newHero->setIsOnTheStage(true);
                newHero->setDragable(false);
                newHero->setPosition(Vec2(it->_x, it->_y));

                // Update grid reference and add to scene
                it->_phero = newHero;
                this->addChild(newHero);
                _heroes.push_back(newHero);
            }
        }
    }

    // Schedule win/lose condition check
    this->schedule(CC_CALLBACK_1(HelloWorld::checkWinLose, this), 
                  0.05f, "checkWinLose");
}
int HelloWorld::cheak() {
    //һû
    int win=2;
    for (auto it = _heroes.begin(); it < _heroes.end(); it++)
    {
        if ((*it)->getCamp() == RED && (*it)->getIsDead() == FALSE)
        {
            win = 0;
            break;
        }
    }
    if (win == 2)//
        return 0;
    for (auto it = _heroes.begin(); it < _heroes.end(); it++)
    {
        if ((*it)->getCamp() == BLUE && (*it)->getIsDead() == FALSE)
        {
            win = 1;
            break;
        }
    }
    if (win == 0)//
        return 1;
    return 2;
}
void HelloWorld::checkWinLose(float ft) {
    int c = cheak();
   
    if (c == 0) {//
        // 
        myMiniHero->getHpBar()->setCurrentState(myMiniHero->getHpBar()->getCurrentState() - 4);
        myMiniHero->getHpBar()->updatePercent();
       


        this->unschedule("checkWinLose");
        //Ӣۻָ״̬
        for (auto it = myMiniHero->Grid_Battle.begin(); it < myMiniHero->Grid_Battle.end(); it++)
        {
            if (it->_phero)
            {
                it->_phero->stopAllActions();
                it->_phero->getHpBar()->setCurrentState(it->_phero->getHpBar()->getMaxState());
                it->_phero->getHpBar()->updatePercent();
            }
        }
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("endgame_lose.mp3", false);
        audio->setEffectsVolume(1.0f);

        //


        myMiniHero->setLv(myMiniHero->getLv() + 1);
        if (myMiniHero->getMaxOnBoard() < 10)
            myMiniHero->setMaxOnBoard(myMiniHero->getMaxOnBoard() + 1);
        myMiniHero->setMoney(myMiniHero->getMoney() + 1);

        //  this->scheduleOnce(CC_CALLBACK_1(HelloWorld::startGame1, this), 10.0, "startGame");
        this->scheduleOnce([this](float dt) {
            // ��Lambda����ʽ�д�������
            auto sprite = Sprite::create("Defeat.jpg");
            sprite->setScaleX(1.5f);
            sprite->setScaleY(1.6f);
            sprite->setAnchorPoint(Vec2(0, 0));
            sprite->setPosition(Vec2(0, 0));
            this->addChild(sprite, 0);

            // ��ʱ2���ִ��Lambda����ʽ
            this->scheduleOnce([this, sprite](float dt) {
                // ��Lambda����ʽ���Ƴ�����
                this->removeChild(sprite);
                }, 2.0f, "removeSprite");
            }, 1.0f, "createSprite");
        Gamewinorlose(false);

        // ������Ϸ�Ѿ�����

    }
    else if (c == 1) {//�Է�Ӣ��������
        // ��Ӯ�����߼�
        this->unschedule("checkWinLose");
        // ������Ϸ�Ѿ�����

        //Ӣ�ۻָ���״̬
        for (auto it = myMiniHero->Grid_Battle.begin(); it < myMiniHero->Grid_Battle.end(); it++)
        {
            if (it->_phero)
            {
                it->_phero->stopAllActions();
                it->_phero->getHpBar()->setCurrentState(it->_phero->getHpBar()->getMaxState());
                it->_phero->getHpBar()->updatePercent();
            }
        }
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("endgame_win.mp3", false);
        audio->setEffectsVolume(1.0f);
        
        myMiniHero->setLv(myMiniHero->getLv() + 1);
        if (myMiniHero->getMaxOnBoard() < 10)
            myMiniHero->setMaxOnBoard(myMiniHero->getMaxOnBoard() + 1);
        myMiniHero->setMoney(myMiniHero->getMoney() + 3);

        this->scheduleOnce([this](float dt) {
            // ��Lambda����ʽ�д�������
            auto sprite = Sprite::create("Victory.jpg");
            sprite->setScaleX(1.5f);
            sprite->setScaleY(1.6f);
            sprite->setAnchorPoint(Vec2(0, 0));
            sprite->setPosition(Vec2(0, 0));
            this->addChild(sprite, 0);
            // ��ʱ2���ִ��Lambda����ʽ
            this->scheduleOnce([this, sprite](float dt) {
                // ��Lambda����ʽ���Ƴ�����
                this->removeChild(sprite);
                }, 2.0f, "removeSprite");
            }, 1.0f, "createSprite");
        Gamewinorlose(true);

    }
    else {
        // ��Ϸ��δ����ʤ��
       // CCLOG("��Ϸ��δ����ʤ��");
    }
}

void HelloWorld::Gamewinorlose(int c) {
 
    if (_player == 1)
    {
        _AiMiniHero[_order]->setVisible(false);
        _order++;
        if (_order >= Ainum||(myMiniHero->getHpBar()->getCurrentState() <= 0))
        {

            auto scene = MusicSetting::createScene();
            Director::getInstance()->replaceScene(scene);
        }
        for (int i = 0; i < Ainum; i++)
        {
            _AiMiniHero[i]->setLv(_AiMiniHero[i]->getLv() + 1);
            if(_AiMiniHero[i]->getMaxOnBoard()<10)
                _AiMiniHero[i]->setMaxOnBoard(_AiMiniHero[i]->getMaxOnBoard() + 1);
            _AiMiniHero[i]->setMoney(_AiMiniHero[i]->getMoney() + dis(gen));
        }
        //�ӽ�ҡ�����
    }
    else if (_player == 2)
    {
        if (_order>=Max_order||myMiniHero->getHpBar()->getCurrentState() <= 0)
        {
            auto scene = MusicSetting::createScene();
            Director::getInstance()->replaceScene(scene);

        }
        //�˳����߼���
        _order++;
    }
    //��ʾ��Ӯ
    this->scheduleOnce(CC_CALLBACK_1(HelloWorld::resetGame, this), 3.0f, "resetGame");
}

void HelloWorld::resetGame(float dt) {
  
        for (int i = 0; i < 4; i++)
            if (valid[i] == 1)
                menu->removeChild(menuItemImageBuy[i], 1);
        for (int i = 0; i < 4; i++)
        {
            int _Tagofhero = dis(gen);
            switch (_Tagofhero) {
            case 1:
                menuItemImageBuy[i] = MenuItemImage::create("Dajibuy.png", "Daji1.png", "Daji1.png", CC_CALLBACK_1(HelloWorld::buydaji, this));
                break;
            case 2:
                menuItemImageBuy[i] = MenuItemImage::create("Minato.png", "Minato1.png", "Minato1.png", CC_CALLBACK_1(HelloWorld::buyyase, this));
                break;
            case 3:
                menuItemImageBuy[i] = MenuItemImage::create("Houyi.png", "Houyi1.png", "Houyi1.png", CC_CALLBACK_1(HelloWorld::buyhouyi, this));
                break;
            }
            menuItemImageBuy[i]->setPosition(Vec2(140.0f, static_cast<float>(240 + 75 * i)));
            menuItemImageBuy[i]->setScale(0.4f);
            menuItemImageBuy[i]->setTag(100 + i);
            menu->addChild(menuItemImageBuy[i]);
            valid[i] = 1;
        }
    
    // ���֮ǰ����Ϸ����
   // this->removeAllChildren();
    /////�Ȱ������ϳ�Ӣ����Ϊ���ɼ�������
    for (auto it = _heroes.begin(); it < _heroes.end(); it++)
    {
        //
        (*it)->stopAllActions();
        (*it)->setVisible(false);
        (*it)->setIsDead(true);
    }
    ////�ٰѵ��˵ľ����Ƴ�
   for (auto it = _heroes.begin(); it < _heroes.end(); it++)
    {
       if ((*it)->getCamp() == BLUE)
           this->removeChild(*it);
    }
   //�������Ӣ�۵�����
    _heroes.clear();
    //�����ҵ��ϳ�Ӣ�ۣ������ǻָ���λ�á�������������
    for (auto it = myMiniHero->Grid_Battle.begin(); it < myMiniHero->Grid_Battle.end(); it++)
    {
        //������ҵ�Ӣ��
        if (it->_phero != nullptr)
        {
            it->_phero->setIsDead(false);
            it->_phero->setVisible(true);
            it->_phero->setPosition(Vec2(it->_x, it->_y));
        }
    }
        // ���³�ʼ����Ϸ
   // reinitGame();
   countdownValue = 9;
   this->scheduleOnce([this](float dt) {
       // ÿ��1�����һ�ε���ʱ
       this->schedule([this](float dt) {
           countdownValue--;
           if (countdownValue >= 0) {
               // ����Label��ʾ
               countdownLabel->setString(StringUtils::format("%d", countdownValue));
           }
           else {
               // ����ʱ������ȡ��schedule
               this->unschedule("countdown");
           }
           }, 1.0f, "countdown");
       }, 1.0f, "startCountdown");
   myMiniHero->setDragable(false);
   setAlldragable(true);
   menu->setEnabled(true);
  enableMouseEvents();
   myMiniHero->setPosition(Vec2(120, 120));
    // ��ʱһ��ʱ���ʼ�µ�һ��
    this->scheduleOnce(CC_CALLBACK_1(HelloWorld::startGame1, this), 10.0f, "restartGame");
}

void HelloWorld::setAlldragable(bool _BOOL)
{
    for (auto it = myMiniHero->Grid_Battle.begin(); it < myMiniHero->Grid_Battle.end(); it++)
    {

        if (it->_phero != nullptr)
            it->_phero->setDragable(_BOOL);
    }
    for (auto it = myMiniHero->Grid_Wait.begin(); it < myMiniHero->Grid_Wait.end(); it++)
    {
        if (it->_phero != nullptr)
            it->_phero->setDragable(_BOOL);
    }
}
void HelloWorld::checkWindow(float ft) {

    if (this->getPosition() != Vec2(0, 0))
        this->setPosition(Vec2(0, 0));
}
void HelloWorld::onMouseDown(Event* event) {
    Sale->setVisible(true);
}

void HelloWorld::onMouseUp(Event* event) {
    Sale->setVisible(false);
}

void HelloWorld::OnMouseMove(Event* event) {
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
    if (mouseEvent) {
        Vec2 mpos = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
        for (const auto& item : myMiniHero->Grid_Battle) {
            if (mpos.getDistance(Vec2(item._x, item._y)) < item._rad) {
                Highlight->setColor(Color3B::WHITE);
                Highlight->setVisible(true);
                Highlight->setPosition(Vec2(item._x, item._y));
                return;
            }
        }
        for (const auto& item : myMiniHero->Grid_Wait) {
            if (mpos.getDistance(Vec2(item._x, item._y)) < item._rad) {
                Highlight->setColor(Color3B::GREEN);
                Highlight->setVisible(true);
                Highlight->setPosition(Vec2(item._x, item._y));
                return;
            }
        }
        Highlight->setVisible(false);
    }
}

void HelloWorld::buyExp(Ref* obj)
{
    //ÿ��һ�λ���������ң���5�㾭��
    if (myMiniHero->getMoney() >= 2) {
        myMiniHero->setMoney(myMiniHero->getMoney() - 2);
        myMiniHero->takeExp(5);
    }
    else {
    }
    float percent = static_cast<float>(myMiniHero->getExp()) / myMiniHero->getCurExpLim()*20;
    this->myMiniHero->getEnergyBar()->setCurrentState(static_cast<INT32>(percent));
    this->myMiniHero->getEnergyBar()->updatePercent();
    lvLabel->setString("lv:" + std::to_string(myMiniHero->getLv()));
}
void HelloWorld::Refresh(Ref* obj) {
    if (myMiniHero->getMoney() >= 1)
    {
        for (int i = 0; i < 4; i++)
            if(valid[i]==1)
            menu->removeChild(menuItemImageBuy[i], 1);
        for (int i = 0; i < 4; i++)
        {
            int _Tagofhero = dis(gen);
            switch (_Tagofhero) {
            case 1:
                menuItemImageBuy[i] = MenuItemImage::create("Dajibuy.png", "Daji1.png", "Daji1.png", CC_CALLBACK_1(HelloWorld::buydaji, this));
                break;
            case 2:
                menuItemImageBuy[i] = MenuItemImage::create("Minato.png", "Minato1.png", "Minato1.png", CC_CALLBACK_1(HelloWorld::buyyase, this));
                break;
            case 3:
                menuItemImageBuy[i] = MenuItemImage::create("Houyi.png", "Houyi1.png", "Houyi1.png", CC_CALLBACK_1(HelloWorld::buyhouyi, this));
                break;
            }
            menuItemImageBuy[i]->setPosition(Vec2(140.0f, static_cast<float>(240 + 75 * i)));
            menuItemImageBuy[i]->setScale(0.4f);
            menuItemImageBuy[i]->setTag(100 + i);
            menu->addChild(menuItemImageBuy[i]);
            valid[i] = 1;
        }
   
    myMiniHero->setMoney(myMiniHero->getMoney() - 1);
    }
}
