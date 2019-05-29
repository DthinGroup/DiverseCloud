/*
* DiverseCloudLIB_TLV.c
*
*  Created on: 2019年5月29日
*      Author: shb
*     version: 1.1
*/

#include <stdio.h>
#include <string.h>
#include "diversecloudlib_tlv.h"
#include "diversecloudlib_tmpl_dev_v000.h"

uint8_t g_TlvBuff[TLV_PROP_BUFFSIZE];

/*************************************
*使用演示
*************************************/
void main()
{
	TLV_ERRORCODE errorCode;
	uint8_t customerAddr[] = { 1,2,3,4,5,6,7,8 };//标识地址（非雅鲁标识）
	uint8_t customerAddrLen = 8;//标识地址（非雅鲁标识）长度
	TLV_PROP_DEF tlv_prop;
	TLV_MSG_DEF tlv_msg;
	uint8_t i = 0;

	/*step1 数值属性初始化*************/
	tlv_prop.BO = tlv_prop_byteorder_check();//数据格式检测，tlv_prop_byteorder_check()返回值或BYTEORDER_BE（大端格式）、BYTEORDER_LE（小端格式）
	tlv_prop.Buff = g_TlvBuff;//指向tlv消息缓存区地址
	tlv_prop.BuffSize = TLV_PROP_BUFFSIZE;//tlv消息缓存区大小	
	tlv_prop.AddressType = TLV_PROP_ADDRESSTYPE_YL;//标识地址类型;TLV_PROP_ADDRESSTYPE_YL:标识地址(雅鲁) ;TLV_PROP_ADDRESSTYPE_CUSTOMER:标识地址(非雅鲁)
	tlv_prop.YLAddress = tlv_prop_yladdress_set(56, 44, 33, 0); //标识地址类型一：（雅鲁标识）56.44.33.0	
	//tlv_prop.CustomerAddress = tlv_prop_customeraddress_set(customerAddr, customerAddrLen); ;//标识地址类型二：（非雅鲁标识）0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08
	tlv_prop.DateTime = tlv_prop_datetime_set(19, 5, 20, 12, 34, 56);//消息上报时间设置，方法一：2019-05-20 12:34:56
	//tlv_prop.DateTime = tlv_prop_datetime_set_timestamp(1558971098);//消息上报时间设置，方法二：时间戳(S)格式化
	//tlv_prop.DateTime = tlv_prop_datetime_set_serverdatetime();//消息上报时间设置，方法三：使用服务器接收时间
	if (TLV_SUCCESS != tlv_msg_init(&tlv_prop))//消息属性初始化
		while (1);

	/*step2 添加消息数据，请注意需要足够的缓存空间可使用返回检测*************/
	//exp:新增2条数据，电表A相电压232.45，电表B相电压232.45
	tlv_msg.Port = 2;//消息数据所属端口号
	tlv_msg.Channel = 1;//消息数据所属通道号即IO号
	tlv_msg.Tag = TLV_MSGTAG_DEV_EMETER_VPHASEA;//消息数据TAG键值，参考TLV_MSGTAG_XXX_XXX_XXX如TLV_MSGTAG_DEV_EMETER_VPHASEA
	tlv_msg.Type = TLV_MSGTYPE_DEV_EMETER_VPHASEA;//消息数据类型，参考TLV_MSGTYPE_XXX_XXX_XXX如TLV_MSGTYPE_DEV_EMETER_VPHASEA
	tlv_msg.Value = 232.44;//消息数据值,请注意数值允许范围，可使用返回检测。
	if(TLV_SUCCESS != tlv_msg_Add(&tlv_prop, &tlv_msg))//增加第一条消息数据
		while(1);//若失败请重新初始化：tlv_msg_init

	tlv_msg.Port = 2;
	tlv_msg.Channel = 1;
	tlv_msg.Tag = TLV_MSGTAG_DEV_EMETER_VPHASEB;
	tlv_msg.Type = TLV_MSGTYPE_DEV_EMETER_VPHASEB;
	tlv_msg.Value = 230;
	if (TLV_SUCCESS != tlv_msg_Add(&tlv_prop, &tlv_msg))//增加第二条消息数据
		while (1);//若失败请重新初始化：tlv_msg_init
	
	/*step3 完成消息帧*************/
	tlv_msg_finish(&tlv_prop);

	for (i = 0; i<tlv_prop.Length; i++)
		printf("%02x ", tlv_prop.Buff[i]);
	while (1);
}

