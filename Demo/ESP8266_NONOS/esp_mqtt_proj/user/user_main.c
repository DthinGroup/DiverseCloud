/* main.c -- MQTT client example
*
* Copyright (c) 2014-2015, Tuan PM <tuanpm at live dot com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of Redis nor the names of its contributors may be used
* to endorse or promote products derived from this software without
* specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/
#include "ets_sys.h"
#include "driver/uart.h"
#include "osapi.h"
#include "mqtt.h"
#include "wifi.h"
#include "config.h"
#include "debug.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"
#include "YLTlvPac.h"

MQTT_Client mqttClient;

TLV_DEF g_tlvpac;
/*************************************
*CRC8数据校验函数
*通过查表的方式实现
*p		输入数组
*len	长度
*返回校验值
*************************************/
uint8_t util_crc8(uint8_t *p, uint32_t len)
{
	uint32_t i;
	uint8_t CRC8=0;
	for(i=0;i<len;i++)
	{
		CRC8 = CRC8_TAB[p[i]^CRC8];
	}
	return(CRC8);//返回校验值
}
/*************************************
 *本函数为设置消息体数据
 *pdata为数据buff指针
 *devport为数据帧识别时提供的端口号
 *devchannel为数据帧识别时提供的通道号即IO号
 *datatag 参考TLVTAG_XXXX如TLVTAG_ELECTRIC_EMVA
 *datatype 请参考TLVTYPE_XXX如TLVTYPE_ELECTRIC_EMVA
 *val 发送的数值范围为 -2^128 ~ +2^128
 *返回此段数据段长度
*************************************/
uint32_t set_tlv_body(uint8_t *pdata,uint8_t devport,uint8_t devchannel,uint8_t datatag,uint8_t datatype,float val)
{
	uint32_t index,ucval,times,i;
	uint8_t type,numafterdecimal;
	type = datatype&0xF0;
	numafterdecimal = datatype&0X0F;
	//目前禁止小数点后位数大于7
	if(numafterdecimal > 7)
		return 0;
	if(numafterdecimal != 0)
	{
		//根据numafterdecimal即小数点后有效数确定放大倍数
		times = 1;
		for(i=0;i<numafterdecimal;i++)
		{
			times *= 10;
		}
		ucval = val*times;
	}
	index = 0;
	*(pdata+index++) = devport;//端口号
	*(pdata+index++) = devchannel;//IO号
	*(pdata+index++) = datatag;//数据tag
	*(pdata+index++) = datatype;//参考文档
	switch(type)
	{
		case TLVTAG_TYPEH_UCHAR:
			*(pdata+index++) = ucval&0xff;
		break;
		case TLVTAG_TYPEH_SSHORT:
		case TLVTAG_TYPEH_USHORT:
#ifdef BIGEDIAN
			*(pdata+index++) = (ucval>>8)&0xff;
			*(pdata+index++) = ucval&0xff;
#else
			*(pdata+index++) = ucval&0xff;
			*(pdata+index++) = (ucval>>8)&0xff;
#endif
		break;
		case TLVTAG_TYPEH_SINT:
		case TLVTAG_TYPEH_UINT:
#ifdef BIGEDIAN
			*(pdata+index++) = (ucval>>24)&0xff;
			*(pdata+index++) = (ucval>>16)&0xff;
			*(pdata+index++) = (ucval>>8)&0xff;
			*(pdata+index++) = ucval&0xff;
#else
			*(pdata+index++) = ucval&0xff;
			*(pdata+index++) = (ucval>>8)&0xff;
			*(pdata+index++) = (ucval>>16)&0xff;
			*(pdata+index++) = (ucval>>24)&0xff;
#endif
		break;
		default:
		return 0;
	}
	return index;//返回增加的数据长度
}

/*************************************
*设置消息包内容
*返回数据包长度
*************************************/
uint32_t set_tlv_package()
{
	int i;
	INFO("topic: %s \r\n", MQTT_TOPIC);
	g_tlvpac.length = 0;
	g_tlvpac.buff[g_tlvpac.length++] = TLVHEAD1;//帧头 1
	g_tlvpac.buff[g_tlvpac.length++] = TLVHEAD2;//帧头 2
	g_tlvpac.buff[g_tlvpac.length++] = TLVVERSION;//规范版本号
	g_tlvpac.buff[g_tlvpac.length++] = 0x00;//总长度高8位等待消息体填充后在赋值
	g_tlvpac.buff[g_tlvpac.length++] = 0x00;//总长度低8位等待消息体填充后在赋值
	g_tlvpac.buff[g_tlvpac.length++] = (TLVBID_WATER_PUMP>>8)&0XFF;//行业编码
	g_tlvpac.buff[g_tlvpac.length++] = TLVBID_WATER_PUMP&0XFF;//子行业编码
	g_tlvpac.buff[g_tlvpac.length++] = (TLVADDR>>24)&0xFF;//地址1
	g_tlvpac.buff[g_tlvpac.length++] = (TLVADDR>>16)&0xFF;//地址2
	g_tlvpac.buff[g_tlvpac.length++] = (TLVADDR>>8)&0xFF;//地址3
	g_tlvpac.buff[g_tlvpac.length++] = TLVADDR&0XFF;//地址4
	g_tlvpac.buff[g_tlvpac.length++] = TLVFUN;//功能码
	g_tlvpac.buff[g_tlvpac.length++] = 0;//时间YY 10进制 为0时使用服务器时间
	g_tlvpac.buff[g_tlvpac.length++] = 0;//时间MM
	g_tlvpac.buff[g_tlvpac.length++] = 0;//时间DD
	g_tlvpac.buff[g_tlvpac.length++] = 0;//时间HH
	g_tlvpac.buff[g_tlvpac.length++] = 0;//时间MM
	g_tlvpac.buff[g_tlvpac.length++] = 0;//时间SS
	//填充端口为1，IO口为0,数据表示tag为0x25(电表A相电压值),小数点后有效位数为2，数值为210.1的值
	g_tlvpac.length += set_tlv_body(g_tlvpac.buff+g_tlvpac.length,
			0x01,
			0x00,
			TLVTAG_ELECTRIC_EMVA,
			TLVTYPE_ELECTRIC_EMVA,
			210.11);
	//填充端口为1，IO口为0,数据表示tag为0x26(电表B相电压值),小数点后有效位数为2，数值为220.2的值
	g_tlvpac.length += set_tlv_body(g_tlvpac.buff+g_tlvpac.length,
			0x01,
			0x00,
			TLVTAG_ELECTRIC_EMVB,
			TLVTYPE_ELECTRIC_EMVB,
			220.22);
	//填充端口为1，IO口为0,数据表示tag为0x27(电表C相电压值),小数点后有效位数为2，数值为230.3的值
	g_tlvpac.length += set_tlv_body(g_tlvpac.buff+g_tlvpac.length,
			0x01,
			0x00,
			TLVTAG_ELECTRIC_EMVC,
			TLVTYPE_ELECTRIC_EMVC,
			230.33);
	//填充端口为1，IO口为1,数据表示tag为0x27(电表C相电压值),小数点后有效位数为2，数值为240.4的值
	g_tlvpac.length += set_tlv_body(g_tlvpac.buff+g_tlvpac.length,
			0x01,
			0x01,
			TLVTAG_ELECTRIC_EMVC,
			TLVTYPE_ELECTRIC_EMVC,
			240.44);
	//填充端口为2，IO口为0,数据表示tag为0x27(电表C相电压值),小数点后有效位数为2，数值为250.5的值
	g_tlvpac.length += set_tlv_body(g_tlvpac.buff+g_tlvpac.length,
			0x02,
			0x00,
			TLVTAG_ELECTRIC_EMVC,
			TLVTYPE_ELECTRIC_EMVC,
			250.55);

	g_tlvpac.buff[3] = (g_tlvpac.length+3)>>8;//完成消息体填充后填充帧长信息，高8位
	g_tlvpac.buff[4] = g_tlvpac.length+3;//完成消息体填充后填充帧长信息，低8位
	g_tlvpac.buff[g_tlvpac.length++] = util_crc8(g_tlvpac.buff,g_tlvpac.length - 1);//crc8校验
	g_tlvpac.buff[g_tlvpac.length++] = TLVTAIL1;//帧尾
	g_tlvpac.buff[g_tlvpac.length++] = TLVTAIL2;//帧尾
	return g_tlvpac.length;//返回数据包长度
}

void mqtt_publish_data()
{
	int len;
	len = set_tlv_package();
	MQTT_Publish(&mqttClient, MQTT_TOPIC, g_tlvpac.buff, len, 0, 0);
}

void wifiConnectCb(uint8_t status)
{
	if(status == STATION_GOT_IP){
		MQTT_Connect(&mqttClient);
	} else {
		MQTT_Disconnect(&mqttClient);
	}
}
void mqttConnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Connected\r\n");
	//MQTT_Subscribe(client, "/mqtt/topic/0", 0);//取消连接时就发布数据
	//MQTT_Subscribe(client, "/mqtt/topic/1", 1);//取消连接时就发布数据
	//MQTT_Subscribe(client, "/mqtt/topic/2", 2);//取消连接时就发布数据

	//MQTT_Publish(client, "/mqtt/topic/0", "hello0", 6, 0, 0);//取消连接时就发布数据
	//MQTT_Publish(client, "/mqtt/topic/1", "hello1", 6, 1, 0);//取消连接时就发布数据

}

void mqttDisconnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Disconnected\r\n");
}

void mqttPublishedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Published\r\n");
	os_delay_us(1000000);
	mqtt_publish_data();
}

void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
	char *topicBuf = (char*)os_zalloc(topic_len+1),
			*dataBuf = (char*)os_zalloc(data_len+1);

	MQTT_Client* client = (MQTT_Client*)args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	INFO("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);
	os_free(topicBuf);
	os_free(dataBuf);
}


/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
 *******************************************************************************/
uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    
    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;
            
        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;
            
        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;
            
        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;
            
        default:
            rf_cal_sec = 0;
            break;
    }
    
    return rf_cal_sec;
}

void user_init(void)
{
	//uart_init(BIT_RATE_115200, BIT_RATE_115200);//沿用74880的波特率，避免串口调试时需要切换因此注释掉这句
	os_delay_us(1000000);

	CFG_Load();

	MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
	//MQTT_InitConnection(&mqttClient, "192.168.11.122", 1880, 0);

	MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
	//MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);

	MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);

	WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);

	mqtt_publish_data();//发布一次
	INFO("\r\nSystem started ...\r\n");

}
