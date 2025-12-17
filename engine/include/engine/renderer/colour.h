
#ifndef ENGINE_COLOUR_H
#define ENGINE_COLOUR_H

#include <cstdint>
#include <Windows.h>

namespace Engine {

    struct Colour {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 255;

        // Default constructor
        constexpr Colour() = default;

        // RGBA constructor
        constexpr Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
            : r(r), g(g), b(b), a(a) {
        }

        // Hex constructor: Color(0xFF5733)
        constexpr explicit Colour(uint32_t hex)
            : r((hex >> 16) & 0xFF)
            , g((hex >> 8) & 0xFF)
            , b(hex & 0xFF)
            , a(255) {
        }

        // Convert to Windows COLORREF (what GDI uses)
        COLORREF ToColourRef() const {
            return RGB(r, g, b);
        }

        static constexpr Colour White() { return Colour(255, 255, 255); }
        static constexpr Colour Black() { return Colour(0, 0, 0); }
        static constexpr Colour Red() { return Colour(255, 0, 0); }
        static constexpr Colour Green() { return Colour(0, 255, 0); }
        static constexpr Colour Blue() { return Colour(0, 0, 255); }
        static constexpr Colour Yellow() { return Colour(255, 255, 0); }
        static constexpr Colour Transparent() { return Colour(0, 0, 0, 0); }
    };

} // namespace Engine

#endif