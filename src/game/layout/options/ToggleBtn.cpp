#include "ToggleBtn.h"

#include "game/AppContext.h"
#include "system/GraphicsContext.h"
#include "system/Localize.h"
#include "system/Font.h"
#include "system/Paths.h"
#include "system/Texture.h"


namespace Layout {
namespace Options {

ToggleButton::ToggleButton(AppContext& app,
                           bool initial_state,
                           std::string&& text,
                           std::string&& description,
                           std::function<void(bool)>&& on_toggle)
    : OptionsItem(app, std::forward<std::string>(text), std::forward<std::string>(description))
    , switch_state(initial_state)
    , callback(on_toggle)
{
    bounding_box.h = 40;
    setWidth(750);

    padding_ver = (height() - tex_label->height()) / 2;

    auto font = app.gcx().loadFont(Paths::data() + "fonts/PTS55F.ttf", 24);
    tex_onoff.at(0) = font->renderText(tr("OFF"), 0xEEEEEE_rgb);
    tex_onoff.at(1) = font->renderText(tr("ON"), 0xEEEEEE_rgb);
}

void ToggleButton::setWidth(int w)
{
    OptionsItem::setWidth(w);
    rightswitch_centerx = x() + width() - padding_right - switch_half_width;
    leftswitch_centerx = rightswitch_centerx - switch_half_width * 2 - 10;
}

void ToggleButton::onLeftPress()
{
    if (switch_state) {
        switch_state = false;
        callback(switch_state);
    }
}

void ToggleButton::onRightPress()
{
    if (!switch_state) {
        switch_state = true;
        callback(switch_state);
    }
}

void ToggleButton::draw(GraphicsContext& gcx) const
{
    if (is_active) {
        gcx.drawFilledRect(bounding_box, 0x0060BF_rgb);
        gcx.drawFilledRect({x(), y(), 6, height()}, 0xCE8000_rgb);
        tex_label->drawAt(x() + padding_left, y() + padding_ver);
    }
    else {
        gcx.drawFilledRect(bounding_box, 0x002687_rgb);
        tex_label->drawAt(x() + padding_left, y() + padding_ver);
    }

    int switch_rect_centerx = switch_state ? rightswitch_centerx : leftswitch_centerx;
    gcx.drawFilledRect({
        switch_rect_centerx - switch_half_width, y() + 5,
        switch_half_width * 2, height() - 10}, 0xCE8000_rgb);
    tex_onoff.at(0)->drawAt(leftswitch_centerx - tex_onoff.at(0)->width() / 2, y() + padding_ver);
    tex_onoff.at(1)->drawAt(rightswitch_centerx - tex_onoff.at(1)->width() / 2, y() + padding_ver);
}

} // namespace Options
} // namespace Layout
