#include "scene/settings/SettingsLayer.h"

USING_NS_AX;

void SettingsLayer::buildControlsTab()
{
    // 1. Dọn dẹp giao diện cũ
    _rightContainer->removeAllChildren();
    _activeRows.clear();
    _currentRowIndex = 0;

    auto ud = UserDefault::getInstance();

    // 2. Đọc phím hiện tại, nếu chưa có thì gán mặc định (W, S, A, D)
    auto keyUp    = (EventKeyboard::KeyCode)ud->getIntegerForKey("Key_Up", (int)EventKeyboard::KeyCode::KEY_W);
    auto keyDown  = (EventKeyboard::KeyCode)ud->getIntegerForKey("Key_Down", (int)EventKeyboard::KeyCode::KEY_S);
    auto keyLeft  = (EventKeyboard::KeyCode)ud->getIntegerForKey("Key_Left", (int)EventKeyboard::KeyCode::KEY_A);
    auto keyRight = (EventKeyboard::KeyCode)ud->getIntegerForKey("Key_Right", (int)EventKeyboard::KeyCode::KEY_D);
    auto keyAtk   = (EventKeyboard::KeyCode)ud->getIntegerForKey(
        "Key_Attack", (int)EventKeyboard::KeyCode::KEY_J);  // Ví dụ thêm nút đánh

    // 3. Tạo các hàng UI (Tọa độ Y giảm dần từ trên xuống)
    createKeybindRow(400, "MOVE UP", "Key_Up", keyUp);
    createKeybindRow(330, "MOVE DOWN", "Key_Down", keyDown);
    createKeybindRow(260, "MOVE LEFT", "Key_Left", keyLeft);
    createKeybindRow(190, "MOVE RIGHT", "Key_Right", keyRight);
    createKeybindRow(120, "ATTACK", "Key_Attack", keyAtk);

    // Cập nhật lại khung highlight cho hàng đầu tiên
    updateFocusUI();
}

void SettingsLayer::createKeybindRow(float yPos,
                                     const std::string& title,
                                     const std::string& actionName,
                                     ax::EventKeyboard::KeyCode defaultKey)
{
    // 1. Tạo nền Highlight
    auto rowBg = ax::ui::Layout::create();
    rowBg->setContentSize(ax::Size(500, 50));
    rowBg->setPosition(ax::Vec2(50, yPos));
    rowBg->setBackGroundColorType(ax::ui::Layout::BackGroundColorType::SOLID);
    rowBg->setBackGroundColor(ax::Color3B(100, 100, 255));  // Màu xanh dương nhạt
    rowBg->setBackGroundColorOpacity(0);                    // Ẩn đi khi không focus
    _rightContainer->addChild(rowBg);

    // 2. Tiêu đề hành động (Ví dụ: "MOVE UP")
    auto lblTitle = ax::Label::createWithTTF(title, "fonts/Marker Felt.ttf", 24);
    lblTitle->setAnchorPoint(ax::Vec2(0, 0.5f));
    lblTitle->setPosition(ax::Vec2(20, 25));
    rowBg->addChild(lblTitle);

    // 3. Chữ hiển thị phím (Ví dụ: "W")
    auto lblValue = ax::Label::createWithTTF(getKeyName(defaultKey), "fonts/Marker Felt.ttf", 24);
    lblValue->setAnchorPoint(ax::Vec2(1, 0.5f));
    lblValue->setPosition(ax::Vec2(480, 25));
    rowBg->addChild(lblValue);

    // 4. Lưu vào mảng quản lý
    SettingRow row;
    row.type        = RowType::KEYBIND;
    row.highlightBg = rowBg;
    row.valueLabel  = lblValue;

    // ĐÂY LÀ HÀNH ĐỘNG KHI NGƯỜI CHƠI BẤM ENTER VÀO HÀNG NÀY
    row.onAction = [this, lblValue, actionName]() {
        this->_isBindingKey  = true;
        this->_bindingAction = actionName;
        this->_bindingLabel  = lblValue;

        lblValue->setString("[ Press Any Key ]");
        lblValue->setColor(ax::Color3B::YELLOW);
    };

    _activeRows.push_back(row);
}

// Hàm Xử lý khi nhận được phím mới từ MenuInput
void SettingsLayer::processKeybind(ax::EventKeyboard::KeyCode keyCode)
{
    // Nếu bấm ESC -> Hủy, không lưu, trả lại chữ cũ
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        _isBindingKey = false;
        int oldKey    = UserDefault::getInstance()->getIntegerForKey(_bindingAction.c_str());
        _bindingLabel->setString(getKeyName((EventKeyboard::KeyCode)oldKey));
        _bindingLabel->setColor(Color3B::WHITE);
        return;
    }

    // Nếu bấm phím khác -> Lưu vào UserDefault
    UserDefault::getInstance()->setIntegerForKey(_bindingAction.c_str(), (int)keyCode);
    UserDefault::getInstance()->flush();  // Bắt buộc lưu ngay lập tức

    // Cập nhật lên UI
    _bindingLabel->setString(getKeyName(keyCode));
    _bindingLabel->setColor(Color3B::WHITE);
    _isBindingKey = false;
}

// Hàm dịch mã phím thành Chữ
std::string SettingsLayer::getKeyName(ax::EventKeyboard::KeyCode code)
{
    if (code >= EventKeyboard::KeyCode::KEY_A && code <= EventKeyboard::KeyCode::KEY_Z)
        return std::string(1, 'A' + ((int)code - (int)EventKeyboard::KeyCode::KEY_A));
    if (code >= EventKeyboard::KeyCode::KEY_0 && code <= EventKeyboard::KeyCode::KEY_9)
        return std::string(1, '0' + ((int)code - (int)EventKeyboard::KeyCode::KEY_0));

    if (code == EventKeyboard::KeyCode::KEY_UP_ARROW)
        return "UP ARROW";
    if (code == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
        return "DOWN ARROW";
    if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
        return "LEFT ARROW";
    if (code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
        return "RIGHT ARROW";
    if (code == EventKeyboard::KeyCode::KEY_SPACE)
        return "SPACE";
    if (code == EventKeyboard::KeyCode::KEY_ENTER)
        return "ENTER";
    if (code == EventKeyboard::KeyCode::KEY_ESCAPE)
        return "ESC";
    if (code == EventKeyboard::KeyCode::KEY_NONE)
        return "UNBOUND";

    return "KEY_" + std::to_string((int)code);
}
