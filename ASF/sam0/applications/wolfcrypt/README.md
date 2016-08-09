# wolfCrypt Example for Atmel ATECC508A

This example demonstrates the wolfCrypt test and benchmark applications with the Atmel ATECC508 ECC 256-bit hardware accelerator.

## Installation
### Setup

The Atmel ATECC508A chips come from the factory un-programmed and need to be provisioned. Atmel provided us code as reference which exists in `cryptoauthlib/certs/provision.c`. The function is `atcatls_device_provision` and can be called more than once. If the device is not provisioned it will set it up with default slot settings. If its already provisioned it will skip.

### Building

The wolfCrypt test example is setup to be built from a terminal using GCC ARM and a Makefile in the `ASF/sam0/applications/wolfcrypt/samd21j18a_samd21_xplained_pro/gcc` directory.

Example:

```
cd ASF/sam0/applications/wolfcrypt/samd21j18a_samd21_xplained_pro/gcc
make
MKDIR   common/utils/interrupt/
CC      common/utils/interrupt/interrupt_sam_nvic.o
MKDIR   common2/services/delay/sam0/
CC      common2/services/delay/sam0/systick_counter.o
MKDIR   sam0/applications/wolfcrypt/
CC      sam0/applications/wolfcrypt/main.o
MKDIR   sam0/boards/samd21_xplained_pro/
CC      sam0/boards/samd21_xplained_pro/board_init.o
MKDIR   sam0/drivers/port/
CC      sam0/drivers/port/port.o
MKDIR   sam0/drivers/sercom/i2c/i2c_sam0/
CC      sam0/drivers/sercom/i2c/i2c_sam0/i2c_master.o
CC      sam0/drivers/sercom/sercom.o
CC      sam0/drivers/sercom/sercom_interrupt.o
MKDIR   sam0/drivers/rtc/rtc_sam_d_r/
CC      sam0/drivers/rtc/rtc_sam_d_r/rtc_count.o
CC      sam0/drivers/rtc/rtc_sam_d_r/rtc_count_interrupt.o
CC      sam0/drivers/rtc/rtc_sam_d_r/rtc_calendar.o
MKDIR   sam0/drivers/tcc/
CC      sam0/drivers/tcc/tcc.o
CC      sam0/drivers/tcc/tcc_callback.o
MKDIR   sam0/drivers/sercom/usart/
CC      sam0/drivers/sercom/usart/usart.o
CC      sam0/drivers/sercom/usart/usart_interrupt.o
MKDIR   sam0/drivers/system/clock/clock_samd21_r21_da/
CC      sam0/drivers/system/clock/clock_samd21_r21_da/clock.o
CC      sam0/drivers/system/clock/clock_samd21_r21_da/gclk.o
MKDIR   sam0/drivers/system/interrupt/
CC      sam0/drivers/system/interrupt/system_interrupt.o
MKDIR   sam0/drivers/system/pinmux/
CC      sam0/drivers/system/pinmux/pinmux.o
CC      sam0/drivers/system/system.o
MKDIR   sam0/utils/cmsis/samd21/source/gcc/
CC      sam0/utils/cmsis/samd21/source/gcc/startup_samd21.o
CC      sam0/utils/cmsis/samd21/source/system_samd21.o
MKDIR   sam0/utils/stdio/
CC      sam0/utils/stdio/read.o
CC      sam0/utils/stdio/write.o
MKDIR   sam0/utils/syscalls/gcc/
CC      sam0/utils/syscalls/gcc/syscalls.o
MKDIR   ../wolfssl/wolfcrypt/src/
CC      ../wolfssl/wolfcrypt/src/random.o
CC      ../wolfssl/wolfcrypt/src/logging.o
CC      ../wolfssl/wolfcrypt/src/memory.o
CC      ../wolfssl/wolfcrypt/src/wc_encrypt.o
CC      ../wolfssl/wolfcrypt/src/wc_port.o
CC      ../wolfssl/wolfcrypt/src/error.o
CC      ../wolfssl/wolfcrypt/src/signature.o
CC      ../wolfssl/wolfcrypt/src/hash.o
CC      ../wolfssl/wolfcrypt/src/asn.o
CC      ../wolfssl/wolfcrypt/src/aes.o
CC      ../wolfssl/wolfcrypt/src/dh.o
CC      ../wolfssl/wolfcrypt/src/md5.o
CC      ../wolfssl/wolfcrypt/src/hmac.o
CC      ../wolfssl/wolfcrypt/src/rsa.o
CC      ../wolfssl/wolfcrypt/src/sha.o
CC      ../wolfssl/wolfcrypt/src/sha256.o
CC      ../wolfssl/wolfcrypt/src/sha512.o
CC      ../wolfssl/wolfcrypt/src/curve25519.o
CC      ../wolfssl/wolfcrypt/src/ed25519.o
CC      ../wolfssl/wolfcrypt/src/ecc.o
CC      ../wolfssl/wolfcrypt/src/tfm.o
CC      ../wolfssl/wolfcrypt/src/integer.o
CC      ../wolfssl/wolfcrypt/src/fe_low_mem.o
CC      ../wolfssl/wolfcrypt/src/ge_low_mem.o
MKDIR   ../wolfssl/wolfcrypt/src/port/atmel/
CC      ../wolfssl/wolfcrypt/src/port/atmel/atmel.o
MKDIR   ../wolfssl/wolfcrypt/test/
CC      ../wolfssl/wolfcrypt/test/test.o
MKDIR   ../wolfssl/wolfcrypt/benchmark/
CC      ../wolfssl/wolfcrypt/benchmark/benchmark.o
MKDIR   ../cryptoauthlib/basic/
CC      ../cryptoauthlib/basic/atca_basic.o
CC      ../cryptoauthlib/basic/atca_helpers.o
MKDIR   ../cryptoauthlib/tls/
CC      ../cryptoauthlib/tls/atcatls.o
CC      ../cryptoauthlib/atca_iface.o
CC      ../cryptoauthlib/atca_command.o
CC      ../cryptoauthlib/atca_device.o
CC      ../cryptoauthlib/atca_cfgs.o
MKDIR   ../cryptoauthlib/host/
CC      ../cryptoauthlib/host/atca_host.o
MKDIR   ../cryptoauthlib/hal/
CC      ../cryptoauthlib/hal/atca_hal.o
CC      ../cryptoauthlib/hal/hal_samd21_i2c_asf.o
CC      ../cryptoauthlib/hal/hal_samd21_timer_asf.o
MKDIR   ../cryptoauthlib/certs/
CC      ../cryptoauthlib/certs/provision.o
CC      ../cryptoauthlib/certs/cert_def_1_signer.o
CC      ../cryptoauthlib/certs/cert_def_2_device.o
MKDIR   ../cryptoauthlib/crypto/
CC      ../cryptoauthlib/crypto/atca_crypto_sw_sha1.o
CC      ../cryptoauthlib/crypto/atca_crypto_sw_sha2.o
MKDIR   ../cryptoauthlib/crypto/hashes/
CC      ../cryptoauthlib/crypto/hashes/sha1_routines.o
CC      ../cryptoauthlib/crypto/hashes/sha2_routines.o
MKDIR   ../cryptoauthlib/atcacert/
CC      ../cryptoauthlib/atcacert/atcacert_date.o
CC      ../cryptoauthlib/atcacert/atcacert_client.o
CC      ../cryptoauthlib/atcacert/atcacert_def.o
CC      ../cryptoauthlib/atcacert/atcacert_der.o
CC      ../cryptoauthlib/atcacert/atcacert_host_hw.o
CC      ../cryptoauthlib/atcacert/atcacert_host_sw.o
LN      wolfcrypt_flash.elf
SIZE    wolfcrypt_flash.elf
wolfcrypt_flash.elf  :
section               size         addr
.text              0x1b598          0x0
.relocate            0x1b8   0x20000000
.bss                 0x984   0x200001b8
.stack              0x4004   0x20000b3c
.ARM.attributes       0x28          0x0
.comment              0x6e          0x0
.debug_info        0x400cd          0x0
.debug_abbrev       0x5ea8          0x0
.debug_aranges      0x19b0          0x0
.debug_ranges       0x16d0          0x0
.debug_macro       0x227b2          0x0
.debug_line        0x1af9d          0x0
.debug_str         0x9f17e          0x0
.debug_frame        0x6670          0x0
Total             0x14c0a0


   text	   data	    bss	    dec	    hex	filename
0x1b598	  0x1b8	 0x4988	 131288	  200d8	wolfcrypt_flash.elf
OBJDUMP wolfcrypt_flash.lss
NM      wolfcrypt_flash.sym
OBJCOPY wolfcrypt_flash.hex
OBJCOPY wolfcrypt_flash.bin
```

### Programming
Use the resulting wolfcrypt_flash.bin to program your microcontroller using JTAG.

Using edgb (see included `gcc/flash.sh` script):
`edbg -bpv -t atmel_cm0p -f ./wolfcrypt_flash.bin`

### Debugging

GDB with pipe (see included `gcc/debug.sh` script):
`arm-none-eabi-gdb wolfcrypt_flash.elf -ex 'target remote | openocd -c "gdb_port pipe;" -f ../../../../utils/openocd/atmel_samd21_xplained_pro.cfg'
load`

GDB with remote port:
`arm-none-eabi-gdb wolfcrypt_flash.elf -ex 'target remote localhost:9993'`
`openocd -c "gdb_port 9993;" -f ../../../../utils/openocd/atmel_samd21_xplained_pro.cfg`


## Benchmarks
Software only implementation (SAMD21 48Mhz Cortex-M0, Fast Math TFM-ASM):
`ECC  256 key generation  3123.000 milliseconds, avg over 5 iterations`
`EC-DHE   key agreement   3117.000 milliseconds, avg over 5 iterations`
`EC-DSA   sign   time     1997.000 milliseconds, avg over 5 iterations`
`EC-DSA   verify time     5057.000 milliseconds, avg over 5 iterations`

ATECC508A HW accelerated implementation:
`ECC  256 key generation  144.400 milliseconds, avg over 5 iterations`
`EC-DHE   key agreement   134.200 milliseconds, avg over 5 iterations`
`EC-DSA   sign   time     293.400 milliseconds, avg over 5 iterations`
`EC-DSA   verify time     208.400 milliseconds, avg over 5 iterations`

For reference the benchmarks for RNG, AES, MD5, SHA and SHA256 are:
`RNG      25 kB took 0.784 seconds,    0.031 MB/s (coming from the ATECC508A)`
`AES      25 kB took 0.177 seconds,    0.138 MB/s`
`MD5      25 kB took 0.050 seconds,    0.488 MB/s`
`SHA      25 kB took 0.141 seconds,    0.173 MB/s`
`SHA-256  25 kB took 0.352 seconds,    0.069 MB/s`