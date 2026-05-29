#include "AudioManager.h"
#include "audio/AudioEngine.h"

USING_NS_AX;

AudioManager* AudioManager::_instance = nullptr;

AudioManager* AudioManager::getInstance()
{
    if (!_instance)
    {
        _instance = new AudioManager();
        _instance->updateVolumes();  // Lấy volume từ Settings khi vừa khởi tạo
    }
    return _instance;
}

AudioManager::AudioManager() {}

AudioManager::~AudioManager()
{
    AudioEngine::end();
}

void AudioManager::updateVolumes()
{
    auto ud = UserDefault::getInstance();

    _masterVol = ud->getIntegerForKey("MasterVol", 75) / 100.0f;
    _musicVol  = ud->getIntegerForKey("MusicVol", 75) / 100.0f;
    _sfxVol    = ud->getIntegerForKey("SFXVol", 75) / 100.0f;

    // Cập nhật volume nhạc nền ngay lập tức nếu đang phát
    if (_currentBgmId != -1)
    {
        AudioEngine::setVolume(_currentBgmId, _musicVol * _masterVol);
    }
}

void AudioManager::playBGM(const std::string& filePath, bool loop)
{
    if (_currentBgmPath == filePath && AudioEngine::getState(_currentBgmId) == AudioEngine::AudioState::PLAYING)
        return;

    stopBGM();
    _currentBgmPath = filePath;

    // Áp dụng công thức: Volume Nhạc * Volume Tổng
    _currentBgmId = AudioEngine::play2d(filePath, loop, _musicVol * _masterVol);
}

void AudioManager::stopBGM()
{
    if (_currentBgmId != -1)
    {
        AudioEngine::stop(_currentBgmId);
        _currentBgmId   = -1;
        _currentBgmPath = "";
    }
}

int AudioManager::playSFX(const std::string& filePath, bool loop)
{
    // Áp dụng công thức: Volume SFX * Volume Tổng
    return AudioEngine::play2d(filePath, loop, _sfxVol * _masterVol);
}
