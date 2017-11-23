

#ifndef _SOCKET_BUFFER_H_
#define _SOCKET_BUFFER_H_

#include <stdio.h>

class CSocketBuffer
{
public:
	CSocketBuffer();
	~CSocketBuffer();

	typedef struct _STC_BUFFER
	{
		char *buffer;
		char *orig_buffer;

		size_t misalign;
		size_t totallen;
		size_t off;
	} STC_BUFFER;
	static const int STC_BUFFER_SIZE = sizeof(STC_BUFFER);

	//ÿ��read���ֽ���
	static const int BUFFER_MAX_READ = 4096;
	//Ĭ�ϳ�ʼ����С
	static const int BUFFER_INIT_LEN = 4096 * 2;


	//��ʼ��
	//�ⲿnew���ռ�
	void Init(char* szBuf, size_t uLen);
	//Ĭ�� �Ƽ�����
	void Init();

	//appendһ��buffer class
	int Appends(const CSocketBuffer * pPwBuffer);

	int Append(const void * pBuffer, size_t uLen);

	//ɾ��buff��uLen���ֽ�
	void Erase(size_t uLen);

	//����buff
	void Reset();

	//��ȡbuffָ��
	const char * GetBuffer() const;

	//��ȡbuffָ��
	const char * GetBuffer(size_t & size) const;

	//��ȡbuff��size
	size_t GetSize() const;

	//��buff�ж� ���ض������ֽ��� ����Ϊuser�Ļ�������size
	int BufferRead(char * szBuffer, size_t uLen);

	//��curl�ж���buff��nHowmuch��ʾһ�ζ����ֽ�����-1 ��ʾ����Ĭ�� 4096�ֽ�
	int SockRead(int fd, int nHowmuch = -1);

	//��curl ����buff  ���ط��ͳɹ����ֽ���
	int Sends(int fd);

	void Clear();

private:
	
	void Align();
	int Expand(size_t uLen);


private:
	STC_BUFFER m_stcBuffer;
	int m_nBufferInitLen;

	static int m_nMaxBufferLen;


};


#endif

