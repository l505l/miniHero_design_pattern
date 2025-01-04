#pragma once
#ifndef __SERVERSCENE_H__
#define __SERVERSCENE_H__
#include"cocos2d.h"
#include"ui/CocosGUI.h"
class ServerScene : public cocos2d::Scene, public cocos2d::ui::EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    //void addHero(int ID);
    void initStartServerButton();
    void StartServer(cocos2d::Ref* ref);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    void update(float);
    void back(Ref* obj);
    //Net part
    //hero
    // implement the "static create()" method manually
    CREATE_FUNC(ServerScene);
private:
    int server_port;
    std::string IP;
};

#endif
