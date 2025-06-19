Qt MDI Window Manager

This project is a Qt-based MDI (Multiple Document Interface) application that includes a custom taskbar for minimized subwindows, offering enhanced window organization and control within a single main window. It is built using Qt Widgets and C++.
✨ Features

    📁 MDI Interface with resizable and movable subwindows

    🧩 Four pre-positioned subwindows arranged in a 2x2 layout

    🧭 Toolbar & Menu actions to:

        Cascade windows

        Tile windows

        Minimize all subwindows

        Reset layout

        List open windows

    🧰 Custom taskbar at the bottom to show buttons for minimized windows

    🖱️ Right-click context menu in each subwindow for minimization

    ♻️ Resizes and resets layout on window resize


🚀 Getting Started
Prerequisites

    Qt 5 or 6 (Tested with Qt 5.15+)

    Qt Creator or CMake (for building)

Build Instructions

Using Qt Creator:

    Open the .pro or CMakeLists.txt file.

    Click on "Run" or build the project.

Using CMake:

mkdir build
cd build
cmake ..
make
./YourAppExecutable

🧩 Code Structure

Main functionality is implemented in MainWindow:

    createPositionedSubWindows(): Adds four labeled subwindows.

    minimizeSubWindow(): Hides the subwindow and adds a taskbar button.

    restoreSubWindow(): Restores a minimized window from the taskbar.

    resetLayout(): Re-arranges windows in a 2x2 layout.

    setupMenuBar() and setupToolBar(): Add UI actions.

    resizeEvent(): Automatically adjusts layout on resizing.

📁 File Overview
File	Description
mainwindow.h	Header file for the main window class
mainwindow.cpp	Implementation of the main window logic
main.cpp	Entry point of the application (not shown)
📜 License

This project is licensed under the MIT License - see the LICENSE file for details.
🙌 Acknowledgments

    Built with Qt

    Inspired by common desktop environments for window management
