#!/bin/sh

arm-none-eabi-gdb ./wolfssl_client_flash.elf -ex 'target remote | openocd -c "gdb_port pipe;" -f ../../../ASF/sam0/utils/openocd/atmel_samd21_xplained_pro.cfg'
