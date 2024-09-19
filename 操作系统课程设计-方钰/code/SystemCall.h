#include "OSKernel.h"

/* ���ļ�ģ��ϵͳ���ã���FileManager��User�ṹ������������ */

/**
 * ���ļ�
 * @param path �ļ�·��
 * @param mode ��ģʽ
 * @return fd
 */
int Sys_Open(const char* path, int mode);

/**
 * �ر��ļ�
 * @param fd
 */
void Sys_Close(int fd);

/**
 * �����ļ�
 * @param fd
 * @param buffer ���뻺���׵�ַ
 * @param count �����ֽ���
 * @return ������ֽ���
 */
int Sys_Read(int fd, unsigned char* buffer, int count);

/**
 * д���ļ�
 * @param fd
 * @param buffer д�뻺���׵�ַ
 * @param count д���ֽ���
 * @return д����ֽ���
 */
int Sys_Write(int fd, unsigned char* buffer, int count);

/**
 * �½��ļ���
 * @param path �½�·��
 * @param mode
 */
void Sys_MkNod(const char* path, int mode = 040755 /* Unix V6++��mkdir��default mode */ );

/**
 * �ı䵱ǰĿ¼
 * @param path�ı��·��
 */
void Sys_ChDir(const char* path);

/**
 * �����ļ�
 * @param path�������ļ�·��
 * @return fd
 */
int Sys_Creat(const char* path, int mode = Inode::IRWXU);

/**
 * �����ļ���дָ��
 * @param fd �ļ�������
 * @param offset
 * @param mode seekģʽ(beg, cur, ate)
 */
void Sys_Seek(int fd, int offset, int mode);

/**
 * ɾ���ļ�
 * @param path �ļ�·��
 */
void Sys_Unlink(const char* path);

/**
 * ��ȡInode��Ϣ
 * @param path �ļ�·��
 * @param
 */
void Sys_Stat(const char* path, const DiskInode* inode);



