#ifndef __STATE_BAR_H__
#define __STATE_BAR_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "const.h"

USING_NS_CC;

// Bridge Pattern: StateBar 作为实现部分的接口类
// 定义了状态栏的基本行为和属性，具体实现由子类完成
class StateBar : public ui::LoadingBar {
protected:
    // Bridge Pattern: 实现部分的核心属性
    // 这些属性被所有具体实现类共享，支持不同类型状态栏的个性化表现
    CC_SYNTHESIZE(Ecamp, _camp, Camp);                    // 阵营属性
    CC_SYNTHESIZE(INT32, _maxValue, MaxValue);           // 最大值
    CC_SYNTHESIZE(INT32, _currentValue, CurrentValue);   // 当前值
    CC_SYNTHESIZE(INT32, _recoverRate, RecoverRate);     // 恢复速率
    CC_SYNTHESIZE(std::string, _type, Type);             // 状态栏类型标识

public:
    // Bridge Pattern: 定义实现部分的接口方法
    // 这些纯虚函数强制子类提供具体实现，支持多态行为
    virtual bool init(INT32 maxValue, INT32 recoverRate, Ecamp camp) = 0;  // 初始化接口
    virtual void updateValue(INT32 delta) = 0;                             // 更新数值接口
    virtual void recover(float delta) = 0;                                 // 恢复数值接口
    
    // Bridge Pattern: 共享的默认实现
    // 可以被子类重写，展示了接口的灵活性
    virtual void updatePercent();
    
    // Bridge Pattern: 类型访问接口
    // 用于在桥接模式中识别不同的实现类
    const std::string& getType() const { return _type; }
};

#endif 