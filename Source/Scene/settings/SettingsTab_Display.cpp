#include "scene/settings/SettingsLayer.h"
#include "axmol.h"
#include "platform/GLViewImpl.h"

USING_NS_AX;
using namespace ax::ui;

void SettingsLayer::buildDisplayTab()
{
    // Đọc dữ liệu (Chỉ tạo biến tạm cho những thứ cần nút Apply)
    _tempResIndex     = _resIndex;
    _tempIsFullscreen = _isFullscreen;

    // 1. DROPDOWN: CHẾ ĐỘ MÀN HÌNH
    std::vector<std::string> modeOpts = {"Windowed", "Fullscreen"};
    createDropdownRow(420, "DISPLAY MODE", modeOpts, _tempIsFullscreen ? 1 : 0,
                      [this](int idx) { _tempIsFullscreen = (idx == 1); });

    // 2. DROPDOWN: ĐỘ PHÂN GIẢI
    std::vector<std::string> resOpts = {"1280 x 720 (16:9)", "1280 x 800 (16:10)", "960 x 720 (4:3)"};
    createDropdownRow(340, "RESOLUTION", resOpts, _tempResIndex, [this](int idx) { _tempResIndex = idx; });

    // 3. BUTTON: ÁP DỤNG THAY ĐỔI (Chỉ áp dụng Màn hình & Độ phân giải)
    createButtonRow(260, "--- APPLY CHANGES ---", [this]() {
        _resIndex     = _tempResIndex;
        _isFullscreen = _tempIsFullscreen;

        auto glview = dynamic_cast<ax::GLViewImpl*>(Director::getInstance()->getGLView());
        if (glview)
        {
            float w = 1280, h = 720;
            if (_resIndex == 1)
                h = 800;
            else if (_resIndex == 2)
            {
                w = 960;
                h = 720;
            }

            if (_isFullscreen)
            {
                glview->setFullscreen();
            }
            else
            {
                glview->setWindowed(w, h);
            }

            // Tự fill viền đen nếu màn hình không cùng tỷ lệ
            glview->setDesignResolutionSize(w, h, ResolutionPolicy::SHOW_ALL);

            if (_mainPanel)
            {
                _mainPanel->setPosition(Vec2(w / 2.0f, h / 2.0f));
            }
        }

        auto ud = UserDefault::getInstance();
        ud->setIntegerForKey("ResIndex", _resIndex);
        ud->setBoolForKey("IsFullscreen", _isFullscreen);
        ud->flush();

        // Vẽ lại UI để cập nhật các dòng hiển thị sau khi áp dụng
        this->switchTab(0);
    });

    // 4. TOGGLE: HIỂN THỊ FPS (Gạt là ăn luôn, không chờ Apply)
    createToggleRow(160, "SHOW FPS", _isShowFPS, [this](bool isOn) {
        _isShowFPS = isOn;  // Lưu thẳng vào biến thật

        // Thực thi lệnh của Engine NGAY LẬP TỨC
        Director::getInstance()->setStatsDisplay(_isShowFPS);

        // Lưu vào ổ cứng NGAY LẬP TỨC
        auto ud = UserDefault::getInstance();
        ud->setBoolForKey("ShowFPS", _isShowFPS);
        ud->flush();
    });
}
