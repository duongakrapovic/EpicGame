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

    auto ud       = ax::UserDefault::getInstance();
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

void SettingsLayer::navigateUp()
{
    if (_isDropdownOpen)
    {
        auto& row           = _activeRows[_currentRowIndex];
        _dropdownHoverIndex = (_dropdownHoverIndex - 1 + row.dropdownItems.size()) % row.dropdownItems.size();
        updateFocusUI();
        return;
    }

    int maxTabs = _isFromGame ? static_cast<int>(_tabButtons.size()) + 1 : static_cast<int>(_tabButtons.size());
    if (_currentFocusLevel == 0)
    {
        _currentTabIndex = (_currentTabIndex - 1 + maxTabs) % maxTabs;
        switchTab(_currentTabIndex);
    }
    else if (_currentFocusLevel == 1)
    {
        if (!_activeRows.empty())
            _currentRowIndex = (_currentRowIndex - 1 + _activeRows.size()) % _activeRows.size();
        updateFocusUI();
    }
}

void SettingsLayer::navigateDown()
{
    if (_isDropdownOpen)
    {
        auto& row           = _activeRows[_currentRowIndex];
        _dropdownHoverIndex = (_dropdownHoverIndex + 1) % row.dropdownItems.size();
        updateFocusUI();
        return;
    }

    int maxTabs = _isFromGame ? static_cast<int>(_tabButtons.size()) + 1 : static_cast<int>(_tabButtons.size());
    if (_currentFocusLevel == 0)
    {
        _currentTabIndex = (_currentTabIndex + 1) % maxTabs;
        switchTab(_currentTabIndex);
    }
    else if (_currentFocusLevel == 1)
    {
        if (!_activeRows.empty())
            _currentRowIndex = (_currentRowIndex + 1) % _activeRows.size();
        updateFocusUI();
    }
}

void SettingsLayer::navigateLeft()
{
    if (_isDropdownOpen)
    {
        _isDropdownOpen = false;  // Đang xổ danh sách thì bấm Trái để đóng
        updateFocusUI();
        return;
    }

    if (_currentFocusLevel == 1)
    {
        auto& r = _activeRows[_currentRowIndex];

        if (r.type == RowType::SPINBOX)
        {
            // SpinBox (Âm thanh): Bấm trái để giảm volume
            if (r.onLeftAction)
                r.onLeftAction(r.valueLabel);
        }
        else if (r.type == RowType::TOGGLE)
        {
            // Nút Gạt (FPS): Kiểm tra trạng thái hiện tại
            if (r.toggleState == true)
            {
                // Nếu đang ON -> Bấm trái để gạt về OFF
                r.toggleState = false;
                if (r.onToggle)
                    r.onToggle(false);
            }
            else
            {
                // Nếu đã OFF sẵn rồi -> Bấm trái để thoát ra Menu Trái
                _currentFocusLevel = 0;
            }
        }
        else
        {
            // Dropdown và Button: Không có tương tác Trái/Phải -> Thoát thẳng ra Menu Trái
            _currentFocusLevel = 0;
        }

        updateFocusUI();
    }
}

void SettingsLayer::navigateRight()
{
    if (_isDropdownOpen)
        return;

    if (_currentFocusLevel == 0)
    {
        if (!_activeRows.empty())
        {
            _currentFocusLevel = 1;
            _currentRowIndex   = 0;
            updateFocusUI();
        }
    }
    else if (_currentFocusLevel == 1)
    {
        auto& r = _activeRows[_currentRowIndex];
        // Bấm Phải gạt SpinBox hoặc Toggle sang ON luôn
        if (r.type == RowType::SPINBOX)
        {
            if (r.onRightAction)
                r.onRightAction(r.valueLabel);
        }
        else if (r.type == RowType::TOGGLE)
        {
            r.toggleState = true;
            if (r.onToggle)
                r.onToggle(true);
        }
        updateFocusUI();
    }
}

void SettingsLayer::confirmSelection()
{
    if (_isDropdownOpen)
    {
        auto& row = _activeRows[_currentRowIndex];
        if (row.onDropdownSelect)
            row.onDropdownSelect(_dropdownHoverIndex);

        // CẬP NHẬT CHỮ HIỂN THỊ MÀ KHÔNG CẦN SWITCHTAB
        if (row.valueLabel)
            row.valueLabel->setString(row.dropdownItems[_dropdownHoverIndex]->getString());

        _isDropdownOpen = false;
        updateFocusUI();
        return;
    }

    if (_currentFocusLevel == 0)
    {
        if (_currentTabIndex == _tabButtons.size())
            ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(0.5f, StartScene::create()));
        else if (!_activeRows.empty())
        {
            _currentFocusLevel = 1;
            _currentRowIndex   = 0;
            updateFocusUI();
        }
    }
    else if (_currentFocusLevel == 1)
    {
        auto& row = _activeRows[_currentRowIndex];
        if (row.type == RowType::DROPDOWN)
        {
            _isDropdownOpen     = true;
            _dropdownHoverIndex = 0;
            updateFocusUI();
        }
        if (row.type == RowType::BUTTON || row.type == RowType::KEYBIND)
        {
            // Kích hoạt hàm đổi phím (onAction) mà ta đã định nghĩa ở file Controls
            if (row.onAction)
            {
                row.onAction();
            }
        }
    }
}

void SettingsLayer::onClose()
{
    // ESC có nhiệm vụ lùi lại dần dần: Đóng Dropdown -> Về Menu Trái -> Thoát
    if (_isDropdownOpen)
    {
        _isDropdownOpen = false;
        updateFocusUI();
        return;
    }
    if (_currentFocusLevel == 1)
    {
        _currentFocusLevel = 0;
        updateFocusUI();
        return;
    }

    if (_onCloseCallback)
        _onCloseCallback();
    this->removeFromParent();
}

SettingsLayer::~SettingsLayer()
{
    AX_SAFE_DELETE(_menuInput);
}
