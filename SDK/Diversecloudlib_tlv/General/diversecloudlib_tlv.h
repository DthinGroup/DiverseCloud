/*
* DiverseCloudLIB_TLV_.h
*
*  Created on: 2019年5月29日
*      Author: shb
*     version: 1.1
*/


#ifndef __DIVERSECLOUDLIB_TLV_H_
#define __DIVERSECLOUDLIB_TLV_H_

//使用雅鲁标识地址时：数据帧缓存大小 65536≥size≥消息内容+21
//使用非雅鲁标识地址时：数据帧缓存大小 65536≥size≥消息内容+18+标识地址长度(最大31)
#define TLV_PROP_BUFFSIZE			(200)


/*适用于32bits CPU*******************/
#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif
#ifndef uint32_t
typedef unsigned int uint32_t;
#endif
#ifndef sint32_t
typedef signed int sint32_t;
#endif
#ifndef sint64_t
typedef long long sint64_t;
#endif
/*消息属性**************************/
//消息数据类型
#define TLV_MSGPROP_TYPE_BOOL		(0x00)
#define TLV_MSGPROP_TYPE_SCHAR		(0x10)
#define TLV_MSGPROP_TYPE_UCHAR		(0x20)
#define TLV_MSGPROP_TYPE_SSHORT		(0x30)
#define TLV_MSGPROP_TYPE_USHORT		(0x40)
#define TLV_MSGPROP_TYPE_SINT		(0x50)
#define TLV_MSGPROP_TYPE_UINT		(0x60)
#define TLV_MSGPROP_TYPE_SLONG		(0x70)
#define TLV_MSGPROP_TYPE_ULONG		(0x80)
//消息小数位
#define TLV_MSGPROP_DECIMAL_0		(0x00)
#define TLV_MSGPROP_DECIMAL_1		(0x01)
#define TLV_MSGPROP_DECIMAL_2		(0x02)
#define TLV_MSGPROP_DECIMAL_3		(0x03)
#define TLV_MSGPROP_DECIMAL_4		(0x04)
#define TLV_MSGPROP_DECIMAL_5		(0x05)
#define TLV_MSGPROP_DECIMAL_6		(0x06)

typedef struct
{
	uint8_t	DateY;//日期 年
	uint8_t	DateM;//日期 月
	uint8_t	DateD;//日期 日
	uint8_t	TimeH;//时间 时
	uint8_t	TimeM;//时间 分
	uint8_t	TimeS;//时间 秒
}TLV_DOUBLE_DEF;
/*TLV帧结构定义*********************/
typedef struct
{
	uint8_t	Addr4;//地址位 4
	uint8_t	Addr3;//地址位 3
	uint8_t	Addr2;//地址位 2
	uint8_t	Addr1;//地址位 1
}TLV_PROP_YLADDRESS_DEF;
typedef struct
{
	uint8_t	Len;//标识地址 长度
	uint8_t	*Addr;//地址 指针
}TLV_PROP_CUSTOMERADDRESS_DEF;
typedef struct
{
	uint8_t	DateY;//日期 年
	uint8_t	DateM;//日期 月
	uint8_t	DateD;//日期 日
	uint8_t	TimeH;//时间 时
	uint8_t	TimeM;//时间 分
	uint8_t	TimeS;//时间 秒
}TLV_PROP_DATETIME_DEF;

/*系统参数定义**********************/
typedef enum
{
	TLV_PROP_BYTEORDER_BE = 0,//大端格式
	TLV_PROP_BYTEORDER_LE//小端格式
}TLV_PROP_BYTEORDER_DEF;
/*TLV帧属性_定义*************/
typedef struct
{
	uint8_t BO;//数据格式检测，tlv_prop_byteorder_check()返回值或BYTEORDER_BE（大端格式）、BYTEORDER_LE（小端格式）
	uint8_t *Buff;//tlv消息缓存区指针
	uint8_t BuffSize;//tlv消息缓存区大小
	uint8_t Length;//tlv消息长度标志
	uint8_t AddressType;//tlv地址（标识地址）类型,TLV_PROP_ADDRESSTYPE_YL:标识地址(雅鲁) ;TLV_PROP_ADDRESSTYPE_CUSTOMER:标识地址(非雅鲁)
	TLV_PROP_YLADDRESS_DEF YLAddress;//标识地址(雅鲁)参见：diversecloudlib_tmpl_xxx_vxxx.h
	TLV_PROP_CUSTOMERADDRESS_DEF CustomerAddress;//标识地址(非雅鲁)参见：diversecloudlib_tmpl_xxx_vxxx.h
	TLV_PROP_DATETIME_DEF DateTime;//上报时间
}TLV_PROP_DEF;

typedef struct
{
	uint8_t Port;//消息数据所属端口号
	uint8_t Channel;//消息数据所属通道号即IO号
	uint8_t Tag;//消息数据TAG键值，参考TLV_MSGTAG_XXX_XXX_XXX如TLV_MSGTAG_DEV_EMETER_VPHASEA
	uint8_t Type;//消息数据类型，参考TLV_MSGTYPE_XXX_XXX_XXX如TLV_MSGTYPE_DEV_EMETER_VPHASEA
	double Value;//消息数据值,最大支持6位小数
}TLV_MSG_DEF;
typedef enum
{
	TLV_SUCCESS = 0,//成功
	TLV_ERROR,//错误
	TLV_VALOUTOFRANGE,//数值超出限制
	TLV_ERRORMSGTYPE,//消息类型错误
	TLV_ISFCBUFFSIZE,//缓存不足
	TLV_ERRORADDRESSTYPE,//标识类型错误
}TLV_ERRORCODE;
typedef enum
{
	TLV_PROP_ADDRESSTYPE_YL = 0,//使用标识地址(雅鲁)
	TLV_PROP_ADDRESSTYPE_CUSTOMER,//使用标识地址(非雅鲁)
}TLV_PROP_ADDRESSTYPE_DEF;

/*************************************
*消息初始化
*prop		属性结构体指针
*return		错误码，参考TLV_ERRORCODE定义
*************************************/
TLV_ERRORCODE tlv_msg_init(TLV_PROP_DEF *prop);

/*************************************
*添加消息数据
*prop		属性结构体指针
*msg		消息结构体指针
*return		错误码，参考TLV_ERRORCODE定义
*************************************/
TLV_ERRORCODE tlv_msg_Add(TLV_PROP_DEF *prop, TLV_MSG_DEF *msg);
/*************************************
*完成消息
*prop		属性结构体指针
*return		错误码，参考TLV_ERRORCODE定义
*************************************/
TLV_ERRORCODE tlv_msg_finish(TLV_PROP_DEF *prop);

/*************************************
*TLV帧雅鲁标识地址设置
*devAddr4	设备标识地址4
*devAddr3	设备标识地址3
*devAddr2	设备标识地址2
*devAddr1	设备标识地址1
*return		TLV帧雅鲁格式化标识地址
*************************************/
TLV_PROP_YLADDRESS_DEF tlv_prop_yladdress_set(uint8_t devAddr4, uint8_t devAddr3, uint8_t devAddr2, uint8_t devAddr1);

/*************************************
*TLV帧非雅鲁自定义标识地址设置
*addr		标识地址指针
*len		标识地址长度
*return		TLV帧非雅鲁格式化标识地址
*************************************/
TLV_PROP_CUSTOMERADDRESS_DEF tlv_prop_customeraddress_set(uint8_t *addr, uint8_t len);

/*************************************
*TLV帧数据上报时间（UTC+8）设置
*dateY		年
*dateM		月
*dateD		日
*timeH		时
*timeM		分
*timeS		秒
*return		TLV帧数据上报格式化时间
*************************************/
TLV_PROP_DATETIME_DEF tlv_prop_datetime_set(uint8_t dateY, uint8_t dateM, uint8_t dateD, uint8_t timeH, uint8_t timeM, uint8_t timeS);

/*************************************
*TLV帧数据上报时间（UTC+8）设置-时间戳（秒）,不支持ms时间戳转换
*timestamp	long long型 时间戳（秒）
*return		TLV帧数据上报格式化时间
*************************************/
TLV_PROP_DATETIME_DEF tlv_prop_datetime_set_timestamps(const sint64_t timestampS);

/*************************************
*TLV帧数据上报时间（UTC+8）设置，使用服务器接收时间作为上报时间
*return		TLV帧数据上报格式化时间
*************************************/
TLV_PROP_DATETIME_DEF tlv_prop_datetime_set_serverdatetime(void);

/*************************************
*平台大小端检测
*return	BYTEORDER_BE	大端格式
		BYTEORDER_LE	小端格式
*************************************/
TLV_PROP_BYTEORDER_DEF tlv_prop_byteorder_check(void);
#endif
