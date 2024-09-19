#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include "BlockDevice.h"
#include "Logger.h"

class DeviceManager
{
	/* static const member */
public:
	static const int MAX_DEVICE_NUM = 10;		/* ϵͳ���������豸���� */
	static const int NODEV = -1;				/* NODEV�豸�� */
	static const short ROOTDEV = (0 << 8) | 0;	/* ���̵��������豸�Ŷ�Ϊ0 */

public:
	DeviceManager();
	~DeviceManager();

	int GetNBlkDev();							/* ��ȡϵͳ��ʵ�ʿ��豸����nblkdev */
	BlockDevice& GetBlockDevice(short major);	/* �������豸��major��ȡ��Ӧ���豸����ʵ�� */


private:
	int nblkdev;							/* ϵͳ�п��豸������ */
	BlockDevice* bdevsw[MAX_DEVICE_NUM];	/* ָ����豸�����ָ�����飬�൱��Unix V6�п��豸���ر� */
};

#endif
