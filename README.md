Scons STM32H573I-DK Template
============================

This repository contains a SCons template project as a starting point for STM32 H573I-DK projects. ST's official *Hal Library* is already included in this repository and will automatically be linked against the target application.

Both **C and C++** sources are supported.   *flash* target for easy programming of the microcontroller.

This template includes a sample application which turns on the Green LED on the STM32H573I-DK board for demonstration purposes.(Includes a uart1 print debug)

# Prerequisites
Get SCons for your platform. You should use at least **SCons 4.5.0**.
Get LLVM for your platform. You should use at least **LLVM 19.1.0**.
Get JLink for your platform. You should use at least **JLink**.

This repository contains build scripts and auxiliary material for building a
bare-metal LLVM based toolchain targeting Arm based on:(only support Armv8-M Mainline and Baseline)
* clang + llvm version 19.1.0
* lld 
* libc++abi
* libc++
* compiler-rt
* picolibc, or optionally newlib or LLVM's libc

for the complete LLVM Embedded Toolchain for Arm, please visit [GNU ARM Embedded Toolchain](https://github.com/ARM-software/LLVM-embedded-toolchain-for-Arm).


# Quick start

1. Clone the project
```bash
git clone https://github.com/your-username/scons-stm32h573i-dk-template.git
cd scons-stm32h573i-dk-template
```

2. Environment Configuration
- Ensure SCons 4.5.0 or higher is installed
- Install LLVM 19.1.0 or higher
- Install the JLink software package
- Add the paths of the above tools to the system environment variables

3. Project Structure
```
.
├── .cache/               # SCons 缓存目录
├── .vscode/              # VSCode 配置文件
├── arm-none-eabi/        # ARM 工具链目录
├── build/                # 构建工具目录
├── Core/                 # 核心代码目录
├── Drivers/              # STM32 HAL 驱动库
├── LLVM19/               # LLVM 工具链目录
├── output/               # 最终输出目录
├── startup/              # 启动文件目录
├── compile_commands.json # 编译命令数据库
├── STM32H573.svd         # SVD 外设描述文件
└── README.md             # 项目说明文档
```

4. Compile the project
```bash
# Compile the project
cd build 
build.bat

# Clean the compiled files
build -c
```

5. Burn the program
```bash
# Use JLink to burn the program to the development board
scons flash
```

6. Debug
- Debug using JLink
```bash
scons debug
```
- Serial debug output configuration:
  - Baud rate: 115200
  - Data bits: 8
  - Stop bits: 1
  - Parity: None
  - Flow control: None

7. Custom development
- Add new source files in the `src/` directory
- Modify the `SConstruct` file to add new source files or configurations
- All user code should be placed in the `src/` directory

8. Notes
- Ensure the development board is properly connected to the computer
- Please read the STM32H573I-DK user manual carefully before use
- It is recommended to use a USB to serial port module to monitor debug output

# Troubleshooting

If you encounter compilation or burning problems, please check:
1. Whether the environment variables are correctly configured
2. Whether the development board is connected properly
3. Whether the JLink driver is installed correctly
4. Whether the serial port is occupied by other programs




