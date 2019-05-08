/*
 * YLTlvPac.h
 *
 *  Created on: 2019��3��4��
 *      Author: hb
 *     version: 1.0
 */

#ifndef YLTLVPAC_H_
#define YLTLVPAC_H_

//#include "c_types.h"
#include "stdio.h"	//uint8_t �����Ͷ����ļ�

#define BIGEDIAN			//����оƬ���������ж���
//TLV����֡����
#define TLVHEAD1			(0XAA)//֡ͷ 1
#define TLVHEAD2			(0XFE)//֡ͷ 2
#define TLVTAIL1			(0XDD)//֡β1
#define TLVTAIL2			(0XEE)//֡β2
#define TLVVERSION			(2)//�淶�汾��

//TLVADDR���ñ�ʶΪip:1.0.1.0,ʹ��ip��Ϊ��ʶʱ���һλΪ0����
#define TLVADDR				(0x01000200)



//TLV�����룬�̶�Ϊ1
#define TLVFUN				(1)

#define TLVBUFFSIZE 		(200)//MQTT����buff��С

/***********TLV֡��ҵ����*************/
#define TLVBID_ELECTRIC		(0x0100)//����ͨ��
#define TLVBID_WATER		(0x0200)//ˮ��ͨ��
#define TLVBID_WATER_PUMP	(0x0201)//ˮ��÷�
#define TLVBID_WATER_WM		(0x0202)//ˮ��ˮ��
#define TLVBID_CITY			(0x0300)//����
#define TLVBID_ENV			(0x0400)//����
#define TLVBID_FACTORY		(0x0500)//����

/***********TAG����*************/
//�������͸�λ
//#define TLVTAG_TYPEH_BOOL		(0X00)//δ֧��
//#define TLVTAG_TYPEH_SCHAR	(0X10)//δ֧��
#define TLVTAG_TYPEH_UCHAR	(0X20)
#define TLVTAG_TYPEH_SSHORT	(0X30)
#define TLVTAG_TYPEH_USHORT	(0X40)
#define TLVTAG_TYPEH_SINT	(0X50)
#define TLVTAG_TYPEH_UINT	(0X60)
//#define TLVTAG_TYPEH_SLONG	(0X70)//δ֧��
//#define TLVTAG_TYPEH_ULONG	(0X80)//δ֧��
//#define TLVTAG_TYPEH_FLOAT	(0X90)//δ֧��
//#define TLVTAG_TYPEH_DOUBLE	(0XA0)//δ֧��
//�������͵�λ
#define TLVTAG_TYPEL_NFD0	(0X00)
#define TLVTAG_TYPEL_NFD1	(0X01)
#define TLVTAG_TYPEL_NFD2	(0X02)
#define TLVTAG_TYPEL_NFD3	(0X03)
#define TLVTAG_TYPEL_NFD4	(0X04)
#define TLVTAG_TYPEL_NFD5	(0X05)
#define TLVTAG_TYPEL_NFD6	(0X06)
#define TLVTAG_TYPEL_NFD7	(0X07)
/***********ˮ����ҵ*************/
//����
#define TLVTAG_WATER_PUMPTEMP1				(21)//�ñ����¶�(��)
#define TLVTYPE_WATER_PUMPTEMP1				(TLVTAG_TYPEH_UCHAR|TLVTAG_TYPEL_NFD2)//�������� uchar.2
#define TLVTAG_WATER_PUMPVABRATION1			(22)//�ñ�����(um)
#define TLVTYPE_WATER_PUMPVABRATION1		(TLVTAG_TYPEH_UCHAR|TLVTAG_TYPEL_NFD2)//�������� uchar.2
//��ˮ��
#define TLVTAG_WATER_PH						(23)//PHֵ(PH)
#define TLVTYPE_WATER_PH					(TLVTAG_TYPEH_UCHAR|TLVTAG_TYPEL_NFD2)//�������� uchar.2
#define TLVTAG_WATER_TURBIDI				(24)//�Ƕ�(NTU)
#define TLVTYPE_WATER_TURBIDI				(TLVTAG_TYPEH_UCHAR|TLVTAG_TYPEL_NFD3)//�������� uchar.3
#define TLVTAG_WATER_CHLORINE				(25)//����(mg/L)
#define TLVTYPE_WATER_CHLORINE				(TLVTAG_TYPEH_UCHAR|TLVTAG_TYPEL_NFD3)//�������� uchar.3
#define TLVTAG_WATER_WATERLEVEL				(26)//ˮλ�߶�(m)
#define TLVTYPE_WATER_WATERLEVEL			(TLVTAG_TYPEH_UCHAR|TLVTAG_TYPEL_NFD2)//�������� uchar.2
#define TLVTAG_WATER_WATERPRESSURE			(27)//ˮѹ��(Mpa)
#define TLVTYPE_WATER_WATERPRESSURE			(TLVTAG_TYPEH_UCHAR|TLVTAG_TYPEL_NFD3)//�������� uchar.3
#define TLVTAG_WATER_WATERTEMP				(28)//ˮ�¶�(Mpa)
#define TLVTYPE_WATER_WATERTEMP				(TLVTAG_TYPEH_UCHAR|TLVTAG_TYPEL_NFD2)//�������� uchar.2
#define TLVTAG_WATER_WATERSPEED				(29)//ˮ����(m3/h)
#define TLVTYPE_WATER_WATERSPEED			(TLVTAG_TYPEH_UINT|TLVTAG_TYPEL_NFD3)//�������� uint.3
#define TLVTAG_WATER_WATERFLOW				(30)//ˮ����(m3/h)
#define TLVTYPE_WATER_WATERFLOW				(TLVTAG_TYPEH_UINT|TLVTAG_TYPEL_NFD2)//�������� uint.3
//�÷�����
#define TLVTAG_WATER_ENVTEMP				(31)//�����¶�(��)
#define TLVTYPE_WATER_ENVTEMP				(TLVTAG_TYPEH_SSHORT|TLVTAG_TYPEL_NFD2)//�������� sshort.2
#define TLVTAG_WATER_ENVHUM					(32)//����ʪ��(%)
#define TLVTYPE_WATER_ENVHUM				(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//�������� ushort.2
#define TLVTAG_WATER_ENVNOISE				(33)//��������(db)
#define TLVTYPE_WATER_ENVNOISE				(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//�������� ushort.2
#define TLVTAG_WATER_ENVFIREWORK			(34)//�����̻� 0-���� 1-����
#define TLVTYPE_WATER_ENVFIREWORK			(TLVTAG_TYPEH_UCHAR|TLVTAG_TYPEL_NFD0)//�������� uchar.0
#define TLVTAG_WATER_ENVFLOOD				(35)//������ˮ 0-���� 1-����
#define TLVTYPE_WATER_ENVFLOOD				(TLVTAG_TYPEH_UCHAR|TLVTAG_TYPEL_NFD0)//�������� uchar.0
#define TLVTAG_WATER_ENVACCESSSTA			(36)//�Ž�״̬ 0-��  1-��
#define TLVTYPE_WATER_ENVACCESSSTA			(TLVTAG_TYPEH_UCHAR|TLVTAG_TYPEL_NFD0)//�������� uchar.0
//���
#define TLVTAG_WATER_EMVA					(37)//��ѹVA (V)
#define TLVTYPE_WATER_EMVA					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//�������� ushort.2
#define TLVTAG_WATER_EMVB					(38)//��ѹVB (V)
#define TLVTYPE_WATER_EMVB					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//�������� ushort.2
#define TLVTAG_WATER_EMVC					(39)//��ѹVC (V)
#define TLVTYPE_WATER_EMVC					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//�������� ushort.2
#define TLVTAG_WATER_EMIA					(40)//����IA (A)
#define TLVTYPE_WATER_EMIA					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD3)//�������� ushort.3
#define TLVTAG_WATER_EMIB					(41)//����IB (A)
#define TLVTYPE_WATER_EMIB					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD3)//�������� ushort.3
#define TLVTAG_WATER_EMIC					(42)//����IC (A)
#define TLVTYPE_WATER_EMIC					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD3)//�������� ushort.3
#define TLVTAG_WATER_EMCT					(43)//CT
#define TLVTYPE_WATER_EMCT					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD0)//�������� ushort.0
#define TLVTAG_WATER_EMPT					(44)//PT
#define TLVTYPE_WATER_EMPT					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD0)//�������� ushort.0
#define TLVTAG_WATER_EMFREQ					(45)//Ƶ�� (HZ)
#define TLVTYPE_WATER_EMFREQ				(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//��������sint.2
#define TLVTAG_WATER_EMACTIVEPOW			(46)//���й����� (KW)
#define TLVTYPE_WATER_EMACTIVEPOW			(TLVTAG_TYPEH_SINT|TLVTAG_TYPEL_NFD2)//��������sint.2
#define TLVTAG_WATER_EMREACTIVEPOW			(47)//���޹����� (KV.A)
#define TLVTYPE_WATER_EMREACTIVEPOW			(TLVTAG_TYPEH_SINT|TLVTAG_TYPEL_NFD2)//��������sint.2
#define TLVTAG_WATER_EMPOWERFACTOR			(48)//�������� ()
#define TLVTYPE_WATER_EMPOWERFACTOR			(TLVTAG_TYPEH_SSHORT|TLVTAG_TYPEL_NFD3)//��������sshort.3
#define TLVTAG_WATER_EMWATTHOUR				(49)//�й���� (KWH)
#define TLVTYPE_WATER_EMWATTHOUR			(TLVTAG_TYPEH_UINT|TLVTAG_TYPEL_NFD2)//��������uint.2
#define TLVTAG_WATER_EMVARHOUR				(50)//�޹���� (KV.AH)
#define TLVTYPE_WATER_EMVARHOUR				(TLVTAG_TYPEH_UINT|TLVTAG_TYPEL_NFD2)//��������uint.2
//ˮ�����
#define TLVTAG_WATER_WMFLOW					(51)//ˮ������ (m3)
#define TLVTYPE_WATER_WMFLOW				(TLVTAG_TYPEH_UINT|TLVTAG_TYPEL_NFD3)//��������uint.3
#define TLVTAG_WATER_WMPRESSURE				(52)//ˮѹ (Mpa)
#define TLVTYPE_WATER_WMPRESSURE			(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD3)//��������ushort.3
//�����-PLC�ɼ�
#define TLVTAG_WATER_PUMPRUNSTA				(53)//��������� ()
#define TLVTYPE_WATER_PUMPRUNSTA			(TLVTAG_TYPEH_UCHAR|TLVTAG_TYPEL_NFD0)//��������UCHAR.0
#define TLVTAG_WATER_PUMPVOLTAGE			(54)//�õ�ѹ (V)
#define TLVTYPE_WATER_PUMPVOLTAGE			(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//��������ushort.2
#define TLVTAG_WATER_PUMPCURRENT			(55)//�õ��� (A)
#define TLVTYPE_WATER_PUMPCURRENT			(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD3)//��������ushort.3
#define TLVTAG_WATER_PUMPRUNPOWER			(56)//�����й��� ()
#define TLVTYPE_WATER_PUMPRUNPOWER			(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//��������ushort.2
#define TLVTAG_WATER_PUMPFREQ				(57)//������Ƶ�� ()
#define TLVTYPE_WATER_PUMPFREQ				(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//��������ushort.2
#define TLVTAG_WATER_PUMPTEMP				(58)//�ù����¶� ()
#define TLVTYPE_WATER_PUMPTEMP				(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//��������ushort.2
#define TLVTAG_WATER_PUMPVIBRATION			(59)//����� ()
#define TLVTYPE_WATER_PUMPVIBRATION			(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//��������ushort.2
#define TLVTAG_WATER_PUMPRUNTIME			(60)//������ʱ�� ()
#define TLVTYPE_WATER_PUMPRUNTIME			(TLVTAG_TYPEH_UINT|TLVTAG_TYPEL_NFD2)//��������ushort.2
#define TLVTAG_WATER_PUMPWATERSPEED			(61)//��ˮ���� ()
#define TLVTYPE_WATER_PUMPWATERSPEED		(TLVTAG_TYPEH_UINT|TLVTAG_TYPEL_NFD3)//��������ushort.3
#define TLVTAG_WATER_PUMPINFLOW				(62)//�ý����ۻ����� ()
#define TLVTYPE_WATER_PUMPINFLOW			(TLVTAG_TYPEH_UINT|TLVTAG_TYPEL_NFD3)//��������ushort.3
#define TLVTAG_WATER_PUMPINPRESSURE			(63)//�ý���ѹ��()
#define TLVTYPE_WATER_PUMPINPRESSURE		(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD3)//��������ushort.3
#define TLVTAG_WATER_PUMPOUTFLOW			(64)//�ó����ۻ����� ()
#define TLVTYPE_WATER_PUMPOUTFLOW			(TLVTAG_TYPEH_UINT|TLVTAG_TYPEL_NFD3)//��������ushort.3
#define TLVTAG_WATER_PUMPOUTPRESSURE		(65)//�ó���ѹ��()
#define TLVTYPE_WATER_PUMPOUTPRESSURE		(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD3)//��������ushort.3
/***********����ҵ*************/
//���
#define TLVTAG_ELECTRIC_EMVA					(37)//��ѹVA (V)
#define TLVTYPE_ELECTRIC_EMVA					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//�������� ushort.2
#define TLVTAG_ELECTRIC_EMVB					(38)//��ѹVB (V)
#define TLVTYPE_ELECTRIC_EMVB					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//�������� ushort.2
#define TLVTAG_ELECTRIC_EMVC					(39)//��ѹVC (V)
#define TLVTYPE_ELECTRIC_EMVC					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//�������� ushort.2
#define TLVTAG_ELECTRIC_EMIA					(40)//����IA (A)
#define TLVTYPE_ELECTRIC_EMIA					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD3)//�������� ushort.3
#define TLVTAG_ELECTRIC_EMIB					(41)//����IB (A)
#define TLVTYPE_ELECTRIC_EMIB					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD3)//�������� ushort.3
#define TLVTAG_ELECTRIC_EMIC					(42)//����IC (A)
#define TLVTYPE_ELECTRIC_EMIC					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD3)//�������� ushort.3
#define TLVTAG_ELECTRIC_EMCT					(43)//CT
#define TLVTYPE_ELECTRIC_EMCT					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD0)//�������� ushort.0
#define TLVTAG_ELECTRIC_EMPT					(44)//PT
#define TLVTYPE_ELECTRIC_EMPT					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD0)//�������� ushort.0
#define TLVTAG_ELECTRIC_EMFREQ					(45)//Ƶ�� (HZ)
#define TLVTYPE_ELECTRIC_EMFREQ					(TLVTAG_TYPEH_USHORT|TLVTAG_TYPEL_NFD2)//��������sint.2
#define TLVTAG_ELECTRIC_EMACTIVEPOW				(46)//���й����� (KW)
#define TLVTYPE_ELECTRIC_EMACTIVEPOW			(TLVTAG_TYPEH_SINT|TLVTAG_TYPEL_NFD2)//��������sint.2
#define TLVTAG_ELECTRIC_EMREACTIVEPOW			(47)//���޹����� (KV.A)
#define TLVTYPE_ELECTRIC_EMREACTIVEPOW			(TLVTAG_TYPEH_SINT|TLVTAG_TYPEL_NFD2)//��������sint.2
#define TLVTAG_ELECTRIC_EMPOWERFACTOR			(48)//�������� ()
#define TLVTYPE_ELECTRIC_EMPOWERFACTOR			(TLVTAG_TYPEH_SSHORT|TLVTAG_TYPEL_NFD3)//��������sshort.3
#define TLVTAG_ELECTRIC_EMWATTHOUR				(49)//�й���� (KWH)
#define TLVTYPE_ELECTRIC_EMWATTHOUR				(TLVTAG_TYPEH_UINT|TLVTAG_TYPEL_NFD2)//��������uint.2
#define TLVTAG_ELECTRIC_EMVARHOUR				(50)//�޹���� (KV.AH)
#define TLVTYPE_ELECTRIC_EMVARHOUR				(TLVTAG_TYPEH_UINT|TLVTAG_TYPEL_NFD2)//��������uint.2


typedef struct
{
	uint8_t port;
	uint8_t channel;
	uint8_t tag;
	uint8_t numafterdecimal;
}TLV_PROP_DEF;
typedef struct
{
	uint8_t buff[TLVBUFFSIZE];
	uint32_t length;
}TLV_DEF;

//CRC8У���
static const uint8_t CRC8_TAB[]=	{
	0x00,0x5e,0xbc,0xe2,0x61,0x3f,0xdd,0x83,//0~7
	0xc2,0x9c,0x7e,0x20,0xa3,0xfd,0x1f,0x41,//8~15
	0x9d,0xc3,0x21,0x7f,0xfc,0xa2,0x40,0x1e,//16_23
	0x5f,0x01,0xe3,0xbd,0x3e,0x60,0x82,0xdc,//24~31
	0x23,0x7d,0x9f,0xc1,0x42,0x1c,0xfe,0xa0,//32~39
	0xe1,0xbf,0x5d,0x03,0x80,0xde,0x3c,0x62,//40~47
	0xbe,0xe0,0x02,0x5c,0xdf,0x81,0x63,0x3d,//48~55
	0x7c,0x22,0xc0,0x9e,0x1d,0x43,0xa1,0xff,//56~63
	0x46,0x18,0xfa,0xa4,0x27,0x79,0x9b,0xc5,//64~71
	0x84,0xda,0x38,0x66,0xe5,0xbb,0x59,0x07,//72~79
	0xdb,0x85,0x67,0x39,0xba,0xe4,0x06,0x58,//80~87
	0x19,0x47,0xa5,0xfb,0x78,0x26,0xc4,0x9a,//88~95
	0x65,0x3b,0xd9,0x87,0x04,0x5a,0xb8,0xe6,//96~103
	0xa7,0xf9,0x1b,0x45,0xc6,0x98,0x7a,0x24,//104~111
	0xf8,0xa6,0x44,0x1a,0x99,0xc7,0x25,0x7b,//112~119
	0x3a,0x64,0x86,0xd8,0x5b,0x05,0xe7,0xb9,//120~127
	0x8c,0xd2,0x30,0x6e,0xed,0xb3,0x51,0x0f,//128~135
	0x4e,0x10,0xf2,0xac,0x2f,0x71,0x93,0xcd,//136~143
	0x11,0x4f,0xad,0xf3,0x70,0x2e,0xcc,0x92,//144~151
	0xd3,0x8d,0x6f,0x31,0xb2,0xec,0x0e,0x50,//152~159
	0xaf,0xf1,0x13,0x4d,0xce,0x90,0x72,0x2c,//160~167
	0x6d,0x33,0xd1,0x8f,0x0c,0x52,0xb0,0xee,//168~175
	0x32,0x6c,0x8e,0xd0,0x53,0x0d,0xef,0xb1,//176~183
	0xf0,0xae,0x4c,0x12,0x91,0xcf,0x2d,0x73,//184~191
	0xca,0x94,0x76,0x28,0xab,0xf5,0x17,0x49,//192~199
	0x08,0x56,0xb4,0xea,0x69,0x37,0xd5,0x8b,//200~207
	0x57,0x09,0xeb,0xb5,0x36,0x68,0x8a,0xd4,//208~215
	0x95,0xcb,0x29,0x77,0xf4,0xaa,0x48,0x16,//216~223
	0xe9,0xb7,0x55,0x0b,0x88,0xd6,0x34,0x6a,//224~231
	0x2b,0x75,0x97,0xc9,0x4a,0x14,0xf6,0xa8,//232~239
	0x74,0x2a,0xc8,0x96,0x15,0x4b,0xa9,0xf7,//240~247
	0xb6,0xe8,0x0a,0x54,0xd7,0x89,0x6b,0x35,//248~255
};

#endif
