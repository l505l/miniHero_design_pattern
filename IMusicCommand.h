#pragma once
// 命令接口
class IMusicCommand {
public:
    virtual ~IMusicCommand() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
}; 