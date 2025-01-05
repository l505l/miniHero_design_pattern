#pragma once
// Command interface
// This interface defines the structure for command objects
// which encapsulate all information needed to perform an action.
class IMusicCommand {
public:
    virtual ~IMusicCommand() {}
    virtual void execute() = 0; // Execute the command
    virtual void undo() = 0;    // Undo the command
}; 