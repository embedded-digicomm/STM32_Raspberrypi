# STM32F427/429 USART1 polling driver

Minimal bare-metal USART1 polling driver for STM32F427/429-class parts.

- USART: `USART1`
- Pins: `PA9` = TX, `PA10` = RX
- Alternate function: `AF7`
- Clock assumption for the included test: `PCLK2 = 16 MHz`
- Baud rate: `115200 8N1`
- Method: polling `TXE` and `RXNE` flags

The test program sends a banner, then echoes every received character.

## Keil uVision

1. Create a new project for the exact MCU or board, for example `STM32F429ZITx`.
2. Let Keil add the normal CMSIS startup file for the device.
3. Add these files to the project:
   - `main.c`
   - `usart1_driver.c`
   - `usart1_driver.h`
4. Do not add the provided `startup_stm32f427xx.s` or `linker.ld` if Keil already generated startup/scatter files.
5. Build and flash.
6. Open the ST-LINK VCP COM port or an external USB-UART adapter at `115200 8N1`.

If your Keil project configures clocks so APB2 is not 16 MHz, change the first argument:

```c
USART1_Init(APB2_CLOCK_HZ, 115200UL);
```

For STM32F429I-DISC1, ST-LINK VCP is connected to `USART1 PA9/PA10` when solder bridges `SB11` and `SB15` are ON.

Build, if `arm-none-eabi-gcc` is installed:

```sh
make
```

Generated image:

```text
build/uart1_polling.elf
build/uart1_polling.bin
```

Flash with your preferred tool, for example:

```sh
st-flash write build/uart1_polling.bin 0x08000000
```
