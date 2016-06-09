// pcapAnalyze.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "pcapAnalyze.h"
#include "pcapToSip.h"
using namespace std;

//ʮ������IPת��ʮ����IP���ĵ�ַ��ʾ
string PCAP::print_ip(u_int32 ip)
{
	u_int8 bytes[4];
    string ip_str;
    char ip_cstr[16];

    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    sprintf_s(ip_cstr, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
    ip_str = ip_cstr;
    return ip_str;
}

//��32λ�����ֵ�����
u_int32 PCAP::swapInt32(u_int32 value)
{
     return ((value & 0x000000FF) << 24) |
            ((value & 0x0000FF00) << 8)  |
            ((value & 0x00FF0000) >> 8)  |
            ((value & 0xFF000000) >> 24) ;
}

//��16λ�����ֵ�����
u_int16 PCAP::swapInt16(u_int16 value)
{
     return ((value & 0x00FF) << 8) |
            ((value & 0xFF00) >> 8) ;
}



//��ʼ������������ڴ�
void PCAP::init()
{
	//��հ�����
	package_list.clear();

	//�����Ԫ��
	package_group.clear();
}


 PCAP:: PCAP()
{

}

 PCAP::~ PCAP()
{

}

//pcap������
int PCAP::fileInput(string path, string pcapFile)
{
	ifstream in;

	if(_access((path + "\\result").c_str(), 0) == 0)
	{
        //system("RD /s/q result");
		if(_access((path + "\\result\\tcp").c_str(), 0) == 0)
			system(("RD /s/q " + path + "\\result\\tcp").c_str());

		if(_access((path + "\\result\\udp").c_str(), 0) == 0)
			system(("RD /s/q " + path + "\\result\\udp").c_str());

		if(_access((path + "\\result\\tcpstruct").c_str(), 0) == 0)
			system(("RD /s/q " + path + "\\result\\tcpstruct").c_str());

		if(_access((path + "\\result\\udpstruct").c_str(), 0) == 0)
			system(("RD /s/q " + path + "\\result\\udpstruct").c_str());

		if(_access((path + "\\result\\data").c_str(), 0) == 0)
			system(("RD /s/q " + path + "\\result\\data").c_str());

		if(_access((path + "\\result\\sip-rtp-struct").c_str(), 0) == 0)
			system(("RD /s/q " + path + "\\result\\sip-rtp-struct").c_str());

		if(_access((path + "\\result\\sip-rtp-data").c_str(), 0) == 0)
			system(("RD /s/q " + path + "\\result\\sip-rtp-data").c_str());
	}
	else
	{
		_mkdir((path + "\\result").c_str());
	}
    _mkdir((path + "\\result\\tcp").c_str());
    _mkdir((path + "\\result\\udp").c_str());
	_mkdir((path + "\\result\\data").c_str());
    _mkdir((path + "\\result\\tcpstruct").c_str());
    _mkdir((path + "\\result\\udpstruct").c_str());
	_mkdir((path + "\\result\\sip-rtp-struct").c_str());
    _mkdir((path + "\\result\\sip-rtp-data").c_str());

    in.open(pcapFile, ios::in | ios::binary);
    if(!in.is_open()) 
	{
        //system("pause");
        return 0;
    }

	//����pcap��ͷ, fhΪpcap��ͷ
	if (in.read((char*)&pfh, sizeof(pfh)).gcount()!=sizeof(pfh)) return 0;

	//���ݰ�vector
    while(!in.eof())
	{
        package pg;
		dataHeader ph;

		//�������ݰ�ͷ
		int temp = (int) in.read((char*)&ph, sizeof(ph)).gcount();
		if (temp == 0) break;
		else
			if (temp != sizeof(ph)) return 0;
		pg.ph = ph;

		const u_int32 total_len = static_cast<u_int32>(sizeof(pcap_data) + ph.caplen);
        pg.pd = static_cast<pcap_data*>(::malloc(total_len));
		pg.pd->len = ph.caplen;
		in.read((char*)pg.pd->data, ph.caplen);

        package_list.push_back(pg);
    }
    in.close();

	return 1;
}

//pcap������
void PCAP::pcapDivideIntoGroups()
{
    
    for(size_t i = 0; i < package_list.size(); i++) 
	{
		//�����ж��ǲ�����̫��֡,ʮ����Ϊ2048��ʮ������0800
		u_int16 frameType;
		memcpy(&frameType, &package_list[i].pd->data[12], 2);
		frameType = swapInt16(frameType);

		//TCPͷ8λЭ��Ϊ6��UDPͷ8λЭ��Ϊ17
		if ((frameType == 2048) && ((int) package_list[i].pd->data[23] == 6 || (int) package_list[i].pd->data[23] == 17))
		{
			five f;
			u_int8  ipLen;
			u_int32 seq;
			u_int32 ack;
			u_int32 srcIP;
			u_int32 dstIP;

			//Э�����ͣ�Դ��ַ��Ŀ�ĵ�ַ
			f.protcol = package_list[i].pd->data[23];
			memcpy(&f.srcIP, &package_list[i].pd->data[26], 4);
			memcpy(&f.dstIP, &package_list[i].pd->data[30], 4);

			//IPͷ�ײ�����
			memcpy(&ipLen, &package_list[i].pd->data[14], 1);
			ipLen = (ipLen & 0xF) << 0x2;

			//Դ�˿ڣ�Ŀ�Ķ˿�
			memcpy(&f.srcPort, &package_list[i].pd->data[34 + ipLen - 20], 2);
			f.srcPort = swapInt16(f.srcPort);
			memcpy(&f.dstPort, &package_list[i].pd->data[36 + ipLen - 20], 2);
			f.dstPort = swapInt16(f.dstPort);

			//������
			srcIP = swapInt32(f.srcIP);
			dstIP = swapInt32(f.dstIP);
			if (srcIP > dstIP)
			{
				swap(f.srcIP, f.dstIP);
				swap(f.srcPort, f.dstPort);
				package_list[i].direction = 1;
			}
			else
			{
				package_list[i].direction = 0;
			}

			//TCPЭ��
			if((int) package_list[i].pd->data[23] == 6 )
			{
				//seq, ack
				memcpy(&seq, &package_list[i].pd->data[38 + ipLen - 20], 4);
				seq = swapInt32(seq);
				memcpy(&ack, &package_list[i].pd->data[42 + ipLen - 20], 4);
				ack = swapInt32(ack);

				package_list[i].pd->seq = seq;
				package_list[i].pd->ack = ack;
			}

			//TCPЭ��Ϊ06 UDPЭ��Ϊ11
			if((int) package_list[i].pd->data[23] == 6 || (int) package_list[i].pd->data[23] == 17 ) 
			{
				package_group[f].push_back(package_list[i]);
			}
		}
    }
}


//����pcap�����
void PCAP::groupsOutput(string path)
{
	if (package_group.empty()) return;
	
	map<five, vector<package> >::iterator it;
    for(it = package_group.begin(); it != package_group.end(); ++it) 
	{
        ofstream pcap;
        string file_name;
        string protcol;
		string dir;
        ostringstream srcPort, dstPort;

		srcPort << dec << (*it).first.srcPort;
		dstPort << dec << (*it).first.dstPort;
        if((*it).first.protcol == 6) 
		{
			//TCP���ݰ�����
			sort((*it).second.begin(), (*it).second.end());
            protcol = "TCP";
            dir = path + "\\result\\tcp\\";
        }
		else 
		{
            protcol = "UDP";
            dir = path + "\\result\\udp\\";
        }

		file_name = protcol + "[" + print_ip((*it).first.srcIP) + "][" + srcPort.str() + "][" + print_ip((*it).first.dstIP) + "][" + dstPort.str() + "].pcap";
        pcap.open(dir + file_name, ios::out | ios::binary);
		
		//д��pcapͷ
        pcap.write((char*)&pfh, sizeof(pfh));

		//д�����ݰ�
        vector<package>::iterator it2;
        for(it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) 
		{
			pcap.write((char*)&(*it2).ph, sizeof(dataHeader));
            pcap.write((char*)(*it2).pd->data, (*it2).pd->len);
        }
        pcap.close();
    }
}

//�ṹ����Ϣ���
void PCAP::structOutput(string path)
{
	if (package_group.empty()) return;
	
	map<five, vector<package> >::iterator it;
    for(it = package_group.begin(); it != package_group.end(); ++it) 
	{
        ofstream structData;
		//FILE* structData1 = NULL;
        string struct_name;
        string protcol;
		string dir;
        ostringstream srcPort, dstPort;

		srcPort << dec << (*it).first.srcPort;
		dstPort << dec << (*it).first.dstPort;
        if((*it).first.protcol == 6) 
		{
            protcol = "TCP";
            dir = path + "\\result\\tcpstruct\\";
        }
		else 
		{
            protcol = "UDP";
            dir = path + "\\result\\udpstruct\\";
        }

		struct_name = protcol + "[" + print_ip((*it).first.srcIP) + "][" + srcPort.str() + "][" + print_ip((*it).first.dstIP) + "][" + dstPort.str() + "]-.txt";
		structData.open(dir + struct_name, ios::out | ios::binary);
		
		//�����Ԫ����Ϣ
		structData << "��Ԫ����Ϣ��" << endl;
		structData << "[" << endl;
		structData << "	[protcol]:[" << protcol << "]" << endl;
		structData << "	[srcIP]:[" << print_ip((*it).first.srcIP) << "]" << endl;
		structData << "	[dstIP]:[" << print_ip((*it).first.dstIP) << "]" << endl;
		structData << "	[srcPort]:[" << srcPort.str() << "]" << endl;
		structData << "	[dstPort]:[" << dstPort.str() << "]" << endl;
		structData << "]" << endl;
		structData << endl << endl;

		//������Ľṹ����Ϣ
		structData << "���ݰ���Ϣ��" << endl;
        vector<package>::iterator it2;
        for(it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) 
		{
			structData << "[" << endl;
			if((*it).first.protcol == 6)
			{
				structData << "	[seq]:[" << (*it2).pd->seq << "]" << endl;
				structData << "	[ack]:[" << (*it2).pd->ack << "]" << endl;
			}
			structData << "	[len]:[" << (*it2).pd->len << "]" << endl;
			structData << "	[direction]:[" << (*it2).direction << "]" << endl;
			structData << "]" << endl;
			structData << endl << endl;
        }
		structData.close();
    }
}

//������Ϣ���
void PCAP::informationOutput(string path)
{
	if (package_group.empty()) return;
	
	map<five, vector<package> >::iterator it;
    for(it = package_group.begin(); it != package_group.end(); ++it) 
	{
        ofstream data;
        string data_name;
        string protcol;
		string dir;
        ostringstream srcPort, dstPort;

		srcPort << dec << (*it).first.srcPort;
		dstPort << dec << (*it).first.dstPort;
        if((*it).first.protcol == 6)
		{
            protcol = "TCP";
            dir = path + "\\result\\data\\";
        }
		else 
		{
            protcol = "UDP";
            dir = path + "\\result\\data\\";
        }

		data_name = protcol + "[" + print_ip((*it).first.srcIP) + "][" + srcPort.str() + "][" + print_ip((*it).first.dstIP) + "][" + dstPort.str() + "].txt";
        data.open(dir + data_name, ios::out | ios::binary);
	
		//д�븺����Ϣ
		bool direction = 0;
        vector<package>::iterator it2;
        for(it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) 
		{
			u_int16 udpLen,totalLen;
			u_int8  ipLen, tcpLen;
			int headerLen, dataLen;

			//����
			if (it2 != (*it).second.begin())
			{
				if ((*it2).direction != direction)
					data << endl << endl << endl << endl;
			}
			direction = (*it2).direction;

			//IPͷ�ײ�����
			memcpy(&ipLen, &(*it2).pd->data[14], 1);
			ipLen = (ipLen & 0xF) << 0x2;

			//IP+TCP/UDP+������Ϣ ����
			memcpy(&totalLen, &(*it2).pd->data[16], 2);
			totalLen = swapInt16(totalLen);

			
			if((*it).first.protcol == 6)
			{
				//TCPͷ�ײ�����
				memcpy(&tcpLen, &(*it2).pd->data[46 + ipLen - 20], 1);
				tcpLen = (tcpLen >> 0x4) << 0x2;
				headerLen = 14 + ipLen + tcpLen;
				dataLen = totalLen - ipLen - tcpLen;
			}
			else
			{
				memcpy(&udpLen, &(*it2).pd->data[38 + ipLen - 20], 2);
				udpLen = swapInt16(udpLen);
				headerLen = 14 + ipLen + 8;
				dataLen = udpLen - 8;
			}

			//д�븺����Ϣ
			data.write((char*)(*it2).pd->data + headerLen, dataLen);
        }
        data.close();
    }
}


int PCAP::start(string path, string pcapFile)
{
	init();
	if (fileInput(path, pcapFile))
	{
		pcapDivideIntoGroups();
		groupsOutput(path);
		structOutput(path);
		informationOutput(path);
	}
	else
	{
		//cout << "failed" << endl;
		return -1;
	}
	return 0;
}

//����

/*
int main()
{
	string path= "F:\\data";;
	string pcapFile = "F:\\data\\MagicJack+_short_call.pcap";

	SIP sip;
	cout << sip.start(path,pcapFile) << endl;

	system("pause");
	return 0;
}
*/