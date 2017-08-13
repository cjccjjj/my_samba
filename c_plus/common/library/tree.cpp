#include "tree.h"
#include "string_x.h"

#include <stdio.h>
#include <stdlib.h>

#include "../../windows/library/util.h"
#include <tchar.h>
#include <strsafe.h>

XTreeKey::XTreeKey()
{
	//m_key = 0;
	m_pKey = NULL;
	m_p_object = NULL;
	m_p_data = NULL;
	m_len = 0;
	//m_type = 0;
}
XTreeKey::~XTreeKey()
{
	if (m_pKey)
	{
		delete m_pKey;
	}
	if (m_p_data)
	{
		free(m_p_data);
	}
}
bool XTreeKey::create(const TCHAR* p_key, void* p_data, size_t len, void* p_object)
{
	m_pKey = new (std::nothrow) XString;
	if (!m_pKey)
	{
		return false;
	}
	if (false == m_pKey->create(p_key))
	{
		return false;
	}

	if (p_data)
	{
		if (m_p_data)
		{
			free(m_p_data);
		}
		m_p_data = malloc(len);
		if (!m_p_data)
		{
			return false;
		}
		m_len = len;
		memcpy(m_p_data, p_data, len);
	}
	m_p_object = p_object;
	return true;
}
int XTreeKey::compare (XTreeKey* pTreeKey)
{
	return this->compare(pTreeKey->m_pKey->c_str());
}

int XTreeKey::compare (XNode234* pNode)
{
	return this->compare(pNode->m_pKeys[0]);
}

//int XTreeKey::compare (uint32 key)
//{
//	if (m_key < key)
//	{
//		return -1;
//	}
//	if (key < m_key)
//	{
//		return 1;
//	}
//	return 0;	
//}

int XTreeKey::compare (const TCHAR* pKey)
{
	return m_pKey->compare(pKey);
}

XTreeKey* XTreeKey::copy ()
{
	XTreeKey* pNewTreeKey = new (std::nothrow) XTreeKey;
	if (!pNewTreeKey)
	{
		return NULL;
	}
		
	if (!pNewTreeKey->create(m_pKey->c_str(), m_p_data, m_len, m_p_object))
	{
		return NULL;
	}

	return pNewTreeKey;
}

XTreeKey* XTreeKey::copy (XTreeKey* pTreekey)
{
	if (!pTreekey->create(m_pKey->c_str(), m_p_data, m_len, m_p_object))
	{
		return NULL;
	}

	return pTreekey;
}

TCHAR* XTreeKey::get_key ()
{
	return m_pKey->c_str();
}
bool XTreeKey::set_key (const TCHAR* p_key)
{
	m_pKey->clear();
	if (m_pKey->create(p_key))
	{
		return true;
	}
	return false;
}

XNode234::XNode234()
{
	ZeroMemory(this, sizeof(*this));
}
XNode234::~XNode234()
{
	// m_pKeys �� �������� �ʰ� �Ǹ� �����ϱ� ������ �ּ�ó����
	//for (int i=0;i<3;++i)
	//{
	//	delete m_pKeys[i];
	//}
}

int XNode234::compare (XNode234* pNode)
{
	return this->m_pKeys[0]->compare(pNode->m_pKeys[0]);
}
uint32 XNode234::get_key_count ()
{
	for (uint32 i=0;i<3;++i)
	{
		if (!m_pKeys[i])
		{
			return i;
		}		
	}
	return 3;
}

uint32 XNode234::get_child_count ()
{
	for (uint32 i=0;i<4;++i)
	{
		if (!m_pChildren[i])
		{
			return i;
		}		
	}
	return 4;
}

void XNode234::sort_keys ()
{
	uint32 key_count = get_key_count();
	for (uint32 i=0; i<key_count; ++i)
	{
		for (uint32 j=i+1; j<key_count; ++j)
		{
			if (0 < m_pKeys[i]->compare(m_pKeys[j]))
			{
				XTreeKey* pTemp = m_pKeys[i];
				m_pKeys[i] = m_pKeys[j];
				m_pKeys[j] = pTemp;
			}
		}
	}
}

void XNode234::sort_children ()
{
	uint32 child_count = get_child_count();
	for (uint32 i=0; i<child_count; ++i)
	{
		for (uint32 j=i+1; j<child_count; ++j)
		{
			if (0 < m_pChildren[i]->compare(m_pChildren[j]))
			{
				XNode234* pTemp = m_pChildren[i];
				m_pChildren[i] = m_pChildren[j];
				m_pChildren[j] = pTemp;
			}
		}
	}
}

XTree::XTree()
{
	m_pRoot		= NULL;
	m_key_type	= tree234_key_type_number;
}

XTree::~XTree()
{
	clear();
}

void XTree::create (size_t data_len, erase_trigger_t erase_trigger)
{
	//m_key_type = key_type;
	m_key_count = 0;
	m_data_len = data_len;
	m_erase_trigger = erase_trigger;
	return ;
}
void XTree::clear ()
{
	while (m_pRoot)
	{
		erase(m_pRoot->m_pKeys[0]->get_key());
	}
}
bool XTree::insert (const TCHAR* p_key, void* p_data, size_t len)
{
	return insert(p_key, p_data, len, NULL);
}
bool XTree::insert (const TCHAR* p_key, void* p_object)
{
	if (!p_object)
	{
		return false;
	}
	return insert(p_key, NULL, 0, p_object);
}
bool XTree::insert (const TCHAR* p_key, void* p_data, size_t len, void* p_object)
{
	if (tree234_key_type_string == m_key_type)
	{
		return false;
	}

	if (!p_object && m_data_len != len)
	{
		return false;
	}

	if (!m_pRoot)
	{
		m_pRoot = new (std::nothrow) XNode234;
		if (!m_pRoot)
		{
			return false;
		}
		XTreeKey* pNewKey = new (std::nothrow) XTreeKey;
		if (!pNewKey->create(p_key, p_data, len, p_object))
		{
			return false;
		}
		m_pRoot->m_pKeys[0] = pNewKey;
		++m_key_count;
		return true;
	}
	
	// root ��尡 4 ��� �̴�. ���� �Ѵ�.
	if (m_pRoot->m_pKeys[2])
	{
		// left child
		XNode234* pNewLeft = new (std::nothrow) XNode234;
		if (!pNewLeft)
		{
			return false;
		}
		pNewLeft->m_pParent = m_pRoot;

		// 0 ��° Ű�� ���ο� ���� �ڽĿ��� �絵�Ѵ�.
		pNewLeft->m_pKeys[0] = m_pRoot->m_pKeys[0];
		// 0 ��° �ڽ��� ���ο� ���� �ڽĿ��� �絵�Ѵ�.
		pNewLeft->m_pChildren[0] = m_pRoot->m_pChildren[0];
		m_pRoot->m_pChildren[0] = NULL;
		if (pNewLeft->m_pChildren[0])
		{
			pNewLeft->m_pChildren[0]->m_pParent = pNewLeft;
		}
		// 1 ��° �ڽ��� ���ο� ���� �ڽĿ��� �絵�Ѵ�.
		pNewLeft->m_pChildren[1] = m_pRoot->m_pChildren[1];
		m_pRoot->m_pChildren[1] = NULL;
		if (pNewLeft->m_pChildren[1])
		{
			pNewLeft->m_pChildren[1]->m_pParent = pNewLeft;
		}	
		
		// right child
		XNode234* pNewRight = new (std::nothrow) XNode234;
		if (!pNewRight)
		{
			return false;
		}
		pNewRight->m_pParent = m_pRoot;
		// 2 ��° Ű�� ���ο� ������ �ڽĿ��� �絵�Ѵ�.
		pNewRight->m_pKeys[0] = m_pRoot->m_pKeys[2];
		// 2 ��° �ڽ��� ���ο� ������ �ڽĿ��� �絵�Ѵ�.
		pNewRight->m_pChildren[0] = m_pRoot->m_pChildren[2];
		m_pRoot->m_pChildren[2] = NULL;
		if (pNewRight->m_pChildren[0])
		{
			pNewRight->m_pChildren[0]->m_pParent = pNewRight;
		}
		// 3 ��° �ڽ��� ���ο� ������ �ڽĿ��� �絵�Ѵ�.
		pNewRight->m_pChildren[1] = m_pRoot->m_pChildren[3];
		m_pRoot->m_pChildren[3] = NULL;
		if (pNewRight->m_pChildren[1])
		{
			pNewRight->m_pChildren[1]->m_pParent = pNewRight;
		}

		// �Ѹ� �� Ű ����
		m_pRoot->m_pKeys[0] = m_pRoot->m_pKeys[1];
		m_pRoot->m_pKeys[1] = NULL;	// pNewLeft  ���� �絵��
		m_pRoot->m_pKeys[2] = NULL;	// pNewRight ���� �絵��
		// �Ѹ� �� �ڽ� ����
		m_pRoot->m_pChildren[0] = pNewLeft;
		m_pRoot->m_pChildren[1] = pNewRight;
		
		// ��Ʈ�� Ű�� �ϳ��� �����̴�.
		if (0<m_pRoot->m_pKeys[0]->compare(p_key))
		{
			return this->insert(m_pRoot->m_pChildren[0], p_key, p_data, len, p_object);
		}
		return this->insert(m_pRoot->m_pChildren[1], p_key, p_data, len, p_object);
	}

	if (!m_pRoot->m_pChildren[0])	// leaf node �� ���
	{
		// ����Ű �˻�
		uint32 key_count = m_pRoot->get_key_count();
		for (uint32 i=0;i<key_count;++i)
		{
			if (0 == m_pRoot->m_pKeys[i]->compare(p_key))
			{
				return false;
			}
		}

		// ���ο� Ű�� �߰��Ѵ�.
		XTreeKey* pNewKey = new (std::nothrow) XTreeKey;
		if (!pNewKey)
		{
			return false;
		}
		if (false == pNewKey->create(p_key, p_data, len, p_object))
		{
			return false;
		}

		// leaf ��带 ã�� ���� ���� �߱� ������ leaf ����� Ű�� �ִ� 2�� �̴�.
		m_pRoot->m_pKeys[2] = m_pRoot->m_pKeys[1];
		m_pRoot->m_pKeys[1] = m_pRoot->m_pKeys[0];
		m_pRoot->m_pKeys[0] = pNewKey;
		m_pRoot->sort_keys();
		// leaf ��� �̹Ƿ� �ڽ��� ����.
		++m_key_count;
		return true;
	}

	uint32 key_count = m_pRoot->get_key_count();
	for (uint32 i=0; i<key_count; ++i)
	{
		int compare_result = m_pRoot->m_pKeys[i]->compare(p_key);
		if (0 == compare_result)
		{
			return false;
		}
		if (0 < compare_result)
		{
			return this->insert(m_pRoot->m_pChildren[i], p_key, p_data, len, p_object);
		}
	}
	return this->insert(m_pRoot->m_pChildren[key_count], p_key, p_data, len, p_object);
}

//bool XTree::erase (uint32 key)
//{
//	return erase(key);
//}

bool XTree::insert (XNode234* pNode, const TCHAR* p_key, void* p_data, size_t len, void* p_object)
{
	// ���� ��尡 4 ����� ��� ���� �Ѵ�.
	if (pNode->m_pKeys[2])
	{
		if (!this->split(pNode))
		{
			return false;
		}

		// ���� �߱� ������ �θ��� �ڽ� �迭�� ����Ǿ���.
		XNode234* pParent = pNode->m_pParent;
		uint32 key_count = pParent->get_key_count();
		for (uint32 i=0; i<key_count; ++i)
		{
			int compare_result = pParent->m_pKeys[i]->compare(p_key);
			if (0 == compare_result)
			{
				return false;
			}
			if (0 < compare_result)
			{
				return this->insert(pParent->m_pChildren[i], p_key, p_data, len, p_object);
			}
		}
		return this->insert(pParent->m_pChildren[key_count], p_key, p_data, len, p_object);
	}

	if (!pNode->m_pChildren[0])	// leaf node �� ���
	{
		uint32 key_count = pNode->get_key_count();
		for (uint32 i=0;i<key_count;++i)
		{
			if (0 == pNode->m_pKeys[i]->compare(p_key))
			{
				return false;
			}			
		}

		// ���ο� Ű�� �߰��Ѵ�.
		XTreeKey* pNewKey = new (std::nothrow) XTreeKey;
		if (!pNewKey)
		{
			return false;
		}
		if (false == pNewKey->create(p_key, p_data, len, p_object))
		{
			return false;
		}
		// leaf ��带 ã�� ���� ���� �߱� ������ leaf ����� Ű�� �ִ� 2�� �̴�.
		pNode->m_pKeys[2] = pNode->m_pKeys[1];
		pNode->m_pKeys[1] = pNode->m_pKeys[0];
		pNode->m_pKeys[0] = pNewKey;
		pNode->sort_keys();
		// leaf ��� �̹Ƿ� �ڽ��� ����.
		++m_key_count;
		return true;
	}

	// leaf ��带 ã���� ���� �ڽ��� ��ȸ�Ѵ�.
	uint32 key_count = pNode->get_key_count();
	for (uint32 i=0; i<key_count; ++i)
	{
		int compare_result = pNode->m_pKeys[i]->compare(p_key);
		if (0 == compare_result)
		{
			return false;
		}
		if (0 < compare_result)
		{
			return this->insert(pNode->m_pChildren[i], p_key, p_data, len, p_object);
		}
	}
	return this->insert(pNode->m_pChildren[key_count], p_key, p_data, len, p_object);
}

// pNode �� �׻� 4 ����̴�. �θ���� �׻� �����Ѵ�. �θ���� �׻� 4 ��尡 �ƴϴ�.
bool XTree::split (XNode234* pNode)
{
	XNode234* pParent = pNode->m_pParent;

	// ���� ����� ��� Ű�� �θ���� �̵���Ű��, �θ��� Ű ����.
	pParent->m_pKeys[2] = pParent->m_pKeys[1];
	pParent->m_pKeys[1] = pParent->m_pKeys[0];
	pParent->m_pKeys[0] = pNode->m_pKeys[1];
	pParent->sort_keys();

	// ���� ��尡 ���� �����ȴ�.
	XNode234* pNewRight = new (std::nothrow) XNode234;
	if (!pNewRight)
	{
		return false;
	}
	pNewRight->m_pParent = pNode->m_pParent;
	pNewRight->m_pKeys[0] = pNode->m_pKeys[2];
	pNode->m_pKeys[1] = NULL;	// �θ𿡰� �絵 �����Ƿ�
	pNode->m_pKeys[2] = NULL;	// �������� �絵 �����Ƿ�

	// �ڽĵ��� ������ ������.
	pNewRight->m_pChildren[0] = pNode->m_pChildren[2];
	pNode->m_pChildren[2] = NULL;
	if (pNewRight->m_pChildren[0])
	{
		pNewRight->m_pChildren[0]->m_pParent = pNewRight;
	}
	pNewRight->m_pChildren[1] = pNode->m_pChildren[3];
	pNode->m_pChildren[3] = NULL;
	if (pNewRight->m_pChildren[1])
	{
		pNewRight->m_pChildren[1]->m_pParent = pNewRight;
	}

	// �θ��� �ڽĵ��� ��� ��Ƽ� == ���� ������ ��� ��Ƽ� ����
	pParent->m_pChildren[3] = pParent->m_pChildren[2];
	pParent->m_pChildren[2] = pParent->m_pChildren[1];
	pParent->m_pChildren[1] = pParent->m_pChildren[0];
	pParent->m_pChildren[0] = pNewRight;
	pParent->sort_children();
	
	return true;
}

bool XTree::erase (const TCHAR* p_key)
{
	if (!m_pRoot)
	{
		return false;
	}

	uint32 key_offset = 0;
	XNode234* pEraseNode = this->search (m_pRoot, p_key, &key_offset);
	if (!pEraseNode)
	{
		return false;
	}
	if (!pEraseNode->m_pChildren[0])	// leaf ����� ���
	{
		return this->erase(m_pRoot, p_key);	
	}
	// �����ؾ��� Ű�� �߰��� ��ġ�� �ִ�. ��� ������ leaf node �� ã�´�.
	XNode234* pLeafNode = find_delete_leaf_node(pEraseNode->m_pChildren[key_offset+1]);
	XString LeafKey;
	if (false == LeafKey.create(pLeafNode->m_pKeys[0]->get_key()))
	{
		return false;
	}

	// Ű�� �״�� �ΰ�, Ű�� ���εǾ��ִ� �����͸� ��ȯ�Ѵ�.
	change_data(pEraseNode->m_pKeys[key_offset], pLeafNode->m_pKeys[0]);

	// ��� �����ϴ� Ű�� �����Ѵ�. �̶� �����Ǿ�� �ϴ� �����ʹ� ���������� �����ȴ�.
	this->erase(m_pRoot, LeafKey.c_str());
	
	// ��ȯ�� �����Ϳ� ���εǾ� �ִ� ������ Ű�� �����Ѵ�.  key -> leaf_key ��ȯ
	return swap_key (p_key, LeafKey.c_str());

}
#if defined (__WINDOWS)
bool XTree::erase (const char* p_key)
{
	XBuf <TCHAR> Temp;
	Temp.create(strlen(p_key)*2+2);
	Temp.zero_memory();
	m2w (p_key, strlen(p_key), Temp.buf(), (int)Temp.size());

	return erase(Temp.buf());
}
bool XTree::insert (const char* p_key, void* p_data)
{
	XBuf <TCHAR> Temp;
	Temp.create(strlen(p_key)*2+2);
	Temp.zero_memory();
	m2w (p_key, strlen(p_key), Temp.buf(), (int)Temp.size());

	return insert(Temp.buf(), p_data);
}
#endif

void XTree::erase_all ()
{
	while (m_pRoot)
	{
		erase(m_pRoot->m_pKeys[0]->get_key());
	}
}

bool XTree::get_root_key (TCHAR* p_root_key, size_t len)
{
	if (!m_pRoot)
	{
		return false;
	}
	StringCchCopy(p_root_key, len, m_pRoot->m_pKeys[0]->get_key());
	return true;
}

bool XTree::erase (XNode234* pNode, const TCHAR* p_key)
{
	if (!pNode)
	{
		return false;
	}

	// �Ѹ���尡 �ƴϰ� 2 ����� ��� 3 ���� ��ȯ�Ѵ�.
	if (pNode->m_pParent && !pNode->m_pKeys[1])
	{
		pNode = this->merge(pNode);
	}

	uint32 key_count = pNode->get_key_count();
	if (!pNode->m_pChildren[0])	// ���� ��尡 ������ ����� ��� �����ؾ� �� Ű�� ã�� �õ��� �Ѵ�.
	{
		for (uint32 i=0;i<key_count; ++i)
		{
			if (0 == pNode->m_pKeys[i]->compare(p_key))	// �����ؾ� �� Ű�� ã�Ҵ�.
			{
				if (m_erase_trigger)
				{
					m_erase_trigger(pNode->m_pKeys[i]->m_p_data, pNode->m_pKeys[i]->m_len, pNode->m_pKeys[i]->m_p_object);
				}
				
				delete pNode->m_pKeys[i];				// �ش� Ű�� ������
				pNode->m_pKeys[i] = NULL;
				for (;;)								// Ű�� ����� �������� �����ʿ� �ִ� Ű���� �������� ��ĭ�� �̵�
				{
					pNode->m_pKeys[i] = pNode->m_pKeys[i+1];
					++i;
					if (i == 3)
					{
						pNode->m_pKeys[i] = NULL;
						if (!pNode->m_pKeys[0])	// root ����̰� Ű�� ���� ��� root ��带 �����Ѵ�.
						{
							delete m_pRoot;
							m_pRoot = NULL;
						}
						// ������ ����̱� ������ �ڽ��� ����. �ڽ� ���� ���ʿ�
						--m_key_count;
						return true;
					}
				}
			}
		}
		return false;	// 3 ���� Ű�� ��ġ�ϴ� Ű�� ����.
	}

	// �����ؾ��� Ű ã�� ���� ��� �ڽĵ��� ��ȸ�Ѵ�.
	for (uint32 i=0; i<key_count; ++i)
	{
		int compare_result = pNode->m_pKeys[i]->compare(p_key);
		if (0 == compare_result)	// �׻� ������ ��忡�� ������ Ű�� ã�´�.
		{
			return false;
		}
		if (0 < compare_result)
		{
			return this->erase(pNode->m_pChildren[i], p_key);
		}
	}
	return this->erase(pNode->m_pChildren[key_count], p_key);
}

XNode234* XTree::merge (XNode234* pNode)
{
	uint32 my_offset = 0;
	int merge_type = check_brother_merge(pNode, &my_offset);
	
	if (0 == merge_type)	// ������ ���� �Ұ���
	{
		if (0 < my_offset)	// my_offset �� 1, 2, 3 �� �߿� �ϳ��̴�.
		{
			// �θ��� Ű�� ���� ��庸�� ����Ű�� ���� ������ �����Ѵ�. ���� ������ �����̴�. ���ڽ��� �����ȴ�.
			XNode234* pLeftBrother = pNode->m_pParent->m_pChildren[my_offset-1];
			uint32 parent_key_offset = my_offset-1;
			pLeftBrother->m_pKeys[1] = pNode->m_pParent->m_pKeys[parent_key_offset];
			// parent �� Ű�� ������, parent_key_offset ���� �������� ��ĭ �̵�
			while (true)
			{
				if (2 == parent_key_offset)
				{
					pNode->m_pParent->m_pKeys[parent_key_offset] = NULL;
					break;
				}
				pNode->m_pParent->m_pKeys[parent_key_offset] = pNode->m_pParent->m_pKeys[parent_key_offset+1];
				++parent_key_offset;
			}
			pLeftBrother->m_pKeys[2] = pNode->m_pKeys[0];
			pNode->m_pKeys[0] = NULL;
			// �ڽ� 2���� ���� �������� �ѱ��.
			pLeftBrother->m_pChildren[2] = pNode->m_pChildren[0];
			pNode->m_pChildren[0] = NULL;
			if (pLeftBrother->m_pChildren[2])
			{
				pLeftBrother->m_pChildren[2]->m_pParent = pLeftBrother;
			}
			pLeftBrother->m_pChildren[3] = pNode->m_pChildren[1];
			pNode->m_pChildren[1] = NULL;
			if (pLeftBrother->m_pChildren[3])
			{
				pLeftBrother->m_pChildren[3]->m_pParent = pLeftBrother;
			}
			// ���ڽ��� �����Ǳ� ������ ���� �θ��� �ڽ��� �����Ѵ�.
			uint32 order_offset = my_offset;
			while (true)
			{
				if (3 == order_offset)
				{
					pNode->m_pParent->m_pChildren[order_offset] = NULL;
					break;
				}
				pNode->m_pParent->m_pChildren[order_offset] = pNode->m_pParent->m_pChildren[order_offset+1];
				++order_offset;
			}
			delete pNode;	// �� �ڽ��� �����ȴ�.
			// ���� �θ� ��Ʈ�̰� �θ��� Ű�� �������� �ʴ°�� ���� ��Ʈ�� �ȴ�.
			if (!pLeftBrother->m_pParent->m_pParent && !pLeftBrother->m_pParent->m_pKeys[0])
			{
				for (uint32 i=0;i<4;++i)
				{
					pLeftBrother->m_pParent->m_pChildren[i] = NULL;
				}
				delete m_pRoot;
				m_pRoot = pLeftBrother;
				m_pRoot->m_pParent = NULL;
			}
			return pLeftBrother;
		}
		// �ڽ��� ���� ���ʿ� �ֱ⶧���� (0 == my_offset) �θ�Ű�� ���� ���� Ű�� ������ ������ �����Ѵ�. ������ ������ �޸� �����ϰ� �ڽ��� �����Ѵ�.
		XNode234* pRightBrother = pNode->m_pParent->m_pChildren[my_offset+1];
		uint32 parent_key_offset = 0;
		pNode->m_pKeys[1] = pNode->m_pParent->m_pKeys[parent_key_offset];
		// parent �� Ű�� ������, parent_key_offset ���� �������� ��ĭ �̵�
		while (true)
		{
			if (2 == parent_key_offset)
			{
				pNode->m_pParent->m_pKeys[parent_key_offset] = NULL;
				break;
			}
			pNode->m_pParent->m_pKeys[parent_key_offset] = pNode->m_pParent->m_pKeys[parent_key_offset+1];
			++parent_key_offset;
		}
		pNode->m_pKeys[2] = pRightBrother->m_pKeys[0];
		pRightBrother->m_pKeys[0] = NULL;
		// ������ ������ �ڽ� 2 ���� �޴´�.
		pNode->m_pChildren[2] = pRightBrother->m_pChildren[0];
		pRightBrother->m_pChildren[0] = NULL;
		if (pNode->m_pChildren[2])
		{
			pNode->m_pChildren[2]->m_pParent = pNode;
		}
		pNode->m_pChildren[3] = pRightBrother->m_pChildren[1];
		pRightBrother->m_pChildren[1] = NULL;
		if (pNode->m_pChildren[3])
		{
			pNode->m_pChildren[3]->m_pParent = pNode;
		}
		// parent �� �ڽ��� �����Ѵ�.
		uint32 order_offset = my_offset+1;	// ((my_offset+1) == ���� ������ ���� �̴�. ���� ������ ������ �������.
		while (true)
		{
			if (3 == order_offset)
			{
				pNode->m_pParent->m_pChildren[order_offset] = NULL;
				break;
			}
			pNode->m_pParent->m_pChildren[order_offset] = pNode->m_pParent->m_pChildren[order_offset+1];
			++order_offset;
		}
		delete pRightBrother;	// ������ ������ �����ȴ�.
		// ���� �θ� ��Ʈ�̰� Ű�� �������� �ʴ°�� ���� ��Ʈ�� �ȴ�.
		if (!pNode->m_pParent->m_pParent && !pNode->m_pParent->m_pKeys[0])
		{
			for (uint32 i=0;i<4;++i)
			{
				pNode->m_pParent->m_pChildren[i] = NULL;
			}
			delete m_pRoot;
			m_pRoot = pNode;
			m_pRoot->m_pParent = NULL;
		}
		return pNode;
	}

	// ������ ���հ���, �����Ǵ� ��� ����
	if (0>merge_type)	// ���� ������ ����
	{
		// �θ��� Ű�� ���� ���� ����Ű�� �����Ѵ�.
		pNode->m_pKeys[1] = pNode->m_pKeys[0];
		pNode->m_pKeys[0] = pNode->m_pParent->m_pKeys[my_offset-1];

		// ���� ������ ���� ������ Ű�� �θ𿡰� �����Ѵ�.
		XNode234* pLeftBrother = pNode->m_pParent->m_pChildren[my_offset-1];
		uint32 last_offset = pLeftBrother->get_key_count()-1;
		pNode->m_pParent->m_pKeys[my_offset-1] = pLeftBrother->m_pKeys[last_offset];
		pLeftBrother->m_pKeys[last_offset] = NULL;
		
		// ���� ������ ���� ������ �ڽ��� ���� ���� ���� �ڽ����� �д�.
		// ���� Ű 1�� �ڽ��� 2���� ���¿��� Ű2�� �ڽ� 3���� ����Ǵ� �����̴�.
		pNode->m_pChildren[2] = pNode->m_pChildren[1];	// �ڽ��� �����ϱ� ���� ������ �����.
		pNode->m_pChildren[1] = pNode->m_pChildren[0];
		// ���� ������ Ű�� �����ʿ� �ִ� �ڽ� �ɼ�, ������ �ڽ��� �ɼ��� �׻� Ű�� �ɼº��� 1 ũ��.
		++last_offset;	
		pNode->m_pChildren[0] = pLeftBrother->m_pChildren[last_offset];
		pLeftBrother->m_pChildren[last_offset] = NULL;
		if (pNode->m_pChildren[0])
		{
			pNode->m_pChildren[0]->m_pParent = pNode;	
		}
		// ���� ������ �ڽ��� ������ ����, �ڽ����� ���ʿ�
		return pNode;
	}
	// (0 < merge_type) ������ ������ ���� ����
	// �θ��� Ű�� ���� ���� ������Ű�� �����Ѵ�. Ű�� 1���� ���� merge �� �߻��Ѵ�. ���� �������� 1 ��°
	// my_offset �� 0, 1, 2 ���߿� �ϳ��̴�.
	pNode->m_pKeys[1] = pNode->m_pParent->m_pKeys[my_offset];

	// ������ ������ ���� ���� Ű�� �θ𿡰� �����Ѵ�.
	XNode234* pRightBrother = pNode->m_pParent->m_pChildren[my_offset+1];
	pNode->m_pParent->m_pKeys[my_offset] = pRightBrother->m_pKeys[0];	// ������ ������ ���� ���� Ű -> �θ𿡰� ����
	// Ű�� �·� ��ĭ�� �̵�
	pRightBrother->m_pKeys[0] = pRightBrother->m_pKeys[1];
	pRightBrother->m_pKeys[1] = pRightBrother->m_pKeys[2];
	pRightBrother->m_pKeys[2] = NULL;
	// ������ ������ ���� ���� �ڽ��� ���� ���� ������ �ڽ�(����° �ڽ�)���� �д�.
	pNode->m_pChildren[2] = pRightBrother->m_pChildren[0];
	if (pNode->m_pChildren[2])
	{
		pNode->m_pChildren[2]->m_pParent = pNode;
	}
	// ���� ���� �ڽ��� �������� ������ �ڽĵ��� �����ϴ� ���� �ʿ�, �ڽĵ��� �·� ��ĭ�� �̵�
	pRightBrother->m_pChildren[0] = pRightBrother->m_pChildren[1];
	pRightBrother->m_pChildren[1] = pRightBrother->m_pChildren[2];
	pRightBrother->m_pChildren[2] = pRightBrother->m_pChildren[3];
	pRightBrother->m_pChildren[3] = NULL;
	return pNode;
}

int XTree::check_brother_merge (XNode234* pNode, uint32* p_my_offset)
{
	XNode234* pParent = pNode->m_pParent;
	uint32 brother_count = pNode->m_pParent->get_child_count();
	// ���� ���° �ΰ�?
	for (uint32 i=0;i<brother_count;++i)
	{
		if (pNode == pNode->m_pParent->m_pChildren[i])
		{
			*p_my_offset = i;
			break;
		}
	}

	// ������ ���� ���� �ɼ��� �˾Ƴ´�.
	if (0 == *p_my_offset)	// ��°�� ������ �õ��Ѵ�.
	{
		if (1 < pNode->m_pParent->m_pChildren[1]->get_key_count())
		{
			return +1;	// �����ʰ� ���� ����
		}
		return 0;		// ���� ������ ���� ����
	}

	if (*p_my_offset == brother_count-1)	// ���� �����ʿ� �ִ� ���
	{
		if (1 < pNode->m_pParent->m_pChildren[brother_count-2]->get_key_count())
		{
			return -1;	// ���ʰ� ���� ����
		}
		return 0;		// ���� ������ ���� ����
	}

	// �߰��� �ִ� ���
	if (1 < pNode->m_pParent->m_pChildren[(*p_my_offset)-1]->get_key_count())	// ���� ����
	{
		return -1;	// ���ʰ� ���� ����
	}

	if (1 < pNode->m_pParent->m_pChildren[(*p_my_offset)+1]->get_key_count())	// ������ ����
	{
		return +1;	// �����ʰ� ���� ����
	}
	return 0;		// ���� ������ ���� ����
}

XNode234* XTree::find_delete_leaf_node (XNode234* pNode)
{
	// ����: ���� ��尡 2 ����� ��� 3 ���� ��ȯ�Ѵ�.
	// ����: �̰������� �����ϴ� �ڵ尡 �߰��Ǿ� ������ �ȵȴ�. ã���� �ϴ� leaf ��带 ã������ ����.
	//if (!pNode->m_pKeys[1])
	//{
	//	pNode = this->merge(pNode);
	//}
	
	if (!pNode->m_pChildren[0])	// ������ ��带 ã�Ҵ�.
	{
		return pNode;
	}
	
	return find_delete_leaf_node(pNode->m_pChildren[0]);
}

bool XTree::copy_key (XTreeKey* pKey, uint32 in_key_count, uint32* p_out_key_count)
{
	if (in_key_count < m_key_count )
	{
		return false;
	}
	uint32 out_key_count = 0;
	if (!p_out_key_count)
	{
		p_out_key_count = &out_key_count;
	}
	(*p_out_key_count) = 0;
	m_pCopyKey = pKey;
	return copy_key(m_pRoot, p_out_key_count);
}
		
bool XTree::copy_key (XNode234* pNode, uint32* p_out_key_count)
{
	// �ڽ��� Ű�� �����ϰ�
	uint32 key_count = pNode->get_key_count();
	for (uint32 i=0;i<key_count;++i)
	{
		if (NULL == pNode->m_pKeys[i]->copy(&m_pCopyKey[(*p_out_key_count)]))
		{
			return false;
		}
		//++m_copy_count;
		++(*p_out_key_count);
	}

	// �ڽĵ��� Ű�� �����Ѵ�.
	uint32 child_count = pNode->get_child_count();
	for (uint32 i=0;i<child_count;++i)
	{
		if (false == copy_key(pNode->m_pChildren[i], p_out_key_count))
		{
			return false;
		}
	}

	return true;
}

void* XTree::search (const TCHAR* p_key, void* p_data, size_t in_len)
{
	if (!m_pRoot)
	{
		return NULL;
	}
	if (m_data_len != in_len)
	{
		return NULL;
	}

	uint32 key_offset = 0;
	XNode234* pNode = search(m_pRoot, p_key, &key_offset);
	if (!pNode)
	{
		return NULL;
	}
	memcpy(p_data, pNode->m_pKeys[key_offset]->m_p_data, pNode->m_pKeys[key_offset]->m_len);
	//*p_out_len = pNode->m_pKeys[key_offset]->m_len;
	return p_data;
}

void* XTree::search (const TCHAR* p_key)
{
	if (!m_pRoot)
	{
		return NULL;
	}

	uint32 key_offset = 0;
	XNode234* pNode = search(m_pRoot, p_key, &key_offset);
	if (!pNode)
	{
		return NULL;
	}
	return pNode->m_pKeys[key_offset]->m_p_object;
}

#if defined (__WINDOWS)
void* XTree::search (const char* p_key)
{
	XBuf <TCHAR> Temp;
	Temp.create(strlen(p_key)*2+2);
	Temp.zero_memory();
	m2w (p_key, strlen(p_key), Temp.buf(), (int)Temp.size());
	return search(Temp.buf());
}
#endif

XNode234* XTree::search (XNode234* pNode, const TCHAR* p_key, uint32* p_key_offset)
{
	bool leaf_node_flag = false;
	if (!pNode->m_pChildren[0])
	{
		leaf_node_flag = true;
	}
	uint32 key_count = pNode->get_key_count();
	for (uint32 i=0; i<key_count; ++i)
	{
		int compare_result = pNode->m_pKeys[i]->compare(p_key);
		if (0 == compare_result)
		{
			*p_key_offset = i;
			return pNode;
		}
		if (0 < compare_result)
		{
			if (leaf_node_flag)
			{
				return NULL;
			}
			return this->search(pNode->m_pChildren[i], p_key, p_key_offset);
		}
	}
	if (leaf_node_flag)
	{
		return NULL;
	}
	return this->search(pNode->m_pChildren[key_count], p_key, p_key_offset);
}
bool XTree::update (TCHAR* p_key, void* p_data, size_t len)
{
	return update(p_key, p_data, len, NULL);
}
bool XTree::update (TCHAR* p_key, void* p_object)
{
	return update(p_key, NULL, 0, p_object);
}
bool XTree::update (TCHAR* p_key, void* p_data, size_t len, void* p_object)
{
	if (!m_pRoot)
	{
		return false;
	}

	uint32 key_offset = 0;
	XNode234* pNode = search(m_pRoot, p_key, &key_offset);
	if (!pNode)
	{
		return false;
	}


	if (false == pNode->m_pKeys[key_offset]->create(p_key, p_data, len, p_object))
	{
		return false;
	}
	return true;
}

// void*
//bool XTree::insert (uint32 key, void* p_data)
//{
//	
//}
//	void* search (uint32 key);
//	void* update (uint32 key, void* p_data);

void XTree::change_data (XTreeKey* pTreeKeyA, XTreeKey* pTreeKeyB)
{
	void* p_temp_data = pTreeKeyA->m_p_data;
	size_t temp_len = pTreeKeyA->m_len;

	pTreeKeyA->m_p_data = pTreeKeyB->m_p_data;
	pTreeKeyA->m_len = pTreeKeyB->m_len;

	pTreeKeyB->m_p_data = p_temp_data;
	pTreeKeyA->m_len = temp_len;
}

bool XTree::swap_key (const TCHAR* p_key, const TCHAR* p_leaf_key)
{
	if (!m_pRoot)
	{
		return false;
	}
	uint32 key_offset = 0;
	XNode234* pNode = search (m_pRoot, p_key, &key_offset);
	//pNode->m_pKeys[key_offset]->m_key = leaf_key;
	if (false == pNode->m_pKeys[key_offset]->set_key(p_leaf_key))
	{
		return false;
	}
	return true;
}

// Ű�� uint32 Ÿ�Ե� ������ 0 ~ 4,294,967,295		10 �ڸ��� 4294967295
bool XTree::insert (uint32 key, void* p_data, size_t len)
{
	TCHAR string_key [11] = _T("");
	StringCchPrintf(string_key, _countof(string_key), _T("%010u"), key);
	//num2str(key, string_key, _countof(string_key));
	return insert (string_key, p_data, len);
}
bool XTree::insert (void* p_data, size_t len)
{
	uint32 key = m_key_count;
	TCHAR string_key [11] = _T("");
	StringCchPrintf(string_key, _countof(string_key), _T("%010u"), key);
	//num2str(key, string_key, _countof(string_key));
	return insert (string_key, p_data, len);
}
void* XTree::search (uint32 key, void* p_data, size_t in_len)
{
	TCHAR string_key [11] = _T("");
	StringCchPrintf(string_key, _countof(string_key), _T("%010u"), key);
	//num2str(key, string_key, _countof(string_key));
	return search (string_key, p_data, in_len);
}
bool XTree::update (uint32 key, void* p_data, size_t len)
{
	//TCHAR string_key [100] = _T("");
	//num2str(key, string_key, _countof(string_key));
	TCHAR string_key [11] = _T("");
	StringCchPrintf(string_key, _countof(string_key), _T("%010u"), key);
	return update(string_key, p_data, len);
}
bool XTree::erase (uint32 key)
{
	//TCHAR string_key [100] = _T("");
	//num2str(key, string_key, _countof(string_key));
	TCHAR string_key [11] = _T("");
	StringCchPrintf(string_key, _countof(string_key), _T("%010u"), key);
	return erase (string_key);
}
// void* 
bool XTree::insert (uint32 key, void* p_data)
{
	//TCHAR string_key [100] = _T("");
	//num2str(key, string_key, _countof(string_key));
	TCHAR string_key [11] = _T("");
	StringCchPrintf(string_key, _countof(string_key), _T("%010u"), key);
	return insert(string_key, p_data);
}
bool XTree::insert (void* p_object)
{
	uint32 key = m_key_count;
	//TCHAR string_key [100] = _T("");
	//num2str(key, string_key, _countof(string_key));
	TCHAR string_key [11] = _T("");
	StringCchPrintf(string_key, _countof(string_key), _T("%010u"), key);
	return insert(string_key, p_object);
}
void* XTree::search (uint32 key)
{
	//TCHAR string_key [100] = _T("");
	//num2str(key, string_key, _countof(string_key));
	TCHAR string_key [11] = _T("");
	StringCchPrintf(string_key, _countof(string_key), _T("%010u"), key);
	return search (string_key);
}
bool XTree::update (uint32 key, void* p_data)
{
	//TCHAR string_key [100] = _T("");
	//num2str(key, string_key, _countof(string_key));
	TCHAR string_key [11] = _T("");
	StringCchPrintf(string_key, _countof(string_key), _T("%010u"), key);
	return update (string_key, p_data);
}


// ������
XNode234* XTree::get_root_node ()
{
	return m_pRoot;
}
uint32 XTree::get_depth ()
{
	if (m_pRoot)
	{
		uint32 depth = 1;
		calc_depth(m_pRoot, &depth);
		return depth;
	}
	return 0;
}
void XTree::calc_depth (XNode234* pNode, uint32* p_depth)
{
	if (pNode->m_pChildren[0])
	{
		++(*p_depth);
		calc_depth(pNode->m_pChildren[0], p_depth);
	}
}
bool XTree::merge (TCHAR* key)
{
	return false;
	//return erase(key, false);
}

uint32 XTree::get_key_count ()
{
	return m_key_count;
}

//uint32 XTree::copy_node (XNode234* pNode, XNode234** ppNode, uint32* p_count)
//{
//	if (*p_count <= m_copy_count)
//	{
//		return m_copy_count+1;
//	}
//
//	ppNode[m_copy_count] = pNode;	// ���ڽ��� �����ϰ�
//	++(*p_count);
//	uint32 child_count = pNode->get_child_count();	// �ڽĵ��� �����Ѵ�.
//	for (uint32 i=0;i<child_count;++i)
//	{
//		copy_node(pNode->m_pChildren[i], ppNode, p_count);
//	}
//	return *p_count;
//}


//XNode234* XTree::get_first_node ()
//{
//	if (m_pRoot)
//	{
//		if (m_pRoot->m_pChildren[0])
//		{
//			get_first_node(m_pRoot->m_pChildren[0]);
//		}
//		m_pDebugNode = m_pRoot;
//		return m_pRoot;
//	}
//	m_pDebugNode = NULL;
//	return NULL;
//}
//XNode234* XTree::get_first_node (XNode234* pNode)
//{	
//	if (pNode->m_pChildren[0])
//	{
//		return get_first_node(pNode->m_pChildren[0]);
//	}
//	m_pDebugNode = pNode;
//	return pNode;
//}
//XNode234* XTree::get_parent ()
//{
//	if (m_pDebugNode->m_pParent)
//	{
//		m_pDebugNode = m_pDebugNode->m_pParent;
//		return m_pDebugNode;
//	}
//	return NULL;
//}
//XNode234* XTree::get_next_brother ()
//{	
//	if (m_pDebugNode->m_pParent)
//	{
//		uint32 my_offset = 0;
//		uint32 brother_count = m_pDebugNode->m_pParent->get_child_count();
//		// ���� ���° �ΰ�?
//		for (uint32 i=0;i<brother_count;++i)
//		{
//			if (m_pDebugNode == m_pDebugNode->m_pParent->m_pChildren[i])
//			{
//				my_offset = i;
//				break;
//			}
//		}
//
//		if (my_offset == brother_count-1)	// ������ ���
//		{
//			return NULL;
//		}
//		m_pDebugNode = m_pDebugNode->m_pParent->m_pChildren[my_offset+1];
//		return m_pDebugNode;
//	}
//	return NULL;
//}

