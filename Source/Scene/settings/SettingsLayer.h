#pragma once
#include "axmol.h"
#include "ui/CocosGUI.h"
#include <functional>
#include <vector>
#include <string>
class MenuInput;

class SettingsLayer : public ax::LayerColor
{
public:
    static SettingsLayer* create(bool isFromGame, std::function<void()> onCloseCallback = nullptr);
    virtual bool init(bool isFromGame, std::function<void()> onCloseCallback);

    // MỞ KHÓA PUBLIC: Để Component MenuInput gọi vào điều khiển
    
    void onClose();           // Dùng cho phím ESC
    void navigateUp();        // Dùng cho phím W / LÊN
    void navigateDown();      // Dùng cho phím S / XUỐNG
    void navigateLeft();      // Dùng cho phím A / TRÁI
    void navigateRight();     // Dùng cho phím D / PHẢI
    void confirmSelection();  // Dùng cho phím ENTER

    ~SettingsLayer() override;

private:

    void createMainPanel();
    void createLeftMenu();
    void switchTab(int tabIndex);
    void updateFocusUI();

    void buildDisplayTab();
    void buildAudioTab();
    void buildControlsTab();

    void createSpinBoxRow(float yPos,
                          const std::string& title,
                          const std::string& initialVal,
                          std::function<void(ax::Label*)> onMinus,
                          std::function<void(ax::Label*)> onPlus);
    void createSimpleClickRow(float yPos, const std::string& title, std::function<void(ax::Label*)> onClick);

    struct SettingRow
    {
        ax::ui::Layout* highlightBg;
        ax::Label* valueLabel;
        ax::Sprite* minusFrame;
        ax::Sprite* plusFrame;
        std::function<void(ax::Label*)> onLeftAction;
        std::function<void(ax::Label*)> onRightAction;
    };

    // --- Hệ thống điều hướng ---
    int _currentFocusLevel   = 0;  // 0: Menu Tab, 1: Chọn hàng, 2: Chỉnh thông số
    int _currentTabIndex     = 0;
    int _currentRowIndex     = 0;
    int _currentEditColIndex = 0;  // 0: Nút <, 1: Nút >

    std::vector<SettingRow> _activeRows;

    bool _isFromGame = false;
    std::function<void()> _onCloseCallback;
    ax::ui::Layout* _mainPanel = nullptr;
    ax::ui::Layout* _rightContainer = nullptr;
    std::vector<ax::ui::Button*> _tabButtons;
    ax::ui::Button* _btnQuit = nullptr;
    MenuInput* _menuInput = nullptr;

    // Dữ liệu cài đặt
    int _resIndex      = 0;
    bool _isFullscreen = false;
    bool _isShowFPS    = true;
    int _masterVol     = 75;
    int _musicVol      = 75;
    int _sfxVol        = 75;
};
