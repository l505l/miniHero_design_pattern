/*Refactored by Bridge Pattern*/
#ifndef __HEALTH_BAR_H__
#define __HEALTH_BAR_H__

#include "StateBar.h"

// Bridge Pattern: Concrete Implementation Class
// HealthBar is a concrete implementation of StateBar, specifically for handling health value display
class HealthBar : public StateBar {
public:
    // Bridge Pattern: Factory method to create concrete implementation instance
    // Ensures the creation process of implementation part is separated from abstraction
    static HealthBar* create(INT32 maxValue, INT32 recoverRate, Ecamp camp);

    // Bridge Pattern: Implements interface defined by parent class
    // Provides initialization logic specific to health status bar
    virtual bool init(INT32 maxValue, INT32 recoverRate, Ecamp camp) override;
    
    // Bridge Pattern: Implements value update interface defined by parent class
    // Handles health value specific update logic
    virtual void updateValue(INT32 delta) override;
    
    // Bridge Pattern: Implements recovery interface defined by parent class
    // Health does not need auto recovery, so provides empty implementation
    virtual void recover(float delta) override {};  // Health bar does not need auto recovery
};

#endif