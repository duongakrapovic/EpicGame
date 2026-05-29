#include "scene/settings/SettingsLayer.h"
#include "axmol.h"
#include <algorithm>
#include <string>
#include "audio/AudioManager.h"
// Nếu bạn đã tạo class AudioManager, hãy bỏ comment dòng dưới:
// #include "AudioManager.h"

USING_NS_AX;
using namespace ax::ui;

void SettingsLayer::buildAudioTab()
{
    // --- 1. MASTER VOLUME ---
    createSpinBoxRow(400, "MASTER VOLUME", std::to_string(_masterVol) + "%", [this](Label* l) {
        _masterVol = std::max(0, _masterVol - 5);
        l->setString(std::to_string(_masterVol) + "%");

        UserDefault::getInstance()->setIntegerForKey("MasterVol", _masterVol);
        UserDefault::getInstance()->flush();

        AudioManager::getInstance()->updateVolumes();
    }, [this](Label* l) {
        _masterVol = std::min(100, _masterVol + 5);
        l->setString(std::to_string(_masterVol) + "%");

        UserDefault::getInstance()->setIntegerForKey("MasterVol", _masterVol);
        UserDefault::getInstance()->flush();

        AudioManager::getInstance()->updateVolumes();
    });

    // --- 2. MUSIC VOLUME ---
    createSpinBoxRow(320, "MUSIC VOLUME", std::to_string(_musicVol) + "%", [this](Label* l) {
        _musicVol = std::max(0, _musicVol - 5);
        l->setString(std::to_string(_musicVol) + "%");

        UserDefault::getInstance()->setIntegerForKey("MusicVol", _musicVol);
        UserDefault::getInstance()->flush();

        AudioManager::getInstance()->updateVolumes();
    }, [this](Label* l) {
        _musicVol = std::min(100, _musicVol + 5);
        l->setString(std::to_string(_musicVol) + "%");

        UserDefault::getInstance()->setIntegerForKey("MusicVol", _musicVol);
        UserDefault::getInstance()->flush();

        AudioManager::getInstance()->updateVolumes();
    });

    // --- 3. SFX VOLUME (Hiệu ứng) ---
    createSpinBoxRow(240, "SFX VOLUME", std::to_string(_sfxVol) + "%", [this](Label* l) {
        _sfxVol = std::max(0, _sfxVol - 5);
        l->setString(std::to_string(_sfxVol) + "%");

        UserDefault::getInstance()->setIntegerForKey("SFXVol", _sfxVol);
        UserDefault::getInstance()->flush();

        AudioManager::getInstance()->updateVolumes();
    }, [this](Label* l) {
        _sfxVol = std::min(100, _sfxVol + 5);
        l->setString(std::to_string(_sfxVol) + "%");

        UserDefault::getInstance()->setIntegerForKey("SFXVol", _sfxVol);
        UserDefault::getInstance()->flush();

        AudioManager::getInstance()->updateVolumes();
    });
}
