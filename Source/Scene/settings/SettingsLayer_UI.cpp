#include "scene/settings/SettingsLayer.h"

USING_NS_AX;
using namespace ax::ui;

void SettingsLayer::createMainPanel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin      = Director::getInstance()->getVisibleOrigin();

    _mainPanel = Layout::create();
    _mainPanel->setBackGroundImage("AxmolDefault/survivor_panel_bg.png");
    _mainPanel->setBackGroundImageScale9Enabled(true);
    _mainPanel->setContentSize(Size(1000, 600));
    _mainPanel->setAnchorPoint(Vec2(0.5f, 0.5f));
    _mainPanel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(_mainPanel);

    auto titleLabel = Label::createWithTTF("SETTINGS", "fonts/arial.ttf", 36);
    titleLabel->setColor(Color3B(124, 58, 237));
    titleLabel->setAnchorPoint(Vec2(0, 1));
    titleLabel->setPosition(Vec2(40, 560));
    _mainPanel->addChild(titleLabel);

    _rightContainer = Layout::create();
    _rightContainer->setContentSize(Size(700, 500));
    _rightContainer->setPosition(Vec2(280, 20));
    _mainPanel->addChild(_rightContainer);

    if (_isFromGame)
    {
        _btnQuit = Button::create("AxmolDefault/survivor_tab_normal.png");
        _btnQuit->setScale9Enabled(true);
        _btnQuit->setContentSize(Size(220, 50));
        _btnQuit->setTitleText("QUIT TO MENU");
        _btnQuit->setTitleColor(Color3B(255, 50, 50));
        _btnQuit->setTitleFontSize(20);
        _btnQuit->setPosition(Vec2(130, 50));
        _btnQuit->setTouchEnabled(false);
        _mainPanel->addChild(_btnQuit);
    }
}

void SettingsLayer::createLeftMenu()
{
    std::vector<std::string> tabNames = {"DISPLAY", "AUDIO", "CONTROLS"};
    float startY                      = 480.0f;
    float gapY                        = 70.0f;

    for (int i = 0; i < (int)tabNames.size(); i++)
    {
        auto btn = Button::create("AxmolDefault/survivor_tab_normal.png");
        btn->setScale9Enabled(true);
        btn->setContentSize(Size(220, 50));
        btn->setPosition(Vec2(130, startY - (i * gapY)));
        btn->setTitleText(tabNames[i]);
        btn->setTitleFontSize(24);
        btn->setTouchEnabled(false);
        _mainPanel->addChild(btn);
        _tabButtons.push_back(btn);
    }
}

void SettingsLayer::createSpinBoxRow(float yPos,
                                     const std::string& title,
                                     const std::string& initialVal,
                                     std::function<void(Label*)> onMinus,
                                     std::function<void(Label*)> onPlus)
{
    auto rowBg = Layout::create();
    rowBg->setBackGroundImage("AxmolDefault/survivor_tab_normal.png");
    rowBg->setBackGroundImageScale9Enabled(true);
    rowBg->setContentSize(Size(660, 50));
    rowBg->setAnchorPoint(Vec2(0, 0.5));
    rowBg->setPosition(Vec2(10, yPos));
    rowBg->setOpacity(0);
    _rightContainer->addChild(rowBg);

    auto lblTitle = Label::createWithTTF(title, "fonts/arial.ttf", 20);
    lblTitle->setAnchorPoint(Vec2(0, 0.5));
    lblTitle->setPosition(Vec2(20, yPos));
    _rightContainer->addChild(lblTitle);

    auto valueGroup = Layout::create();
    valueGroup->setContentSize(Size(240, 40));
    valueGroup->setPosition(Vec2(450, yPos));
    valueGroup->setAnchorPoint(Vec2(0.5, 0.5));
    _rightContainer->addChild(valueGroup);

    auto btnMinus = Sprite::create("AxmolDefault/survivor_arrow_left_normal.png");
    btnMinus->setPosition(Vec2(20, 20));
    valueGroup->addChild(btnMinus);

    auto minusFrame = Sprite::create("AxmolDefault/survivor_button_focus_frame.png");
    minusFrame->setPosition(Vec2(20, 20));
    minusFrame->setOpacity(0);
    valueGroup->addChild(minusFrame);

    auto lblValue = Label::createWithTTF(initialVal, "fonts/arial.ttf", 20);
    lblValue->setPosition(Vec2(120, 20));
    valueGroup->addChild(lblValue);

    auto btnPlus = Sprite::create("AxmolDefault/survivor_arrow_right_normal.png");
    btnPlus->setPosition(Vec2(220, 20));
    valueGroup->addChild(btnPlus);

    auto plusFrame = Sprite::create("AxmolDefault/survivor_button_focus_frame.png");
    plusFrame->setPosition(Vec2(220, 20));
    plusFrame->setOpacity(0);
    valueGroup->addChild(plusFrame);

    SettingRow row;
    row.type          = RowType::SPINBOX;
    row.highlightBg   = rowBg;
    row.valueLabel    = lblValue;
    row.minusFrame    = minusFrame;
    row.plusFrame     = plusFrame;
    row.onLeftAction  = onMinus;
    row.onRightAction = onPlus;
    _activeRows.push_back(row);
}

void SettingsLayer::createDropdownRow(float yPos,
                                      const std::string& title,
                                      const std::vector<std::string>& options,
                                      int selectedIndex,
                                      std::function<void(int)> onSelect)
{
    auto rowBg = Layout::create();
    rowBg->setBackGroundImage("AxmolDefault/survivor_tab_normal.png");
    rowBg->setBackGroundImageScale9Enabled(true);
    rowBg->setContentSize(Size(660, 50));
    rowBg->setAnchorPoint(Vec2(0, 0.5));
    rowBg->setPosition(Vec2(10, yPos));
    rowBg->setOpacity(0);
    _rightContainer->addChild(rowBg);

    auto lblTitle = Label::createWithTTF(title, "fonts/arial.ttf", 20);
    lblTitle->setAnchorPoint(Vec2(0, 0.5));
    lblTitle->setPosition(Vec2(20, yPos));
    _rightContainer->addChild(lblTitle);

    auto lblValue = Label::createWithTTF(options.empty() ? "" : options[selectedIndex], "fonts/arial.ttf", 20);
    lblValue->setPosition(Vec2(450, yPos));
    _rightContainer->addChild(lblValue);

    auto arrow = Label::createWithTTF("v", "fonts/arial.ttf", 20);
    arrow->setPosition(Vec2(620, yPos));
    _rightContainer->addChild(arrow);

    // Thêm Focus Frame cho nút mũi tên Dropdown
    auto arrowFrame = Sprite::create("AxmolDefault/survivor_button_focus_frame.png");
    arrowFrame->setPosition(Vec2(620, yPos));
    arrowFrame->setOpacity(0);
    _rightContainer->addChild(arrowFrame);

    auto listBg = Layout::create();
    listBg->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    listBg->setBackGroundColor(Color3B(40, 40, 50));
    listBg->setContentSize(Size(300, options.size() * 40));
    listBg->setAnchorPoint(Vec2(0.5, 1));
    listBg->setPosition(Vec2(450, yPos - 25));
    listBg->setVisible(false);
    _rightContainer->addChild(listBg, 100);

    std::vector<Label*> items;
    for (int i = 0; i < options.size(); i++)
    {
        auto itemLbl = Label::createWithTTF(options[i], "fonts/arial.ttf", 18);
        itemLbl->setPosition(Vec2(150, listBg->getContentSize().height - 20 - (i * 40)));
        listBg->addChild(itemLbl);
        items.push_back(itemLbl);
    }

    SettingRow row;
    row.type             = RowType::DROPDOWN;
    row.highlightBg      = rowBg;
    row.valueLabel       = lblValue;
    row.plusFrame        = arrowFrame;  // Mượn biến plusFrame để điều khiển UI arrow
    row.dropdownListBg   = listBg;
    row.dropdownItems    = items;
    row.onDropdownSelect = onSelect;
    _activeRows.push_back(row);
}

void SettingsLayer::createToggleRow(float yPos, const std::string& title, bool isOn, std::function<void(bool)> onToggle)
{
    auto rowBg = Layout::create();
    rowBg->setBackGroundImage("AxmolDefault/survivor_tab_normal.png");
    rowBg->setBackGroundImageScale9Enabled(true);
    rowBg->setContentSize(Size(660, 50));
    rowBg->setAnchorPoint(Vec2(0, 0.5));
    rowBg->setPosition(Vec2(10, yPos));
    rowBg->setOpacity(0);
    _rightContainer->addChild(rowBg);

    auto lblTitle = Label::createWithTTF(title, "fonts/arial.ttf", 20);
    lblTitle->setAnchorPoint(Vec2(0, 0.5));
    lblTitle->setPosition(Vec2(20, yPos));
    _rightContainer->addChild(lblTitle);

    auto offLbl = Label::createWithTTF("OFF", "fonts/arial.ttf", 20);
    offLbl->setPosition(Vec2(400, yPos));
    offLbl->setColor(Color3B::GRAY);
    _rightContainer->addChild(offLbl);

    auto onLbl = Label::createWithTTF("ON", "fonts/arial.ttf", 20);
    onLbl->setPosition(Vec2(500, yPos));
    onLbl->setColor(Color3B::GRAY);
    _rightContainer->addChild(onLbl);

    auto offFrame = Sprite::create("AxmolDefault/survivor_button_focus_frame.png");
    offFrame->setPosition(Vec2(400, yPos));
    offFrame->setOpacity(0);
    _rightContainer->addChild(offFrame);

    auto onFrame = Sprite::create("AxmolDefault/survivor_button_focus_frame.png");
    onFrame->setPosition(Vec2(500, yPos));
    onFrame->setOpacity(0);
    _rightContainer->addChild(onFrame);

    SettingRow row;
    row.type        = RowType::TOGGLE;
    row.highlightBg = rowBg;
    row.offFrame    = offFrame;
    row.onFrame     = onFrame;
    row.toggleState = isOn;  // Gán trạng thái ban đầu
    row.onToggle    = onToggle;
    _activeRows.push_back(row);
}

void SettingsLayer::createButtonRow(float yPos, const std::string& title, std::function<void()> onClick)
{
    auto rowBg = Layout::create();
    rowBg->setBackGroundImage("AxmolDefault/survivor_tab_normal.png");
    rowBg->setBackGroundImageScale9Enabled(true);
    rowBg->setContentSize(Size(660, 50));
    rowBg->setAnchorPoint(Vec2(0, 0.5));
    rowBg->setPosition(Vec2(10, yPos));
    rowBg->setOpacity(0);
    _rightContainer->addChild(rowBg);

    auto lblTitle = Label::createWithTTF(title, "fonts/arial.ttf", 20);
    lblTitle->setAnchorPoint(Vec2(0.5, 0.5));
    lblTitle->setPosition(Vec2(330, yPos));
    lblTitle->setColor(Color3B::YELLOW);
    _rightContainer->addChild(lblTitle);

    SettingRow row;
    row.type        = RowType::BUTTON;
    row.highlightBg = rowBg;
    row.onAction    = onClick;
    _activeRows.push_back(row);
}

void SettingsLayer::switchTab(int tabIndex)
{
    _rightContainer->removeAllChildren();
    _activeRows.clear();
    _isDropdownOpen  = false;
    _currentTabIndex = tabIndex;
    _currentRowIndex = 0;

    if (tabIndex < _tabButtons.size())
    {
        switch (tabIndex)
        {
        case 0:
            buildDisplayTab();
            break;
        case 1:
            buildAudioTab();
            break;
        case 2:
            buildControlsTab();
            break;
        }
    }
    updateFocusUI();
}

void SettingsLayer::updateFocusUI()
{
    for (int i = 0; i < (int)_tabButtons.size(); i++)
    {
        bool isSelected = (i == _currentTabIndex);
        _tabButtons[i]->loadTextureNormal(isSelected ? "AxmolDefault/survivor_tab_active.png"
                                                     : "AxmolDefault/survivor_tab_normal.png");
        _tabButtons[i]->setTitleColor((_currentFocusLevel == 0 && isSelected) ? Color3B(255, 255, 255)
                                                                              : Color3B(150, 150, 150));
    }

    if (_btnQuit)
    {
        bool isQuitSelected = (_currentTabIndex == _tabButtons.size());
        _btnQuit->loadTextureNormal(isQuitSelected ? "AxmolDefault/survivor_tab_active.png"
                                                   : "AxmolDefault/survivor_tab_normal.png");
        _btnQuit->setTitleColor(isQuitSelected ? Color3B(255, 0, 0) : Color3B(150, 50, 50));
    }

    for (int i = 0; i < (int)_activeRows.size(); i++)
    {
        auto& row = _activeRows[i];

        bool isRowSelected = (_currentFocusLevel == 1 && i == _currentRowIndex);
        row.highlightBg->setOpacity(isRowSelected ? 255 : 0);

        if (row.type == RowType::SPINBOX)
        {
            row.minusFrame->setOpacity(isRowSelected ? 255 : 0);
            row.plusFrame->setOpacity(isRowSelected ? 255 : 0);
        }
        else if (row.type == RowType::TOGGLE)
        {
            // Auto-focus khung sáng dựa trên trạng thái thật (ON hay OFF)
            row.offFrame->setOpacity((isRowSelected && !row.toggleState) ? 255 : 0);
            row.onFrame->setOpacity((isRowSelected && row.toggleState) ? 255 : 0);
        }
        else if (row.type == RowType::DROPDOWN)
        {
            // Sáng khung viền của nút V (Mũi tên)
            if (row.plusFrame)
                row.plusFrame->setOpacity(isRowSelected ? 255 : 0);

            if (i == _currentRowIndex && _isDropdownOpen)
            {
                row.dropdownListBg->setVisible(true);
                for (int j = 0; j < row.dropdownItems.size(); j++)
                {
                    row.dropdownItems[j]->setColor(j == _dropdownHoverIndex ? Color3B::YELLOW : Color3B::WHITE);
                    row.dropdownItems[j]->setScale(j == _dropdownHoverIndex ? 1.2f : 1.0f);
                }
            }
            else
            {
                if (row.dropdownListBg)
                    row.dropdownListBg->setVisible(false);
            }
        }
        else if (row.type == RowType::KEYBIND)
        {
            bool isFocused = (_currentFocusLevel == 1 && &row == &_activeRows[_currentRowIndex]);
            if (row.valueLabel)
            {
                // Phóng to chữ và đổi màu vàng khi được trỏ tới
                row.valueLabel->setScale(isFocused ? 1.2f : 1.0f);

                // Lưu ý: Nếu đang ở chế độ chờ bấm phím (_isBindingKey) thì không ép màu trắng để tránh mất chữ [Press
                // Any Key] màu vàng
                if (!this->_isBindingKey)
                {
                    row.valueLabel->setColor(isFocused ? ax::Color3B::YELLOW : ax::Color3B::WHITE);
                }
            }
        }
    }
}
