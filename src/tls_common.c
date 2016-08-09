/**
 *
 * \file
 *
 * \brief WINC1500 TLS Client Example.
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

#ifdef __cplusplus
extern "C" {
#endif

#include "asf.h"
#include "tls_common.h"
#include "cryptoauthlib.h"
#include <stdio.h>

uint16_t tls_socket_status = 0x0000;
uint16_t tls_ntp_socket_status = 0x0000;

static uint8_t wifi_connected = 0;
static SOCKET tls_ntp_socket = -1;
t_time_date curr_time_date;

int _gettimeofday(struct timeval *tv, void *tzvp)
{
    return 0;
}

/**
 * \brief Return string occurred in socket callback  .
 */
const char* tls_get_socket_string(int callback_status)
{
    int status = callback_status;

    if (status < SOCKET_MSG_BIND && status > SOCKET_MSG_RECVFROM) {
        return "UNKNOWN socket event";
    }

    switch (status) {

    case SOCKET_MSG_BIND :
        return "BIND socket event";

    case SOCKET_MSG_LISTEN :
        return "LISTEN socket event";

    case SOCKET_MSG_DNS_RESOLVE :
        return "RESOLVE socket event";

    case SOCKET_MSG_ACCEPT :
        return "ACCEPT socket event";

    case SOCKET_MSG_CONNECT :
        return "CONNECT socket event";

    case SOCKET_MSG_RECV :
        return "RECV socket event";

    case SOCKET_MSG_SEND :
        return "SEND socket event";

    case SOCKET_MSG_SENDTO :
        return "SENDTO socket event";

    case SOCKET_MSG_RECVFROM :
        return "RECVFROM socket event";		

    default :
        return "UNKNOWN socket event";		
    }
}

/**
 * \brief Set WIFI status.
 */
void tls_set_wifi_status(int status)
{
    wifi_connected = status;
}

/**
 * \brief Get WIFI status.
 */
int tls_get_wifi_status(void)
{
    return wifi_connected;
}

/**
 * \brief WIFI callback to be interrupted.
 */
void tls_wifi_callback(uint8_t u8MsgType, void *pvMsg)
{
	switch (u8MsgType) {
		case M2M_WIFI_RESP_CON_STATE_CHANGED:
		{
			tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
            
			if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {
				printf("M2M_WIFI_RESP_CON_STATE_CHANGED: CONNECTED\r\n");
			} else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {
				printf("M2M_WIFI_RESP_CON_STATE_CHANGED: DISCONNECTED\r\n");
				tls_set_wifi_status(M2M_WIFI_DISCONNECTED);
				m2m_wifi_connect((char *)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID), 
					MAIN_WLAN_AUTH, (char *)MAIN_WLAN_PSK, M2M_WIFI_CH_ALL);
			}
			break;
		}

		case M2M_WIFI_REQ_DHCP_CONF:
		{
			uint8_t *pu8IPAddress = (uint8_t *)pvMsg;
			tls_set_wifi_status(M2M_WIFI_CONNECTED);
			printf("M2M_WIFI_REQ_DHCP_CONF: IP is %u.%u.%u.%u\r\n",
					pu8IPAddress[0], pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3]);
			gethostbyname((uint8_t *)MAIN_WORLDWIDE_NTP_POOL_HOSTNAME);
			break;
		}

		default:
		{
			break;
		}
	}
}

/**
 * \brief Set socket to access to the NTP server.
 */
void tls_set_ntp_socket(SOCKET socket)
{
    tls_ntp_socket = socket;    
}

/**
 * \brief Get socket to access to the NTP server.
 */
SOCKET tls_get_ntp_socket(void)
{
    return tls_ntp_socket;
}

/**
 * \brief Set local time and date that came from the NTP server.
 */
int tls_set_curr_time_and_date(uint32_t secsSince1900)
{
    #define YEAR0          1900
    #define EPOCH_YEAR     1970
    #define SECS_DAY       (24L * 60L * 60L)
    #define LEAPYEAR(year) (!((year) % 4) && (((year) % 100) || !((year) %400)))
    #define YEARSIZE(year) (LEAPYEAR(year) ? 366 : 365)

    int ret = 0;
    time_t secs = secsSince1900;
    unsigned long dayclock, dayno;
    int year = EPOCH_YEAR;
    static const int _ytab[2][12] =
    {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
    
    dayclock = (unsigned long)secs % SECS_DAY;
    dayno    = (unsigned long)secs / SECS_DAY;

    curr_time_date.tm_sec  = (int) dayclock % 60;
    curr_time_date.tm_min  = (int)(dayclock % 3600) / 60;
    curr_time_date.tm_hour = (int) dayclock / 3600;
    curr_time_date.tm_wday = (int) (dayno + 4) % 7;        /* day 0 a Thursday */

    while(dayno >= (unsigned long)YEARSIZE(year)) {
        dayno -= YEARSIZE(year);
        year++;
    }

    curr_time_date.tm_year = year - YEAR0;
    curr_time_date.tm_yday = (int)dayno;
    curr_time_date.tm_mon  = 0;

    while(dayno >= (unsigned long)_ytab[LEAPYEAR(year)][curr_time_date.tm_mon]) {
        dayno -= _ytab[LEAPYEAR(year)][curr_time_date.tm_mon];
        curr_time_date.tm_mon++;
    }

    curr_time_date.tm_mday  = (int)++dayno;
    curr_time_date.tm_isdst = 0;

    curr_time_date.tm_year += 1900;
    curr_time_date.tm_mon += 1;
	
	printf("Date of Today : %d / %d / %d\r\n", curr_time_date.tm_year, curr_time_date.tm_mon, curr_time_date.tm_mday);
    return ret;

}

/**
 * \brief Copy received time and date to input param.
 */
void tls_get_curr_time_and_date(t_time_date* tm)
{

    tm->tm_year = curr_time_date.tm_year;
    tm->tm_mon = curr_time_date.tm_mon;
    tm->tm_mday = curr_time_date.tm_mday;
    tm->tm_hour = curr_time_date.tm_hour;
    tm->tm_min = curr_time_date.tm_min;
    tm->tm_sec = curr_time_date.tm_sec;

}

/**
 * \brief Access to the NTP server.
 */
int tls_get_ntp_time_and_date(void)
{
	int ret = 0;
	SOCKET ntp_socket = -1;
	struct sockaddr_in addr;
	
	ntp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (ntp_socket < 0) {
		printf("main: UDP Client Socket Creation Failed.\r\n");
		return -1;
	} else {
		tls_set_ntp_socket(ntp_socket);
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = _htonl(0xFFFFFFFF);
	addr.sin_port = _htons(6666);
	if (bind((SOCKET)tls_get_ntp_socket(), (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) != 0) {
		printf("binding failed.\r\n");
		close(tls_get_ntp_socket);
		return -1;        
	}

	while (!GET_NTP_SOCKET_STATUS(NTP_SOCKET_STATUS_RECEIVE_FROM)) {
		m2m_wifi_handle_events(NULL);
	}

	return ret;    
}

/**
 * \brief Callback to get the Data from socket.
 *
 * \param[in] sock socket handler.
 * \param[in] u8Msg Type of Socket notification.
 * \param[in] pvMsg A structure contains notification informations.
 */
void tls_ntp_socket_cb(SOCKET sock, uint8_t u8Msg, void *pvMsg)
{
	/* Check for socket event on socket. */
	int16_t ret;
  
	switch (u8Msg) {
	case SOCKET_MSG_BIND:
	{
		/* printf("socket_cb: socket_msg_bind!\r\n"); */
		tstrSocketBindMsg *pstrBind = (tstrSocketBindMsg *)pvMsg;
		if (pstrBind && pstrBind->status == 0) {
			ENABLE_NTP_SOCKET_STATUS(NTP_SOCKET_STATUS_BIND);
			ret = recvfrom(sock, gTlsSocketBuf/*gNtpServerSocBuf*/, sizeof(gTlsSocketBuf/*gNtpServerSocBuf*/), 0);
			if (ret != SOCK_ERR_NO_ERROR) {
				printf("socket_cb: recv error!\r\n");
			}
		} else {
		    DISABLE_NTP_SOCKET_STATUS(NTP_SOCKET_STATUS_BIND);
			printf("socket_cb: bind error!\r\n");
		}

		break;
	}

	case SOCKET_MSG_RECVFROM:
	{
        unsigned int years = 0, year = 0, y = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
        static unsigned short days[4][12] =
        {
            {   0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335},
            { 366, 397, 425, 456, 486, 517, 547, 578, 609, 639, 670, 700},
            { 731, 762, 790, 821, 851, 882, 912, 943, 974,1004,1035,1065},
            {1096,1127,1155,1186,1216,1247,1277,1308,1339,1369,1400,1430},
        };
        
		tstrSocketRecvMsg *pstrRx = (tstrSocketRecvMsg *)pvMsg;
		if (pstrRx->pu8Buffer && pstrRx->s16BufferSize) {
			uint8_t packetBuffer[48];
			memcpy(&packetBuffer, pstrRx->pu8Buffer, sizeof(packetBuffer));

   			ENABLE_NTP_SOCKET_STATUS(NTP_SOCKET_STATUS_RECEIVE_FROM);
			if ((packetBuffer[0] & 0x7) != 4) {                   /* expect only server response */
				printf("socket_cb: Expecting response from Server Only!\r\n");
				return;                    /* MODE is not server, abort */
			} else {
				uint32_t secsSince1900 = packetBuffer[40] << 24 |
						packetBuffer[41] << 16 |
						packetBuffer[42] << 8 |
						packetBuffer[43];

				/* Now convert NTP time into everyday time.
				 * Unix time starts on Jan 1 1970. In seconds, that's 2208988800.
				 * Subtract seventy years.
				 */
				const uint32_t seventyYears = 2208988800UL;
				uint32_t epoch = secsSince1900 - seventyYears;
				/* Print the hour, minute and second.
				 * GMT is the time at Greenwich Meridian.
				 */
				tls_set_curr_time_and_date(epoch);

				ret = close(sock);

			}
		} else {
            DISABLE_NTP_SOCKET_STATUS(NTP_SOCKET_STATUS_RECEIVE_FROM);
		}

	}
	break;

	default:
		break;
	}
}

/**
 * \brief Ask date and time to the NTP server.
 */
void tls_ntp_resolve_cb(uint8_t *pu8DomainName, uint32_t u32ServerIP)
{
	struct sockaddr_in addr;
	int8_t cDataBuf[48];
	int16_t ret;

	memset(cDataBuf, 0, sizeof(cDataBuf));
	cDataBuf[0] = '\x1b'; /* time query */


	if (tls_get_ntp_socket() >= 0) {
		/* Set NTP server socket address structure. */
		addr.sin_family = AF_INET;
		addr.sin_port = _htons(MAIN_SERVER_PORT_FOR_UDP);
		addr.sin_addr.s_addr = u32ServerIP;

		/*Send an NTP time query to the NTP server*/
		ret = sendto((SOCKET)tls_get_ntp_socket(), (int8_t *)&cDataBuf, sizeof(cDataBuf), 0, (struct sockaddr *)&addr, sizeof(addr));
		if (ret != M2M_SUCCESS) {
			printf("resolve_cb: failed to send  error!\r\n");
			return;
		}
	}
}

/**
 * \brief Compare today's date and certificate's date.
 */
int tls_compare_date(t_time_date *local, atcacert_tm_utc_t *cert)
{
	uint8_t ret = 0;
	
    if (local->tm_year > cert->tm_year)
        return 1;

    if (local->tm_year == cert->tm_year && local->tm_mon > cert->tm_mon)
        return 1;
    
    if (local->tm_year == cert->tm_year && local->tm_mon == cert->tm_mon && local->tm_mday > cert->tm_mday)
        return 1;

	return ret;
}

int tls_send_packet_cb(WOLFSSL* ssl, char *buf, int sz, void *ctx)
{
    SockCbInfo* info = (SockCbInfo*)ctx;
    int sent = 0;

    sent = (int)send(info->sd, buf, sz, ssl->wflags);

	while (!GET_SOCKET_STATUS(SOCKET_STATUS_SEND)) {
		m2m_wifi_handle_events(NULL);
	}

	DISABLE_SOCKET_STATUS(SOCKET_STATUS_SEND);

    if (sent < 0) {
		printf("Failed to send packet\r\n");
		return -1;
    }
    atcab_printbin_label((const uint8_t*)"\r\nSENT PACKET", buf, sent);

    return sent;
}

int tls_receive_packet_cb(WOLFSSL *ssl, char *buf, int sz, void *ctx)
{
    SockCbInfo* info = (SockCbInfo*)ctx;
    int recvd = 0;

    /* If nothing in the buffer then do read */
    if (info->bufRemain <= 0) {
	    recvd = (int)recv(info->sd, gTlsSocketBuf, sizeof(gTlsSocketBuf), ssl->rflags);
        info->bufRemain = recvd;
        info->bufPos = 0;

    	while (!GET_SOCKET_STATUS(SOCKET_STATUS_RECEIVE)) {
    		m2m_wifi_handle_events(NULL);
    	}
	
    	DISABLE_SOCKET_STATUS(SOCKET_STATUS_RECEIVE);
        
        if (recvd < 0) {
            printf("Failed to receive packet\r\n");
    		return -1;
        }
        else if (recvd == 0) {
            printf("Failed to receive packet, Connection closed\r\n");
            return -1;
        }
	}
    else {
        recvd = info->bufRemain;
    }

	if (sz > recvd) {
	    sz = recvd;
	}
    
    memcpy(buf, &gTlsSocketBuf[info->bufPos], sz);
    info->bufPos += sz;
    info->bufRemain -= sz;
    
    atcab_printbin_label((const uint8_t*)"\r\nRECEIVED PACKET", buf, sz);

    return sz;
}	


#ifdef __cplusplus
}
#endif
