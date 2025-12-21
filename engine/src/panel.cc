#include "engine/panel.h"

#include "engine/widgets/button.h"

using namespace Engine;

uint32_t Panel::cid_ = 101;

void Panel::Register(WNDCLASSEX& wc) {
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
}

void Panel::OnInit(const PanelProps& props) {
    width_ = props.width;
    height_ = props.height;
    x_ = props.x;
    y_ = props.y;
    parent_ = props.parent;
    id_ = cid_;
    label_ = props.label.c_str();

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);

    if (!GetClassInfoEx(GetModuleHandle(nullptr), props.class_name.c_str(),&wc)) {
        Register(wc);
        wc.lpszClassName = props.class_name.c_str();
        RegisterClassEx(&wc);
    }
  
    hwnd_ = CreateWindowEx(
        props.ex_style,
        props.class_name.c_str(),
        props.label.c_str(),
        props.style,
        x_, y_,
        width_, height_,
        parent_,
        (HMENU)(intptr_t)id_,
        GetModuleHandle(nullptr),
        this
    );

    ++cid_;
}

LRESULT Panel::WndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    Panel* instance = nullptr;

    if (msg == WM_NCCREATE) {
        const auto* create_struct = reinterpret_cast<LPCREATESTRUCT>(l_param);
        instance = static_cast<Panel*>(create_struct->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
        instance->hwnd_ = hwnd;
    }
    else {
        instance = reinterpret_cast<Panel*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    if (instance) {
        return instance->HandleMessage(msg, w_param, l_param);
    }

    return DefWindowProc(hwnd, msg, w_param, l_param);
}

LRESULT Panel::HandleMessage(UINT msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd_, &ps);

        GraphicsContext ctx(hdc);
        OnPaint(ctx);

        EndPaint(hwnd_, &ps);
        return 0;
    }
    case WM_SIZE: {
        OnResize(LOWORD(l_param), HIWORD(l_param));
        return 0;
    }
    case WM_COMMAND: {
        if (HIWORD(w_param) == BN_CLICKED) {
            int id = LOWORD(w_param);
            for (auto& child : children_) {
                if (auto* btn = dynamic_cast<Button*>(child.get())) {
                    if (btn->GetId() == id && btn->on_click_) {
                        btn->on_click_();
                    }
                }
            }
        }
        return 0;
    }
    case WM_VSCROLL: {
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
        scroll_pos_ = si.nPos;

        SetScrollPos(hwnd_, SB_VERT, position, TRUE);
        InvalidateRect(hwnd_, nullptr, TRUE);
        return 0;
    }
    default: break;
    }
    return DefWindowProc(hwnd_, msg, w_param, l_param);
}
