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
*CRC8����У�麯��
*ͨ�����ķ�ʽʵ��
*p		��������
*len	����
*����У��ֵ
*************************************/
uint8_t util_crc8(uint8_t *p, uint32_t len)
{
	uint32_t i;
	uint8_t CRC8=0;
	for(i=0;i<len;i++)
	{
		CRC8 = CRC8_TAB[p[i]^CRC8];
	}
	return(CRC8);//����У��ֵ
}
/*************************************
 *������Ϊ������Ϣ������
 *pdataΪ����buffָ��
 *devportΪ����֡ʶ��ʱ�ṩ�Ķ˿ں�
 *devchannelΪ����֡ʶ��ʱ�ṩ��ͨ���ż�IO��
 *datatag �ο�TLVTAG_XXXX��TLVTAG_ELECTRIC_EMVA
 *datatype ��ο�TLVTYPE_XXX��TLVTYPE_ELECTRIC_EMVA
 *val ���͵���ֵ��ΧΪ -2^128 ~ +2^128
 *���ش˶����ݶγ���
*************************************/
uint32_t set_tlv_body(uint8_t *pdata,uint8_t devport,uint8_t devchannel,uint8_t datatag,uint8_t datatype,float val)
{
	uint32_t index,ucval,times,i;
	uint8_t type,numafterdecimal;
	type = datatype&0xF0;
	numafterdecimal = datatype&0X0F;
	//Ŀǰ��ֹС�����λ������7
	if(numafterdecimal > 7)
		return 0;
	if(numafterdecimal != 0)
	{
		//����numafterdecimal��С�������Ч��ȷ���Ŵ���
		times = 1;
		for(i=0;i<numafterdecimal;i++)
		{
			times *= 10;
		}
		ucval = val*times;
	}
	index = 0;
	*(pdata+index++) = devport;//�˿ں�
	*(pdata+index++) = devchannel;//IO��
	*(pdata+index++) = datatag;//����tag
	*(pdata+index++) = datatype;//�ο��ĵ�
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
	return index;//�������ӵ����ݳ���
}

/*************************************
*������Ϣ������
*�������ݰ�����
*************************************/
uint32_t set_tlv_package()
{
	int i;
	INFO("topic: %s \r\n", MQTT_TOPIC);
	g_tlvpac.length = 0;
	g_tlvpac.buff[g_tlvpac.length++] = TLVHEAD1;//֡ͷ 1
	g_tlvpac.buff[g_tlvpac.length++] = TLVHEAD2;//֡ͷ 2
	g_tlvpac.buff[g_tlvpac.length++] = TLVVERSION;//�淶�汾��
	g_tlvpac.buff[g_tlvpac.length++] = 0x00;//�ܳ��ȸ�8λ�ȴ���Ϣ�������ڸ�ֵ
	g_tlvpac.buff[g_tlvpac.length++] = 0x00;//�ܳ��ȵ�8λ�ȴ���Ϣ�������ڸ�ֵ
	g_tlvpac.buff[g_tlvpac.length++] = (TLVBID_WATER_PUMP>>8)&0XFF;//��ҵ����
	g_tlvpac.buff[g_tlvpac.length++] = TLVBID_WATER_PUMP&0XFF;//����ҵ����
	g_tlvpac.buff[g_tlvpac.length++] = (TLVADDR>>24)&0xFF;//��ַ1
	g_tlvpac.buff[g_tlvpac.length++] = (TLVADDR>>16)&0xFF;//��ַ2
	g_tlvpac.buff[g_tlvpac.length++] = (TLVADDR>>8)&0xFF;//��ַ3
	g_tlvpac.buff[g_tlvpac.length++] = TLVADDR&0XFF;//��ַ4
	g_tlvpac.buff[g_tlvpac.length++] = TLVFUN;//������
	g_tlvpac.buff[g_tlvpac.length++] = 0;//ʱ��YY 10���� Ϊ0ʱʹ�÷�����ʱ��
	g_tlvpac.buff[g_tlvpac.length++] = 0;//ʱ��MM
	g_tlvpac.buff[g_tlvpac.length++] = 0;//ʱ��DD
	g_tlvpac.buff[g_tlvpac.length++] = 0;//ʱ��HH
	g_tlvpac.buff[g_tlvpac.length++] = 0;//ʱ��MM
	g_tlvpac.buff[g_tlvpac.length++] = 0;//ʱ��SS
	//���˿�Ϊ1��IO��Ϊ0,���ݱ�ʾtagΪ0x25(���A���ѹֵ),С�������Чλ��Ϊ2����ֵΪ210.1��ֵ
	g_tlvpac.length += set_tlv_body(g_tlvpac.buff+g_tlvpac.length,
			0x01,
			0x00,
			TLVTAG_ELECTRIC_EMVA,
			TLVTYPE_ELECTRIC_EMVA,
			210.11);
	//���˿�Ϊ1��IO��Ϊ0,���ݱ�ʾtagΪ0x26(���B���ѹֵ),С�������Чλ��Ϊ2����ֵΪ220.2��ֵ
	g_tlvpac.length += set_tlv_body(g_tlvpac.buff+g_tlvpac.length,
			0x01,
			0x00,
			TLVTAG_ELECTRIC_EMVB,
			TLVTYPE_ELECTRIC_EMVB,
			220.22);
	//���˿�Ϊ1��IO��Ϊ0,���ݱ�ʾtagΪ0x27(���C���ѹֵ),С�������Чλ��Ϊ2����ֵΪ230.3��ֵ
	g_tlvpac.length += set_tlv_body(g_tlvpac.buff+g_tlvpac.length,
			0x01,
			0x00,
			TLVTAG_ELECTRIC_EMVC,
			TLVTYPE_ELECTRIC_EMVC,
			230.33);
	//���˿�Ϊ1��IO��Ϊ1,���ݱ�ʾtagΪ0x27(���C���ѹֵ),С�������Чλ��Ϊ2����ֵΪ240.4��ֵ
	g_tlvpac.length += set_tlv_body(g_tlvpac.buff+g_tlvpac.length,
			0x01,
			0x01,
			TLVTAG_ELECTRIC_EMVC,
			TLVTYPE_ELECTRIC_EMVC,
			240.44);
	//���˿�Ϊ2��IO��Ϊ0,���ݱ�ʾtagΪ0x27(���C���ѹֵ),С�������Чλ��Ϊ2����ֵΪ250.5��ֵ
	g_tlvpac.length += set_tlv_body(g_tlvpac.buff+g_tlvpac.length,
			0x02,
			0x00,
			TLVTAG_ELECTRIC_EMVC,
			TLVTYPE_ELECTRIC_EMVC,
			250.55);

	g_tlvpac.buff[3] = (g_tlvpac.length+3)>>8;//�����Ϣ���������֡����Ϣ����8λ
	g_tlvpac.buff[4] = g_tlvpac.length+3;//�����Ϣ���������֡����Ϣ����8λ
	g_tlvpac.buff[g_tlvpac.length++] = util_crc8(g_tlvpac.buff,g_tlvpac.length - 1);//crc8У��
	g_tlvpac.buff[g_tlvpac.length++] = TLVTAIL1;//֡β
	g_tlvpac.buff[g_tlvpac.length++] = TLVTAIL2;//֡β
	return g_tlvpac.length;//�������ݰ�����
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
	//MQTT_Subscribe(client, "/mqtt/topic/0", 0);//ȡ������ʱ�ͷ�������
	//MQTT_Subscribe(client, "/mqtt/topic/1", 1);//ȡ������ʱ�ͷ�������
	//MQTT_Subscribe(client, "/mqtt/topic/2", 2);//ȡ������ʱ�ͷ�������

	//MQTT_Publish(client, "/mqtt/topic/0", "hello0", 6, 0, 0);//ȡ������ʱ�ͷ�������
	//MQTT_Publish(client, "/mqtt/topic/1", "hello1", 6, 1, 0);//ȡ������ʱ�ͷ�������

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
	//uart_init(BIT_RATE_115200, BIT_RATE_115200);//����74880�Ĳ����ʣ����⴮�ڵ���ʱ��Ҫ�л����ע�͵����
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

	mqtt_publish_data();//����һ��
	INFO("\r\nSystem started ...\r\n");

}
