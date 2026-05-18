#include "scene/settings/SettingsLayer.h"
#include <algorithm>
#include <string>

USING_NS_AX;
using namespace ax::ui;

void SettingsLayer::buildAudioTab()
{
    createSpinBoxRow(400, "MASTER VOLUME", std::to_string(_masterVol) + "%", [this](Label* l) {
        _masterVol = std::max(0, _masterVol - 5);
        l->setString(std::to_string(_masterVol) + "%");

        // Chỉ cần lưu vào sổ tay là xong nhiệm vụ UI
        UserDefault::getInstance()->setIntegerForKey("MasterVol", _masterVol);
        UserDefault::getInstance()->flush();
    }, [this](Label* l) {
        _masterVol = std::min(100, _masterVol + 5);
        l->setString(std::to_string(_masterVol) + "%");

        UserDefault::getInstance()->setIntegerForKey("MasterVol", _masterVol);
        UserDefault::getInstance()->flush();
    });

    createSpinBoxRow(320, "MUSIC VOLUME", std::to_string(_musicVol) + "%", [this](Label* l) {
        _musicVol = std::max(0, _musicVol - 5);
        l->setString(std::to_string(_musicVol) + "%");

        UserDefault::getInstance()->setIntegerForKey("MusicVol", _musicVol);
        UserDefault::getInstance()->flush();
    }, [this](Label* l) {
        _musicVol = std::min(100, _musicVol + 5);
        l->setString(std::to_string(_musicVol) + "%");

        UserDefault::getInstance()->setIntegerForKey("MusicVol", _musicVol);
        UserDefault::getInstance()->flush();
    });

    createSpinBoxRow(240, "SFX VOLUME", std::to_string(_sfxVol) + "%", [this](Label* l) {
        _sfxVol = std::max(0, _sfxVol - 5);
        l->setString(std::to_string(_sfxVol) + "%");

        UserDefault::getInstance()->setIntegerForKey("SFXVol", _sfxVol);
        UserDefault::getInstance()->flush();
    }, [this](Label* l) {
        _sfxVol = std::min(100, _sfxVol + 5);
        l->setString(std::to_string(_sfxVol) + "%");

        UserDefault::getInstance()->setIntegerForKey("SFXVol", _sfxVol);
        UserDefault::getInstance()->flush();
    });
}
