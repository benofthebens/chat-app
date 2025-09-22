#include "chat_view.h"
#include "chat_app.h"
#include <mutex>

constexpr int kLineHeight = 30;
constexpr int kMargin = 30;
constexpr int kYPadding = 4;
constexpr int kXPadding = 7;
const auto kIncomingMsgBrush = static_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));
const auto kOutgoingMsgBrush = static_cast<HBRUSH>(GetStockObject(DKGRAY_BRUSH));

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

    const INT client_height = ::Height(client);

    const HDC hdc = BeginPaint(hwnd_, &ps);

    FillRect(hdc, &client, nullptr);

    const size_t lines_visible = client_height / kLineHeight;

}

void ChatView::HandleSize(WPARAM w_param, LPARAM l_param) { InvalidateRect(hwnd_, nullptr, TRUE); }
