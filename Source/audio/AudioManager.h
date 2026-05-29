#pragma once
#include "axmol.h"
#include <string>

class AudioManager
{
public:
    static AudioManager* getInstance();

    void updateVolumes();

    // Nhạc nền (BGM)
    void playBGM(const std::string& filePath, bool loop = true);
    void stopBGM();

    // Hiệu ứng (SFX)
    int playSFX(const std::string& filePath, bool loop = false);

private:
    AudioManager();
    ~AudioManager();

    static AudioManager* _instance;

    int _currentBgmId           = -1;
    std::string _currentBgmPath = "";

    float _masterVol = 1.0f;
    float _musicVol  = 1.0f;
    float _sfxVol    = 1.0f;
};
