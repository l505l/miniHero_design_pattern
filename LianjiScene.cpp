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

        // 1. ��ʼ����Ϸ
        initGame();

        // 2. ��ʱһ��ʱ���ʼ��Ϸ
        this->scheduleOnce(CC_CALLBACK_1(GameScene::startGame, this), 3.0f, "startGame");

        return true;
    }

    // ��Ϸ��ʼ��
    void initGame() {
        // �����Ϸ��ʼ���߼�������
        CCLOG("��Ϸ��ʼ��");
    }

    // ��ʼ��Ϸ
    void startGame(float dt) {
        // �����Ϸ��ʼ�߼�������
        CCLOG("��ʼ��Ϸ");
        schedule(���win)
        bool win = true;
        showResult(win);
    }

    // ��Ϸ��������ʾ��Ӯ
    void showResult(bool win) {
        // �����Ӯչʾ�߼�������
        if (win) {
            CCLOG("��Ӯ��");
        } else {
            CCLOG("������");
        }

        // 4. ��ʱһ��ʱ���ʼ�µ�һ��
        this->scheduleOnce(CC_CALLBACK_1(GameScene::resetGame, this), 3.0f, "resetGame");
    }

    // ��ʼ�µ�һ��
    void resetGame(float dt) {
        // 5. ��Ŀ�ʼ��һ�ֵ��߼�������
        CCLOG("��ʼ��һ��");

        // ���³�ʼ����Ϸ
        initGame();

        // 2. ��ʱһ��ʱ���ʼ��Ϸ
        this->scheduleOnce(CC_CALLBACK_1(GameScene::startGame, this), 3.0f, "startGame");
    }

    CREATE_FUNC(GameScene);
};

int main(int argc, char **argv) {
    // ��ʼ��Ӧ�ó���ʵ��
    cocos2d::Application app(argc, argv);

    // ���������г���
    auto scene = GameScene::create();
    cocos2d::Director::getInstance()->runWithScene(scene);

    // ������Ϸѭ��
    app.run();

    return 0;
}

    
    
    */


    //init
    // 
    //schedule(0.001){
    // scheduleonce(50){
    // ����button
    // Ȼ����Ϊundragable
    // };
    // ���win or lose��
    // if(win or lose)
    // {
    // ��ʾ����Ӯ��/��������
    //  win or lose ��ǩת��
    // scheduleonce(3��)
    // {
    // �������Ӯ��/��������
    // ССӢ�����³�ʼ��
    //  ���°ڷ�
    //
    // button����
    //��Ϊdragable
    // }
    // }
    // 
    // 
    // 
    // 
    // 
    // } 
    //if(ʱ�䵽)
    // {
    // this->unschedule();
    // ����button
    // Ȼ����Ϊundragable
    // 
    // }
      // if (a == 0) {
    //  ССӢ�����³�ʼ��
    //  ���°ڷ�
    // a=1:
    // button����
    //��Ϊdragable
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

    // ʹ��schedule������ʱִ�лص�����
    this->scheduleOnce([countdownTime](float dt) {  auto scene2 = Choice::create(); cocos2d::Director::getInstance()->replaceScene(scene2); }, countdownTime, "sceneSwitch");
*/

}

void Lianji::back(Ref* obj) {

    auto scene = Choice::createScene();
    Director::getInstance()->replaceScene(scene);
    //Director::getInstance()->end();
}
