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

#include "LoginScene.h"
#include "ChoiceScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

Scene* LoginScene::createScene()
{
    return LoginScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in ChoiceScene.cpp\n");
}

// on "init" you need to initialize your instance
bool LoginScene::init()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    if (!audio->isBackgroundMusicPlaying()) {
        audio->playBackgroundMusic("music.mp3", true);
    }

    if (!Scene::init())
    {
        return false;
    }
    Size wndSize = getContentSize();
    auto spriteBack = Sprite::create("INIT.PNG");
    spriteBack->setPosition(Vec2(wndSize.width / 2+origin.x, wndSize.height / 2 + origin.y));
    this->addChild(spriteBack);


    auto menuItemImageEnter = MenuItemImage::create("Start.png", "Start1.png",CC_CALLBACK_1(LoginScene::Entergame,this));

    auto menuItemImageExit = MenuItemImage::create("End.png", "End1.png", CC_CALLBACK_1(LoginScene::Exitgame,this));
    menuItemImageEnter->setPosition(Vec2(wndSize.width / 2 + origin.x, wndSize.height / 2 + origin.y + 100));
    menuItemImageExit->setPosition(Vec2(wndSize.width / 2 + origin.x, wndSize.height / 2 + origin.y - 100));
    auto menu = Menu::create(menuItemImageEnter, menuItemImageExit, NULL);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu);
  /*auto spriteLogin = Sprite::create("START.png");
    spriteLogin->setPosition(Vec2(wndSize.width / 2-100, wndSize.height / 2));
    this->addChild(spriteLogin);

    spriteLogin->getPosition();*/
    return true;
}


void LoginScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void LoginScene::Entergame(Ref* obj) {
    auto audio= CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopBackgroundMusic();
    auto scene= Choice::createScene();
    Director::getInstance()->replaceScene(scene);


}

void LoginScene::Exitgame(Ref* obj) {
    Director::getInstance()->end();
}