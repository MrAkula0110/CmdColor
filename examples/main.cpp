#include "../include/CmdColor.hpp"
#include <windows.h>
using namespace cmd_color;

void demo_colors() {
    SetConsoleOutputCP(CP_UTF8);
    TerminalSetup setup;

    std::cout << "\n\n=== CmdColor Library Demo ===\n\n";

    // 1. Базовые цвета
    std::cout << "1. Basic colors:\n";
    std::cout << "   " << colorize("RED", Colors::RED) << " ";
    std::cout << colorize("GREEN", Colors::GREEN) << " ";
    std::cout << colorize("BLUE", Colors::BLUE) << " ";
    std::cout << colorize("YELLOW", Colors::YELLOW) << "\n\n";

    // 2. Радуга (статичная)
    std::cout << "2. Rainbow text:\n";
    std::cout << "   " << rainbow("This is rainbow text!") << "\n\n";

    // 3. Градиент
    std::cout << "3. Gradient:\n";
    std::cout << "   " << gradient("Blue to Green gradient", Colors::BLUE, Colors::GREEN) << "\n\n";

    // 4. Фон
    std::cout << "4. Background color:\n";
    std::cout << "   " << colorize_bg("Black on White", Colors::BLACK, Colors::WHITE) << "\n\n";

    // 5. Цветные диапазоны
    std::cout << "5. Color ranges:\n";
    ColorRange ranges[] = {
        {0, 5, Colors::RED},
        {6, 11, Colors::GREEN},
        {12, 17, Colors::BLUE}
    };
    std::cout << "   " << colorize_ranges("Hello World !!!", ranges) << "\n\n";

    // 6. Радужный диапазон
    std::cout << "6. Rainbow range:\n";
    std::cout << "   " << rainbow_range("Normal RAINBOW normal", 7, 14) << "\n\n";

    std::cout << "\nPress Enter to see animations...\n";
    std::cin.get();
}

void demo_animations() {
    std::cout << "\n\n--- ANIMATIONS ---\n";
    std::cout << "Starting rainbow animation (5 seconds)...\n";
    std::cout << "Press Ctrl+C to exit early\n\n";

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Запускаем анимацию на 5 секунд
    std::jthread rainbow_thread(animate_rainbow, "Rainbow Animation!", 30ms);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    rainbow_thread.request_stop();
    rainbow_thread.join();

    std::cout << "\n\n--- GRADIENT ANIMATION ---\n";
    std::cout << "Starting gradient animation (5 seconds)...\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::jthread gradient_thread(animate_gradient, "Gradient Animation!",
                                 Colors::RED, Colors::BLUE, 50ms);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    gradient_thread.request_stop();
    gradient_thread.join();

    std::cout << "\n\nDemo complete!\n";
}

int main() {
    try {
        // Сначала показываем статичные примеры
        demo_colors();

        // Потом анимации отдельно
        demo_animations();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}