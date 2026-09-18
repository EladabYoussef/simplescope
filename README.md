# SimpleScope

A dual-channel oscilloscope project for the STM32F407 microcontroller. The repository currently contains the hardware foundation and application interfaces for real-time acquisition, channel selection, and communication; those application modules are still being implemented.

For a detailed refresher on the architecture, hardware data path, current status, and next implementation steps, see [ARCHITECTURE.md](ARCHITECTURE.md).

## Architecture Overview

![SimpleScope Architecture](data/architecture.png)

## Project Structure

### Application Modules (`app/`)

The main application logic is implemented in the app folder and integrated into the system via `Core/Src/main.c`.

- **Scope App** (`scope_app.h/c`)
  - Intended application orchestration
  - Currently an empty public interface/source scaffold

- **Acquisition** (`aquisition.h/c`)
  - Intended dual-channel ADC sampling with DMA
  - Header defines the shared buffer and half/full transfer callbacks; source is currently empty

- **Channels** (`channels.h/c`)
  - Intended channel selection and button/LED state
  - Header defines channel objects and callbacks; source is currently empty

- **Display** (`display.h/c`)
  - Reserved for OLED integration
  - Deliberately not documented in detail for now

- **Communication** (`communication.h/c`)
  - Intended sample packet creation and transport
  - Header defines the packet shape and API; source is currently empty

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

The STM32/CubeMX foundation, peripheral setup, USB host middleware, and application headers are present. ADC DMA callbacks, timer-triggered sampling, channel logic, packet transport, and application orchestration still need implementation. See [ARCHITECTURE.md](ARCHITECTURE.md) for the detailed checklist.

## Build System

- **CMake** 3.22+
- **ARM GCC** (arm-none-eabi)
- Generates build files in `build/Debug/`
