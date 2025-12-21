#include "app/chat_panel.h"

void ChatPanel::AddMessage(const std::string& msg) {
    messages_.push_back(msg);
    InvalidateRect(GetHwnd(), nullptr, TRUE);
}

void ChatPanel::OnPaint(Engine::GraphicsContext& ctx) {
    int y = 10;
    RECT client;
    GetClientRect(GetHwnd(), &client);
    const size_t lines_visible = (client.bottom - client.top);

    for (size_t i = 0; i < lines_visible && (i + GetScrollPos()) < messages_.size(); ++i) {
        const auto& msg = messages_[i + GetScrollPos()];

        RECT msg_rect = { client.left + 20, y, client.right / 2, y };
        ctx.Text(msg, &msg_rect, DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL);

        RECT bubble_rect = msg_rect;
        InflateRect(&bubble_rect, 10, 6);
        ctx.DrawRoundRect(&bubble_rect, 15, Engine::Colour::Green());

        RECT tail = { bubble_rect.left, bubble_rect.top + 10, bubble_rect.right - 10, bubble_rect.bottom };
        ctx.DrawRect(&tail, Engine::Colour::Green());

        ctx.Text(msg, &msg_rect, DT_WORDBREAK | DT_EDITCONTROL);
        y += bubble_rect.bottom - bubble_rect.top + 5;
    }
}
