# cpp-gui-postgres

A C++ desktop application with a Qt6 GUI that connects to a PostgreSQL database using `libpqxx`.

## Project Structure

```
cpp-gui-postgres/
├── src/
│   └── main.cpp               # Entry point
├── gui/
│   ├── mainwindow.h
│   └── mainwindow.cpp         # Qt6 main window
├── database/
│   ├── db.h
│   └── db.cpp                 # PostgreSQL connection (libpqxx)
├── models/
│   └── user.h
├── repositories/
│   ├── userrepository.h
│   └── userrepository.cpp
├── services/
│   ├── userservice.h
│   └── userservice.cpp
├── config/
│   ├── env.h
│   └── env.cpp                # .env file loader
├── .env.example               # Environment variable template
├── CMakeLists.txt
└── .vscode/
    ├── c_cpp_properties.json
    └── settings.json
```

---

## Setup

### Configure Environment Variables

Copy the example env file and fill in your PostgreSQL credentials:

```bash
cp .env.example .env        # Linux / macOS
copy .env.example .env      # Windows (Command Prompt)
```

Edit `.env`:
```env
DB_HOST=YOUR_DB_HOST
DB_PORT=YOUR_DB_PORT
DB_USER=YOU_DB_USER
DB_NAME=YOUR_DB_NAME
DB_PASSWORD=YOUR_DB_PASSWORD
```

---

## Platform Setup

### <img src="https://cdn.simpleicons.org/linux/FCC624" width="22" height="22" valign="middle"/> Linux

#### 1. Install Required Tools

```bash
sudo apt install cmake g++ libpqxx-dev qt6-base-dev
```

#### 2. Configure VS Code IntelliSense (optional)

Qt headers are typically located at `/usr/include/x86_64-linux-gnu/qt6`.

Open `.vscode/c_cpp_properties.json` and ensure `includePath` includes:

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "/usr/include/x86_64-linux-gnu/qt6",
                "/usr/include/x86_64-linux-gnu/qt6/QtWidgets",
                "/usr/include/x86_64-linux-gnu/qt6/QtCore",
                "/usr/include/x86_64-linux-gnu/qt6/QtGui"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "c17",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ],
    "version": 4
}
```

#### 3. Build & Run

```bash
mkdir build
cd build
cmake ..
cmake --build .
./app
```

---

### <img src="https://cdn.simpleicons.org/apple/000000" width="22" height="22" valign="middle"/> macOS (Homebrew)

#### 1. Install Homebrew (if not already installed)

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

#### 2. Install Required Tools

```bash
brew install cmake libpqxx qt
```

#### 3. Configure VS Code IntelliSense (optional)

Find your Qt prefix path:

```bash
brew --prefix qt
```

Open `.vscode/c_cpp_properties.json` and update `includePath` (replace `<qt-prefix>` with the output above):

```json
{
    "configurations": [
        {
            "name": "Mac",
            "includePath": [
                "${workspaceFolder}/**",
                "<qt-prefix>/include",
                "<qt-prefix>/include/QtWidgets",
                "<qt-prefix>/include/QtCore",
                "<qt-prefix>/include/QtGui"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/clang",
            "cStandard": "c17",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "macos-clang-x64"
        }
    ],
    "version": 4
}
```

#### 4. Build & Run

```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt)
cmake --build .
./app
```

> The `-DCMAKE_PREFIX_PATH` flag tells CMake where to find Qt since Homebrew doesn't install it to a system path.

---

### <img src="https://static.cdnlogo.com/logos/m/26/microsoft-windows.svg" width="22" height="22" valign="middle"/> Windows (MSVC / Visual Studio)

#### 1. Install Prerequisites

- [Visual Studio 2022](https://visualstudio.microsoft.com/) — select the **"Desktop development with C++"** workload during install
- [CMake](https://cmake.org/download/) — check **"Add CMake to system PATH"** during install
- [Qt6](https://www.qt.io/download-qt-installer) — use the Qt Online Installer, select **Qt 6.x.x > MSVC 2019/2022 64-bit**
- [PostgreSQL](https://www.postgresql.org/download/windows/) — includes headers and `libpq` needed by `libpqxx`
- [libpqxx](https://github.com/jtv/libpqxx) — must be built from source (see step 2)

#### 2. Build libpqxx from Source

Open **Developer Command Prompt for VS** and run:

```cmd
git clone https://github.com/jtv/libpqxx.git
cd libpqxx
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DPostgreSQL_ROOT="C:\Program Files\PostgreSQL\<version>"
cmake --build . --config Release
cmake --install . --prefix "C:\libpqxx"
```

Replace `<version>` with your installed PostgreSQL version (e.g., `16`).

#### 3. Build the Project

Open **Developer Command Prompt for VS** from the project root:

```cmd
mkdir build
cd build
cmake .. ^
  -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\msvc2022_64" ^
  -Dpqxx_DIR="C:\libpqxx\lib\cmake\libpqxx" ^
  -DPostgreSQL_ROOT="C:\Program Files\PostgreSQL\<version>"
cmake --build . --config Release
```

Replace `C:\Qt\6.x.x\msvc2022_64` with your actual Qt installation path.

#### 4. Run the App

```cmd
cd Release
app.exe
```

> Make sure `.env` is placed one directory above the executable (`build\.env`) so the app can find it at `../.env`.

#### 5. Configure VS Code IntelliSense (optional)

Open `.vscode/c_cpp_properties.json` and update with your actual Qt and libpqxx paths:

```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/Qt/6.x.x/msvc2022_64/include",
                "C:/Qt/6.x.x/msvc2022_64/include/QtWidgets",
                "C:/Qt/6.x.x/msvc2022_64/include/QtCore",
                "C:/Qt/6.x.x/msvc2022_64/include/QtGui",
                "C:/libpqxx/include"
            ],
            "defines": ["_DEBUG", "UNICODE", "_UNICODE"],
            "windowsSdkVersion": "10.0.22621.0",
            "compilerPath": "cl.exe",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "windows-msvc-x64"
        }
    ],
    "version": 4
}
```
