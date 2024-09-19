#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include "Buf.h"
#include "DeviceManager.h"

class BufferManager
{
public:
	/* static const member */
	static const int NBUF = 15;					/* ������ƿ顢������������ */
	static const int BUFFER_SIZE = BUF_SIZE;	/* ��������С�� ���ֽ�Ϊ��λ */

public:
	BufferManager();
	~BufferManager();

	Buf* GetBlk(short dev, int blkno);	/* ����һ�黺�棬���ڶ�д�豸dev�ϵ��ַ���blkno��*/
	void Brelse(Buf* bp);				/* �ͷŻ�����ƿ�buf */

	Buf* Bread(short dev, int blkno);	/* ��һ�����̿顣devΪ�������豸�ţ�blknoΪĿ����̿��߼���š� */
	void Bwrite(Buf* bp);				/* дһ�����̿� */
	void Bawrite(Buf* bp);				/* �첽д */
	void Bdwrite(Buf* bp);				/* �ӳ�д */
	void ClrBuf(Buf* bp);				/* ��ջ��������� */
	void Bflush(short dev);				/* ��devָ���豸�������ӳ�д�Ļ���ȫ����������� */
	Buf& GetBFreeList();				/* ��ȡ���ɻ�����п��ƿ�Buf�������� */
	void ShowBFreeList();				/* ��ӡBFreeList������Bufno(Debug��) */

private:
	void NotAvail(Buf* bp);				/* �����ɶ�����ժ��ָ���Ļ�����ƿ�buf */

private:
	Buf bFreeList;								/* ���ɻ�����п��ƿ� */
	Buf m_Buf[NBUF];							/* ������ƿ����� */
	unsigned char Buffer[NBUF][BUFFER_SIZE];	/* ���������� */

	DeviceManager* m_DeviceManager;		/* ָ���豸����ģ��ȫ�ֶ��� */
};

#endif
