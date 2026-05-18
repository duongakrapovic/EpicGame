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
    _mainPanel->setPosition(Vec2(visibleSize.width / 2 + origin.x - 500, visibleSize.height / 2 + origin.y - 300));
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
    row.highlightBg   = rowBg;
    row.valueLabel    = lblValue;
    row.minusFrame    = minusFrame;
    row.plusFrame     = plusFrame;
    row.onLeftAction  = onMinus;
    row.onRightAction = onPlus;
    _activeRows.push_back(row);
}
void SettingsLayer::switchTab(int tabIndex)
{
    _rightContainer->removeAllChildren();
    _activeRows.clear();
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

        if (_currentFocusLevel == 0 && isSelected)
        {
            _tabButtons[i]->setTitleColor(Color3B(255, 255, 255));
        }
        else
        {
            _tabButtons[i]->setTitleColor(Color3B(150, 150, 150));
        }
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

        bool isRowSelected = ((_currentFocusLevel == 1 || _currentFocusLevel == 2) && i == _currentRowIndex);
        if (isRowSelected)
        {
            row.highlightBg->setBackGroundImage("AxmolDefault/survivor_tab_active.png");
            row.highlightBg->setOpacity(255);
        }
        else
        {
            row.highlightBg->setBackGroundImage("AxmolDefault/survivor_tab_normal.png");
            row.highlightBg->setOpacity(0);
        }

        if (_currentFocusLevel == 2 && i == _currentRowIndex)
        {
            row.minusFrame->setOpacity(_currentEditColIndex == 0 ? 255 : 0);
            row.plusFrame->setOpacity(_currentEditColIndex == 1 ? 255 : 0);
        }
        else
        {
            row.minusFrame->setOpacity(0);
            row.plusFrame->setOpacity(0);
        }
    }
}
