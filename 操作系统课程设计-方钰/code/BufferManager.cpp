#include "BufferManager.h"
#include "OSKernel.h"

BufferManager::BufferManager()
{
	this->m_DeviceManager = &OSKernel::Instance().GetDeviceManager();
	bFreeList.b_back = &bFreeList;
	bFreeList.b_forw = &bFreeList;

	for (int i = 0; i < NBUF; i++) {
		Buf* bp = &(m_Buf[i]);
		bp->b_addr = Buffer[i];
		bp->b_bufno = i;
		bp->b_dev = -1;
		bp->b_forw = bFreeList.b_forw;
		bp->b_back = &bFreeList;
		bFreeList.b_forw->b_back = bp;
		bFreeList.b_forw = bp;
	}
}

BufferManager::~BufferManager()
{
	this->Bflush(0);
}

/* Debug�� �鿴BFreeList����Buffer������ */
void BufferManager::ShowBFreeList()
{
	Buf* bp;
	int cnt = 0;
	for (bp = this->bFreeList.b_forw; bp != &(this->bFreeList); bp = bp->b_forw)
	{
		cout << bp->b_bufno << "(" << bFreeList.b_back->b_bufno << ") ";
		cnt++;
	}
	cout << cnt << endl;
}

Buf* BufferManager::GetBlk(short dev, int blkno)
{
	/* �����е�Blk��Ѱ�ң����Ƿ��ܹ����� */
	for (int i = 0; i < NBUF; i++) {
		Buf* bp = &(m_Buf[i]);
		if (bp->b_blkno == blkno && bp->b_dev == dev) {
			Logger::info() << "Found block " << blkno << " at buffer " << i <<". Reusing...\n";
			NotAvail(bp);
			return bp;
		}
	}

	/* û�п��Ը��õ�Blk�����µ����ɻ��� */
	Buf* bp = this->bFreeList.b_forw;
	if (bp != &this->bFreeList) {
		Logger::info() << "Use free buffer " << bp->b_bufno << ".\n";
		NotAvail(bp);
		if (bp->b_flags & Buf::B_DELWRI) {
			Logger::info() << "Encounter dirty buffer " << bp->b_bufno << ". Writing back...\n";
			Bwrite(bp);
			bp->b_blkno = dev;    
			bp->b_blkno = blkno;
			/* ��Ҫע��˴���Bwriteͨ��Brelse�ͷ���Buffer��
			 * ���������NotAvail���ܵ��·���Brelse��
			 * ��Buffer��BFreelistĩβ�ᵼ��BFreelist�ɻ���*/
			NotAvail(bp);
			/* ע��: �����������������λ */
			bp->b_flags = Buf::B_CLEAR;
			return bp;
		}
		bp->b_dev = dev;
		bp->b_blkno = blkno;

		/* ע��: �����������������λ */
		bp->b_flags = Buf::B_CLEAR;
		return bp;
	}
	else {
		/* ���ɻ����ǿյģ�ͬ����/д�����ܳ��֣�Bread���꼰ʱ�ͷţ��������� */
		Logger::err() << "bFreeList is empty.\n";
		return GetBlk(dev, blkno);
	}
}

void BufferManager::NotAvail(Buf* bp)
{
	Logger::info() << "Get buffer " << bp->b_bufno << " from BFreeList\n";

	/* �����ɶ�����ȡ�� */
	bp->b_back->b_forw = bp->b_forw;
	bp->b_forw->b_back = bp->b_back;

	return;
}

Buf* BufferManager::Bread(short dev, int blkno)
{
	Buf* bp;
	Logger::info() << "Executing Bread, dev: " << dev << ", blkno: " << blkno << "\n";
	/* �����豸�ţ��ַ�������뻺�� */
	bp = this->GetBlk(dev, blkno);
	/* ������豸�������ҵ����軺�棬��B_DONE�����ã��Ͳ������I/O���� */
	if (bp->b_flags & Buf::B_DONE)
	{
		return bp;
	}
	/* û���ҵ���Ӧ���棬����I/O������� */
	bp->b_flags |= Buf::B_READ;
	bp->b_wcount = BufferManager::BUFFER_SIZE;

	this->m_DeviceManager->GetBlockDevice(dev<<8).Strategy(bp);
	bp->b_flags |= Buf::B_DONE;

	return bp;
}


void BufferManager::Bwrite(Buf* bp)
{
	bp->b_flags &= ~(Buf::B_READ | Buf::B_DONE | Buf::B_ERROR | Buf::B_DELWRI);
	bp->b_flags |= Buf::B_WRITE;
	bp->b_wcount = BufferManager::BUFFER_SIZE;		/* 512�ֽ� */

	this->m_DeviceManager->GetBlockDevice(bp->b_dev>>8).Strategy(bp);
	this->Brelse(bp);
	
	bp->b_flags |= Buf::B_DONE;

	return;
}


void BufferManager::Bdwrite(Buf* bp)
{
	Logger::info() << "Executing delayed write at buffer " << bp->b_bufno << "\n";
	/* ����B_DONE������������ʹ�øô��̿����� */
	bp->b_flags |= (Buf::B_DELWRI | Buf::B_DONE);
	this->Brelse(bp);
	return;
}

void BufferManager::Bawrite(Buf* bp)
{
	/* �����ļ�ϵͳ���漰�첽д ��ͬ��Ѹ��д */
	this->Bwrite(bp);
	return;
}

void BufferManager::Brelse(Buf* bp)
{
	Logger::info() << "Brelse buffer " << bp->b_bufno << ", blkno:" << bp->b_blkno << "\n";

	(this->bFreeList.b_back)->b_forw = bp;
	bp->b_back = this->bFreeList.b_back;
	bp->b_forw = &(this->bFreeList);
	this->bFreeList.b_back = bp;
}

void BufferManager::Bflush(short dev)
{
	Buf* bp, * next;

	for (bp = this->bFreeList.b_forw; bp != &(this->bFreeList); bp = next)
	{
		next = bp->b_forw;
		/* �ҳ����ɶ����������ӳ�д�Ŀ� */
		if ((bp->b_flags & Buf::B_DELWRI) && dev == bp->b_dev)
		{
			this->NotAvail(bp);
			this->Bwrite(bp);
		}
	}

	return;
}

void BufferManager::ClrBuf(Buf* bp)
{
	int* pInt = (int*)bp->b_addr;

	/* ������������������ */
	for (unsigned int i = 0; i < BufferManager::BUFFER_SIZE / sizeof(int); i++)
	{
		pInt[i] = 0;
	}
	return;
}

Buf& BufferManager::GetBFreeList()
{
	return this->bFreeList;
}