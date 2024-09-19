#ifndef BLOCK_DEVICE_H
#define BLOCK_DEVICE_H

#include <iostream>
#include <fstream>
#include <string>

#include "Buf.h"
#include "Logger.h"

using namespace std;


/*
 * ���豸���࣬������豸���Ӵ˻���̳С�
 */
class BlockDevice
{
public:
	virtual ~BlockDevice() {};
	/*
	 * ����Ϊ�麯���������������overrideʵ���豸
	 * �ض���������������£������к�����Ӧ�����õ���
	 */
	virtual int Strategy(Buf* bp) = 0;
};


/* һ���ļ������豸�����ࡣ�ӿ��豸����BlockDevice�̳ж����� */
class VirtualFileDevice : public BlockDevice
{
public:
	static const int SECTOR_SIZE = BUF_SIZE;
	static const int DEVICE_MEMORY = 40 << 20;
	static const int NSECTOR = DEVICE_MEMORY / SECTOR_SIZE;

	VirtualFileDevice(string filename);
	~VirtualFileDevice();
	/*
	 * Override����BlockDevice�е��麯����ʵ��
	 * ������VirtualFileDevice�ض����豸�����߼���
	 */
	int Strategy(Buf* bp);

private:
	string filename;
	fstream file;

	int Bno2Addr(int bno);
	int Read(Buf* bp);
	int Write(Buf* bp);
};

#endif
