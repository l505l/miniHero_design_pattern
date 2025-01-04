#pragma once

#include "IMusicCommand.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

// 具体命令类
class PlayMusicCommand : public IMusicCommand {
private:
    std::string newMusic;
    std::string previousMusic;
    CocosDenshion::SimpleAudioEngine* audioEngine;

public:
    PlayMusicCommand(const std::string& music) 
        : newMusic(music)
        , audioEngine(CocosDenshion::SimpleAudioEngine::getInstance()) {
        // 由于SimpleAudioEngine没有获取当前背景音乐的方法
        // 我们可以在外部维护一个当前音乐的状态
        static std::string currentMusic;
        previousMusic = currentMusic;
        currentMusic = music;
    }

    virtual void execute() override {
        audioEngine->stopBackgroundMusic();
        audioEngine->playBackgroundMusic(newMusic.c_str(), true);
    }

    virtual void undo() override {
        if (!previousMusic.empty()) {
            audioEngine->stopBackgroundMusic();
            audioEngine->playBackgroundMusic(previousMusic.c_str(), true);
        }
    }
};

class VolumeCommand : public IMusicCommand {
private:
    float newVolume;
    float previousVolume;
    CocosDenshion::SimpleAudioEngine* audioEngine;

public:
    VolumeCommand(float volume) 
        : newVolume(volume)
        , audioEngine(CocosDenshion::SimpleAudioEngine::getInstance()) {
        previousVolume = audioEngine->getBackgroundMusicVolume();
    }

    void execute() override {
        audioEngine->setBackgroundMusicVolume(newVolume);
    }

    void undo() override {
        audioEngine->setBackgroundMusicVolume(previousVolume);
    }
};