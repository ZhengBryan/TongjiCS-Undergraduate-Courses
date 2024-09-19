#ifndef BUF_H
#define BUF_H

#define BUF_SIZE 512

/*
 * ������ƿ�buf����
 * ��¼����Ӧ�����ʹ���������Ϣ��
 * ͬʱ����I/O����飬��¼�û���
 * ��ص�I/O�����ִ�н����
 */
class Buf
{
public:
	enum BufFlag	/* b_flags�б�־λ */
	{
		B_CLEAR		= 0x0,		/* b_flags���б����� */
		B_WRITE		= 0x1,		/* д�������������е���Ϣд��Ӳ����ȥ */
		B_READ		= 0x2,		/* �����������̶�ȡ��Ϣ�������� */
		B_DONE		= 0x4,		/* I/O�������� */
		B_ERROR		= 0x8,		/* I/O��������ֹ */
		B_DELWRI	= 0x10		/* �ӳ�д������Ӧ����Ҫ��������ʱ���ٽ�������д����Ӧ���豸�� */
	};

public:
	unsigned int b_flags;	/* ������ƿ��־λ */

	Buf* b_forw;
	Buf* b_back;

	short	b_dev;			/* �������豸�ţ����и�8λ�����豸�ţ���8λ�Ǵ��豸�� */
	int		b_wcount;		/* �贫�͵��ֽ��� */
	unsigned char* b_addr;	/* ָ��û�����ƿ�������Ļ��������׵�ַ */
	int		b_bufno;		/* ������ڻ��������е��±� */
	int		b_blkno;		/* �����߼���� */
	int		b_error;		/* I/O����ʱ��Ϣ */
	int		b_resid;		/* I/O����ʱ��δ���͵�ʣ���ֽ��� */
};

#endif
