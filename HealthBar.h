#ifndef __HEALTH_BAR_H__
#define __HEALTH_BAR_H__

#include "StateBar.h"

// Bridge Pattern: 具体实现类
// HealthBar 是 StateBar 的一个具体实现，专门用于处理生命值显示
class HealthBar : public StateBar {
public:
    // Bridge Pattern: 工厂方法创建具体实现实例
    // 确保实现部分的创建过程与抽象部分分离
    static HealthBar* create(INT32 maxValue, INT32 recoverRate, Ecamp camp);

    // Bridge Pattern: 实现父类定义的接口
    // 提供生命值状态栏特定的初始化逻辑
    virtual bool init(INT32 maxValue, INT32 recoverRate, Ecamp camp) override;
    
    // Bridge Pattern: 实现父类定义的数值更新接口
    // 处理生命值特有的更新逻辑
    virtual void updateValue(INT32 delta) override;
    
    // Bridge Pattern: 实现父类定义的恢复接口
    // 生命值不需要自动恢复，因此提供空实现
    virtual void recover(float delta) override {};  // 血条不需要自动恢复
};

#endif