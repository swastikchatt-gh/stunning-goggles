# MyGedit

MyGedit is a lightweight Qt6-based text editor inspired by GNOME Gedit.

It focuses on simplicity, responsiveness, and zero unnecessary overhead.
No Electron. No telemetry. Just Qt.

---

## ✨ Features

- New / Open / Save / Save As
- Cut / Copy / Paste
- Unsaved document protection (prompt on close)
- Line & Column indicator
- Toolbar with themed icons
- Native file dialogs
- Clean Qt6 Widgets UI

---

## 🖥️ Built With

- C++17
- Qt6 (Widgets)
- QPlainTextEdit
- QAction / QMenuBar / QToolBar / QStatusBar
- CMake

---

## 📦 Requirements

- Qt6 (Widgets module)
- CMake 3.16+
- C++17 compatible compiler (GCC / Clang)

---

## 🔧 Build Instructions (CMake Only)

```bash
git clone <your-repo-url>
cd MyGedit

mkdir build
cd build

cmake ..
make
./MyGedit
```

---

## 📁 Project Structure

```
MyGedit/
 ├── main.cpp
 ├── CMakeLists.txt
 └── resources.qrc   (if using embedded icons)
```

---

## 🚀 Planned Improvements

- Syntax highlighting
- Find / Replace
- Multiple document tabs
- Dark mode toggle
- Font & theme customization
- Auto-save
- Large file optimization

---

## 📜 License

This project is licensed under **SLIC**.

See the LICENSE file for details.

---

## 👨‍💻 Author

Swastik Chatterjee

---

A simple text editor.  
Built because we can.
