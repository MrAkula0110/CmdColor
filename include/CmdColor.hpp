#pragma once

#include <cassert>
#include <iostream>
#include <string_view>
#include <string>
#include <array>
#include <thread>
#include <chrono>
#include <stop_token>
#include <cmath>
#include <vector>
#include <memory>
#include <span>

#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>
#else
    #include <cstdlib>
#endif

namespace cmd_color {

using namespace std::literals::chrono_literals;

// ============================================================================
// ЦВЕТОВЫЕ КОНСТАНТЫ
// ============================================================================

struct Color {
    uint8_t r, g, b;

    constexpr Color(uint8_t r_, uint8_t g_, uint8_t b_) : r(r_), g(g_), b(b_) {}

    std::string to_ansi_fg() const {
        return "\033[38;2;" + std::to_string(r) + ";" +
               std::to_string(g) + ";" + std::to_string(b) + "m";
    }

    std::string to_ansi_bg() const {
        return "\033[48;2;" + std::to_string(r) + ";" +
               std::to_string(g) + ";" + std::to_string(b) + "m";
    }
};

namespace Colors {
    constexpr Color RED{255, 0, 0};
    constexpr Color GREEN{0, 255, 0};
    constexpr Color BLUE{0, 0, 255};
    constexpr Color YELLOW{255, 255, 0};
    constexpr Color CYAN{0, 255, 255};
    constexpr Color MAGENTA{255, 0, 255};
    constexpr Color WHITE{255, 255, 255};
    constexpr Color BLACK{0, 0, 0};
    constexpr Color ORANGE{255, 165, 0};
    constexpr Color PURPLE{128, 0, 128};
}

// ============================================================================
// НАСТРОЙКА ТЕРМИНАЛА
// ============================================================================

class TerminalSetup {
public:
    TerminalSetup() {
#ifdef _WIN32
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            GetConsoleMode(hOut, &originalMode);
            DWORD dwMode = originalMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
        // UTF-8 для Windows
        SetConsoleOutputCP(CP_UTF8);
#else
        // Установка UTF-8 для Unix
        std::setlocale(LC_ALL, "en_US.UTF-8");
#endif
        std::cout << "\033[?25l" << std::flush; // Скрыть курсор
    }

    ~TerminalSetup() {
        std::cout << "\033[0m\033[?25h" << std::flush;
#ifdef _WIN32
        if (hOut != INVALID_HANDLE_VALUE) {
            SetConsoleMode(hOut, originalMode);
        }
#endif
    }

private:
#ifdef _WIN32
    HANDLE hOut = INVALID_HANDLE_VALUE;
    DWORD originalMode = 0;
#endif
};

// ============================================================================
// УТИЛИТЫ
// ============================================================================

inline Color get_rainbow_color(double phase) {
    uint8_t r = static_cast<uint8_t>(std::sin(phase) * 127 + 128);
    uint8_t g = static_cast<uint8_t>(std::sin(phase + 2.0 * M_PI / 3.0) * 127 + 128);
    uint8_t b = static_cast<uint8_t>(std::sin(phase + 4.0 * M_PI / 3.0) * 127 + 128);
    return Color{r, g, b};
}

inline Color lerp_color(const Color& from, const Color& to, double t) {
    t = std::clamp(t, 0.0, 1.0);
    return Color{
        static_cast<uint8_t>(from.r + (to.r - from.r) * t),
        static_cast<uint8_t>(from.g + (to.g - from.g) * t),
        static_cast<uint8_t>(from.b + (to.b - from.b) * t)
    };
}

// ============================================================================
// ОСНОВНЫЕ ФУНКЦИИ РАСКРАСКИ
// ============================================================================

/// Раскрасить весь текст одним цветом
inline std::string colorize(std::string_view text, const Color& color) {
    return color.to_ansi_fg() + std::string(text) + "\033[0m";
}

/// Раскрасить текст с фоном
inline std::string colorize_bg(std::string_view text, const Color& fg, const Color& bg) {
    return fg.to_ansi_fg() + bg.to_ansi_bg() + std::string(text) + "\033[0m";
}

/// Создать радужный текст (каждый символ свой цвет)
inline std::string rainbow(std::string_view text, double start_phase = 0.0) {
    std::string result;
    result.reserve(text.size() * 25);

    const double char_diff = 0.3;

    for (size_t i = 0; i < text.size(); ++i) {
        Color color = get_rainbow_color(start_phase + i * char_diff);
        result += color.to_ansi_fg();
        result += text[i];
    }

    result += "\033[0m";
    return result;
}

/// Создать градиент между двумя цветами
inline std::string gradient(std::string_view text, const Color& from, const Color& to) {
    if (text.empty()) return "";

    std::string result;
    result.reserve(text.size() * 25);

    for (size_t i = 0; i < text.size(); ++i) {
        double t = static_cast<double>(i) / (text.size() - 1);
        Color color = lerp_color(from, to, t);
        result += color.to_ansi_fg();
        result += text[i];
    }

    result += "\033[0m";
    return result;
}

// ============================================================================
// РАСКРАСКА КОНКРЕТНЫХ ЧАСТЕЙ ТЕКСТА
// ============================================================================

/// Раскрасить конкретные диапазоны текста разными цветами
/// Пример: colorize_ranges("Hello World", {{0, 5, RED}, {6, 11, BLUE}})
struct ColorRange {
    size_t start;
    size_t end;
    Color color;
};

inline std::string colorize_ranges(std::string_view text, std::span<const ColorRange> ranges) {
#ifndef NDEBUG
    for (const auto& range : ranges) {
        assert(range.start <= range.end && "ColorRange: start must be <= end");
        assert(range.end <= text.size() && "ColorRange: end out of bounds");
    }
#endif

    std::string result;
    result.reserve(text.size() * 2);

    size_t pos = 0;

    for (const auto& range : ranges) {
        // Добавить текст до диапазона
        if (pos < range.start) {
            result += text.substr(pos, range.start - pos);
        }

        // Добавить цветной диапазон
        result += range.color.to_ansi_fg();
        result += text.substr(range.start, range.end - range.start);
        result += "\033[0m";

        pos = range.end;
    }

    // Добавить оставшийся текст
    if (pos < text.size()) {
        result += text.substr(pos);
    }

    return result;
}

/// Раскрасить конкретные части текста радугой
inline std::string rainbow_range(std::string_view text, size_t start, size_t end) {
    if (start >= text.size() || end > text.size() || start >= end) {
        return std::string(text);
    }

    std::string result;
    result += text.substr(0, start);
    result += rainbow(text.substr(start, end - start));
    result += text.substr(end);

    return result;
}

// ============================================================================
// АНИМАЦИИ
// ============================================================================

/// Анимированная радуга
inline void animate_rainbow(std::stop_token st,
                            std::string_view text,
                            std::chrono::milliseconds interval = 30ms)
{
    TerminalSetup setup;

    double phase = 0.0;
    const double step = 0.15;

    while (!st.stop_requested()) {
        std::string output = "\033[H\033[2K" + rainbow(text, phase);
        std::cout << output << std::flush;

        std::this_thread::sleep_for(interval);
        phase += step;
    }
}

/// Анимированный градиент
inline void animate_gradient(std::stop_token st,
                             std::string_view text,
                             const Color& color1,
                             const Color& color2,
                             std::chrono::milliseconds interval = 50ms)
{
    TerminalSetup setup;

    double t = 0.0;
    double direction = 0.01;

    while (!st.stop_requested()) {
        Color from = lerp_color(color1, color2, t);
        Color to = lerp_color(color2, color1, t);

        std::string output = "\033[H\033[2K" + gradient(text, from, to);
        std::cout << output << std::flush;

        t += direction;
        if (t >= 1.0 || t <= 0.0) direction = -direction;

        std::this_thread::sleep_for(interval);
    }
}

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
// ============================================================================

inline void clear_screen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

inline void move_cursor(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H" << std::flush;
}

inline void hide_cursor() {
    std::cout << "\033[?25l" << std::flush;
}

inline void show_cursor() {
    std::cout << "\033[?25h" << std::flush;
}

inline void reset_color() {
    std::cout << "\033[0m" << std::flush;
}

} // namespace cmd_color