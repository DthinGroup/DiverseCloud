/*
* DiverseCloudLIB_TLV_.h
*
*  Created on: 2019��5��29��
*      Author: shb
*     version: 1.1
*/


#ifndef __DIVERSECLOUDLIB_TLV_H_
#define __DIVERSECLOUDLIB_TLV_H_

//ʹ����³��ʶ��ַʱ������֡�����С 65536��size����Ϣ����+21
//ʹ�÷���³��ʶ��ַʱ������֡�����С 65536��size����Ϣ����+18+��ʶ��ַ����(���31)
#define TLV_PROP_BUFFSIZE			(200)


/*������32bits CPU*******************/
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
/*��Ϣ����**************************/
//��Ϣ��������
#define TLV_MSGPROP_TYPE_BOOL		(0x00)
#define TLV_MSGPROP_TYPE_SCHAR		(0x10)
#define TLV_MSGPROP_TYPE_UCHAR		(0x20)
#define TLV_MSGPROP_TYPE_SSHORT		(0x30)
#define TLV_MSGPROP_TYPE_USHORT		(0x40)
#define TLV_MSGPROP_TYPE_SINT		(0x50)
#define TLV_MSGPROP_TYPE_UINT		(0x60)
#define TLV_MSGPROP_TYPE_SLONG		(0x70)
#define TLV_MSGPROP_TYPE_ULONG		(0x80)
//��ϢС��λ
#define TLV_MSGPROP_DECIMAL_0		(0x00)
#define TLV_MSGPROP_DECIMAL_1		(0x01)
#define TLV_MSGPROP_DECIMAL_2		(0x02)
#define TLV_MSGPROP_DECIMAL_3		(0x03)
#define TLV_MSGPROP_DECIMAL_4		(0x04)
#define TLV_MSGPROP_DECIMAL_5		(0x05)
#define TLV_MSGPROP_DECIMAL_6		(0x06)

typedef struct
{
	uint8_t	DateY;//���� ��
	uint8_t	DateM;//���� ��
	uint8_t	DateD;//���� ��
	uint8_t	TimeH;//ʱ�� ʱ
	uint8_t	TimeM;//ʱ�� ��
	uint8_t	TimeS;//ʱ�� ��
}TLV_DOUBLE_DEF;
/*TLV֡�ṹ����*********************/
typedef struct
{
	uint8_t	Addr4;//��ַλ 4
	uint8_t	Addr3;//��ַλ 3
	uint8_t	Addr2;//��ַλ 2
	uint8_t	Addr1;//��ַλ 1
}TLV_PROP_YLADDRESS_DEF;
typedef struct
{
	uint8_t	Len;//��ʶ��ַ ����
	uint8_t	*Addr;//��ַ ָ��
}TLV_PROP_CUSTOMERADDRESS_DEF;
typedef struct
{
	uint8_t	DateY;//���� ��
	uint8_t	DateM;//���� ��
	uint8_t	DateD;//���� ��
	uint8_t	TimeH;//ʱ�� ʱ
	uint8_t	TimeM;//ʱ�� ��
	uint8_t	TimeS;//ʱ�� ��
}TLV_PROP_DATETIME_DEF;

/*ϵͳ��������**********************/
typedef enum
{
	TLV_PROP_BYTEORDER_BE = 0,//��˸�ʽ
	TLV_PROP_BYTEORDER_LE//С�˸�ʽ
}TLV_PROP_BYTEORDER_DEF;
/*TLV֡����_����*************/
typedef struct
{
	uint8_t BO;//���ݸ�ʽ��⣬tlv_prop_byteorder_check()����ֵ��BYTEORDER_BE����˸�ʽ����BYTEORDER_LE��С�˸�ʽ��
	uint8_t *Buff;//tlv��Ϣ������ָ��
	uint8_t BuffSize;//tlv��Ϣ��������С
	uint8_t Length;//tlv��Ϣ���ȱ�־
	uint8_t AddressType;//tlv��ַ����ʶ��ַ������,TLV_PROP_ADDRESSTYPE_YL:��ʶ��ַ(��³) ;TLV_PROP_ADDRESSTYPE_CUSTOMER:��ʶ��ַ(����³)
	TLV_PROP_YLADDRESS_DEF YLAddress;//��ʶ��ַ(��³)�μ���diversecloudlib_tmpl_xxx_vxxx.h
	TLV_PROP_CUSTOMERADDRESS_DEF CustomerAddress;//��ʶ��ַ(����³)�μ���diversecloudlib_tmpl_xxx_vxxx.h
	TLV_PROP_DATETIME_DEF DateTime;//�ϱ�ʱ��
}TLV_PROP_DEF;

typedef struct
{
	uint8_t Port;//��Ϣ���������˿ں�
	uint8_t Channel;//��Ϣ��������ͨ���ż�IO��
	uint8_t Tag;//��Ϣ����TAG��ֵ���ο�TLV_MSGTAG_XXX_XXX_XXX��TLV_MSGTAG_DEV_EMETER_VPHASEA
	uint8_t Type;//��Ϣ�������ͣ��ο�TLV_MSGTYPE_XXX_XXX_XXX��TLV_MSGTYPE_DEV_EMETER_VPHASEA
	double Value;//��Ϣ����ֵ,���֧��6λС��
}TLV_MSG_DEF;
typedef enum
{
	TLV_SUCCESS = 0,//�ɹ�
	TLV_ERROR,//����
	TLV_VALOUTOFRANGE,//��ֵ��������
	TLV_ERRORMSGTYPE,//��Ϣ���ʹ���
	TLV_ISFCBUFFSIZE,//���治��
	TLV_ERRORADDRESSTYPE,//��ʶ���ʹ���
}TLV_ERRORCODE;
typedef enum
{
	TLV_PROP_ADDRESSTYPE_YL = 0,//ʹ�ñ�ʶ��ַ(��³)
	TLV_PROP_ADDRESSTYPE_CUSTOMER,//ʹ�ñ�ʶ��ַ(����³)
}TLV_PROP_ADDRESSTYPE_DEF;

/*************************************
*��Ϣ��ʼ��
*prop		���Խṹ��ָ��
*return		�����룬�ο�TLV_ERRORCODE����
*************************************/
TLV_ERRORCODE tlv_msg_init(TLV_PROP_DEF *prop);

/*************************************
*�����Ϣ����
*prop		���Խṹ��ָ��
*msg		��Ϣ�ṹ��ָ��
*return		�����룬�ο�TLV_ERRORCODE����
*************************************/
TLV_ERRORCODE tlv_msg_Add(TLV_PROP_DEF *prop, TLV_MSG_DEF *msg);
/*************************************
*�����Ϣ
*prop		���Խṹ��ָ��
*return		�����룬�ο�TLV_ERRORCODE����
*************************************/
TLV_ERRORCODE tlv_msg_finish(TLV_PROP_DEF *prop);

/*************************************
*TLV֡��³��ʶ��ַ����
*devAddr4	�豸��ʶ��ַ4
*devAddr3	�豸��ʶ��ַ3
*devAddr2	�豸��ʶ��ַ2
*devAddr1	�豸��ʶ��ַ1
*return		TLV֡��³��ʽ����ʶ��ַ
*************************************/
TLV_PROP_YLADDRESS_DEF tlv_prop_yladdress_set(uint8_t devAddr4, uint8_t devAddr3, uint8_t devAddr2, uint8_t devAddr1);

/*************************************
*TLV֡����³�Զ����ʶ��ַ����
*addr		��ʶ��ַָ��
*len		��ʶ��ַ����
*return		TLV֡����³��ʽ����ʶ��ַ
*************************************/
TLV_PROP_CUSTOMERADDRESS_DEF tlv_prop_customeraddress_set(uint8_t *addr, uint8_t len);

/*************************************
*TLV֡�����ϱ�ʱ�䣨UTC+8������
*dateY		��
*dateM		��
*dateD		��
*timeH		ʱ
*timeM		��
*timeS		��
*return		TLV֡�����ϱ���ʽ��ʱ��
*************************************/
TLV_PROP_DATETIME_DEF tlv_prop_datetime_set(uint8_t dateY, uint8_t dateM, uint8_t dateD, uint8_t timeH, uint8_t timeM, uint8_t timeS);

/*************************************
*TLV֡�����ϱ�ʱ�䣨UTC+8������-ʱ������룩,��֧��msʱ���ת��
*timestamp	long long�� ʱ������룩
*return		TLV֡�����ϱ���ʽ��ʱ��
*************************************/
TLV_PROP_DATETIME_DEF tlv_prop_datetime_set_timestamps(const sint64_t timestampS);

/*************************************
*TLV֡�����ϱ�ʱ�䣨UTC+8�����ã�ʹ�÷���������ʱ����Ϊ�ϱ�ʱ��
*return		TLV֡�����ϱ���ʽ��ʱ��
*************************************/
TLV_PROP_DATETIME_DEF tlv_prop_datetime_set_serverdatetime(void);

/*************************************
*ƽ̨��С�˼��
*return	BYTEORDER_BE	��˸�ʽ
		BYTEORDER_LE	С�˸�ʽ
*************************************/
TLV_PROP_BYTEORDER_DEF tlv_prop_byteorder_check(void);
#endif
