/*Refactored by Bridge Pattern*/
#include "StateBar.h"

// Bridge Pattern: Implementation of shared methods
// This is a common functionality that can be used by all concrete state bar implementation classes
void StateBar::updatePercent() {
    // Bridge Pattern: Implements conversion from state value to display percentage
    // This implementation decouples display logic from specific state bar types
    // Subclasses can use this method directly or override it as needed
    setPercent(_currentValue * 100.0f / _maxValue);
}

// Note: Other virtual functions (like init, updateValue, recover)
// Are implemented by concrete subclasses, demonstrating the extensibility of the Bridge pattern implementation