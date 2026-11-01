# Raspberry Pi Pico W + AiP1640 16x8 LED Matrix Counter

[繁體中文](#繁體中文) | [English](#english)

---

# 繁體中文

本專案使用 **Raspberry Pi Pico W**（採用 Raspberry Pi Pico C/C++ SDK）驅動基於 **AiP1640** 晶片的 16x8 LED Matrix，實現數位計數器（Counter）功能。

由於 AiP1640 採用專屬的兩線序列通訊協定，本專案透過 Pico SDK 之 GPIO 以 **Bit-Banging（軟體模擬時序）** 方式實作通訊控制。

## 專案特色 (Features)

- **C/C++ SDK 原生開發**：使用 Pico C SDK 進行硬體底層控制。
- **GPIO Bit-Banging 實作**：控制 CLK 與 DIN 脈衝，實現類似 I²C 的兩線序列通訊。
- **16x8 點陣顯示控制**：支援自動地址增加模式與多階顯示亮度切換。

## 硬體接線 (Hardware Wiring)

| Raspberry Pi Pico W 引腳 | AiP1640 引腳 | 說明 |
| :--- | :--- | :--- |
| **VBUS (5V)** | VDD | 電源供應（建議接 5V 以獲取較佳顯示亮度） |
| **GND** | GND | 共地 |
| **GPIO 0** (預設) | CLK | 時脈訊號 (Clock) |
| **GPIO 1** (預設) | DIN / DATA | 資料輸入 (Data In) |

## 專案結構 (Project Directory)

```text
.
├── CMakeLists.txt        # CMake 建置配置文件
├── main.c                # 主程式與計數器邏輯
└── README.md             # 專案說明文件
```

## 開發環境與建置 (Build & Flash Instructions)

### 1. 環境需求

- [Raspberry Pi Pico C/C++ SDK](https://github.com/raspberrypi/pico-sdk)
- CMake (3.13+)
- ARM GCC 工具鏈 (`arm-none-eabi-gcc`)

建議於 VS Code 安裝 Raspberry Pi Pico Extension，即可直接使用。

### 2. 編譯步驟

在專案根目錄下執行以下命令：

```bash
# 1. 建立並進入 build 目錄
mkdir build
cd build

# 2. 進行 CMake 設定
cmake ..

# 3. 開始編譯專案
make -j4
```

### 3. 燒錄 (Flashing)

1. 按住 Pico W 上的 **BOOTSEL** 鍵，並透過 USB 數據線連接至電腦。
2. 將 `build/` 目錄下產生的 `.uf2` 檔案拖入名為 `RPI-RP2` 的虛擬隨身碟中。
3. Pico W 將自動重啟並執行程式。

## 通訊協定與原理 (Protocol & Implementation)

1. **Start / Stop 時序**：在 CLK 為高電位期間，DIN 由高拉低觸發 Start；在 CLK 為高電位期間，DIN 由低拉高觸發 Stop。

2. **資料傳輸**：每個 Byte 採取 LSB（最低有效位元）優先傳送，傳送時在 CLK 的低電位變更資料，高電位取樣資料。

3. **指令配置**：
   - `0x40`：設定為地址自增模式。
   - `0x44`：設定為固定地址模式。
   - `0xC0` ~ `0xCF`：記憶體位址（GRID1 ~ GRID16）。
   - `0x88` ~ `0x8F`：控制 8 階亮度調整。

4. **指令運作流程**
   1. Data command setting
   2. Address command setting
   3. Display Control

5. **注意事項**
   - 地址自增模式：僅需一組 Start-End 訊號包裹。
   - 固定地址模式：每個固定地址皆需一組 Start-End 訊號。

---

# English

This project uses the **Raspberry Pi Pico W** with the Raspberry Pi Pico C/C++ SDK to drive a 16x8 LED Matrix based on the **AiP1640** driver IC, implementing a digital counter.

The AiP1640 uses a proprietary two-wire serial communication protocol. This project implements the communication through **GPIO Bit-Banging (software-based timing control)** using the Pico SDK.

## Features

- **Native C/C++ SDK Development**: Uses the Raspberry Pi Pico C SDK for low-level hardware control.
- **GPIO Bit-Banging Implementation**: Manually controls CLK and DIN signals to implement a two-wire serial communication protocol similar to I²C.
- **16x8 LED Matrix Control**: Supports automatic address increment mode and multiple brightness levels.

## Hardware Wiring

| Raspberry Pi Pico W Pin | AiP1640 Pin | Description |
| :--- | :--- | :--- |
| **VBUS (5V)** | VDD | Power supply (5V is recommended for better display brightness) |
| **GND** | GND | Common ground |
| **GPIO 0** (Default) | CLK | Clock signal |
| **GPIO 1** (Default) | DIN / DATA | Data input |

## Project Directory

```text
.
├── CMakeLists.txt        # CMake build configuration
├── main.c                # Main program and counter logic
└── README.md             # Project documentation
```

## Build & Flash Instructions

### 1. Requirements

- [Raspberry Pi Pico C/C++ SDK](https://github.com/raspberrypi/pico-sdk)
- CMake (3.13+)
- ARM GCC Toolchain (`arm-none-eabi-gcc`)

Installing the Raspberry Pi Pico Extension for VS Code is recommended.

### 2. Build

Run the following commands from the project root directory:

```bash
# 1. Create and enter the build directory
mkdir build
cd build

# 2. Configure the project with CMake
cmake ..

# 3. Build the project
make -j4
```

### 3. Flashing

1. Hold down the **BOOTSEL** button on the Pico W while connecting it to your computer via USB.
2. Copy the generated `.uf2` file from the `build/` directory to the `RPI-RP2` USB mass storage device.
3. The Pico W will automatically reboot and run the program.

## Communication Protocol & Implementation

1. **Start / Stop Timing**: A Start condition is triggered when DIN transitions from HIGH to LOW while CLK is HIGH. A Stop condition is triggered when DIN transitions from LOW to HIGH while CLK is HIGH.

2. **Data Transmission**: Each byte is transmitted in LSB (Least Significant Bit) first order. Data is changed while CLK is LOW and sampled while CLK is HIGH.

3. **Command Configuration**:
   - `0x40`: Sets automatic address increment mode.
   - `0x44`: Sets fixed address mode.
   - `0xC0` ~ `0xCF`: Memory addresses (GRID1 ~ GRID16).
   - `0x88` ~ `0x8F`: Controls the 8 brightness levels.

4. **Command Operation Sequence**
   1. Data command setting
   2. Address command setting
   3. Display Control

5. **Notes**
   - **Automatic Address Increment Mode**: Only one Start-End sequence is required to wrap the data transmission.
   - **Fixed Address Mode**: Each fixed address requires its own Start-End sequence.

---

# Build Configuration / 建置設定

## CMake Configuration Example (CMakeLists.txt)

The following CMake configuration is shared by both the Chinese and English sections.

以下 CMake 設定為中英文共用內容。

```cmake
# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, cmake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(AiP1640_LED_Counter C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(AiP1640_LED_Counter main.c )

pico_set_program_name(AiP1640_LED_Counter "AiP1640_LED_Counter")
pico_set_program_version(AiP1640_LED_Counter "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(AiP1640_LED_Counter 0)
pico_enable_stdio_usb(AiP1640_LED_Counter 0)

# Add the standard library to the build
target_link_libraries(AiP1640_LED_Counter
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(AiP1640_LED_Counter PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(AiP1640_LED_Counter
        pico_cyw43_arch_none
        )

pico_add_extra_outputs(AiP1640_LED_Counter)
```

**備註 / Note：**

`pico_cyw43_arch_none` 提供 CYW43 晶片的基礎初始化支援，但不啟用完整的 Wi-Fi 網路堆疊。

`pico_cyw43_arch_none` provides basic initialization support for the CYW43 chip without enabling the full Wi-Fi networking stack.

---

[返回頁首 / Back to top](#raspberry-pi-pico-w--aip1640-16x8-led-matrix-counter)