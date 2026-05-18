#include "scene/settings/SettingsLayer.h"

USING_NS_AX;
using namespace ax::ui;
void SettingsLayer::buildControlsTab()
{
    createSpinBoxRow(400, "MOVE UP", "W", [](Label* l) {}, [](Label* l) {});
    createSpinBoxRow(330, "ATTACK", "SPACE", [](Label* l) {}, [](Label* l) {});
}
