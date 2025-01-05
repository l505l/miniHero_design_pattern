#pragma once

#include "IMusicCommand.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

// Concrete command class
// These classes implement the command interface and define the binding
// between a Receiver object and an action.
class PlayMusicCommand : public IMusicCommand {
private:
    std::string newMusic;
    std::string previousMusic;
    CocosDenshion::SimpleAudioEngine* audioEngine;

public:
    PlayMusicCommand(const std::string& music) 
        : newMusic(music)
        , audioEngine(CocosDenshion::SimpleAudioEngine::getInstance()) {
        // Since SimpleAudioEngine does not have a method to get the current background music
        // We can maintain a current music state externally
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