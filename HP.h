#ifndef __HP_H_
#define __HP_H_

#include "ui/CocosGUI.h"
#include "base/CCScheduler.h"

USING_NS_CC;
enum twoStateType {
	HEALTH,
	ENERGY
};
#include"const.h"
class HP :public ui::LoadingBar
{
	CC_SYNTHESIZE(Ecamp, ccamp, Ccamp);
	CC_SYNTHESIZE(twoStateType, stateType, StateType);
	//生命值上限
	CC_SYNTHESIZE(INT32, maxState, MaxState);
	//当前生命值
	CC_SYNTHESIZE(INT32, currentState, CurrentState);
	CC_SYNTHESIZE(INT32, recoverRate, RecoverRate);

public:
	static HP* create(twoStateType Type, INT32 defaultState, INT32 defaultRecoverRate, Ecamp ccamp);
	virtual bool HP::init(twoStateType Type, INT32 defaultState, INT32 defaultRecoverRate, Ecamp ccamp);
	void energyRecover(float delta);
	void changeMaxBy(INT32 delta);
	void updatePercent();
	void changeStateBy(INT32 delta);
	void setFellow(Sprite*& sprite);
};


#endif