# 🎨 CmdColor

A lightweight, header-only C++20 library for colored terminal output with animations, gradients, and rainbow effects.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey.svg)](https://github.com/MrAkula0110/CmdColor)

**English** | [Русский](README_RU.md)

---

## 📋 Table of Contents

- [Features](#-features)
- [Requirements](#-requirements)
- [Installation](#-installation)
- [Quick Start](#-quick-start)
- [API Documentation](#-api-documentation)
  - [Basic Colorization](#basic-colorization)
  - [Rainbow Text](#rainbow-text)
  - [Gradients](#gradients)
  - [Color Ranges](#color-ranges)
  - [Animations](#animations)
  - [Utility Functions](#utility-functions)
- [Usage Examples](#-usage-examples)
- [Platform Support](#️-platform-support)
- [License](#-license)
- [Support the Project](#-support-the-project)
- [Author](#-author)

---

## ✨ Features

- 🎨 **True Color support** — 16.7 million colors (RGB)
- 🌈 **Rainbow effects** — automatic text coloring in rainbow colors
- 🎭 **Gradients** — smooth transitions between two colors
- 🎬 **Animations** — live rainbow and gradient effects
- 🎯 **Precise colorization** — highlight specific text ranges
- 📦 **Header-only** — just copy one file!
- ⚡ **Zero dependencies** — only C++20 standard library
- 🖥️ **Cross-platform** — Windows and Linux/macOS
- 🔧 **UTF-8 support** — correct work with Unicode and emojis

---

## 🔧 Requirements

- **Compiler:** GCC 10+, Clang 11+, MSVC 2019+ (with C++20 support)
- **Standard:** C++20 or higher
- **Platform:** Windows 10+, Linux, macOS

### Check C++20 support:

```bash
# GCC
g++ --version  # Should be version 10 or higher

# Clang
clang++ --version  # Should be version 11 or higher

# MSVC
cl  # Visual Studio 2019 or newer
```

---

## 📦 Installation

### Method 1: Copy the file (recommended)

Simply copy `CmdColor.hpp` into your project:

```bash
# Download the file
curl -O https://raw.githubusercontent.com/MrAkula0110/CmdColor/main/include/CmdColor.hpp

# Or clone the repository
git clone https://github.com/MrAkula0110/CmdColor.git
```

### Method 2: CMake submodule

```bash
# Add as git submodule
git submodule add https://github.com/MrAkula0110/CmdColor.git external/CmdColor
```

In your `CMakeLists.txt`:

```cmake
include_directories(external/CmdColor/include)
```

### Method 3: Direct inclusion

Just place `CmdColor.hpp` next to your source and include it:

```cpp
#include "CmdColor.hpp"
```

---

## 🚀 Quick Start

### Minimal Example

```cpp
#include "CmdColor.hpp"
#include <iostream>

int main() {
    using namespace cmd_color;
    
    // Terminal setup (automatic cleanup on exit)
    TerminalSetup setup;
    
    // Simple colored text
    std::cout << colorize("Red text!", Colors::RED) << "\n";
    
    // Rainbow
    std::cout << rainbow("Rainbow text!") << "\n";
    
    // Gradient
    std::cout << gradient("Gradient from blue to green", 
                          Colors::BLUE, Colors::GREEN) << "\n";
    
    return 0;
}
```

### Compilation

```bash
# GCC/Clang
g++ -std=c++20 main.cpp -o myapp

# MSVC
cl /std:c++20 main.cpp

# CMake
cmake -B build -DCMAKE_CXX_STANDARD=20
cmake --build build
```

---

## 📚 API Documentation

### Basic Colorization

#### `colorize(text, color)`
Colors entire text with one color.

```cpp
std::cout << colorize("Green text", Colors::GREEN) << "\n";
std::cout << colorize("Blue text", Colors::BLUE) << "\n";
```

#### `colorize_bg(text, fg_color, bg_color)`
Colors text with background color.

```cpp
std::cout << colorize_bg("Black on white", 
                         Colors::BLACK, 
                         Colors::WHITE) << "\n";
```

#### Available Colors

The library provides predefined colors in the `Colors::` namespace:

```cpp
Colors::RED        // Red
Colors::GREEN      // Green
Colors::BLUE       // Blue
Colors::YELLOW     // Yellow
Colors::CYAN       // Cyan
Colors::MAGENTA    // Magenta
Colors::WHITE      // White
Colors::BLACK      // Black
Colors::ORANGE     // Orange
Colors::PURPLE     // Purple
```

#### Custom Colors

Create your own color using the `Color` structure:

```cpp
Color custom_color{128, 0, 255};  // RGB: purple
std::cout << colorize("My color", custom_color) << "\n";
```

---

### Rainbow Text

#### `rainbow(text, start_phase = 0.0)`
Colors each character in rainbow colors.

```cpp
std::cout << rainbow("This is rainbow text!") << "\n";

// With custom starting phase
std::cout << rainbow("Shifted rainbow", 1.5) << "\n";
```

#### `rainbow_range(text, start, end)`
Colors only the specified character range in rainbow.

```cpp
// "Hello" will be rainbow, rest normal
std::cout << rainbow_range("Hello World!", 0, 5) << "\n";
```

---

### Gradients

#### `gradient(text, from_color, to_color)`
Creates a smooth transition between two colors.

```cpp
// From red to blue
std::cout << gradient("Beautiful gradient", 
                      Colors::RED, 
                      Colors::BLUE) << "\n";

// From yellow to magenta
std::cout << gradient("Sunset gradient", 
                      Colors::YELLOW, 
                      Colors::MAGENTA) << "\n";

// Custom colors
Color dark_blue{0, 0, 139};
Color light_blue{173, 216, 230};
std::cout << gradient("Ocean gradient", dark_blue, light_blue) << "\n";
```

---

### Color Ranges

#### `colorize_ranges(text, ranges)`
Colors specific parts of text with different colors.

```cpp
ColorRange ranges[] = {
    {0, 5, Colors::RED},      // Characters 0-4: red
    {6, 11, Colors::GREEN},   // Characters 6-10: green
    {12, 17, Colors::BLUE}    // Characters 12-16: blue
};

std::cout << colorize_ranges("Hello World !!!", ranges) << "\n";
```

**Result:** `Hello` (red) `World` (green) `!!!` (blue)

---

### Animations

⚠️ **Important:** Animations run in a separate thread and block the screen. Start them at the end of your program or in a separate thread.

#### `animate_rainbow(stop_token, text, interval)`
Animated rainbow, colors smoothly change.

```cpp
#include <thread>

// Run animation for 5 seconds
std::jthread thread(animate_rainbow, "Animation!", 30ms);
std::this_thread::sleep_for(std::chrono::seconds(5));
thread.request_stop();  // Stop animation
```

**Parameters:**
- `stop_token` — token for stopping (automatic with `std::jthread`)
- `text` — text for animation
- `interval` — update interval (default 30ms)

#### `animate_gradient(stop_token, text, color1, color2, interval)`
Animated gradient between two colors.

```cpp
std::jthread thread(animate_gradient, 
                    "Gradient!", 
                    Colors::RED, 
                    Colors::BLUE, 
                    50ms);
                    
std::this_thread::sleep_for(std::chrono::seconds(5));
thread.request_stop();
```

---

### Utility Functions

#### `clear_screen()`
Clears the terminal screen.

```cpp
clear_screen();
```

#### `move_cursor(x, y)`
Moves cursor to specified position.

```cpp
move_cursor(10, 5);  // Column 10, row 5
std::cout << "Text at position (10, 5)";
```

#### `hide_cursor()` / `show_cursor()`
Hides or shows the cursor.

```cpp
hide_cursor();
// Your code...
show_cursor();
```

#### `reset_color()`
Resets all colors to default.

```cpp
std::cout << colorize("Colored", Colors::RED);
reset_color();
std::cout << " Normal text";
```

---

## 💡 Usage Examples

### Example 1: Colored Logging

```cpp
#include "CmdColor.hpp"
#include <iostream>

void log_info(const std::string& msg) {
    std::cout << colorize("[INFO] ", Colors::CYAN) << msg << "\n";
}

void log_warning(const std::string& msg) {
    std::cout << colorize("[WARN] ", Colors::YELLOW) << msg << "\n";
}

void log_error(const std::string& msg) {
    std::cout << colorize("[ERROR] ", Colors::RED) << msg << "\n";
}

int main() {
    using namespace cmd_color;
    TerminalSetup setup;
    
    log_info("Program started");
    log_warning("Low memory");
    log_error("Failed to open file");
    
    return 0;
}
```

### Example 2: Colored Menu

```cpp
#include "CmdColor.hpp"
#include <iostream>

void show_menu() {
    using namespace cmd_color;
    
    clear_screen();
    
    std::cout << rainbow("=== MAIN MENU ===") << "\n\n";
    
    std::cout << colorize("1.", Colors::CYAN) << " Start game\n";
    std::cout << colorize("2.", Colors::GREEN) << " Settings\n";
    std::cout << colorize("3.", Colors::YELLOW) << " About\n";
    std::cout << colorize("4.", Colors::RED) << " Exit\n\n";
    
    std::cout << gradient("Choose option: ", Colors::BLUE, Colors::PURPLE);
}

int main() {
    using namespace cmd_color;
    TerminalSetup setup;
    
    show_menu();
    
    int choice;
    std::cin >> choice;
    
    return 0;
}
```

### Example 3: Progress Bar

```cpp
#include "CmdColor.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void show_progress(int percent) {
    using namespace cmd_color;
    
    int bar_width = 50;
    int filled = (bar_width * percent) / 100;
    
    std::cout << "\r[";
    
    // Filled part (green)
    for (int i = 0; i < filled; ++i) {
        std::cout << colorize("█", Colors::GREEN);
    }
    
    // Empty part (gray)
    Color gray{100, 100, 100};
    for (int i = filled; i < bar_width; ++i) {
        std::cout << colorize("░", gray);
    }
    
    std::cout << "] " << percent << "%";
    std::cout << std::flush;
}

int main() {
    using namespace cmd_color;
    TerminalSetup setup;
    
    for (int i = 0; i <= 100; ++i) {
        show_progress(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    std::cout << "\n" << colorize("Done!", Colors::GREEN) << "\n";
    
    return 0;
}
```

### Example 4: ASCII Art with Colors

```cpp
#include "CmdColor.hpp"
#include <iostream>

int main() {
    using namespace cmd_color;
    TerminalSetup setup;
    
    std::cout << gradient("    ____            __  ", Colors::CYAN, Colors::BLUE) << "\n";
    std::cout << gradient("   / __ \\__  ____  / /_ ", Colors::CYAN, Colors::BLUE) << "\n";
    std::cout << gradient("  / /_/ / / / / / / / / ", Colors::CYAN, Colors::BLUE) << "\n";
    std::cout << gradient(" / _, _/ /_/ / /_/ / /  ", Colors::BLUE, Colors::PURPLE) << "\n";
    std::cout << gradient("/_/ |_|\\__,_/\\__,_/_/   ", Colors::BLUE, Colors::PURPLE) << "\n\n";
    
    std::cout << rainbow("Welcome to CmdColor!") << "\n";
    
    return 0;
}
```

---

## 🖥️ Platform Support

### Windows

- ✅ Windows 10 and above (with ANSI support enabled)
- ✅ Windows Terminal (recommended)
- ✅ PowerShell
- ✅ cmd.exe (with limitations)

**Note:** The library automatically enables ANSI support through `ENABLE_VIRTUAL_TERMINAL_PROCESSING`.

### Linux / macOS

- ✅ All modern terminals
- ✅ GNOME Terminal
- ✅ Konsole
- ✅ iTerm2
- ✅ macOS Terminal

---

## 📄 License

This project is licensed under the MIT License. See [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2025 MrAkula

Permission is hereby granted to use, copy, modify, and distribute this software
with preservation of copyright notice.
```

---

## 💖 Support the Project

If you like the CmdColor library, you can support its development:

**[☕ Support on DonationAlerts](https://www.donationalerts.com/r/mrakula228)**

Your support helps develop the project and add new features! 🙏

---

## 👤 Author

**MrAkula** (Daniel)

- GitHub: [@MrAkula0110](https://github.com/MrAkula0110)
- DonationAlerts: [mrakula228](https://www.donationalerts.com/r/mrakula228)

---

## 🤝 Contributing

Contributions are welcome!

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📝 Changelog

### v1.0.0 (2025-01-01)
- ✨ Initial release
- 🎨 True Color support
- 🌈 Rainbow effects
- 🎭 Gradients
- 🎬 Animations
- 🖥️ Windows and Linux support

---

## ⭐ Star the Project!

If you find this project useful, give it a ⭐ on GitHub — it's very motivating!

**Thank you for using CmdColor!** 🎉