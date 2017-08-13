#ifndef __BUF_H__
#define __BUF_H__

#include "data_type.h"
#include <new>

template <typename T>
class XBuf
{
public:
	XBuf  ();
	~XBuf ();
		
	bool	create		(int size);

	bool	is_create	();
	void	zero_memory	();
	void	clear		();
	
	T*			buf			();
	int			size		();
	
	T&		operator [] (int offset)
	{
		return m_pData [offset];
	}

private:
	bool	m_create_flag;
	T*		m_pData;
	int		m_size;
};

template <class T>
XBuf<T>::XBuf()
{
	m_create_flag	= false;
	m_pData			= NULL;
	m_size			= 0;
}

template <class T>
XBuf<T>::~XBuf()
{
	clear ();
}

template <class T>
bool XBuf<T>::create (int size)
{
	if (true == m_create_flag)
	{
		clear ();
	}

	if (0 >= size)
	{
		return false;
	}

	m_size = size;

	m_pData = new (std::nothrow) T [(size_t)m_size];
	if (NULL == m_pData)
	{
		return false;
	}

	m_create_flag = true;

	return true;
}

template <class T>
bool XBuf<T>::is_create ()
{
	return m_create_flag;
}

template <class T>
void XBuf<T>::zero_memory ()
{
	if (false == m_create_flag)
	{
		return ;
	}
	ZeroMemory (m_pData, sizeof(T) * (size_t)m_size);
}

template <class T>
void XBuf<T>::clear ()
{
	if (false == m_create_flag)
	{
		return;
	}
	
	delete [] m_pData;
	m_pData = NULL;
	m_size = 0;
	m_create_flag = false;
}

template <class T>
T* XBuf<T>::buf ()
{
	return m_pData;
}

template <class T>
int XBuf<T>::size ()
{
	return m_size;
}

#endif
