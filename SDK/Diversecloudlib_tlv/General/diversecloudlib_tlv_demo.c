/*
* DiverseCloudLIB_TLV.c
*
*  Created on: 2019��5��29��
*      Author: shb
*     version: 1.1
*/

#include <stdio.h>
#include <string.h>
#include "diversecloudlib_tlv.h"
#include "diversecloudlib_tmpl_dev_v000.h"

uint8_t g_TlvBuff[TLV_PROP_BUFFSIZE];

/*************************************
*ʹ����ʾ
*************************************/
void main()
{
	TLV_ERRORCODE errorCode;
	uint8_t customerAddr[] = { 1,2,3,4,5,6,7,8 };//��ʶ��ַ������³��ʶ��
	uint8_t customerAddrLen = 8;//��ʶ��ַ������³��ʶ������
	TLV_PROP_DEF tlv_prop;
	TLV_MSG_DEF tlv_msg;
	uint8_t i = 0;

	/*step1 ��ֵ���Գ�ʼ��*************/
	tlv_prop.BO = tlv_prop_byteorder_check();//���ݸ�ʽ��⣬tlv_prop_byteorder_check()����ֵ��BYTEORDER_BE����˸�ʽ����BYTEORDER_LE��С�˸�ʽ��
	tlv_prop.Buff = g_TlvBuff;//ָ��tlv��Ϣ��������ַ
	tlv_prop.BuffSize = TLV_PROP_BUFFSIZE;//tlv��Ϣ��������С	
	tlv_prop.AddressType = TLV_PROP_ADDRESSTYPE_YL;//��ʶ��ַ����;TLV_PROP_ADDRESSTYPE_YL:��ʶ��ַ(��³) ;TLV_PROP_ADDRESSTYPE_CUSTOMER:��ʶ��ַ(����³)
	tlv_prop.YLAddress = tlv_prop_yladdress_set(56, 44, 33, 0); //��ʶ��ַ����һ������³��ʶ��56.44.33.0	
	//tlv_prop.CustomerAddress = tlv_prop_customeraddress_set(customerAddr, customerAddrLen); ;//��ʶ��ַ���Ͷ���������³��ʶ��0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08
	tlv_prop.DateTime = tlv_prop_datetime_set(19, 5, 20, 12, 34, 56);//��Ϣ�ϱ�ʱ�����ã�����һ��2019-05-20 12:34:56
	//tlv_prop.DateTime = tlv_prop_datetime_set_timestamp(1558971098);//��Ϣ�ϱ�ʱ�����ã���������ʱ���(S)��ʽ��
	//tlv_prop.DateTime = tlv_prop_datetime_set_serverdatetime();//��Ϣ�ϱ�ʱ�����ã���������ʹ�÷���������ʱ��
	if (TLV_SUCCESS != tlv_msg_init(&tlv_prop))//��Ϣ���Գ�ʼ��
		while (1);

	/*step2 �����Ϣ���ݣ���ע����Ҫ�㹻�Ļ���ռ��ʹ�÷��ؼ��*************/
	//exp:����2�����ݣ����A���ѹ232.45�����B���ѹ232.45
	tlv_msg.Port = 2;//��Ϣ���������˿ں�
	tlv_msg.Channel = 1;//��Ϣ��������ͨ���ż�IO��
	tlv_msg.Tag = TLV_MSGTAG_DEV_EMETER_VPHASEA;//��Ϣ����TAG��ֵ���ο�TLV_MSGTAG_XXX_XXX_XXX��TLV_MSGTAG_DEV_EMETER_VPHASEA
	tlv_msg.Type = TLV_MSGTYPE_DEV_EMETER_VPHASEA;//��Ϣ�������ͣ��ο�TLV_MSGTYPE_XXX_XXX_XXX��TLV_MSGTYPE_DEV_EMETER_VPHASEA
	tlv_msg.Value = 232.44;//��Ϣ����ֵ,��ע����ֵ����Χ����ʹ�÷��ؼ�⡣
	if(TLV_SUCCESS != tlv_msg_Add(&tlv_prop, &tlv_msg))//���ӵ�һ����Ϣ����
		while(1);//��ʧ�������³�ʼ����tlv_msg_init

	tlv_msg.Port = 2;
	tlv_msg.Channel = 1;
	tlv_msg.Tag = TLV_MSGTAG_DEV_EMETER_VPHASEB;
	tlv_msg.Type = TLV_MSGTYPE_DEV_EMETER_VPHASEB;
	tlv_msg.Value = 230;
	if (TLV_SUCCESS != tlv_msg_Add(&tlv_prop, &tlv_msg))//���ӵڶ�����Ϣ����
		while (1);//��ʧ�������³�ʼ����tlv_msg_init
	
	/*step3 �����Ϣ֡*************/
	tlv_msg_finish(&tlv_prop);

	for (i = 0; i<tlv_prop.Length; i++)
		printf("%02x ", tlv_prop.Buff[i]);
	while (1);
}

