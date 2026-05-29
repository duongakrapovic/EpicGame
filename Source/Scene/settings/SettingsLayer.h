#pragma once
#include "axmol.h"
#include "ui/CocosGUI.h"
#include <functional>
#include <vector>
#include <string>

// Khai báo trước class MenuInput để tránh include chéo vòng tròn
class MenuInput;

class SettingsLayer : public ax::LayerColor
{
public:
    // =========================================================
    // 1. KHỞI TẠO VÀ VÒNG ĐỜI (LIFECYCLE)
    // =========================================================
    static SettingsLayer* create(bool isFromGame, std::function<void()> onCloseCallback = nullptr);
    virtual bool init(bool isFromGame, std::function<void()> onCloseCallback);
    ~SettingsLayer() override;

    // =========================================================
    // 2. HỆ THỐNG ĐIỀU HƯỚNG BẰNG PHÍM (NAVIGATION)
    // =========================================================
    void onClose();           // Gọi khi bấm ESC hoặc nút Quit
    void navigateUp();        // Lên trên (Mũi tên Lên)
    void navigateDown();      // Xuống dưới (Mũi tên Xuống)
    void navigateLeft();      // Sang trái / Giảm giá trị (Mũi tên Trái)
    void navigateRight();     // Sang phải / Tăng giá trị (Mũi tên Phải)
    void confirmSelection();  // Bấm chọn (Enter / Space)
    // hỗ trợ macro
    bool isBindingKey() const { return _isBindingKey; }
    void processKeybind(ax::EventKeyboard::KeyCode keyCode);

private:
    // =========================================================
    // 3. XÂY DỰNG KHUNG GIAO DIỆN (UI FRAMEWORK)
    // =========================================================
    void createMainPanel();        // Tạo khung nền chính của Settings
    void createLeftMenu();         // Tạo cột menu bên trái (chứa các nút Tab)
    void switchTab(int tabIndex);  // Đổi sang Tab khác (Display, Audio, Controls)
    void updateFocusUI();          // Cập nhật hiệu ứng (màu nền, khung viền) khi chuyển dòng

    // Các hàm nạp nội dung chi tiết cho từng Tab
    void buildDisplayTab();
    void buildAudioTab();
    void buildControlsTab();

    // =========================================================
    // 4. CÁC HÀM TẠO HÀNG CHỨC NĂNG (ROW FACTORIES)
    // =========================================================
    // Hàng có nút Trái/Phải (Dùng cho Âm lượng, Độ phân giải)
    void createSpinBoxRow(float yPos,
                          const std::string& title,
                          const std::string& initialVal,
                          std::function<void(ax::Label*)> onMinus,
                          std::function<void(ax::Label*)> onPlus);

    // Hàng dạng xổ xuống (Dùng cho chọn Màn hình)
    void createDropdownRow(float yPos,
                           const std::string& title,
                           const std::vector<std::string>& options,
                           int selectedIndex,
                           std::function<void(int)> onSelect);

    // Hàng dạng Bật/Tắt (Dùng cho Fullscreen, V-Sync)
    void createToggleRow(float yPos, const std::string& title, bool isOn, std::function<void(bool)> onToggle);

    // Hàng dạng Nút bấm thông thường (Dùng cho Apply, Back)
    void createButtonRow(float yPos, const std::string& title, std::function<void()> onClick);

    // [MỚI] Hàng gán phím (Dùng cho Tab Controls - Đổi phím di chuyển/kỹ năng)
    void createKeybindRow(float yPos,
                          const std::string& title,
                          const std::string& actionName,
                          ax::EventKeyboard::KeyCode defaultKey);

    // =========================================================
    // 5. CẤU TRÚC LƯU TRỮ DỮ LIỆU CỦA 1 HÀNG (ROW DATA)
    // =========================================================
    enum class RowType
    {
        SPINBOX,   // Hàng Tăng/Giảm
        DROPDOWN,  // Hàng Xổ xuống
        TOGGLE,    // Hàng Bật/Tắt
        BUTTON,    // Hàng Nút bấm
        KEYBIND    // [MỚI] Hàng chờ gõ phím
    };

    struct SettingRow
    {
        RowType type                = RowType::SPINBOX;
        ax::ui::Layout* highlightBg = nullptr;  // Khung nền sáng lên khi được trỏ tới
        ax::Label* valueLabel       = nullptr;  // Chữ hiển thị giá trị hiện tại

        // --- Dữ liệu cho SpinBox & Dropdown Arrow ---
        ax::Sprite* minusFrame = nullptr;
        ax::Sprite* plusFrame  = nullptr;
        std::function<void(ax::Label*)> onLeftAction;
        std::function<void(ax::Label*)> onRightAction;

        // --- Dữ liệu cho Dropdown ---
        ax::ui::Layout* dropdownListBg = nullptr;
        std::vector<ax::Label*> dropdownItems;
        std::function<void(int)> onDropdownSelect;

        // --- Dữ liệu cho Toggle ---
        bool toggleState     = false;
        ax::Sprite* onFrame  = nullptr;
        ax::Sprite* offFrame = nullptr;
        std::function<void(bool)> onToggle;

        // --- Dữ liệu cho Button & Keybind ---
        std::function<void()> onAction;  // Hành động khi bấm Enter (hoặc kích hoạt đổi phím)
    };

    // =========================================================
    // 6. BIẾN QUẢN LÝ TRẠNG THÁI HIỆN TẠI CỦA UI
    // =========================================================
    int _currentFocusLevel = 0;  // Vị trí con trỏ: 0 = Đang ở Menu Trái | 1 = Đang ở Bảng nội dung Phải
    int _currentTabIndex   = 0;  // Tab đang mở (0: Display, 1: Audio, 2: Controls)
    int _currentRowIndex   = 0;  // Hàng đang được trỏ tới trong bảng nội dung

    bool _isDropdownOpen    = false;  // Dropdown có đang mở ra không?
    int _dropdownHoverIndex = 0;      // Mục đang được trỏ tới trong bảng Dropdown

    std::vector<SettingRow> _activeRows;  // Danh sách các hàng đang có trên màn hình hiện tại

    // =========================================================
    // 7. CON TRỞ UI VÀ LOGIC HỆ THỐNG
    // =========================================================
    bool _isFromGame = false;                // Cờ kiểm tra: Bật từ StartMenu hay bật từ lúc đang chơi GameScene?
    std::function<void()> _onCloseCallback;  // Hàm gọi ngược để báo cho Scene biết Settings đã tắt (để unpause)
    MenuInput* _menuInput = nullptr;         // Con trỏ quản lý input bàn phím

    ax::ui::Layout* _mainPanel      = nullptr;  // Nền to nhất
    ax::ui::Layout* _rightContainer = nullptr;  // Khung chứa nội dung bên phải
    std::vector<ax::ui::Button*> _tabButtons;   // Mảng chứa 3 nút (Display, Audio, Controls) bên trái
    ax::ui::Button* _btnQuit = nullptr;         // Nút Quit dưới cùng

    // =========================================================
    // 8. LƯU TRỮ DỮ LIỆU CÀI ĐẶT (SETTINGS DATA)
    // =========================================================
    // --- Dữ liệu Gốc (Đã lưu) ---
    int _resIndex      = 0;
    bool _isFullscreen = false;
    bool _isShowFPS    = true;
    int _masterVol     = 75;
    int _musicVol      = 75;
    int _sfxVol        = 75;

    // --- Dữ liệu Tạm (Chờ bấm Apply mới lưu) ---
    int _tempResIndex      = 0;
    bool _tempIsFullscreen = false;
    bool _tempShowFPS      = true;

    // =========================================================
    // 9. DỮ LIỆU DÀNH RIÊNG CHO TAB CONTROLS (ĐỔI PHÍM)
    // =========================================================
    bool _isBindingKey         = false;    // Cờ chặn mọi thao tác: Bật = True khi đang đợi người chơi gõ phím mới
    std::string _bindingAction = "";       // Tên Key lưu vào hệ thống (vd: "Key_Up")
    ax::Label* _bindingLabel   = nullptr;  // Con trỏ trỏ tới cái Label đang nhấp nháy chữ "[ Press Any Key ]"

    // Hàm tiện ích: Chuyển mã phím máy tính thành Chữ cái để in lên màn hình
    std::string getKeyName(ax::EventKeyboard::KeyCode code);
};
