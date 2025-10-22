# ARMC4-OS
<p align="center">
  <img src="https://img.shields.io/badge/ARM-Cortex--M4-blue?style=for-the-badge&logo=arm" />
  <img src="https://img.shields.io/badge/Microkernel-OS-green?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Language-C%2FASM-orange?style=for-the-badge&logo=c" />
  <img src="https://img.shields.io/badge/License-Apache 2.0-yellow?style=for-the-badge" />
</p>

A lightweight **microkernel-based operating system** designed for ARM Cortex-M4 microcontrollers.  
This project is built from scratch using **C** and **ARM Assembly**, with a focus on **simplicity, modularity, and extensibility** for IoT and embedded systems.

Video : 
https://youtube.com/shorts/I3MKKgjhtqE

---
## 🚀 Why ARMC4-OS?

This project aims to combine **lightweight performance** with **modular security**.  

---
### ✨ Key Advantages of MicrokernelOS
- 🔒 **Secure by design**: Drivers and services run in isolated spaces.  
- ⚡ **Lightweight**: Minimal overhead, suitable for ARM Cortex-M4.  
- 🔧 **Modular**: Easy to extend with new drivers without touching the kernel.  

## ✨ Features

- 🔹 **Preemptive Scheduler** – basic round-robin task scheduling.  
- 🔹 **Task Management** – create, switch, and terminate tasks.  
- 🔹 **Inter-Process Communication (IPC)** – simple message passing.  
- 🔹 **UART Driver** – minimal serial I/O for debugging and communication.  
- 🔹 **Interrupt Handling** – basic Cortex-M4 interrupt service routines.  
- 🔹 **Heap Management** – dynamic memory allocation.  
- 🔹 **Modular Design** – kernel, scheduler, IPC, and drivers separated for clarity.

---

## 📂 Project Structure

```
ARMC4-OS/
├── boot/ # startup.s, linker.ld, boot.c ..etc
├── kernel/ # kernel.c, scheduler.c, task.c, ipc.c ..etc
├── arch/arm/ # cortex_m4.c, context_switch.s, interrupt.c ..etc
├── mm/ # mm.c, heap.c, mpu.c ..etc
├── drivers/ # uart.c, gpio.c, timer.c, oled.c ..etc
├── apps/ # blinky.c, shell.c, game.c ..etc
├── include/ # kernel.h, task.h, drivers.h, oled.h ..etc
└── Makefile
```

---

## 🛠️ Build Instructions

### Prerequisites
- **ARM GCC Toolchain** (`arm-none-eabi-gcc`, `arm-none-eabi-ld`, `arm-none-eabi-objcopy`)  
- **Make** utility  
- (Optional) **Renode** or hardware board (STM32F4 or equivalent) for testing  

### Build
```bash
make -f makefile.mak
```

This will generate the kernel binary (`kernel.elf` and `kernel.bin`).

### Flash to Hardware
```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program kernel.bin verify reset exit 0x08000000"
```

Or upload with your preferred flashing tool.

---

## 🚀 Running the OS

You can test **ARMC4-OS** on:  
- **Real hardware** – STM32F4 or similar Cortex-M4 boards.  
- **Emulator** - run with RENODE:
  1. make run_kernel.resc file :
  ```bash
  mach create
  machine LoadPlatformDescription @platforms/boards/stm32f4_discovery.repl
  sysbus LoadELF @ukernelos.elf

  start
  ```
  2. Running .resc file :
  ```bash
  renode run_kernel.resc
  ```
---

## 📌 Roadmap

- [ ] Implement priority-based scheduling  
- [ ] Extend IPC with message queues  
- [ ] Add filesystem support (lightweight)  
- [ ] Networking stack for IoT use cases  
- [ ] Improve documentation and examples  

---

## 📖 Documentation

Currently, documentation is minimal. More details on APIs, scheduler, IPC, and drivers will be added soon.  

For now, browse the `kernel/` and `arch/arm/` source code for implementation details.

---

## 📜 License

This project is licensed under the **MIT License**.  
See [LICENSE](LICENSE) for details.

---

## 🌟 Acknowledgements

- ARM Cortex-M4 architecture documentation  
- Open-source RTOS and microkernel projects for inspiration  
