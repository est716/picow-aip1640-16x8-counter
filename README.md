# Raspberry Pi Pico W + AiP1640 16x8 LED Matrix Counter

本專案使用 **Raspberry Pi Pico W**（採用 Raspberry Pi Pico C/C++ SDK）驅動基於 **AiP1640** 晶片的 16x8 LED Matrix ，實現數位計數器（Counter）功能。

由於 AiP1640 採用非標準 I<sup>2</sup>C 的兩線序列協定，本專案透過 Pico SDK 之 GPIO 以 **Bit-Banging（軟體模擬時序）** 方式實作通訊控制。

---

## 專案特色 (Features)

- **C/C++ SDK 原生開發**：使用 Pico C SDK 進行硬體底層控制。
- **GPIO Bit-Banging 實作**：控制 CLK 與 DIN 脈衝，實現類 I<sup>2</sup>C 通訊模擬。
- **16x8 點陣顯示控制**：支援自動地址增加模式與多階顯示亮度切換。

---

## 硬體接線 (Hardware Wiring)

| Raspberry Pi Pico W 引腳 | AiP1640 引腳 | 說明 |
| :--- | :--- | :--- |
| **VBUS (5V)** 或 **3V3** | VDD | 電源供應（建議接 5V 以獲取較佳顯示亮度） |
| **GND** | GND | 共地 |
| **GPIO 16** (預設) | CLK | 時脈訊號 (Clock) |
| **GPIO 17** (預設) | DIN / DATA | 資料輸入 (Data In) |

---

## 專案結構 (Project Directory)

```text
.
├── CMakeLists.txt        # CMake 建置配置文件
├── src/
│   ├── main.c            # 主程式與計數器邏輯
│   ├── aip1640.c         # AiP1640 通訊與驅動實作
│   └── aip1640.h         # AiP1640 標頭檔
└── README.md             # 專案說明文件
```

---

## 開發環境與建置 (Build & Flash Instructions)

### 1. 環境需求
- [Raspberry Pi Pico C/C++ SDK](https://github.com/raspberrypi/pico-sdk)
- CMake (3.13+)
- ARM GCC 工具鏈 (`arm-none-eabi-gcc`)

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

---

## CMake 範例設定 (CMakeLists.txt)

```cmake
cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(aip1640_counter C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(aip1640_counter
    src/main.c
    src/aip1640.c
)

# 連結 Pico SDK 必要函式庫
target_link_libraries(aip1640_counter
    pico_stdlib
    hardware_gpio
)

pico_add_extra_outputs(aip1640_counter)
```

---

## 通訊協定與原理 (Protocol & Implementation)

1. **Start / Stop 時序**：在 CLK 為高電位期間拉低 DIN 觸發 Start；在 CLK 為高電位期間拉高 DIN 觸發 Stop。
2. **資料傳輸**：每個 Byte 採取 LSB（最低有效位元）優先傳送，傳送時在 CLK 的低電位變更資料，高電位鎖定資料。
3. **暫存器配置**：
   - `0x40`：設定為自動地址加一模式。
   - `0xC0`：設定起始記憶體位址（GRID1）。
   - `0x88` ~ `0x8F`：控制顯示開關與 8 階亮度調整。
