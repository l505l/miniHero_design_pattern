#include "ServerScene.h"
#include"Net.h"
#include"Command.h"
#include"HelloWorldScene.h"
#include"ChoiceScene.h"
USING_NS_CC;

Scene* ServerScene::createScene()
{
    return ServerScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in ServerSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool ServerScene::init()
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
        CC_CALLBACK_1(ServerScene::menuCloseCallback, this));

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

    auto menuItemImageBack = MenuItemImage::create("back.png", "back1.png", CC_CALLBACK_1(ServerScene::back, this));
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


    auto menu = Menu::create(closeItem,menuItemImageBack, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    Size backGroundSize = getContentSize();
    auto spriteBack = Sprite::create("ServerBack.png");
    spriteBack->setPosition(Vec2(backGroundSize.width / 2 + origin.x, backGroundSize.height / 2 + origin.y));
    this->addChild(spriteBack);

    // 创建 EditBox 对象
    auto portSize = Size(500, 50); // 设置大小
    auto portBox = ui::EditBox::create(portSize, ui::Scale9Sprite::create("boxPlayground.png"));

    // 配置 EditBox 属性
    portBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2)); // 设置位置
    portBox->setFontColor(Color3B::WHITE); // 设置字体颜色
    portBox->setPlaceHolder("Enter port here"); // 设置占位符文本
    portBox->setPlaceholderFontColor(Color3B::GRAY); // 设置占位符字体颜色
    portBox->setMaxLength(7); // 设置最大字符数
    portBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE); // 设置键盘返回类型
    this->addChild(portBox);

    portBox->setDelegate(this);

    auto portlabel = Label::createWithTTF("port:", "fonts/Marker Felt.ttf", 40);
    portlabel->setColor(Color3B::WHITE);
    portlabel->setPosition(Vec2(-60, 25));
    portBox->addChild(portlabel);

    //创建服务器按钮
    initStartServerButton();
    return true;
}

void ServerScene::editBoxReturn(cocos2d::ui::EditBox* editBox) {
    // 用户完成输入
    std::string inputText = editBox->getText();
    if (inputText == "") return;
    else
    {
        try {
            server_port = std::stoi(inputText);
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
        
        // 处理文本，例如打印输出或保存
        CCLOG("Input Text: %s", inputText);
    }
}

void ServerScene::back(Ref* obj) {
    //auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    //audio->stopBackgroundMusic();
    auto scene = Choice::createScene();
    Director::getInstance()->replaceScene(scene);

}

void ServerScene::initStartServerButton()
{
    Menu* menu = Menu::create();
    addChild(menu);

    MenuItem* item = MenuItemFont::create("create!", CC_CALLBACK_1(ServerScene::StartServer, this));
    menu->addChild(item);
    item->setPosition(Vec2(300, -100));

}

void ServerScene::StartServer(cocos2d::Ref* ref)
{
    MenuItem* item = dynamic_cast<MenuItem*> (ref);
    item->setEnabled(false);

    if (Net::getInstance()->StartServer(server_port))
    {
        CCLOG("start server OK");
        scheduleUpdate();
        /*char his_ref[2], my_ref[2];
        his_ref[0] = '0';
        my_ref[0] = '1';
        Net::getInstance()->Recv((char*)&my_ref, 1);
        Net::getInstance()->Send((char*)&his_ref, 1);
        CCLOG("%c  %c", my_ref[0], his_ref[0]);*/
    }
    else
    {
        CCLOG("Start server error!");
    }
}

void ServerScene::update(float)
{

    if (Net::getInstance()->Accept())
    {
        CCLOG("Accept success!");
        auto nextScene = HelloWorld::create();
        Director::getInstance()->replaceScene(
            TransitionSlideInT::create(1.0f / 60, nextScene));
    }
    else
        CCLOG("Error Accept");
}


void ServerScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();


}
