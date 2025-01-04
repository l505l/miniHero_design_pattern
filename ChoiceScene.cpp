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

#include "ChoiceScene.h"
#include "MusicSettingScene.h"
#include "LoginScene.h"
#include "HelloWorldScene.h"
#include "LianjiScene.h"
#include "SimpleAudioEngine.h"
extern int _player;
USING_NS_CC;

Scene* Choice::createScene()
{
    return Choice::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in ChoiceScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Choice::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
   
    if (!audio->isBackgroundMusicPlaying()) {
        audio->playBackgroundMusic("longtimenosee.mp3", true);
        audio->setBackgroundMusicVolume(0.5f);
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
                                           CC_CALLBACK_1(Choice::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
   // auto menu = Menu::create(closeItem, NULL);
    //--------------------------张睿康写的
    auto menuItemImageBack = MenuItemImage::create("back.png", "back1.png", CC_CALLBACK_1(Choice::back, this));
    menuItemImageBack->setPosition(Vec2( 40,visibleSize.height - 20));
  
    auto menuItemImageMusic = MenuItemImage::create("qiehuanyinyue.png", "qiehuanyinyue1.png", CC_CALLBACK_1(Choice::setmusic, this));
    menuItemImageMusic->setPosition(Vec2(visibleSize.width-40, visibleSize.height - 20));
    menuItemImageMusic->setScale(0.25);
   
    auto menuItemImageHelloWorld = MenuItemImage::create("Danji.png", "Danji1.png", CC_CALLBACK_1(Choice::gotoHelloWorld, this));
    menuItemImageHelloWorld->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 + 100));

    auto menuItemImageLianji = MenuItemImage::create("Lianji.png", "Lianji1.png", CC_CALLBACK_1(Choice::gotolianji, this));
    menuItemImageLianji->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 - 100));

   // --------------------张睿康写的结束

    auto menu = Menu::create(closeItem, menuItemImageBack, menuItemImageMusic, menuItemImageHelloWorld, menuItemImageLianji,NULL);

    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

/*
>>>>>>> mybranch
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }
<<<<<<< HEAD

    // add "Choice" splash screen"
    auto sprite = Sprite::create("Choice.png");
    if (sprite == nullptr)
    {
        problemLoading("'Choice.png'");
=======
*/
    // add "Choice" splash screen"
    auto sprite = Sprite::create("youxijiemian.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'youxijiemian.jpg'");

    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }


   

    /*
    auto menu = Menu::create(menuItemImageBack, NULL);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu);

*/

  /*  auto spriteduola = Sprite::create("duola.png");
    this->addChild(spriteduola, 0);
    spriteduola->setPosition(100, 200);*/
    return true;
}


void Choice::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
void Choice::back(Ref* obj) {
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopBackgroundMusic();
    auto scene = LoginScene::createScene();
    Director::getInstance()->replaceScene(scene);

}
void Choice::setmusic(Ref* obj) {
    auto scene = MusicSetting::createScene();
    Director::getInstance()->replaceScene(scene);

}

void Choice::gotoHelloWorld(Ref* obj) {
    _player = 1;
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);
}

void Choice::gotolianji(Ref* obj) {
    _player = 2;
    auto scene = Lianji::createScene();
    Director::getInstance()->replaceScene(scene);
}