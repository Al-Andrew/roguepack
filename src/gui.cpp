#include "roguepack/gui/gui.hpp"

void DrawPanel(Rectangle rect, Color color, i32 borderSize_px, Color borderColor) {
    DrawRectangleRec(rect, color);
    DrawRectangleLinesEx(rect, (f32)borderSize_px, borderColor);
}