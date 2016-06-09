#pragma once

#include "stdafx.h"
using namespace std;

typedef unsigned long	u_int32;
typedef unsigned short	u_int16;
typedef unsigned char	u_int8;

//pcap�ļ�ͷ�ṹ��,24��byte
struct fileHeader
{
	u_int32 magic;			// ��ʶλ�������ʶλ��ֵ��16���Ƶ� 0xa1b2c3d4��4���ֽڣ�
	u_int16 magorVersion;	// ���汾�ţ�2���ֽڣ�
    u_int16	minorVersion;	// ���汾�ţ�2���ֽڣ�
	u_int32 timezone;       // ����ʱ�䣨4���ֽڣ�
	u_int32 sigflags;       // ��ȷʱ�����4���ֽڣ�
    u_int32 snaplen;        // ���ݰ���󳤶ȣ�4���ֽڣ�
	u_int32 linktype;       // ��·�����ͣ�4���ֽڣ�

};

//pcap�ļ��е����ݰ�ͷ,16��byte
struct dataHeader
{
	/**
     * ʱ������룩����¼���ݰ�ץ���ʱ��
     * ��¼��ʽ�ǴӸ�������ʱ���1970��1��1�� 00:00:00 ��ץ��ʱ������������4���ֽڣ�
     */
	u_int32 timeS;  
    /**
     * ʱ�����΢�룩��ץȡ���ݰ�ʱ��΢��ֵ��4���ֽڣ�
     */
    u_int32 timeMs;                     
    /**
     * ���ݰ����ȣ���ʶ��ץ������ݰ������� pcap �ļ��е�ʵ�ʳ��ȣ����ֽ�Ϊ��λ��4���ֽڣ�
     */
    u_int32 caplen;
    /**
     * ���ݰ�ʵ�ʳ��ȣ� ��ץ������ݰ�����ʵ���ȣ�4���ֽڣ�
     * ����ļ��б��治�����������ݰ�����ô���ֵ����Ҫ��ǰ������ݰ����ȵ�ֵ��
     */
	u_int32 len;                        
};

//����֡ͷ����̫��֡�����ܻ�Я���ӳ���Ϣ,14��byte
struct frameHeader
{
	u_int8	dstMac[6];		//Ŀ��MAC��ַ
	u_int8	srcMac[6];		//ԴMAC��ַ
	u_int16 frameType;		//����֡����
};

//IP���ݰ�,��С����Ϊ20byte
struct ipHeader
{
	/*
	�汾+��ͷ����
	Э��汾��(4 bit)����ͷ����(4bit) =��1 �ֽ�)
	�汾��(Version):һ���ֵΪ0100��IPv4����0110��IPv6��
	*/
	u_int8	varHLen;
    u_int8	tos;			//��������
	u_int16	totalLen;       //�ܳ���
	u_int16	id;				//��ʶ
	u_int16	flagSegment;    //��־+Ƭƫ��
	u_int8	ttl;            //��������
	u_int8	protocol;       //Э������
	u_int16	checkSum;       //ͷ��У���
	u_int32	srcIP;			//ԴIP��ַ
	u_int32	dstIP;			//Ŀ��IP��ַ
};

//TCP���ݰ�����С����Ϊ20byte
struct tcpHeader
{
	u_int16	srcPort;		//Դ�˿�
	u_int16	dstPort;		//Ŀ�Ķ˿�
	u_int32	seq;			//���
	u_int32	ack;			//ȷ�Ϻ�
	u_int8	headerLen;		//���ݱ�ͷ�ĳ���(4 bit) + ����(4 bit)
	u_int8	flags;			//��ʶTCP��ͬ�Ŀ�����Ϣ
	u_int16	window;			//���ڴ�С
	u_int16	checkSum;		//У���
	u_int16	urgentPointer;  //����ָ��
};

//UDP���ݰ�����С����Ϊ8byte
struct udpHeader
{
	u_int16 srcPort;		//Դ�˿�
	u_int16 dstPort;        //Ŀ�Ķ˿�
	u_int16 len;			//���ݰ���
	u_int16 checkSum;		//У���
};

//���ݰ���Ϣ
struct pcap_data
{
	u_int32 len;		    //������ܳ��ȣ����ܴ���padding
	u_int32 seq;			//seq
	u_int32 ack;			//ack
	u_int8  data[0];		//�����÷����������ݰ�
};

//���ݰ�
struct package
{
	bool direction;		    //��С��ip�������ip����Ϊ0��������Ϊ1
	dataHeader ph;
    pcap_data *pd;

	bool operator<(const package& __x) const 
	{
		return (pd->ack + pd->seq) < 
			(__x.pd->ack + __x.pd->seq);
	}
};

//��Ԫ��
struct five
{
    u_int8	protcol;
	u_int32 srcIP;
	u_int32 dstIP;
	u_int16 srcPort;
	u_int16 dstPort;

    bool operator<(const five& __x) const 
	{
		return (srcIP + dstIP + srcPort + dstPort) <
			(__x.srcIP + __x.dstIP + __x.srcPort+__x.dstPort);
    }
};

class  PCAP
{
public:
	 PCAP();
	~ PCAP();
	int start(string path, string pcapFile);

protected:
	map<five, vector<package>> package_group;
	vector<package> package_list;
	fileHeader pfh;

	//ʮ������IPת��ʮ����IP���ĵ�ַ��ʾ
	string print_ip(u_int32 ip);

	//��32λ�����ֵ�����
	u_int32 swapInt32(u_int32 value);

	//��16λ�����ֵ�����
	u_int16 swapInt16(u_int16 value);

	//��ʼ������������ڴ�
	void init();

	//pcap������
	int fileInput(string path, string pcapFile);

	//pcap������
	void pcapDivideIntoGroups();

	//����pcap�����
	void groupsOutput(string path);

	//�ṹ����Ϣ���
	void structOutput(string path);

	//������Ϣ���
	void informationOutput(string path);
};