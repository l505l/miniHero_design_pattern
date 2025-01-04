/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "LianjiScene.h"
#include "ChoiceScene.h"
#include"ClientScene.h"
#include"ServerScene.h"
USING_NS_CC;

Scene* Lianji::createScene()
{
    return Lianji::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in ChoiceScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Lianji::init()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    if (!Scene::init())
    {
        return false;
    }
    auto spriteBack = Sprite::create("INIT.PNG");
    spriteBack->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(spriteBack);


    auto menuItemImageCreateroom = MenuItemImage::create("Createroom.png", "Createroom1.png", CC_CALLBACK_1(Lianji::gotocreateroom, this));
    auto menuItemImageEnterroom = MenuItemImage::create("Enterroom.png", "Enterroom1.png","Enterroom1.png", CC_CALLBACK_1(Lianji::gotoenterroom, this));
    auto menuItemImageBack = MenuItemImage::create("back.png", "back1.png", CC_CALLBACK_1(Lianji::back, this));
    menuItemImageCreateroom->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 100));
    menuItemImageEnterroom->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 100));
    menuItemImageBack->setPosition(Vec2(40, visibleSize.height - 20));
    auto menu = Menu::create(menuItemImageCreateroom, menuItemImageEnterroom, menuItemImageBack, NULL);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu);

    /*auto spriteLogin = Sprite::create("START.png");
      spriteLogin->setPosition(Vec2(wndSize.width / 2-100, wndSize.height / 2));
      this->addChild(spriteLogin);

      spriteLogin->getPosition();*/



   
    /*
class GameScene : public cocos2d::Scene {
public:
    virtual bool init() {
        if (!Scene::init()) {
            return false;
        }

        // 1. 初始化游戏
        initGame();

        // 2. 延时一段时间后开始游戏
        this->scheduleOnce(CC_CALLBACK_1(GameScene::startGame, this), 3.0f, "startGame");

        return true;
    }

    // 游戏初始化
    void initGame() {
        // 你的游戏初始化逻辑在这里
        CCLOG("游戏初始化");
    }

    // 开始游戏
    void startGame(float dt) {
        // 你的游戏开始逻辑在这里
        CCLOG("开始游戏");
        schedule(检测win)
        bool win = true;
        showResult(win);
    }

    // 游戏结束后显示输赢
    void showResult(bool win) {
        // 你的输赢展示逻辑在这里
        if (win) {
            CCLOG("你赢啦");
        } else {
            CCLOG("你输啦");
        }

        // 4. 延时一段时间后开始新的一局
        this->scheduleOnce(CC_CALLBACK_1(GameScene::resetGame, this), 3.0f, "resetGame");
    }

    // 开始新的一局
    void resetGame(float dt) {
        // 5. 你的开始新一局的逻辑在这里
        CCLOG("开始新一局");

        // 重新初始化游戏
        initGame();

        // 2. 延时一段时间后开始游戏
        this->scheduleOnce(CC_CALLBACK_1(GameScene::startGame, this), 3.0f, "startGame");
    }

    CREATE_FUNC(GameScene);
};

int main(int argc, char **argv) {
    // 初始化应用程序实例
    cocos2d::Application app(argc, argv);

    // 创建并运行场景
    auto scene = GameScene::create();
    cocos2d::Director::getInstance()->runWithScene(scene);

    // 运行游戏循环
    app.run();

    return 0;
}

    
    
    */


    //init
    // 
    //schedule(0.001){
    // scheduleonce(50){
    // 禁用button
    // 然后设为undragable
    // };
    // 检测win or lose；
    // if(win or lose)
    // {
    // 显示“你赢啦/你输啦”
    //  win or lose 标签转化
    // scheduleonce(3秒)
    // {
    // 清除“你赢啦/你输啦”
    // 小小英雄重新初始化
    //  重新摆放
    //
    // button启用
    //设为dragable
    // }
    // }
    // 
    // 
    // 
    // 
    // 
    // } 
    //if(时间到)
    // {
    // this->unschedule();
    // 禁用button
    // 然后设为undragable
    // 
    // }
      // if (a == 0) {
    //  小小英雄重新初始化
    //  重新摆放
    // a=1:
    // button启用
    //设为dragable
    //this->schedule();
    //}

    return true;
}


void Lianji::gotocreateroom(Ref* pSender)
{
    auto scene = ServerScene::createScene();
    Director::getInstance()->replaceScene(scene);
    //Director::getInstance()->end();
}

void Lianji::gotoenterroom(Ref* obj) 
{
    auto scene = ClientScene::createScene();
    Director::getInstance()->replaceScene(scene);
    //Director::getInstance()->end();
    //    auto scene = Choice::createScene();
       // Director::getInstance()->replaceScene(scene);
   /* float countdownTime = 1.0f;

    // 使用schedule方法定时执行回调函数
    this->scheduleOnce([countdownTime](float dt) {  auto scene2 = Choice::create(); cocos2d::Director::getInstance()->replaceScene(scene2); }, countdownTime, "sceneSwitch");
*/

}

void Lianji::back(Ref* obj) {

    auto scene = Choice::createScene();
    Director::getInstance()->replaceScene(scene);
    //Director::getInstance()->end();
}
