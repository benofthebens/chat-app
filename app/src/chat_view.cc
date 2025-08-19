#include "chat_view.h"


constexpr int kLineHeight = 30;
constexpr int kMargin = 30;
constexpr int kYPadding = 4;
constexpr int kXPadding = 7;

ChatView::ChatView() {

}

void ChatView::HandleScroll(WPARAM w_param, LPARAM l_param) {
    const INT scroll_type = LOWORD(w_param);

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
    GetScrollInfo(hwnd_, SB_VERT, &si);

    int position = 0;
    switch (scroll_type) {
    case SB_TOP:position = si.nMin; break;
    case SB_BOTTOM:position = si.nMax; break;
    case SB_LINEUP:position = --si.nPos; break;
    case SB_LINEDOWN:position = ++si.nPos; break;
    case SB_THUMBTRACK:position = si.nTrackPos; break;
    default:
    case SB_THUMBPOSITION:position = si.nPos; break;
    }

    SetScrollPos(hwnd_, SB_VERT, position, TRUE);
    scroll_position_ = position;
    InvalidateRect(hwnd_, nullptr, TRUE);

}

void ChatView::HandlePaint(WPARAM w_param, LPARAM l_param) {
    PAINTSTRUCT ps;

    RECT client;
    GetClientRect(hwnd_, &client);
    const INT client_width = Width();

    const HDC hdc = BeginPaint(hwnd_, &ps);

    FillRect(hdc, &client, nullptr);

    const size_t lines_visible = Height() / kLineHeight;

    for (size_t i = 0; i < lines_visible && (i + scroll_position_) < messages_.size(); ++i) {
        const Message msg = messages_[i + scroll_position_];
        const int y = i * kLineHeight;

        RECT server_msg_rect = { client.left, y, client.right,y + kLineHeight };

        switch (msg.sender) {
        case Sender::kUser: {
            RECT text_rect = { 0, y, client_width / 2, 0 };
            DrawText(hdc, msg.data, -1, &text_rect, DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL);

            const LONG text_width = ::Width(text_rect);
            text_rect.left = msg.is_sender ? client.right - text_width - kMargin : text_rect.left + kMargin;
            text_rect.right = msg.is_sender ? client.right - kMargin : text_rect.right + kMargin;

            const HGDIOBJ brush = msg.is_sender ? GetStockObject(DKGRAY_BRUSH) : GetStockObject(GRAY_BRUSH);

            SelectObject(hdc, brush);
            SelectObject(hdc, GetStockObject(NULL_PEN));

            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);

            RECT text_bubble = text_rect;
            InflateRect(&text_bubble, kXPadding, kYPadding);

            RECT tail_rect = text_bubble;
            tail_rect.top += kYPadding;
            if (msg.is_sender) {
                tail_rect.left += kXPadding;
            }
            else {
                tail_rect.right -= kXPadding;
            }

            FillRect(hdc, &tail_rect, static_cast<HBRUSH>(brush));

            RoundRect(hdc, text_bubble.left, text_bubble.top, text_bubble.right, text_bubble.bottom, 15, 15);
            DrawText(hdc, msg.data, -1, &text_rect, DT_WORDBREAK | DT_EDITCONTROL);

            break;
        }
        case Sender::kServer: {
            SetTextColor(hdc, RGB(0, 0, 0));
            DrawText(hdc, msg.data, -1, &server_msg_rect, DT_CENTER);
            break;
        }
        default:
            break;
        }
    }
    EndPaint(hwnd_, &ps);
}

void ChatView::HandleSize(WPARAM w_param, LPARAM l_param) {
     }


