#include "StateBar.h"

// Bridge Pattern: 实现部分的共享方法实现
// 这是一个所有具体状态栏实现类都可以使用的通用功能
void StateBar::updatePercent() {
    // Bridge Pattern: 实现了状态值到显示百分比的转换
    // 这种实现方式使得显示逻辑与具体状态栏类型解耦
    // 子类可以直接使用此方法，也可以根据需要重写它
    setPercent(_currentValue * 100.0f / _maxValue);
}

// 注意：其他虚函数（如init、updateValue、recover）
// 由具体的子类实现，体现了Bridge模式中实现部分的可扩展性