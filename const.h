#ifndef _c_
#define _c_
#include"cocos2d.h"
#define InitHp 10
#define MINISPEED 500
#define MINIhp 20
#define BATTLE 0
#define DELETE 1
#define WAIT 2
#define DAJIDAJI 1
#define YASEYASE 2
#define HOUYIHOUYI 3
#define colArow(c,r) 7*c+r //行列对应下标转换
#define HeroType int //英雄类型
#define Probability1 92 //初始化阵营概率
#define Probability2 70 //合成概率
#define Probability3 80 //
#define Probability4 70 //

struct HeroMessage {
	int HeroLv;//等级
	HeroType herotype;//类型
	// 定义 `<` 运算符重载
	bool operator<(const HeroMessage& other) const {
		// 优先比较 camp，然后是 id，最后是 herotype
		if (HeroLv != other.HeroLv) return HeroLv < other.HeroLv;
		return herotype < other.herotype;
	}

	bool operator==(const HeroMessage& rhs) const {
		return HeroLv == rhs.HeroLv && herotype == rhs.herotype;
	}
};
const int Max_order = 6;

const Vec2 DleteOrigin = { 800,200 };
const Vec2 DleteRange = { 160,400 };
const Vec2 myBattleOrigin = { 280,100 };
const Vec2 myBattleRange = { 450,220 };
const Vec2 myBattleRowcol = { 4,7 };
const Vec2 myWaitOrigin = { 200,0 };
const Vec2 myWaitRange = { 860,100 };
const Vec2 myWaitRowcol = { 1,10 };
const Vec2 myDir = { 1,1 };
enum Ecamp {
	RED, BLUE
};
const INT32 mageHpLim = 2000;
const float mageSpeed = 40;
const float mageAttackScope = 150;
const float assasinAttackScope = 60;

const INT32 mageAttackPower = 100;//
const INT32 mageEnergyRecoverRate = 10;
const INT32 mageSkillPower = 250;

const INT32 shooterHpLim = 2000;
const float shooterSpeed = 60;
const float shooterAttackScope = 150;
const INT32 shooterAttackPower = 120;//
const INT32 shooterEnergyRecoverRate = 15;
const INT32 shooterSkillPower = 180;

const INT32 warriorHpLim = 3000;
const float warriorSpeed = 80;
const float warriorAttackScope = 100;
const INT32 warriorAttackPower = 200;//
const INT32 warriorEnergyRecoverRate = 8;
const INT32 warriorSkillPower = 320;

const INT32 energyLim = 100;

const INT32 beginLevel = 1;

const float scalingWidth = 80.0f;
const float scalingHeight = 120.0f;

const Vec2 opBattleOrigin = { 730,540 };
const Vec2 opBattleRange = { 450,220 };
const Vec2 opBattleRowcol = { 4,7 };
const Vec2 opDir = { -1,-1 };

const int Ainum = 100;



#endif 
