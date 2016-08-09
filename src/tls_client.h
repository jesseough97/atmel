/**
 * \file
 *
 * \brief MAIN configuration.
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

#ifndef TLS_CLIENT_H_INCLUDED
#define TLS_CLIENT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <wolfssl/wolfcrypt/settings.h>
#include "tls_common.h"

#define STRING_EOL    	"\r\n"
#define STRING_HEADER 	"-- TLS starts with ATECC508A over WINC module --"STRING_EOL
#define FAILED_MSG		"-- Failed to carry out tls client application, Check error logs --"STRING_EOL
#define SUCCEED_MSG		"-- TLS client securely communicated with following cipher suite --"STRING_EOL \
						"-- TLS-ECDH-ECDSA-AES128-GCM-SHA256 --"STRING_EOL

/** Server address Settings */
#define TLS_SERVER_IP		"192.168.1.241"//	/**< Destination IP */
#define TLS_SERVER_PORT		11111				/**< Destination PORT */

/* Cipher suite for client */
#define CLIENT_CIPHER_LIST	"ECDH-ECDSA-AES128-GCM-SHA256"

/** Function prototype for TLS client. */
SOCKET tls_access_server(const char* ip, uint16 port);
void tls_client_socket_cb(SOCKET sock, uint8_t u8Msg, void *pvMsg);
int tls_send_message(void);
int tls_receive_message(void);
int tls_start_handshake(void);
int tls_load_wolfssl_objects(void);
int tls_release_objects(void);
void tls_start_client(void);

#ifdef __cplusplus
}
#endif

#endif /* TLS_CLIENT_H_INCLUDED */
