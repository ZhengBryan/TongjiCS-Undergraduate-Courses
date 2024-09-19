#ifndef OSKernel_H
#define OSKernel_H

#include "User.h"
#include "BufferManager.h"
#include "DeviceManager.h"
#include "FileManager.h"
#include "FileSystem.h"


/*
 * OSKernel�����ڷ�װ�����ں���ص�ȫ����ʵ������
 *
 * OSKernel�����ڴ���Ϊ����ģʽ����֤�ں��з�װ���ں�
 * ģ��Ķ���ֻ��һ��������
 */
class OSKernel
{
public:
	OSKernel();
	~OSKernel();
	void Restart();
	static OSKernel& Instance();

	BufferManager& GetBufferManager();
	DeviceManager& GetDeviceManager();
	FileSystem& GetFileSystem();
	FileManager& GetFileManager();
	User& GetUser();


private:
	static OSKernel instance;		/* OSKernel������ʵ�� */

	BufferManager* m_BufferManager;
	DeviceManager* m_DeviceManager;
	FileSystem* m_FileSystem;
	FileManager* m_FileManager;
	User* m_User;
};

#endif
