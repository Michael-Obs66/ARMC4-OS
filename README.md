# μKernelOS

<p align="center">
  <img src="https://img.shields.io/badge/ARM-Cortex--M4-blue?style=for-the-badge&logo=arm" />
  <img src="https://img.shields.io/badge/Microkernel-OS-green?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Language-C%2FASM-orange?style=for-the-badge&logo=c" />
  <img src="https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge" />
</p>

A lightweight **microkernel-based operating system** designed for ARM Cortex-M4 microcontrollers.  
This project is built from scratch using **C** and **ARM Assembly**, with a focus on **simplicity, modularity, and extensibility** for IoT and embedded systems.

---
## 🚀 Why This Microkernel OS?

Unlike existing RTOS solutions, this project aims to combine **lightweight performance** with **modular security**.  
Here’s how it compares with well-known technologies:

| Feature / OS        | FreeRTOS ⚡ | Zephyr 🌐 | μKernelOS 🔒 |
|---------------------|------------|-----------|------------------|
| **Kernel Type**     | Monolithic | Hybrid    | Pure Microkernel |
| **Footprint**       | Ultra-small (<10KB) | Medium–Large (100KB+) | Small–Medium (scalable) |
| **Task Isolation**  | ❌ Limited | ⚠️ Partial | ✅ Strong (drivers isolated) |
| **Real-time**       | ✅ Deterministic | ✅ RT-capable | ✅ RT-capable |
| **Networking**      | ⚠️ Limited (needs extension) | ✅ Built-in (TCP/IP, Bluetooth, etc.) | ⚠️ Custom / extendable |
| **Security**        | ❌ Minimal | ⚠️ Moderate | ✅ Higher (fault isolation) |
| **Modularity**      | ❌ Low     | ✅ High    | ✅ High |
| **Best Use Case**   | Ultra-light IoT | Complex IoT & networking | Mission-critical IoT (secure + efficient) |
---
### ✨ Key Advantages of MicrokernelOS
- 🔒 **Secure by design**: Drivers and services run in isolated spaces.  
- ⚡ **Lightweight**: Minimal overhead, suitable for ARM Cortex-M4.  
- 🔧 **Modular**: Easy to extend with new drivers without touching the kernel.  
- 🛡️ **Reliable**: A faulty driver won’t crash the whole system.  
- 📡 **IoT-ready**: Designed with mission-critical IoT use cases in mind (e.g., gas monitoring, safety systems).  

## ✨ Features

- 🔹 **Preemptive Scheduler** – basic round-robin task scheduling.  
- 🔹 **Task Management** – create, switch, and terminate tasks.  
- 🔹 **Inter-Process Communication (IPC)** – simple message passing.  
- 🔹 **Memory Protection (MPU)** – experimental memory safety support.  
- 🔹 **UART Driver** – minimal serial I/O for debugging and communication.  
- 🔹 **Interrupt Handling** – basic Cortex-M4 interrupt service routines.  
- 🔹 **Heap Management** – dynamic memory allocation.  
- 🔹 **Modular Design** – kernel, scheduler, IPC, and drivers separated for clarity.

---

## 📂 Project Structure

```
μKernelOS/
├── boot/
│   ├── startup.s
│   ├── linker.ld
│   └── boot.c
├── kernel/
│   ├── kernel.c
│   ├── scheduler.c
│   ├── task.c
│   └── ipc.c
├── arch/arm/
│   ├── cortex_m4.c
│   ├── context_switch.s
│   └── interrupt.c
├── mm/
│   ├── mm.c
│   ├── heap.c
│   └── mpu.c
├── drivers/
│   ├── uart.c
│   ├── gpio.c
│   ├── timer.c
│   └── oled.c
├── apps/
│   ├── blinky.c
│   ├── shell.c
│   └── game.c
├── include/
│   ├── kernel.h
│   ├── task.h
│   └── drivers.h
└── Makefile
```

---

## 🛠️ Build Instructions

### Prerequisites
- **ARM GCC Toolchain** (`arm-none-eabi-gcc`, `arm-none-eabi-ld`, `arm-none-eabi-objcopy`)  
- **Make** utility  
- (Optional) **QEMU** or hardware board (STM32F4 or equivalent) for testing  

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

You can test **μKernelOS** on:  
- **Real hardware** – STM32F4 or similar Cortex-M4 boards.  
- **Emulator** – run with QEMU:  
  ```bash
  qemu-system-arm -M stm32-p103 -kernel kernel.elf -serial stdio
  ```
- **Emulator** - run with RENODE:
  ```bash
  renode
  mach create
  machine LoadPlatformDescription @platforms/boards/stm32f4_discovery.repl
  sysbus LoadELF @kernel.elf
  showAnalyzer sysbus.usart2
  start
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

## 🤝 Contributing

Contributions are welcome!  
- Fork this repository  
- Create a feature branch (`git checkout -b feature/my-feature`)  
- Commit changes and push  
- Open a Pull Request  

---

## 📜 License

This project is licensed under the **MIT License**.  
See [LICENSE](LICENSE) for details.

---

## 🌟 Acknowledgements

- ARM Cortex-M4 architecture documentation  
- Open-source RTOS and microkernel projects for inspiration  
