#include "ClientScene.h"
#include"Net.h"
#include"Command.h"
#include"HelloWorldScene.h"
#include"ChoiceScene.h"
#include<regex>
USING_NS_CC;

Scene* ClientScene::createScene()
{
    return ClientScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in ClientSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool ClientScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(ClientScene::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    auto menuItemImageBack = MenuItemImage::create("back.png", "back1.png", CC_CALLBACK_1(ClientScene::back, this));
    menuItemImageBack->setPosition(Vec2(40, visibleSize.height - 20));
    if (menuItemImageBack == nullptr ||
        menuItemImageBack->getContentSize().width <= 0 ||
        menuItemImageBack->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        menuItemImageBack->setPosition(Vec2(40, visibleSize.height - 20));
    }

    auto menu = Menu::create(closeItem, menuItemImageBack, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    Size backGroundSize = getContentSize();
    auto spriteBack = Sprite::create("ClientBack.png");
    spriteBack->setPosition(Vec2(backGroundSize.width / 2 + origin.x, backGroundSize.height / 2 + origin.y));
    this->addChild(spriteBack);

    // 创建 端口输入框
    auto portSize = Size(500, 50); // 设置大小
    portInputBox = cocos2d::ui::EditBox::create(portSize, cocos2d::ui::Scale9Sprite::create("boxPlayground.png"));

    // 配置属性
    portInputBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2)); // 设置位置
    portInputBox->setFontColor(Color3B::WHITE); // 设置字体颜色
    portInputBox->setPlaceHolder("Enter Your connect port here"); // 设置占位符文本
    portInputBox->setPlaceholderFontColor(Color3B::GRAY); // 设置占位符字体颜色
    portInputBox->setMaxLength(6); // 设置最大字符数
    portInputBox->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE); // 设置键盘返回类型
    this->addChild(portInputBox);
    portInputBox->setDelegate(this);

    auto portlabel = Label::createWithTTF("port:", "fonts/Marker Felt.ttf", 40);
    portlabel->setColor(Color3B::WHITE);
    portlabel->setPosition(Vec2(-60, 25));
    portInputBox->addChild(portlabel);

    // 创建 IP 对象
    auto IPSize = Size(500, 50); // 设置大小
    IPInputBox = cocos2d::ui::EditBox::create(IPSize, cocos2d::ui::Scale9Sprite::create("boxPlayground.png"));

    // 配置 EditBox 属性
    IPInputBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2-100)); // 设置位置
    IPInputBox->setFontColor(Color3B::WHITE); // 设置字体颜色
    IPInputBox->setPlaceHolder("Enter connect IP here"); // 设置占位符文本
    IPInputBox->setPlaceholderFontColor(Color3B::GRAY); // 设置占位符字体颜色
    IPInputBox->setMaxLength(15); // 设置最大字符数
    IPInputBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE); // 设置键盘返回类型
    this->addChild(IPInputBox);

    IPInputBox->setDelegate(this);

    auto IPlabel = Label::createWithTTF("IP:", "fonts/Marker Felt.ttf", 40);
    IPlabel->setColor(Color3B::WHITE);
    IPlabel->setPosition(Vec2(-60, 25));
    IPInputBox->addChild(IPlabel);

    initConnectServerButton();
    //scheduleUpdate();
    return true;
}

void ClientScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) {
    // 检查哪个编辑框发送了文本更改事件
    if (editBox == portInputBox) {
        // 对 portInputBox 输入的文本进行操作
        if (text == "") return;
        else
        {
            try {
                client_port = std::stoi(text);
            }
            catch (const std::invalid_argument& e) {
                // 处理无效输入
                std::cerr << "Invalid argument: " << e.what() << std::endl;
                return;
            }
            catch (const std::out_of_range& e) {
                // 处理数值超出范围
                std::cerr << "Out of range: " << e.what() << std::endl;
                return;
            }
        }
    }
    else if (editBox == IPInputBox) {
        // 对 editBox2 输入的文本进行操作
        std::regex ipRegex(R"(^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)");
        //可以对标准的Ipv4地址进行匹配
        this->removeChild(label);
        if (std::regex_match(text, ipRegex))
        {
            //匹配成功
            label = Label::createWithTTF("Correct!", "fonts/Marker Felt.ttf", 40); 
            label->setColor(Color3B::GREEN);
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            label->setPosition(Vec2(editBox->getPositionX(), editBox->getPositionY() - 60));//
            label->setVisible(TRUE);
            this->addChild(label);
            // 设置一个定时器来在10秒后自动移除这个标签
            this->runAction(Sequence::create(DelayTime::create(7.0f), CallFunc::create([=]() {
                this->removeChild(label);
                }), nullptr));
        }
        else
        {
            //匹配失败
            label = Label::createWithTTF("IP input format error! please input aggain!", "fonts/Marker Felt.ttf", 40);
            label->setColor(Color3B::RED);
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            label->setPosition(Vec2(editBox->getPositionX(), editBox->getPositionY() - 60));//
            label->setVisible(TRUE);
            this->addChild(label);
            // 设置一个定时器来在3秒后自动移除这个标签
            this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=]() {
                this->removeChild(label);
                }), nullptr));
        }
    }

}

void ClientScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    std::string text = editBox->getText();
    if (editBox == portInputBox) {
        // 对 portInputBox 输入的文本进行操作
        if (text == "") return;
        else
        {
            try {
                client_port = std::stoi(text);
            }
            catch (const std::invalid_argument& e) {
                // 处理无效输入
                std::cerr << "Invalid argument: " << e.what() << std::endl;
                return;
            }
            catch (const std::out_of_range& e) {
                // 处理数值超出范围
                std::cerr << "Out of range: " << e.what() << std::endl;
                return;
            }
        }
    }
    else if (editBox == IPInputBox) {
        // 对 editBox 输入的文本进行操作
        if(text!="")
        {
            IP = text;
        }
    }
}

void ClientScene::initConnectServerButton()
{
    // 获取屏幕尺寸
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Menu* menu = Menu::create();
    addChild(menu);
    MenuItem* joinMenu = MenuItemFont::create("connect", CC_CALLBACK_1(ClientScene::ConnectServer, this));
    /*joinMenu->setPosition(Vec2(origin.x + visibleSize.width - joinMenu->getContentSize().width / 2-200,
        origin.y + joinMenu->getContentSize().height / 2 + 100));*/
    menu->addChild(joinMenu);
    joinMenu->setPosition(Vec2(300, -200));

    // 设置菜单项位置为屏幕右下角
}

void ClientScene::ConnectServer(cocos2d::Ref* ref)
{
    if (Net::getInstance()->Connect(client_port, &IP[0]))
    {
        /*std::string heromessage = "100101010101";
        Command cmd;
        cmd.sendHeroMessage(heromessage);
        cmd.recvHeroMessage();
        CCLOG("%s", cmd.HeroMessage);*/
        CCLOG("Connect success!");
        auto nextScene = HelloWorld::create();
        Director::getInstance()->replaceScene(
            TransitionSlideInT::create(1.0f / 60, nextScene));
    }
    else
    {
        auto errorlabel = Label::createWithTTF("the server can not find!", "fonts/Marker Felt.ttf", 40);
        errorlabel->setColor(Color3B::WHITE);
        errorlabel->setPosition(Vec2(400,100));
        this->addChild(errorlabel);
        this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
            this->removeChild(errorlabel);
            }), nullptr));
    }
}

void ClientScene::update(float)
{

    if (Net::getInstance()->Accept())
    {
        /*std::string heromessage = "100101010101";
        Command cmd;
        if (Net::getInstance()->getisConnect())
        {
            cmd.sendHeroMessage(heromessage);
            cmd.recvHeroMessage();
            CCLOG("%s", cmd.HeroMessage);
        }*/
        CCLOG("Accept success!");
    }
    else
        CCLOG("Error Accept");
}

void ClientScene::back(Ref* obj) {
    //auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    //audio->stopBackgroundMusic();
    auto scene = Choice::createScene();
    Director::getInstance()->replaceScene(scene);

}

void ClientScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

