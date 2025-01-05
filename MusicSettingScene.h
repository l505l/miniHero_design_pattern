/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __MUSICSETTING_SCENE_H__
#define __MUSICSETTING_SCENE_H__

#include "cocos2d.h"
#include "IMusicCommand.h"

// This class uses the Command Pattern to manage music settings.
// The Command Pattern encapsulates a request as an object, thereby
// allowing for parameterization of clients with queues, requests, and operations.
class MusicSetting : public cocos2d::Scene
{
private:
    // Command invoker
    // This class is responsible for executing commands and maintaining
    // a history of executed commands to support undo and redo operations.
    class MusicInvoker {
    private:
        std::stack<std::unique_ptr<IMusicCommand>> commandHistory;
        std::stack<std::unique_ptr<IMusicCommand>> undoHistory;

    public:
        void executeCommand(IMusicCommand* command) {
            command->execute();
            commandHistory.push(std::unique_ptr<IMusicCommand>(command));
        }

        void undo() {
            if (!commandHistory.empty()) {
                auto command = std::move(commandHistory.top());
                commandHistory.pop();
                command->undo();
                undoHistory.push(std::move(command));
            }
        }

        void redo() {
            if (!undoHistory.empty()) {
                auto command = std::move(undoHistory.top());
                undoHistory.pop();
                command->execute();
                commandHistory.push(std::move(command));
            }
        }
    };

    MusicInvoker invoker;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    void back(Ref* obj);
    void setmusic1(Ref* obj);
    void setmusic2(Ref* obj);
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void Volumehigh(Ref* obj);
    void Volumelow(Ref* obj);
    void Volumemiddle(Ref* obj);
    // implement the "static create()" method manually
    CREATE_FUNC(MusicSetting);
};

#endif // __Choice_SCENE_H__