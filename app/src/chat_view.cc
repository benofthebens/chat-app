#include "chat_view.h"

constexpr int kLineHeight = 30;
constexpr int kMargin = 30;
constexpr int kYPadding = 4;
constexpr int kXPadding = 7;
const auto kIncomingMsgBrush = static_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));
const auto kOutgoingMsgBrush = static_cast<HBRUSH>(GetStockObject(DKGRAY_BRUSH));

void ChatView::AddMessage(Message* message) {
    messages_.push_back(*message);
    InvalidateRect(hwnd_, nullptr, TRUE);
}

void ChatView::HandleScroll(WPARAM w_param, LPARAM l_param) {
    const INT scroll_type = LOWORD(w_param);

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
    GetScrollInfo(hwnd_, SB_VERT, &si);

    int position = 0;
    switch (scroll_type) {
    case SB_TOP:        position = si.nMin;         break;
    case SB_BOTTOM:     position = si.nMax;         break;
    case SB_LINEUP:     position = --si.nPos;       break;
    case SB_LINEDOWN:   position = ++si.nPos;       break;
    case SB_THUMBTRACK: position = si.nTrackPos;    break;
    default:
    case SB_THUMBPOSITION:position = si.nPos;       break;
    }

    SetScrollPos(hwnd_, SB_VERT, position, TRUE);
    InvalidateRect(hwnd_, nullptr, TRUE);
}

void ChatView::HandlePaint(WPARAM w_param, LPARAM l_param) {
    // Get Scroll Info
    SCROLLINFO scroll_info;
    scroll_info.cbSize = sizeof(SCROLLINFO);
    scroll_info.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
    GetScrollInfo(hwnd_, SB_VERT, &scroll_info);

    PAINTSTRUCT ps;
    RECT client;

    GetClientRect(hwnd_, &client);

    const INT client_height = ClientHeight();

    const HDC hdc = BeginPaint(hwnd_, &ps);

    FillRect(hdc, &client, nullptr);

    const size_t lines_visible = client_height / kLineHeight;

    int y = 0;
    for (size_t i = 0; i < lines_visible && (i + scroll_info.nPos) < messages_.size(); ++i) {
        const Message msg = messages_[i + scroll_info.nPos];
        RECT msg_rect = { client.left, y, client.right, 0 };

        switch (msg.sender) {
        case Sender::kUser: {
            msg_rect.right /= 2;
            DrawText(hdc, msg.data, -1, &msg_rect, DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL);

            const LONG text_width = ::Width(msg_rect);
            msg_rect.left = msg.is_sender ? client.right - text_width - kMargin : msg_rect.left + kMargin;
            msg_rect.right = msg.is_sender ? client.right - kMargin : msg_rect.right + kMargin;

            const HGDIOBJ brush = msg.is_sender ? kOutgoingMsgBrush : kIncomingMsgBrush;

            SelectObject(hdc, brush);
            SelectObject(hdc, GetStockObject(NULL_PEN));

            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);

            RECT bubble = msg_rect;
            InflateRect(&bubble, kXPadding, kYPadding);

            RECT tail = bubble;
            tail.top += kYPadding;
            if (msg.is_sender) {
                tail.left += kXPadding;
            }
            else {
                tail.right -= kXPadding;
            }

            FillRect(hdc, &tail, static_cast<HBRUSH>(brush));

            RoundRect(hdc, bubble.left, bubble.top, bubble.right, bubble.bottom, 15, 15);
            DrawText(hdc, msg.data, -1, &msg_rect, DT_WORDBREAK | DT_EDITCONTROL);
            y += ::Height(bubble) + 7;

            break;
        }
        case Sender::kServer: {
            msg_rect.bottom += kLineHeight + y;
            SetTextColor(hdc, RGB(0, 0, 0));
            DrawText(hdc, msg.data, -1, &msg_rect, DT_CENTER);
            y += ::Height(msg_rect) + 7;
            break;
        }
        default:
            break;
        }
    }
    EndPaint(hwnd_, &ps);
}

void ChatView::HandleSize(WPARAM w_param, LPARAM l_param) { InvalidateRect(hwnd_, nullptr, TRUE); }
