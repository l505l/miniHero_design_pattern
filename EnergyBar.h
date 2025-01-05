/*Refactored by Bridge Pattern*/
#ifndef __ENERGY_BAR_H__
#define __ENERGY_BAR_H__

#include "StateBar.h"

// Bridge Pattern: Concrete Implementation Class
// EnergyBar is another concrete implementation of StateBar, specifically for handling energy value display
// Demonstrates Bridge pattern's support for multiple different implementations
class EnergyBar : public StateBar {
public:
    // Bridge Pattern: Factory method to create concrete implementation instance
    // Similar to HealthBar, but creates energy status bar instance
    static EnergyBar* create(INT32 maxValue, INT32 recoverRate, Ecamp camp);

    // Bridge Pattern: Implements interface defined by parent class
    // Provides initialization logic specific to energy status bar
    virtual bool init(INT32 maxValue, INT32 recoverRate, Ecamp camp) override;
    
    // Bridge Pattern: Implements value update interface defined by parent class
    // Handles energy value specific update logic
    virtual void updateValue(INT32 delta) override;
    
    // Bridge Pattern: Implements recovery interface defined by parent class
    // Energy needs auto recovery, so provides concrete implementation
    virtual void recover(float delta) override;
};

#endif