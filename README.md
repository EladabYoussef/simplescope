# SimpleScope

A dual-channel oscilloscope implementation for STM32F407 microcontroller with real-time data acquisition and display.

## Architecture Overview

![SimpleScope Architecture](data/architecture.png)

## Project Structure

### Application Modules (`app/`)

The main application logic is implemented in the app folder and integrated into the system via `Core/Src/main.c`.

- **Scope App** (`scope_app.h/c`)
  - Main application orchestration
  - Task coordination and state management

- **Acquisition** (`aquisition.h/c`)
  - Dual-channel ADC sampling with DMA
  - Timer-triggered acquisition

- **Channels** (`channels.h/c`)
  - Per-channel ring buffer management
  - Decouples high-speed sampling from consumer tasks

- **Display** (`display.h/c`)
  - SSD1306 OLED display control
  - Shows selected channel data

- **Communication** (`communication.h/c`)
  - Data streaming to PC
  - Protocol handling

- **Configuration** (`config.h`)
  - Application-wide configuration constants

### Hardware Drivers

- **SSD1306 OLED Display** (`Drivers/SSD1306/`)
  - Graphics and font support
  - Hardware abstraction layer

- **STM32F4 HAL** (`Drivers/STM32F4xx_HAL_Driver/`)
  - Hardware abstraction for ADC, DMA, Timer, GPIO
  - USB Host support via Middlewares

### Core System

- **Main** (`Core/Src/main.c`)
  - System initialization and main loop
  - STM32F407 system configuration

## Hardware Platform

- **MCU**: STM32F407VG (ARM Cortex-M4)
- **Display**: SSD1306 OLED (128x64, I2C/SPI)
- **Acquisition**: Dual ADC channels with DMA
- **Communication**: USB Host support

## Current Implementation Status

The following modules are scaffolded and ready for implementation:
- ADC acquisition framework
- Ring buffer channel management
- Display driver integration
- Communication interface
- Button/UI input handling
- System configuration

## Build System

- **CMake** 3.22+
- **ARM GCC** (arm-none-eabi)
- Generates build files in `build/Debug/`
