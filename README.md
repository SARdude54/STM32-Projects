# STM32L476 Flash and Debug Setup on Ubuntu

This setup uses the GNU Arm toolchain, CMake, OpenOCD, `gdb-multiarch`, and the Nucleo board's on-board ST-LINK.

Project:

```text
~/STM32-Workspace/l476_baremetal
```

Firmware:

```text
build/Debug/l476_baremetal.elf
```

## 1. Install tools

```bash
sudo apt update
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi \
  libnewlib-arm-none-eabi openocd gdb-multiarch
```

Verify:

```bash
arm-none-eabi-gcc --version
openocd --version
gdb-multiarch --version
```

## 2. Build the project

```bash
cd ~/STM32-Workspace/l476_baremetal
cmake --preset Debug
cmake --build --preset Debug
```

Confirm the ELF exists:

```bash
ls -lh build/Debug/l476_baremetal.elf
```

## 3. Start OpenOCD

In terminal 1:

```bash
cd ~/STM32-Workspace/l476_baremetal

openocd \
  -f interface/stlink.cfg \
  -f target/stm32l4x.cfg
```

Leave this terminal running. A successful connection includes:

```text
Listening on port 3333 for gdb connections
```

## 4. Start GDB

In terminal 2:

```bash
cd ~/STM32-Workspace/l476_baremetal
gdb-multiarch build/Debug/l476_baremetal.elf
```

At the GDB prompt:

```gdb
target remote localhost:3333
monitor reset halt
load
break main
continue
```

The MCU should stop at `main()`.

## 5. Useful GDB commands

```gdb
list
step
next
continue
finish
backtrace
info registers
info breakpoints
```

Stop execution with `Ctrl+C`.

Exit GDB:

```gdb
quit
```

Stop OpenOCD in terminal 1 with `Ctrl+C`.

## 6. Inspect peripheral registers

```gdb
print/x RCC->AHB2ENR
print/x GPIOA->MODER
print/x GPIOA->OTYPER
print/x GPIOA->OSPEEDR
print/x GPIOA->PUPDR
print/x GPIOA->ODR
```

For PA5:

```gdb
print/x RCC->AHB2ENR & 1
print/x (GPIOA->MODER >> 10) & 3
print/x (GPIOA->OTYPER >> 5) & 1
print/x (GPIOA->OSPEEDR >> 10) & 3
print/x (GPIOA->PUPDR >> 10) & 3
print/x (GPIOA->ODR >> 5) & 1
```

Expected after GPIO initialization:

```text
GPIOA clock enable = 1
PA5 mode           = 1  (output)
PA5 output type    = 0  (push-pull)
PA5 speed          = 0  (low)
PA5 pull           = 0  (none)
PA5 output         = 0 or 1
```

## 7. Reflash after rebuilding

```bash
cmake --build --preset Debug
```

Then in GDB:

```gdb
monitor reset halt
load
break main
continue
```

## 8. OpenOCD port already in use

If OpenOCD reports `Address already in use`:

```bash
pkill openocd
```

Then start OpenOCD again.

Use one OpenOCD process and one GDB process.