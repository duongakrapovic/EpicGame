#include "scene/settings/SettingsLayer.h"
#include "axmol.h"
#include "platform/GLViewImpl.h"

USING_NS_AX;
using namespace ax::ui;

void SettingsLayer::buildDisplayTab()
{
    // 1. Reset mảng hàng để tránh lỗi tràn dữ liệu (Quan trọng nhất)
    _activeRows.clear();
    _currentRowIndex = 0;

    // 2. Load dữ liệu
    auto ud    = UserDefault::getInstance();
    _resIndex  = ud->getIntegerForKey("ResIndex", 0);
    _isShowFPS = ud->getBoolForKey("ShowFPS", false);
    Director::getInstance()->setStatsDisplay(_isShowFPS);

    // 3. Hàm thực thi đổi độ phân giải
    auto applyResolution = [this](int index) {
        auto glview = dynamic_cast<ax::GLViewImpl*>(Director::getInstance()->getGLView());
        if (!glview)
            return;

        float w = 1280.0f, h = 720.0f;
        if (index == 0)
        {
            w = 1280.0f;
            h = 720.0f;
        }
        else if (index == 1)
        {
            w = 1280.0f;
            h = 800.0f;
        }
        else if (index == 2)
        {
            w = 960.0f;
            h = 720.0f;
        }

        glview->setWindowed(w, h);
        glview->setDesignResolutionSize(w, h, ResolutionPolicy::NO_BORDER);

        if (_mainPanel)
        {
            _mainPanel->setPosition(Vec2(w / 2.0f, h / 2.0f));
            float scale = std::min({1.0f, w / 1280.0f, h / 720.0f});
            _mainPanel->setScale(scale);
        }

        UserDefault::getInstance()->setIntegerForKey("ResIndex", index);
        UserDefault::getInstance()->flush();

        // Load lại tab để cập nhật trạng thái text "Running"
        this->switchTab(0);
    };

    // 4. TIÊU ĐỀ
    std::vector<std::string> resOptions = {"1280 x 720", "1280 x 800", "960 x 720"};
    auto title                          = Label::createWithSystemFont("--- SELECT RESOLUTION ---", "Arial", 24);
    title->setPosition(Vec2(400, 480));
    title->setTextColor(Color4B::YELLOW);
    _mainPanel->addChild(title);

    // 5. TẠO DANH SÁCH LỰA CHỌN (3 hàng dọc)
    for (int i = 0; i < (int)resOptions.size(); ++i)
    {
        std::string name = resOptions[i];
        if (i == _resIndex)
            name += " (RUNNING)";

        // Dùng createSpinBoxRow nhưng xử lý để nó thành 1 nút xác nhận duy nhất
        createSpinBoxRow(400 - (i * 45), name, "SELECT",
                         [=, this](Label* l) { applyResolution(i); },  // Bấm trái -> Đổi
                         [=, this](Label* l) { applyResolution(i); }   // Bấm phải -> Đổi
        );
    }

    // 6. HÀNG FPS
    createSpinBoxRow(200, "SHOW FPS", _isShowFPS ? "ON" : "OFF", [this](Label* l) {
        _isShowFPS = !_isShowFPS;
        l->setString(_isShowFPS ? "ON" : "OFF");
        UserDefault::getInstance()->setBoolForKey("ShowFPS", _isShowFPS);
        UserDefault::getInstance()->flush();
        Director::getInstance()->setStatsDisplay(_isShowFPS);
    }, [this](Label* l) {
        _isShowFPS = !_isShowFPS;
        l->setString(_isShowFPS ? "ON" : "OFF");
        UserDefault::getInstance()->setBoolForKey("ShowFPS", _isShowFPS);
        UserDefault::getInstance()->flush();
        Director::getInstance()->setStatsDisplay(_isShowFPS);
    });
}
