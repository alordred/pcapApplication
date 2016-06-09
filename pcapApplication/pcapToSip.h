#pragma once

#include "pcapAnalyze.h"

//sip�ṹ����Ϣ
struct sipHeader
{
	u_int32	srcIP;			//sip�Ự��ԴIP��ַ ������ָ�м������
	u_int32	dstIP;			//sip�Ự��Ŀ��IP��ַ ������ָ�м������
	u_int16	srcMediaPort;	//RTP�����Դ�˿�
	u_int16	dstMediaPort;	//RTP�����Ŀ�Ķ˿�

	bool INVITE_OK;			//�Ƿ��յ�INVITE��OK����
	bool BYE;				//�Ƿ��ͽ�����Ϣ
	bool BYE_OK;			//�Ƿ��յ������ķ���

	bool operator<(const sipHeader& __x) const
	{
		return (srcIP + dstIP + srcMediaPort + dstMediaPort) <
			(__x.srcIP + __x.dstIP + __x.srcMediaPort+__x.dstMediaPort);
	}

};

//RTP�ļ�ͷ�ṹ��
struct rtpHeader
{
	/*
	�汾�ţ�V����2���أ�������־ʹ�õ�RTP�汾��
	���λ��P����1���أ������λ��λΪ1�����RTP����β���Ͱ������ӵ�����ֽڡ�
	��չλ��X����1���أ������λ��λΪ1����RTP�̶�ͷ������͸���һ����չͷ����
	CSRC��������CC����4���أ����й̶�ͷ��������ŵ�CSRC����Ŀ��
	*/
	u_int8	stream;
	u_int8	pt;			//��������
	u_int16 sn;			//���кţ������������
	u_int32 timeStamp;	//ʱ���
	u_int32 ssrc;		//ͬ��Դ��ʶ��
	//u_int8  payLoad[0];	//������Ϣ
	u_int8  *payLoad;	//������Ϣ
	size_t  payLoadLen;	//����
	bool	direction;  //��ʶ����, 0, С�ĵ���ģ� 1�� ��ĵ�С��

	//����sn����
	bool operator<(const rtpHeader& __x) const 
	{
		if (direction < __x.direction) return true;
		if (direction > __x.direction) return false;
		return (sn < __x.sn);
	}
};

//auͷ https://en.wikipedia.org/wiki/Au_file_format
struct auHeader
{
	u_int32 magicNum;	//the value 0x2e736e64 (four ASCII characters ".snd")
	u_int32 dataOffset;	//the offset to the data in bytes, must be divisible by 8.
	u_int32 dataSize;	//data size in bytes. If unknown, the value 0xffffffff should be used.
	u_int32 encoding;	//Data encoding format:
	u_int32 sampleRate;	//the number of samples/second, e.g., 8000
	u_int32 channel;	//the number of interleaved channels, e.g., 1 for mono, 2 for stereo; more channels possible, but may not be supported by all readers.
};

class SIP:PCAP
{
public:
	SIP();
	~SIP();
	
	int start(string path, string pcapFile);

private:
	//��CALLID��ȷ���Ự,������Ϣ�������˶���
	map<string, sipHeader> CALLID2SIP;
	//�ɶ˿���Ϣ��ȷ��CALLID
	map<sipHeader, string> SIP2CALLID;
	//��CALLID��ȷ����һ��RTPͨ�����������˫����ͨ������Ҫ����srcIP��dstIP��ȡ
	map<string, vector<rtpHeader>> RTP_group;

	/*�ж��Ƿ�ΪSIPЭ��
	  -1:����SipЭ��
	  0:ack
	  1:invite
	  2:bye
	  3:1xx ��ʱӦ���ѽ���
	  4:2xx �ɹ����Ѿ���ȷ����
	  5:4xx ʧ��
	*/
	int isSip(u_int8 *data, size_t dataLen);

	//�������
	void init();

	//�ж��Ƿ�ΪRTPЭ��
	bool isRtp(size_t i, string & CALLID, bool & direction);

	////ʮ������IPת��ʮ����IP���ĵ�ַ��ʾ
	string print_ip(u_int32 ip);

	//��ip1.ip2.ip3.ip4ת��Ϊu_int32
	u_int32 ip2u_Int32(u_int8 *data, size_t l, size_t r);

	//�Ѷ˿�ת��Ϊ������Ϣ
	u_int16 SIP::port2u_Int16(u_int8 *data, size_t l, size_t r);

	//����sip��Ϣ
	int sipInformationOutput(string path);

	//����ṹ����Ϣ
	void structOutput(string path);

	//����CALLID
	string findCALLID(u_int8 *data, size_t dataLen);

	//����srcIP����dstIP
	u_int32 findSrcIPOrDstIP(u_int8 *data, size_t dataLen);

	//����srcPort����dstPort
	u_int16 findSrcPortOrDstPort(u_int8 *data, size_t dataLen);

	//���룬����
	//int fileInput(string path, string pcapFile);

	//����������Ϣ
	void analyzePayLoad(string path, string CALLID);
};