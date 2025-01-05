/*Refactored by Bridge Pattern*/
#ifndef __STATE_BAR_H__
#define __STATE_BAR_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "const.h"

USING_NS_CC;

// Bridge Pattern: StateBar as the interface class for implementation part
// Defines basic behaviors and properties of status bars, with concrete implementation by subclasses
class StateBar : public ui::LoadingBar {
protected:
    // Bridge Pattern: Core properties of implementation part
    // These properties are shared by all concrete implementation classes, supporting customized behavior for different types of status bars
    CC_SYNTHESIZE(Ecamp, _camp, Camp);                    // Camp property
    CC_SYNTHESIZE(INT32, _maxValue, MaxValue);           // Maximum value
    CC_SYNTHESIZE(INT32, _currentValue, CurrentValue);   // Current value  
    CC_SYNTHESIZE(INT32, _recoverRate, RecoverRate);     // Recovery rate
    CC_SYNTHESIZE(std::string, _type, Type);             // Status bar type identifier

public:
    // Bridge Pattern: Define interface methods for implementation part
    // These pure virtual functions force subclasses to provide concrete implementations, supporting polymorphic behavior
    virtual bool init(INT32 maxValue, INT32 recoverRate, Ecamp camp) = 0;  // Initialization interface
    virtual void updateValue(INT32 delta) = 0;                             // Update value interface
    virtual void recover(float delta) = 0;                                 // Recovery value interface
    
    // Bridge Pattern: Shared default implementation
    // Can be overridden by subclasses, demonstrating interface flexibility
    virtual void updatePercent();
    
    // Bridge Pattern: Type access interface
    // Used to identify different implementation classes in bridge pattern
    const std::string& getType() const { return _type; }
};

#endif 