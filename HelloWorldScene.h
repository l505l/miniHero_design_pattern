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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "miniHero.h"
#include "AiminiHero.h"
#include<vector>
class HelloWorld : public cocos2d::Scene
{

public:
    CC_SYNTHESIZE(Label*, label, Lable);
    std::vector<Hero* > _heroes;
    MenuItemImage* menuItemImageBuy[4];
    MenuItemImage* menuItemImageRefresh;
    MenuItemImage* menuItemImageBuyExp;
    Label* lvLabel;
    Menu* menu;
    void Waiting();
    static cocos2d::Scene* createScene();
    void Refresh(Ref* obj);
    miniHero* myMiniHero;
    std::vector <AiminiHero*> _AiMiniHero;
    int _order = 0;
    bool valid[4] = { 1,1,1,1 };
    void startGame1(float ft);
    void startGame2(float ft);
    void checkWinLose(float ft);
    int cheak();
    char _message_[56];
   void setAlldragable(bool _BOOL);
   void checkWindow(float ft);
    void Gamewinorlose(int c);
    void resetGame(float dt);
    virtual bool init();
    
    int countdownValue;
    Label* countdownLabel;
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    ////////////////////////////
    void buydaji(Ref* obj);
    void buyyase(Ref* obj);
    void buyhouyi(Ref* obj);
    void buyExp(Ref* obj);

    //add_to_private
    Sprite* Highlight;
    Sprite* Sale;

    EventListenerMouse* mouseListener;
    void OnMouseMove(Event* event);
    void onMouseDown(Event* event);
    void onMouseUp(Event* event);

    //add_to_public
    void disableMouseEvents() {
        _eventDispatcher->removeEventListener(mouseListener);
        Highlight->setVisible(false);
    }

    // 启用鼠标事件
    void enableMouseEvents() {
        if (!_eventDispatcher->isEnabled()) {
            _eventDispatcher->setEnabled(true);
        }
        mouseListener = EventListenerMouse::create();
        mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::OnMouseMove, this);
        mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
        mouseListener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    }
    // /////////////////////////
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);  //带参宏
    void disPlayMoney();
};

#endif // __HELLOWORLD_SCENE_H__
