#include "ui/HUD.h"

USING_NS_AX;

bool HUD::init()
{
    if (!Layer::init())
        return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin      = Director::getInstance()->getVisibleOrigin();

    // 1. Tạo dòng chữ KILLS màu Đỏ ở góc trên bên trái
    _lblKills = Label::createWithTTF("KILLS: 0", "fonts/Marker Felt.ttf", 28);
    if (_lblKills)
    {
        _lblKills->setAnchorPoint(Vec2(0, 1));
        _lblKills->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - 20));
        _lblKills->setColor(Color3B::RED);
        this->addChild(_lblKills);
    }

    // 2. Tạo dòng chữ GOLD màu Vàng nằm ngay dưới KILLS
    _lblGold = Label::createWithTTF("GOLD: 0", "fonts/Marker Felt.ttf", 28);
    if (_lblGold)
    {
        _lblGold->setAnchorPoint(Vec2(0, 1));
        _lblGold->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - 60));
        _lblGold->setColor(Color3B::YELLOW);
        this->addChild(_lblGold);
    }

    float barWidth  = 200.0f;
    float barHeight = 20.0f;
    float startX    = origin.x + 20;

    // 3. VẼ THANH MÁU (HP)
        auto hpBg = ax::ui::Layout::create();
        hpBg->setContentSize(ax::Size(barWidth, barHeight));
        hpBg->setPosition(Vec2(startX, origin.y + visibleSize.height - 110));  // Nằm dưới Gold
        hpBg->setBackGroundColorType(ax::ui::Layout::BackGroundColorType::SOLID);
        hpBg->setBackGroundColor(Color3B(50, 0, 0));  // Đỏ sậm làm nền
        this->addChild(hpBg);

        _hpBarFill = ax::ui::Layout::create();
        _hpBarFill->setContentSize(ax::Size(barWidth, barHeight));
        _hpBarFill->setBackGroundColorType(ax::ui::Layout::BackGroundColorType::SOLID);
        _hpBarFill->setBackGroundColor(Color3B::RED);  // Đỏ tươi làm lõi máu
        hpBg->addChild(_hpBarFill);

        _lblHpText = Label::createWithTTF("HEALTH:100/100", "fonts/Marker Felt.ttf", 16);
        _lblHpText->setPosition(Vec2(barWidth / 2, barHeight / 2));
        hpBg->addChild(_lblHpText);

    // 4. VẼ THANH STAMINA
        auto staminaBg = ax::ui::Layout::create();
        staminaBg->setContentSize(ax::Size(barWidth, barHeight));
        staminaBg->setPosition(Vec2(startX, origin.y + visibleSize.height - 140));
        staminaBg->setBackGroundColorType(ax::ui::Layout::BackGroundColorType::SOLID);
        staminaBg->setBackGroundColor(Color3B(0, 50, 0));  // Đổi thành màu Xanh Lá Cây đậm cho giống Stamina cổ điển
        this->addChild(staminaBg);

        _staminaBarFill = ax::ui::Layout::create();
        _staminaBarFill->setContentSize(ax::Size(barWidth, barHeight));
        _staminaBarFill->setBackGroundColorType(ax::ui::Layout::BackGroundColorType::SOLID);
        _staminaBarFill->setBackGroundColor(Color3B::GREEN);  // Ruột màu Xanh Lá sáng rực
        staminaBg->addChild(_staminaBarFill);

        _lblStaminaText = Label::createWithTTF("STAMINA:100/100", "fonts/Marker Felt.ttf", 16);
        _lblStaminaText->setPosition(Vec2(barWidth / 2, barHeight / 2));
        staminaBg->addChild(_lblStaminaText);

    // 5. VẼ THANH MANA
        auto manaBg = ax::ui::Layout::create();
        manaBg->setContentSize(ax::Size(barWidth, barHeight));
        manaBg->setPosition(Vec2(startX, origin.y + visibleSize.height - 170));  // Nằm dưới stamina
        manaBg->setBackGroundColorType(ax::ui::Layout::BackGroundColorType::SOLID);
        manaBg->setBackGroundColor(Color3B(0, 0, 50));  // Xanh sậm làm nền
        this->addChild(manaBg);

        _manaBarFill = ax::ui::Layout::create();
        _manaBarFill->setContentSize(ax::Size(barWidth, barHeight));
        _manaBarFill->setBackGroundColorType(ax::ui::Layout::BackGroundColorType::SOLID);
        _manaBarFill->setBackGroundColor(Color3B::BLUE);  // Xanh dương làm lõi Mana
        manaBg->addChild(_manaBarFill);

        _lblManaText = Label::createWithTTF("MANA:100/100", "fonts/Marker Felt.ttf", 16);
        _lblManaText->setPosition(Vec2(barWidth / 2, barHeight / 2));
        manaBg->addChild(_lblManaText);
    return true;
}

void HUD::addKill(int amount)
{
    _killCount += amount;
    if (_lblKills)
    {
        _lblKills->setString("KILLS: " + std::to_string(_killCount));

        // Hiệu ứng giật nảy: Phình to ra 1.5 lần rồi xẹp về 1.0 trong 0.2 giây
        _lblKills->stopAllActions();
        _lblKills->setScale(1.5f);
        _lblKills->runAction(ScaleTo::create(0.2f, 1.0f));
    }
}

void HUD::addGold(int amount)
{
    _goldCount += amount;
    if (_lblGold)
    {
        _lblGold->setString("GOLD: " + std::to_string(_goldCount));
    }
}
void HUD::updateHP(int currentHp, int maxHp)
{
    if (currentHp < 0)
        currentHp = 0;
    float percent = 0.0f;
    if (maxHp > 0)
    {
        percent = (float)currentHp / maxHp;
    }

    // Co giãn chiều dài của lõi màu đỏ
    _hpBarFill->setContentSize(ax::Size(200.0f * percent, 20.0f));
    _lblHpText->setString("HEALTH:   " + std::to_string(currentHp) + "/" + std::to_string(maxHp));
}
void HUD::updateStamina(int currentStamina, int maxStamina)
{
    if (currentStamina < 0)
        currentStamina = 0;
    // chống chia 0 int
    float percent = 0.0f;
    if (maxStamina > 0)
    {
        percent = (float)currentStamina / maxStamina;
    }

    _staminaBarFill->setContentSize(ax::Size(200.0f * percent, 20.0f));
    _lblStaminaText->setString("STAMINA:    " + std::to_string(currentStamina) + "/" + std::to_string(maxStamina));
}
void HUD::updateMana(int currentMana, int maxMana)
{
    if (currentMana < 0)
        currentMana = 0;
    // chống chia 0 int
    float percent = 0.0f;
    if (maxMana > 0)
    {
        percent = (float)currentMana / maxMana;
    }
    // Co giãn chiều dài của lõi màu xanh
    _manaBarFill->setContentSize(ax::Size(200.0f * percent, 20.0f));
    _lblManaText->setString("MANA:   " + std::to_string(currentMana) + "/" + std::to_string(maxMana));
}
