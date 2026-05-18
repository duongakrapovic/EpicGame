#include "scene/settings/SettingsLayer.h"
#include "scene/StartScene.h"
#include <algorithm>
#include "input/MenuInput.h" 

USING_NS_AX;
using namespace ax::ui;

SettingsLayer* SettingsLayer::create(bool isFromGame, std::function<void()> onCloseCallback)
{
    SettingsLayer* layer = new SettingsLayer();
    if (layer && layer->init(isFromGame, onCloseCallback))
    {
        layer->autorelease();
        return layer;
    }
    AX_SAFE_DELETE(layer);
    return nullptr;
}

bool SettingsLayer::init(bool isFromGame, std::function<void()> onCloseCallback)
{
    _isFromGame      = isFromGame;
    _onCloseCallback = onCloseCallback;

    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))
        return false;

    
    auto ud = ax::UserDefault::getInstance();

    // Đọc số cũ, nếu chưa từng lưu bao giờ thì dùng số mặc định (ví dụ: 75, true, false)
    _masterVol    = ud->getIntegerForKey("MasterVol", 75);
    _musicVol     = ud->getIntegerForKey("MusicVol", 75);
    _sfxVol       = ud->getIntegerForKey("SFXVol", 75);
    _isShowFPS    = ud->getBoolForKey("ShowFPS", true);
    _isFullscreen = ud->getBoolForKey("IsFullscreen", false);
    _resIndex     = ud->getIntegerForKey("ResIndex", 0);


    createMainPanel();
    createLeftMenu();
    switchTab(0);
    _menuInput = new MenuInput(this);

    return true;
}

// 5 HÀM ĐIỀU KHIỂN ĐÃ ĐƯỢC XẺ NHỎ 

void SettingsLayer::navigateUp()
{
    // size() trả về size_t (unsigned), nhưng logic menu đang dùng int.
    // Cast về int để tránh warning kiểu dữ liệu và tránh bug khi so sánh/chạy modulo.
    int maxTabs = _isFromGame ? static_cast<int>(_tabButtons.size()) + 1
                              : static_cast<int>(_tabButtons.size());
    if (_currentFocusLevel == 0)
    {
        _currentTabIndex = (_currentTabIndex - 1 + maxTabs) % maxTabs;
        switchTab(_currentTabIndex);
    }
    else if (_currentFocusLevel == 1 || _currentFocusLevel == 2)
    {
        if (!_activeRows.empty())
            _currentRowIndex = (_currentRowIndex - 1 + _activeRows.size()) % _activeRows.size();
        updateFocusUI();
    }
}

void SettingsLayer::navigateDown()
{
    // size() trả về size_t (unsigned), nhưng logic menu đang dùng int.
    // Cast về int để tránh warning kiểu dữ liệu và tránh bug khi so sánh/chạy modulo.
    int maxTabs = _isFromGame ? static_cast<int>(_tabButtons.size()) + 1
                              : static_cast<int>(_tabButtons.size());
    // _tabButtons.size() trả về size_t còn _currentTabIndex trả về int là số signed.
    if (_currentFocusLevel == 0)
    {
        _currentTabIndex = (_currentTabIndex + 1) % maxTabs;
        switchTab(_currentTabIndex);
    }
    else if (_currentFocusLevel == 1 || _currentFocusLevel == 2)
    {
        if (!_activeRows.empty())
            _currentRowIndex = (_currentRowIndex + 1) % _activeRows.size();
        updateFocusUI();
    }
}

void SettingsLayer::navigateLeft()
{
    if (_currentFocusLevel == 1)
    {
        _currentFocusLevel = 0;
        updateFocusUI();
    }
    else if (_currentFocusLevel == 2)
    {
        if (_currentEditColIndex == 1)
        {
            _currentEditColIndex = 0;
            updateFocusUI();
        }
        else if (_currentEditColIndex == 0)
        {
            _currentFocusLevel = 1;
            updateFocusUI();
        }
    }
}

void SettingsLayer::navigateRight()
{
    if (_currentFocusLevel == 0)
    {
        if (_currentTabIndex < _tabButtons.size() && !_activeRows.empty())
        {
            _currentFocusLevel = 1;
            _currentRowIndex   = 0;
            updateFocusUI();
        }
    }
    else if (_currentFocusLevel == 1)
    {
        _currentFocusLevel   = 2;
        _currentEditColIndex = 0;
        updateFocusUI();
    }
    else if (_currentFocusLevel == 2)
    {
        if (_currentEditColIndex == 0)
        {
            _currentEditColIndex = 1;
            updateFocusUI();
        }
    }
}

void SettingsLayer::confirmSelection()
{
    if (_currentFocusLevel == 0)
    {
        if (_currentTabIndex == _tabButtons.size())
        {
            ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(0.5f, StartScene::create()));
        }
        else if (!_activeRows.empty())
        {
            _currentFocusLevel = 1;
            _currentRowIndex   = 0;
            updateFocusUI();
        }
    }
    else if (_currentFocusLevel == 1)
    {
        _currentFocusLevel   = 2;
        _currentEditColIndex = 0;
        updateFocusUI();
    }
    else if (_currentFocusLevel == 2)
    {
        auto& r = _activeRows[_currentRowIndex];
        if (_currentEditColIndex == 0 && r.onLeftAction)
            r.onLeftAction(r.valueLabel);
        else if (_currentEditColIndex == 1 && r.onRightAction)
            r.onRightAction(r.valueLabel);
    }
}

void SettingsLayer::onClose()
{
    if (_onCloseCallback)
        _onCloseCallback();
    this->removeFromParent();
}

SettingsLayer::~SettingsLayer()
{
    AX_SAFE_DELETE(_menuInput);
}
