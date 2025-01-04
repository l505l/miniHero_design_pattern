#ifndef __MINIHERO_H__
#define __MINIHERO_H__
#include "Hero.h"
#include "cocos2d.h"
#include<vector>

USING_NS_CC;
struct mapPoint {
	int _x;
	int _y;
	double _rad;
	Hero* _phero;
	int tag;
	mapPoint(int x=-1, int y=-1, double rad = 0, Hero* phero = nullptr) { _x = x; _y = y; _rad = rad; _phero = phero; }
};

class miniHero : public Hero
{
	CC_SYNTHESIZE(int, _money, Money);
	CC_SYNTHESIZE(int, _exp, Exp);
	CC_SYNTHESIZE(int, _maxOnBoard, MaxOnBoard);

protected:
	void initData(Ecamp Camp);

	void generateGrid(std::vector<mapPoint>& Grid, Vec2 origin, Vec2 range, Vec2 rowcol, Vec2 dir, double rad = 0);
	void initGrid();


	virtual bool onTouchBegan(Touch* t, Event* e);
	virtual void onTouchMoved(Touch* t, Event* e) {};
	virtual void onTouchEnded(Touch* t, Event* e) {};

public:
	mapPoint Delete;
	std::vector<mapPoint> Grid_Battle;
	std::vector<mapPoint> Grid_Wait;
	std::string generateMessage();//英雄标签；等级；位置
	std::vector<mapPoint>opGrid_Battle;

	mapPoint* getEmptyWait();
	mapPoint* landOn(Vec2 vec);

	static miniHero* create(const std::string& filename, Ecamp camp, twoStateType type);
	//miniHero() {};
	void upGrade(Hero* phero);

	/*void generateFormation() {
		for (auto it = Heros.begin(); it != Heros.end(); ++it)
			formation.push_back(info{ (*it)->getTag(), (*it)->getLv(), (*it)->getMapPosition() });
	}*/
	virtual void Flip();

	void takeExp(int Exp) {
		if (_exp + Exp >= getLv() * 5) {//检查有没有初始化exp
			setMaxOnBoard(getMaxOnBoard() + 1);
			setExp(_exp + Exp - getLv() * 5);
			setLv(getLv() + 1);
		}
		else
			_exp += Exp;
	}

	int getCurExpLim() {
		return getLv() * 5;
	}
};
#endif