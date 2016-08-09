/**
 *
 * \file
 *
 * \brief WINC1500 TLS Example.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "asf.h"
#include "main.h"
#if defined(WOLFSSL_ATECC508A)
#include <wolfssl/wolfssl/wolfcrypt/settings.h>
#ifdef NO_WOLFSSL_CLIENT
#include "tls_server.h"
#else
#include "tls_client.h"
#endif
#endif

/** UART module for debug. */
static struct usart_module cdc_uart_module;

#if 1
/* Use EXT3 UART */
#define CONF_STDIO_USART_MODULE  EXT3_UART_MODULE
#define CONF_STDIO_MUX_SETTING   EXT3_UART_SERCOM_MUX_SETTING
#define CONF_STDIO_PINMUX_PAD0   EXT3_UART_SERCOM_PINMUX_PAD0
#define CONF_STDIO_PINMUX_PAD1   EXT3_UART_SERCOM_PINMUX_PAD1
#define CONF_STDIO_PINMUX_PAD2   EXT3_UART_SERCOM_PINMUX_PAD2
#define CONF_STDIO_PINMUX_PAD3   EXT3_UART_SERCOM_PINMUX_PAD3
#define CONF_STDIO_BAUDRATE      115200
#else
/* Use EDBG UART */
#define CONF_STDIO_USART_MODULE  EDBG_CDC_MODULE
#define CONF_STDIO_MUX_SETTING   EDBG_CDC_SERCOM_MUX_SETTING
#define CONF_STDIO_PINMUX_PAD0   EDBG_CDC_SERCOM_PINMUX_PAD0
#define CONF_STDIO_PINMUX_PAD1   EDBG_CDC_SERCOM_PINMUX_PAD1
#define CONF_STDIO_PINMUX_PAD2   EDBG_CDC_SERCOM_PINMUX_PAD2
#define CONF_STDIO_PINMUX_PAD3   EDBG_CDC_SERCOM_PINMUX_PAD3
#define CONF_STDIO_BAUDRATE      115200
#endif

/**
 * \brief Configure UART console.
 */
static void configure_console(void)
{
	struct usart_config usart_conf;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = CONF_STDIO_MUX_SETTING;
	usart_conf.pinmux_pad0 = CONF_STDIO_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = CONF_STDIO_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = CONF_STDIO_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = CONF_STDIO_PINMUX_PAD3;
	usart_conf.baudrate    = CONF_STDIO_BAUDRATE;

	stdio_serial_init(&cdc_uart_module, CONF_STDIO_USART_MODULE, &usart_conf);
	usart_enable(&cdc_uart_module);
}


void HardFault_HandlerC(uint32_t *hardfault_args);

/* Hard fault handler */
void HardFault_HandlerC(uint32_t *hardfault_args)
{
    /* These are volatile to try and prevent the compiler/linker optimizing them
    away as the variables never actually get used.  If the debugger won't show the
    values of the variables, make them global my moving their declaration outside
    of this function. */
    volatile uint32_t stacked_r0;
	volatile uint32_t stacked_r1;
	volatile uint32_t stacked_r2;
	volatile uint32_t stacked_r3;
	volatile uint32_t stacked_r12;
	volatile uint32_t stacked_lr;
    volatile uint32_t stacked_pc;
	volatile uint32_t stacked_psr;
	volatile uint32_t _CFSR;
	volatile uint32_t _HFSR;
	volatile uint32_t _DFSR;
	volatile uint32_t _AFSR;
	volatile uint32_t _BFAR;
	volatile uint32_t _MMAR;

	stacked_r0 = ((uint32_t)hardfault_args[0]);
	stacked_r1 = ((uint32_t)hardfault_args[1]);
	stacked_r2 = ((uint32_t)hardfault_args[2]);
	stacked_r3 = ((uint32_t)hardfault_args[3]);
	stacked_r12 = ((uint32_t)hardfault_args[4]);
	stacked_lr = ((uint32_t)hardfault_args[5]);
	stacked_pc = ((uint32_t)hardfault_args[6]);
	stacked_psr = ((uint32_t)hardfault_args[7]);

    // Configurable Fault Status Register
    // Consists of MMSR, BFSR and UFSR
	_CFSR = (*((volatile uint32_t *)(0xE000ED28)));

	// Hard Fault Status Register
	_HFSR = (*((volatile uint32_t *)(0xE000ED2C)));

	// Debug Fault Status Register
	_DFSR = (*((volatile uint32_t *)(0xE000ED30)));

	// Auxiliary Fault Status Register
	_AFSR = (*((volatile uint32_t *)(0xE000ED3C)));

	// Read the Fault Address Registers. These may not contain valid values.
	// Check BFARVALID/MMARVALID to see if they are valid values
	// MemManage Fault Address Register
	_MMAR = (*((volatile uint32_t *)(0xE000ED34)));
	// Bus Fault Address Register
	_BFAR = (*((volatile uint32_t *)(0xE000ED38)));

    printf ("\n\nHard fault handler (all numbers in hex):\n");
    printf ("R0 = %x\n", (unsigned int)stacked_r0);
    printf ("R1 = %x\n", (unsigned int)stacked_r1);
    printf ("R2 = %x\n", (unsigned int)stacked_r2);
    printf ("R3 = %x\n", (unsigned int)stacked_r3);
    printf ("R12 = %x\n", (unsigned int)stacked_r12);
    printf ("LR [R14] = %x  subroutine call return address\n", (unsigned int)stacked_lr);
    printf ("PC [R15] = %x  program counter\n", (unsigned int)stacked_pc);
    printf ("PSR = %x\n", (unsigned int)stacked_psr);
    printf ("CFSR = %x\n", (unsigned int)_CFSR);
    printf ("HFSR = %x\n", (unsigned int)_HFSR);
    printf ("DFSR = %x\n", (unsigned int)_DFSR);
    printf ("AFSR = %x\n", (unsigned int)_AFSR);
    printf ("MMAR = %x\n", (unsigned int)_MMAR);
    printf ("BFAR = %x\n", (unsigned int)_BFAR);

    // Break into the debugger
	__asm("BKPT #0\n");
}

__attribute__( ( naked ) )
void HardFault_Handler(void)
{
	__asm(
		"  mov r0, #4          \n"
		"  mov r1, lr          \n"
		"  tst r0, r1          \n"
		"  beq using_msp       \n"
		"  mrs r0, psp         \n"
		"  b call_c            \n"
		"using_msp:            \n"
		"  mrs r0, msp         \n"
		"call_c:               \n"
		"  ldr r2, =HardFault_HandlerC \n"
		"  bx r2               \n"
	);
}


/**
 * \brief Main application function.
 *
 * Initialize system, UART console then start cipher tests or server or client application of TLS.
 *
 * \return program return value.
 */
int main(void)
{
	/* Initialize the board. */
	system_init();

    /* Initialize the UART console. */
	configure_console();

#if defined(NO_WOLFSSL_CLIENT)
	/* Start server thread for tls. */
	tls_start_server();
#elif defined(NO_WOLFSSL_SERVER)
	/* Start client thread for tls. */
	tls_start_client();
#else
	/* Nothing to do. */
#endif

	return 0;
}

