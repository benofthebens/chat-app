#include "app/chat_panel.h"

#include "app/chat_app.h"
#include "app/user.h"

void ChatPanel::AddMessage(const Message& msg) {
    messages_.push_back(msg);
    InvalidateRect(GetHwnd(), nullptr, TRUE);
}

void ChatPanel::OnPaint(Engine::GraphicsContext& ctx) {
    int y = 10;
    RECT client;
    GetClientRect(GetHwnd(), &client);
    const size_t lines_visible = (client.bottom - client.top);
    const auto& u = User::Get();

    FontProps message_font_props = FontBuilder()
        .Height(18)
        .Family(FF_ROMAN)
        .Name("Times New Roman")
        .Build();
    FontProps system_font_props = FontBuilder()
        .Height(18)
        .Weight(FW_BOLD)
        .Family(FF_ROMAN)
        .Name("Times New Roman")
        .Build();
    FontProps label_font_props = FontBuilder()
        .Height(15)
        .Weight(FW_THIN)
        .Family(FF_ROMAN)
        .Italic()
        .Name("Times New Roman")
        .Build();

    Font msg_font(message_font_props);
    Font system_font(system_font_props);
    Font label_font(label_font_props);
   
    for (size_t i = 0; i < lines_visible && (i + GetScrollPos()) < messages_.size(); ++i) {
        const auto& msg = messages_[i + GetScrollPos()];
        bool is_user = u.id == msg.user.id;

        RECT msg_rect = { client.left + 20, y, client.right / 2, y };

        ctx.SetFont(msg_font);
        ctx.Text(msg.data, &msg_rect, DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL);

        const int height = msg_rect.bottom - msg_rect.top;
        const int width = msg_rect.right - msg_rect.left;
        if (msg.user.id == -1) {
            RECT join_rect = { client.left, y, client.right, y + height };
            ctx.SetFont(system_font);
            ctx.Text(msg.data, &join_rect, DT_CENTER, Colour::Black());
            y += height + 20;
            continue;
        }

        if (is_user) {
            msg_rect.left = client.right - width - 20;
            msg_rect.right = client.right - 20;
        }

        Colour colour = is_user ? Colour(0x42adfc) : Colour(0xe7e7e9);

        RECT bubble_rect = msg_rect;
        InflateRect(&bubble_rect, 10, 6);
        int bubble_height = bubble_rect.bottom - bubble_rect.top;
        ctx.DrawRoundRect(&bubble_rect, 15, colour);

        RECT tail = { bubble_rect.left, bubble_rect.top + 10, bubble_rect.right - 10, bubble_rect.bottom };
        if (is_user) {
            tail.left += 10;
            tail.right += 10;
        }
        ctx.DrawRect(&tail, colour);
        ctx.Text(msg.data, &msg_rect, DT_WORDBREAK | DT_EDITCONTROL, is_user ? Colour::White() : Colour::Black());

        ctx.SetFont(label_font);
        RECT label = { bubble_rect.left, bubble_rect.bottom, bubble_rect.right, bubble_rect.bottom };
        
        ctx.Text(msg.user.name, &label, DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL);

        int label_width = label.right - label.left;
        int label_height = label.bottom - label.top;

        if (is_user) {
            label.left = (client.right - 10) - label_width;
            label.right = (client.right - 10);
        }
        ctx.Text(msg.user.name, &label);

        y += bubble_height + label_height + 5;
    }
    ctx.ResetFont();
}
