#include "SinglePlayState.h"

#include "game/AppContext.h"
#include "game/WellEvent.h"
#include "game/CommonResources.h"
#include "game/components/GameplayResources.h"
#include "system/Localize.h"


SinglePlayState::SinglePlayState(AppContext& app)
    : paused(false)
    , tex_background(app.gcx->loadTexture("data/gamebg_pattern.png"))
    , next_pieces(4)
{
    using Fonts = CommonResources::Fonts;


    tex_hold = app.gcx->renderText(tr("HOLD"),
                                   Fonts::HEADER, 0xFFFFFF_rgb);
    tex_next = app.gcx->renderText(tr("NEXT"),
                                   Fonts::HEADER, 0xFFFFFF_rgb);


    board.registerObserver(WellEvent::NEXT_REQUESTED, [this](){
        this->addNextPiece();
    });

    board.registerObserver(WellEvent::HOLD_REQUESTED, [this](){
        this->piece_holder.onSwapRequested();
        if (this->piece_holder.swapAllowed()) {
            auto type = this->board.activePiece()->type();
            this->board.deletePiece();
            if (this->piece_holder.isEmpty()) {
                this->piece_holder.swapWithEmpty(type);
                this->addNextPiece();
            }
            else
                this->board.addPiece(this->piece_holder.swapWith(type));
        }
    });

    addNextPiece();
}

void SinglePlayState::addNextPiece()
{
    board.addPiece(next_pieces.next());
    piece_holder.onNextTurn();
}

void SinglePlayState::update(const std::vector<InputEvent>& inputs, AppContext& ctx)
{
    for (const auto& input : inputs) {
        if (input.type() == InputType::GAME_PAUSE && input.down()) {
            paused = !paused;
            return;
        }
    }

    if (paused)
        return;

    board.update(inputs, ctx);
    piece_holder.update();
}

void SinglePlayState::draw(GraphicsContext& gcx)
{
    static const int board_x = gcx.screenWidth() / 2 - 5 * Mino::texture_size_px;
    static const int board_y = 8;
    static const int board_w = 10 * Mino::texture_size_px;
    static const int board_h = 20 * Mino::texture_size_px;
    static const int sidebar_w = 6 * Mino::texture_size_px;
    static const int sidebar_left_x = board_x - sidebar_w;
    static const int sidebar_right_x = board_x + board_w;

    static const Rectangle rect_sidebar_left = {
        board_x - sidebar_w - 15, board_y, 15 + sidebar_w, board_h + 5};
    static const Rectangle rect_sidebar_right = {
        board_x + board_w, board_y, 15 + sidebar_w, board_h + 5};

    static const Rectangle rect_boardborder_left = {
        board_x - 5, board_y, 5, 20 * Mino::texture_size_px};
    static const Rectangle rect_boardborder_right = {
        board_x + board_w, board_y, 5, 20 * Mino::texture_size_px};
    static const Rectangle rect_boardborder_bottom = {
        board_x - 5, board_y + 20 * Mino::texture_size_px, 10 + board_w, 5};

    static const auto sidebar_color = 0x1010AAA0_rgba;
    static const auto boardborder_color = 0x808080_rgb;


    // draw screen background
    for (unsigned x = 0; x < gcx.screenWidth(); x += gcx.textureWidth(tex_background))
        gcx.drawTexture(tex_background, x, 0);

    // draw sidebar background
    gcx.drawFilledRect(rect_sidebar_left, sidebar_color);
    gcx.drawFilledRect(rect_sidebar_right, sidebar_color);

    // draw the well
    gcx.drawFilledRect(rect_boardborder_left, boardborder_color);
    gcx.drawFilledRect(rect_boardborder_right, boardborder_color);
    gcx.drawFilledRect(rect_boardborder_bottom, boardborder_color);
    board.draw(gcx, board_x, board_y);

    // draw sidebars
    gcx.drawTexture(tex_hold, sidebar_left_x, board_y + 16);
    piece_holder.draw(gcx, sidebar_left_x, board_y + 2 * Mino::texture_size_px);
    gcx.drawTexture(tex_next, sidebar_right_x + sidebar_w - gcx.textureWidth(tex_next), board_y + 16);
    next_pieces.draw(gcx, sidebar_right_x + Mino::texture_size_px, board_y + 2 * Mino::texture_size_px);
}
