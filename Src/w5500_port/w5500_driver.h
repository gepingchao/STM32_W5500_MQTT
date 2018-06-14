/*
* File Name          : w5500_driver.h
* Description       : Í·ÎÄ¼þ
*                       
*
*
*
*
*
*/

#ifndef __W5500_DRIVER_H__
#define __W5500_DRIVER_H__

#ifdef __cplusplus
 extern "C" {
#endif
////////////////////////////////////
#include "include.h"

#define SOCKET_BUFF_SIZE 	256

typedef struct
{
	osThreadId server_handle_0;
	osThreadId server_handle_1;
	osThreadId server_handle_2;
	osThreadId server_handle_3;
	osThreadId server_handle_4;
	osThreadId server_handle_5;
	osThreadId server_handle_6;
	osThreadId server_handle_7;
	unsigned short socket_recv_num[8];
	unsigned char socket_is_runing;
}S_Tcp_Server,*P_S_Tcp_Server;

typedef struct 
{
	unsigned char server_ip[4];
	unsigned int server_port;
}S_Server_Info,*p_S_Server_Info;

typedef struct
{
	unsigned char this_client_socket_num;
	S_Server_Info target_server;
	UChar_Function_PUChar_UShort DealRecvData;
}S_Client_Info,*p_S_Client_Info;



typedef struct
{
	unsigned char my_socket_num;
	
	unsigned char target_ip[4];

	unsigned short recv_data_length;
	unsigned short recv_buf_length;
	unsigned char* recv_buf;
	
	unsigned int target_port;
	unsigned int my_port;

	
	osThreadDef_t task;
	
	UChar_Function_PUChar_UShort DealRecvData;	
}S_W5500_Socket_Info,*P_S_W5500_Socket_Info;




typedef struct
{
	unsigned char socket_space;
	S_W5500_Socket_Info _socket[8];
}S_W5500_Info,*P_S_W5500_Info;

extern wiz_NetInfo gWIZNETINFO;
extern netmode_type g_net_mode;
extern unsigned char G_socket_buf[8][SOCKET_BUFF_SIZE];


 void SPI_CrisEnter(void);
void SPI_CrisExit(void);
void SPI_WriteByte(unsigned char TxData);
unsigned char SPI_ReadByte(void);
void SPI_CS_Select(void);
void SPI_CS_Deselect(void);
void network_init(void);
void InitW5500(void);
void socket_init(void);

//unsigned char start_tcp_client(p_S_Client_Info client);
//void start_tcp_client_thread(void const* pvParameters);
void tcp_client(p_S_Client_Info client);


//unsigned char start_tcp_server(unsigned int socket_num,unsigned int port);
//void start_tcp_server_thread(void const* pvParameters);
void tcp_server(unsigned int socket_num,unsigned int port);


void close_socket_server(unsigned int socket_num);

//unsigned char start_udp_server(unsigned int socket_num,unsigned int port);

void udp_server(unsigned int socket_num,unsigned int port);



//int loopback_tcps(unsigned char sn, unsigned char* buf, unsigned short port);
//int loopback_udps(unsigned char sn, unsigned char* buf, unsigned short port);


void deal_all_socket_server(void);
void deal_each_socket_server(unsigned char socket_num);
void deal_all_socket_client(void);
void deal_each_socket_client(unsigned char socket_num);
void deal_all_socket_udp(void);
void deal_each_socket_udp(unsigned char socket_num);


////////////////////////////////////

#ifdef __cplusplus
}
#endif
#endif

