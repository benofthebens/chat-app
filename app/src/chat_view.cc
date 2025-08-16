#include "chat_view.h"

#include <mutex>
#include <vector>
#include <string>
#include "message.h"

std::vector<Message> msgs = {};

constexpr int kLineHeight = 30;
constexpr int kMargin = 30;
constexpr int kYPadding = 4;
constexpr int kXPadding = 7;

static size_t scroll_pos = 0;

void AddMessage(HWND hwnd, Message* msg) {
    msgs.push_back(*msg);
    InvalidateRect(hwnd, nullptr, TRUE);
}


void ChatViewUpdateScrollInfo(const HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    const int lines_visible = (rect.bottom - rect.top) / kLineHeight;

    SCROLLINFO si = {};
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nMax = static_cast<int>(msgs.size());
    si.nPage = lines_visible;
    si.nPos = scroll_pos;

    SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
}

static void ChatViewPaint(const HWND hwnd) {
    PAINTSTRUCT ps;

    RECT client;
    GetClientRect(hwnd, &client);
    const LONG client_width = Width(client);

    const HDC hdc = BeginPaint(hwnd, &ps);

    FillRect(hdc, &client, nullptr);

    const size_t lines_visible = Height(client) / kLineHeight;

    for (size_t i = 0; i < lines_visible && (i + scroll_pos) < msgs.size(); ++i) {
        const Message msg = msgs[i + scroll_pos];
        const int y = i * kLineHeight;

        RECT server_msg_rect = { client.left, y, client.right,y + kLineHeight };

        switch (msg.sender) {
        case Sender::kUser: {
            RECT text_rect = { 0, y, client_width / 2, 0 };
            DrawText(hdc, msg.data, -1, &text_rect, DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL);

            const LONG text_width = Width(text_rect);
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
    EndPaint(hwnd, &ps);
}

static void ChatViewScroll(const HWND hwnd, const WPARAM w_param, const LPARAM l_param) {

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
    GetScrollInfo(hwnd, SB_VERT, &si);

    int position;
    switch (LOWORD(w_param)) {
    case SB_TOP:
        position = si.nMin; break;
    case SB_BOTTOM:
        position = si.nMax; break;
    case SB_LINEUP:
        position = si.nPos - 1; break;
    case SB_LINEDOWN:
        position = si.nPos + 1; break;
    case SB_THUMBTRACK:
        position = si.nTrackPos; break;
    default:
    case SB_THUMBPOSITION:
        position = si.nPos; break;
    }

    SetScrollPos(hwnd, SB_VERT, position, TRUE);
    position = GetScrollPos(hwnd, SB_VERT);

    scroll_pos = position;

    InvalidateRect(hwnd, nullptr, TRUE);
}

LRESULT CALLBACK ChatViewWinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
    case WM_VSCROLL: {
        ChatViewScroll(hwnd, w_param, l_param);
        return 0;
    }
    case WM_PAINT: {
        ChatViewPaint(hwnd);
        return 0;
    }
    default: 
        return DefWindowProc(hwnd, msg, w_param, l_param);
    }
}

void ChatViewRegister() {
    WNDCLASS wnd = {};
    wnd.lpfnWndProc = ChatViewWinProc;
    wnd.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wnd.lpszClassName = CHAT_VIEW_WC;

    RegisterClass(&wnd);
}


