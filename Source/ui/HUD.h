#pragma once
#include "axmol.h"
#include "ui/CocosGUI.h"

class HUD : public ax::Layer
{
public:
    virtual bool init() override;
    CREATE_FUNC(HUD);

    // CombatSystem sẽ gọi hàm này
    void addKill(int amount = 1);
    void addGold(int amount);

    // Hàm để cập nhật thanh bar
    void updateHP(int currentHp, int maxHp);
    void updateStamina(int currentStamina, int maxStamina);
    void updateMana(int currentMana, int maxMana);

    // GameOverScene sẽ gọi hàm này để in điểm
    int getKillCount() const { return _killCount; }
    int getGoldCount() const { return _goldCount; }

private:
    int _killCount = 0;
    int _goldCount = 0;

    ax::Label* _lblKills;
    ax::Label* _lblGold;

    // Giao diện thanh máu và năng lượng
    ax::ui::Layout* _hpBarFill;
    ax::ui::Layout* _staminaBarFill;
    ax::ui::Layout* _manaBarFill;
    ax::Label* _lblHpText;
    ax::Label* _lblStaminaText;
    ax::Label* _lblManaText;
};
