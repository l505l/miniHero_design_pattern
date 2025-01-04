#pragma once
#ifndef __CLIENTSCENE_H__
#define __CLIENTSCENE_H__
#include"cocos2d.h"
#include"ui/CocosGUI.h"
class ClientScene : public cocos2d::Scene, public cocos2d::ui::EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    //void addHero(int ID);
    void initConnectServerButton();
    void ConnectServer(cocos2d::Ref* ref);
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
    // ���༭���ı��ı�ʱ����

    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
    // ���û����»س���༭��ʧȥ����ʱ����
    void update(float);
    void back(Ref* ref);
    //Net part
    //hero
    // implement the "static create()" method manually
    CREATE_FUNC(ClientScene);
private:
    int server_port;
    int client_port;
    std::string IP;
    cocos2d::Label *label;
    cocos2d::ui::EditBox* portInputBox;
    cocos2d::ui::EditBox* IPInputBox;

};

#endif