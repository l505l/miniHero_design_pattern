#ifndef __ENERGY_BAR_H__
#define __ENERGY_BAR_H__

#include "StateBar.h"

// Bridge Pattern: 具体实现类
// EnergyBar 是 StateBar 的另一个具体实现，专门用于处理能量值显示
// 展示了 Bridge 模式支持多种不同实现的特性
class EnergyBar : public StateBar {
public:
    // Bridge Pattern: 工厂方法创建具体实现实例
    // 与 HealthBar 类似，但创建的是能量值状态栏实例
    static EnergyBar* create(INT32 maxValue, INT32 recoverRate, Ecamp camp);

    // Bridge Pattern: 实现父类定义的接口
    // 提供能量值状态栏特定的初始化逻辑
    virtual bool init(INT32 maxValue, INT32 recoverRate, Ecamp camp) override;
    
    // Bridge Pattern: 实现父类定义的数值更新接口
    // 处理能量值特有的更新逻辑
    virtual void updateValue(INT32 delta) override;
    
    // Bridge Pattern: 实现父类定义的恢复接口
    // 能量值需要自动恢复，因此提供具体实现
    virtual void recover(float delta) override;
};

#endif