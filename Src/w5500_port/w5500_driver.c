#include "include.h"


#define DATA_BUF_SIZE 300
#define PC4_1 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET)
#define PC5_1 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET)


//unsigned char G_socket_buf[8][SOCKET_BUFF_SIZE];

wiz_NetInfo gWIZNETINFO;
netmode_type g_net_mode;
unsigned char G_socket_buf[8][SOCKET_BUFF_SIZE];
S_Tcp_Server Socket_Server;

extern S_SYS_Task_Info SYS_Task_Info;
extern osMessageQId mqtt_queueHandle;

void InitW5500(void)
{
	PC4_1;
	PC5_1;
	osDelay(20);
	reg_wizchip_cris_cbfunc(SPI_CrisEnter, SPI_CrisExit);  
	reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect);
	reg_wizchip_spi_cbfunc(SPI_ReadByte, SPI_WriteByte); 
	//test_spi1();
	network_init();
}

 void SPI_CrisEnter(void)
{
    __set_PRIMASK(1);
}
/**
  * @brief  退出临界区
  * @retval None
  */
void SPI_CrisExit(void)
{
    __set_PRIMASK(0);
}

void SPI_WriteByte(unsigned char TxData)
{      
	unsigned char sendbuf;
	sendbuf = TxData;
	HAL_StatusTypeDef res;	
	res = HAL_SPI_Transmit(&hspi1,&sendbuf,1,0xff);
	switch(res)
		{
			case HAL_OK :
				//printf("send ok \r\n");
				break;
			case HAL_ERROR :
				printf("send HAL_ERROR\r\n");
				break;
			case HAL_BUSY :
				printf("send HAL_BUSY\r\n");
				break;
			case HAL_TIMEOUT :
				printf("send HAL_TIMEOUT\r\n");
				break;
		}
	/* while((SPI1->SR&SPI_SR_TXE)==0);   //等待发送区空          
	SPI1->DR=TxData;                                         //发送一个byte 
	while((SPI1->SR&SPI_SR_RXNE)==0); //等待接收完一个byte  
	SPI1->DR;  */
	//SPI_SR_TXE
	//SPI_SR_RXNE

}

unsigned char SPI_ReadByte(void)
{
	unsigned char recvbuf;
	HAL_StatusTypeDef res;	
	res =HAL_SPI_Receive(&hspi1,&recvbuf,1,0xff);
	switch(res)
		{
			case HAL_OK :
				//printf("recv ok \r\n");
				break;
			case HAL_ERROR :
				printf("recv HAL_ERROR\r\n");
				break;
			case HAL_BUSY :
				printf("recv HAL_BUSY\r\n");
				break;
			case HAL_TIMEOUT :
				printf("recv HAL_TIMEOUT\r\n");
				break;
		}
	return recvbuf;
    /*while((SPI1->SR&SPI_SR_TXE)==0);   //等待发送区空              
    SPI1->DR=0xFF;                                               //发送一个空数据产生输入数据的时钟 
    while((SPI1->SR&SPI_SR_RXNE)==0); //等待接收完一个byte  
    return SPI1->DR;    */
    
}

void SPI_CS_Select(void)
{
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_RESET);
	//printf("Select\r\n");
}

void SPI_CS_Deselect(void)
{
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_SET);
	//printf("Deselect\r\n");
}
void network_init(void)
{
	gWIZNETINFO.mac[0] = 1;
	static unsigned char tmpstr[6];
	gWIZNETINFO.mac[0] = 0x12;
	gWIZNETINFO.mac[1] = 0xa2;
	gWIZNETINFO.mac[2] = 0x11;
	gWIZNETINFO.mac[3] = 0x22;
	gWIZNETINFO.mac[4] = 0x33;
	gWIZNETINFO.mac[5] = 0x41;
	gWIZNETINFO.ip[0] = 192;
	gWIZNETINFO.ip[1] = 168;
	gWIZNETINFO.ip[2] = 1;
	gWIZNETINFO.ip[3] = 234;
	gWIZNETINFO.sn[0] = 255;
	gWIZNETINFO.sn[1] = 255;
	gWIZNETINFO.sn[2] = 255;
	gWIZNETINFO.sn[3] = 0;
	gWIZNETINFO.gw[0] = 192;
	gWIZNETINFO.gw[1] = 168;
	gWIZNETINFO.gw[2] = 1;
	gWIZNETINFO.gw[3] = 1;
	gWIZNETINFO.dns[0] = 8;
	gWIZNETINFO.dns[1] = 8;
	gWIZNETINFO.dns[2] = 8;
	gWIZNETINFO.dns[3] = 8;
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);

	ctlnetwork(CN_GET_NETMODE, (void*)&g_net_mode);
	
    // Display Network Information
    ctlwizchip(CW_GET_ID,(void*)tmpstr);
    printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
    printf("mac: %02X:%02X:%02X:%02X:%02X:%02X\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],
          gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
    printf("cip: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
    printf("gtw: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
    printf("msk: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
    printf("dns: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
    printf("======================\r\n");
	//socket_init();
}

void socket_init(void)
{
	printf ("return = %d\r\n",socket(0,Sn_MR_TCP,9999,SF_IGMP_VER2));
}

int loopback_tcps(unsigned char sn, unsigned char* buf, unsigned short port)
{
   int ret;
   unsigned short size = 0, sentsize=0;
   switch(getSn_SR(sn))
   {
      case SOCK_ESTABLISHED :
         if(getSn_IR(sn) & Sn_IR_CON)
         {
            printf("%d:Connected\r\n",sn);
            setSn_IR(sn,Sn_IR_CON);
         }
         if((size = getSn_RX_RSR(sn)) > 0)
         {
            if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
            ret = recv(sn,buf,size);
            if(ret <= 0) return ret;
            sentsize = 0;
            while(size != sentsize)
            {
               ret = send(sn,buf+sentsize,size-sentsize);
               if(ret < 0)
               {
                  close(sn);
                  return ret;
               }
               sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
            }
         }
         break;
      case SOCK_CLOSE_WAIT :
         printf("%d:CloseWait\r\n",sn);
         if((ret=disconnect(sn)) != SOCK_OK) return ret;
         printf("%d:Closed\r\n",sn);
         break;
      case SOCK_INIT :
          printf("%d:Listen, port [%d]\r\n",sn, port);
         if( (ret = listen(sn)) != SOCK_OK) return ret;
         break;
      case SOCK_CLOSED:
         printf("%d:LBTStart\r\n",sn);
         if((ret=socket(sn,Sn_MR_TCP,port,0x00)) != sn)
            return ret;
         printf("%d:Opened\r\n",sn);
         break;
      default:
         break;
   }
   return 1;
}

int loopback_udps(unsigned char sn, unsigned char* buf, unsigned short port)
{
   int  ret;
   unsigned short size, sentsize;
   unsigned char  destip[4];
   unsigned short destport;
   //unsigned char  packinfo = 0;
   switch(getSn_SR(sn))
   {
      case SOCK_UDP :
         if((size = getSn_RX_RSR(sn)) > 0)
         {
            if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
            ret = recvfrom(sn,buf,size,destip,(unsigned short*)&destport);
            if(ret <= 0)
            {
               printf("%d: recvfrom error. %ld\r\n",sn,ret);
               return ret;
            }
            size = (unsigned short) ret;
            sentsize = 0;
            while(sentsize != size)
            {
               ret = sendto(sn,buf+sentsize,size-sentsize,destip,destport);
               if(ret < 0)
               {
                 printf("%d: sendto error. %ld\r\n",sn,ret);
                  return ret;
               }
               sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
            }
         }
         break;
      case SOCK_CLOSED:
         printf("%d:LBUStart\r\n",sn);
         if((ret=socket(sn,Sn_MR_UDP,port,0x00)) != sn)
            return ret;
         printf("%d:Opened, port [%d]\r\n",sn, port);
         break;
      default :
         break;
   }
   return 1;
}



unsigned char start_tcp_server(unsigned int socket_num,unsigned int port)
{
	volatile unsigned char socket_status;
	volatile unsigned int num;
	volatile unsigned int server_port;
	num = socket_num;
	server_port = port;
	save_task_info();
	while(1)
		{
			osDelay(10);
			socket_status = getSn_SR(num);

			switch(socket_status)
				{
					case SOCK_INIT:
						listen(num);//初始化完成就开始监听
						
						break;
					case SOCK_ESTABLISHED://建立连接
						if(getSn_SR(num)&Sn_IR_CON)
							{
								setSn_IR(num,Sn_IR_CON);
							}
						Socket_Server.socket_recv_num[num]= recv(num,G_socket_buf[num],SOCKET_BUFF_SIZE) ;
						if(Socket_Server.socket_recv_num[num] > 0)
							{
								send(num,G_socket_buf[num],Socket_Server.socket_recv_num[num]) ;//收到数据
								deal_all_socket_server();
								deal_each_socket_server(num);
								Socket_Server.socket_recv_num[num] = 0;
							}
						break;
					case SOCK_CLOSE_WAIT://socket等待关闭
						if(disconnect(num) != SOCK_OK)
							{
								break;
							}
						printf("socket %d closed!\r\n",socket_num);
					case SOCK_CLOSED://socket关闭
						if(socket(num,Sn_MR_TCP,port,Sn_MR_ND) != num)
							{
								printf("socket error\r\n");
								while(1);
							}
				}
		}
}


void start_tcp_server_thread(void const* pvParameters)
{
	unsigned char socket_num;
	unsigned int port;
	port =  (unsigned int)pvParameters & 0X00FFFFFF;
	socket_num = ((unsigned int)pvParameters >> 24) & 0X000000FF;
	start_tcp_server(socket_num,port);
}

/*void start_tcp_server_thread_1(void const* pvParameters)
{
	unsigned char socket_num;
	socket_num = (unsigned int)pvParameters;
	start_tcp_server(socket_num);
}*/

void tcp_server(unsigned int socket_num,unsigned int port)
{
	close(socket_num);
	unsigned char loopx;
	unsigned int thread_parameter;
	thread_parameter = 0;
	thread_parameter = socket_num;
	thread_parameter <<= 24;
	port &= 0X00FFFFFF;
	thread_parameter |= port;
	if(socket_num == 0)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_0)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_S_socket_0, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_0 = osThreadCreate(osThread(TCP_S_socket_0), (void*)thread_parameter);
		}
	if(socket_num == 1)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_1)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_S_socket_1, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_1 = osThreadCreate(osThread(TCP_S_socket_1), (void*)thread_parameter);

		}
	if(socket_num == 2)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_2)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_S_socket_2, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_2 = osThreadCreate(osThread(TCP_S_socket_2), (void*)thread_parameter);
		}
	if(socket_num == 3)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_3)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_S_socket_3, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_3 = osThreadCreate(osThread(TCP_S_socket_3), (void*)thread_parameter);
		}
	if(socket_num == 4)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_4)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_S_socket_4, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_4 = osThreadCreate(osThread(TCP_S_socket_4), (void*)thread_parameter);
		}
	if(socket_num == 5)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_5)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_S_socket_5, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_5 = osThreadCreate(osThread(TCP_S_socket_5), (void*)thread_parameter);
		}
	if(socket_num == 6)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_6)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_S_socket_6, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_6 = osThreadCreate(osThread(TCP_S_socket_6), (void*)thread_parameter);
		}
	if(socket_num == 7)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_7)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_S_socket_7, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_7 = osThreadCreate(osThread(TCP_S_socket_7), (void*)thread_parameter);
		}
}

void close_socket_server(unsigned int socket_num)
{
	switch(socket_num)
		{
			case 0:
				vTaskDelete(Socket_Server.server_handle_0);
				delete_task_info(Socket_Server.server_handle_0);
				Socket_Server.server_handle_0 = NULL;
				break;
			case 1:
				vTaskDelete(Socket_Server.server_handle_1);
				delete_task_info(Socket_Server.server_handle_1);
				Socket_Server.server_handle_1 = NULL;
				break;
			case 2:
				vTaskDelete(Socket_Server.server_handle_2);
				delete_task_info(Socket_Server.server_handle_2);
				Socket_Server.server_handle_2= NULL;
				break;
			case 3:
				vTaskDelete(Socket_Server.server_handle_3);
				delete_task_info(Socket_Server.server_handle_3);
				Socket_Server.server_handle_3 = NULL;
				break;
			case 4:
				vTaskDelete(Socket_Server.server_handle_4);
				delete_task_info(Socket_Server.server_handle_4);
				Socket_Server.server_handle_4= NULL;
				break;
			case 5:
				vTaskDelete(Socket_Server.server_handle_5);
				delete_task_info(Socket_Server.server_handle_5);
				Socket_Server.server_handle_5 = NULL;
				break;
			case 6:
				vTaskDelete(Socket_Server.server_handle_6);
				delete_task_info(Socket_Server.server_handle_6);
				Socket_Server.server_handle_6 = NULL;
				break;
			case 7:
				vTaskDelete(Socket_Server.server_handle_7);
				delete_task_info(Socket_Server.server_handle_7);
				Socket_Server.server_handle_7 = NULL;
				break;
			default:
				break;
		}		
}




unsigned char start_tcp_client(p_S_Client_Info client)
{
	volatile unsigned char socket_status =0 ;
	volatile unsigned int num;
	num = client->this_client_socket_num;
	
	save_task_info();
	
	close(num);	
	//unsigned char server_ip[4] = {192,168,1,214};	
	//unsigned int server_port;
	while(1)
		{
			osDelay(10);
			socket_status = getSn_SR(num);

			switch(socket_status)
				{
					case SOCK_INIT:
						//listen(num);//初始化完成就开始尝试连接
						connect(num,client->target_server.server_ip,client->target_server.server_port);						
						break;
					case SOCK_ESTABLISHED://建立连接
						if(getSn_SR(num)&Sn_IR_CON)
							{
								setSn_IR(num,Sn_IR_CON);
							}
						Socket_Server.socket_recv_num[num] = recv(num,G_socket_buf[num],SOCKET_BUFF_SIZE) ;
						if(Socket_Server.socket_recv_num[num] > 0)
							{
								//send(num,G_socket_buf[num],Socket_Server.socket_recv_num[num]) ;//收到数据
								deal_all_socket_client();
								deal_each_socket_client(num);
								Socket_Server.socket_recv_num[num] = 0;
							}
						break;
					case SOCK_CLOSE_WAIT://socket等待关闭
						if(disconnect(num) != SOCK_OK)
							{
								break;
							}
						printf("socket %d closed!\r\n",num);
					case SOCK_CLOSED://socket关闭
						if(socket(num,Sn_MR_TCP,8888+num,Sn_MR_ND) != num)
							{
								printf("socket error\r\n");
							}
				}
		}
}


void start_tcp_client_thread(void const* pvParameters)
{
	S_Client_Info client;
	client.this_client_socket_num = ((p_S_Client_Info)pvParameters)->this_client_socket_num;
	client.target_server.server_ip[0]= ((p_S_Client_Info)pvParameters)->target_server.server_ip[0];
	client.target_server.server_ip[1]= ((p_S_Client_Info)pvParameters)->target_server.server_ip[1];
	client.target_server.server_ip[2]= ((p_S_Client_Info)pvParameters)->target_server.server_ip[2];
	client.target_server.server_ip[3]= ((p_S_Client_Info)pvParameters)->target_server.server_ip[3];
	client.target_server.server_port = ((p_S_Client_Info)pvParameters)->target_server.server_port;
	start_tcp_client(&client);
}

void tcp_client(p_S_Client_Info client)//调用这个函数会产生一个任务为相应的sock作为client服务
{
	unsigned char loopx;
	void* thread_parameter;
	thread_parameter = (void*) client;
	if(client->this_client_socket_num == 0)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_0)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_C_socket_0, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_0 = osThreadCreate(osThread(TCP_C_socket_0), thread_parameter);
		}
	if(client->this_client_socket_num == 1)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_1)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_C_socket_1, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_1 = osThreadCreate(osThread(TCP_C_socket_1), thread_parameter);
		}
	if(client->this_client_socket_num == 2)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_2)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_C_socket_2, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_2 = osThreadCreate(osThread(TCP_C_socket_2), thread_parameter);
		}
	if(client->this_client_socket_num == 3)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_3)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_C_socket_3, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_3 = osThreadCreate(osThread(TCP_C_socket_3), thread_parameter);
		}
	if(client->this_client_socket_num == 4)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_4)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_C_socket_4, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_4 = osThreadCreate(osThread(TCP_C_socket_4), thread_parameter);
		}
	if(client->this_client_socket_num == 5)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_5)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_C_socket_5, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_5 = osThreadCreate(osThread(TCP_C_socket_5), thread_parameter);
		}
	if(client->this_client_socket_num == 6)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_6)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_C_socket_6, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_6 = osThreadCreate(osThread(TCP_C_socket_6), thread_parameter);
		}

	if(client->this_client_socket_num == 7)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_7)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(TCP_C_socket_7, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_7 = osThreadCreate(osThread(TCP_C_socket_7), thread_parameter);
		}

}



unsigned char start_udp_server(unsigned int socket_num,unsigned int port)
{

	int ret;
	unsigned short size, sentsize;
	unsigned char  destip[4];
	unsigned short destport;
	//unsigned char  packinfo = 0;
	
	//volatile unsigned char socket_status;
	volatile unsigned int num;
	//volatile unsigned int server_port;
	num = socket_num;
	//server_port = port;
	save_task_info();
	while(1)
		{
			osDelay(10);
			switch(getSn_SR(socket_num))
				{
				case SOCK_UDP :
					if((size = getSn_RX_RSR(socket_num)) > 0)
					{
						if(size > SOCKET_BUFF_SIZE) size = SOCKET_BUFF_SIZE;
						ret = recvfrom(socket_num,G_socket_buf[num],size,destip,(unsigned short*)&destport);
						if(ret <= 0)
						{
							printf("%d: recvfrom error. %ld\r\n",socket_num,ret);
						}
						size = (unsigned short) ret;
						sentsize = 0;
						
						deal_all_socket_udp();//收到数据处理
						deal_each_socket_udp(socket_num);
						
						while(sentsize != size)
							{
								ret = sendto(socket_num,G_socket_buf[num]+sentsize,size-sentsize,destip,destport);
								if(ret < 0)
									{
										printf("%d: sendto error. %ld\r\n",socket_num,ret);
									}
								sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
							}
					}
					break;
				case SOCK_CLOSED:
					//printf("\r\n%d:LBUStart\r\n",socket_num);
					if((ret=socket(socket_num,Sn_MR_UDP,port,0x00)) != socket_num)//单播广播同一网段内发送广播地址就可以收到likes:192.168.1.255:8800
						{
							printf("%d:Opened error\r\n",socket_num);
						}
					else
						{
							printf("%d:Opened (udp) , port [%d]\r\n",socket_num, port);
						}
					break;
				default :
				break;
				}
		}
}

void start_udp_server_thread(void const* pvParameters)
{
	unsigned char socket_num;
	unsigned int port;
	port =  (unsigned int)pvParameters & 0X00FFFFFF;
	socket_num = ((unsigned int)pvParameters >> 24) & 0X000000FF;
	start_udp_server(socket_num,port);
}


void udp_server(unsigned int socket_num,unsigned int port)
{
	close(socket_num);
	unsigned char loopx;
	unsigned int thread_parameter;
	thread_parameter = 0;
	thread_parameter = socket_num;
	thread_parameter <<= 24;
	port &= 0X00FFFFFF;
	thread_parameter |= port;
	if(socket_num == 0)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_0)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(UDP_socket_0, start_udp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_0 = osThreadCreate(osThread(UDP_socket_0), (void*)thread_parameter);
		}

	if(socket_num == 1)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_1)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(UDP_socket_1, start_udp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_1 = osThreadCreate(osThread(UDP_socket_1), (void*)thread_parameter);
		}

	if(socket_num == 2)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_2)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(UDP_socket_2, start_udp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_2 = osThreadCreate(osThread(UDP_socket_2), (void*)thread_parameter);
		}

	if(socket_num == 3)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_3)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(UDP_socket_3, start_udp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_3 = osThreadCreate(osThread(UDP_socket_3), (void*)thread_parameter);
		}

	if(socket_num == 4)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_4)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(UDP_socket_4, start_udp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_4= osThreadCreate(osThread(UDP_socket_4), (void*)thread_parameter);
		}


	if(socket_num == 5)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_5)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(UDP_socket_5, start_udp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_5 = osThreadCreate(osThread(UDP_socket_5), (void*)thread_parameter);
		}

	if(socket_num == 6)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_6)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(UDP_socket_6, start_udp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_6 = osThreadCreate(osThread(UDP_socket_6), (void*)thread_parameter);
		}


	if(socket_num == 7)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.server_handle_7)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(UDP_socket_7, start_udp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.server_handle_7 = osThreadCreate(osThread(UDP_socket_7), (void*)thread_parameter);
		}

}


void deal_all_socket_udp(void)
{

}

void deal_each_socket_udp(unsigned char socket_num)
{
	switch (socket_num)
		{
			case 0:
				break;

			case 1:
				break;

			case 2:
				break;

			case 3:
				break;

			case 4:
				break;

			case 5:
				break;

			case 6:
				break;

			case 7:
				break;

				
		}
}

void deal_all_socket_server(void)
{
	
}

void deal_each_socket_server(unsigned char socket_num)
{
	switch (socket_num)
		{
			case 0:
				break;

			case 1:
				break;

			case 2:
				break;

			case 3:
				break;

			case 4:
				break;

			case 5:
				break;

			case 6:
				break;

			case 7:
				break;

				
		}
}

void deal_all_socket_client(void)
{

}


void deal_each_socket_client(unsigned char socket_num)
{
	int send_num = 0;
	switch (socket_num)
		{
			case 0:
				break;

			case 1:
				break;

			case 2:
				break;

			case 3:
				break;

			case 4:
				break;

			case 5:
				send_num = 5;
				//xQueueSend(mqtt_queueHandle,&send_num,0xff);
				break;

			case 6:
				break;

			case 7:
				break;

				
		}
}




