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
	// m_pKeys 가 존재하지 않게 되면 삭제하기 때문에 주석처리함
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
	
	// root 노드가 4 노드 이다. 분할 한다.
	if (m_pRoot->m_pKeys[2])
	{
		// left child
		XNode234* pNewLeft = new (std::nothrow) XNode234;
		if (!pNewLeft)
		{
			return false;
		}
		pNewLeft->m_pParent = m_pRoot;

		// 0 번째 키를 새로운 왼쪽 자식에게 양도한다.
		pNewLeft->m_pKeys[0] = m_pRoot->m_pKeys[0];
		// 0 번째 자식을 새로운 왼쪽 자식에게 양도한다.
		pNewLeft->m_pChildren[0] = m_pRoot->m_pChildren[0];
		m_pRoot->m_pChildren[0] = NULL;
		if (pNewLeft->m_pChildren[0])
		{
			pNewLeft->m_pChildren[0]->m_pParent = pNewLeft;
		}
		// 1 번째 자식을 새로운 왼쪽 자식에게 양도한다.
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
		// 2 번째 키를 새로운 오른쪽 자식에게 양도한다.
		pNewRight->m_pKeys[0] = m_pRoot->m_pKeys[2];
		// 2 번째 자식을 새로운 오른쪽 자식에게 양도한다.
		pNewRight->m_pChildren[0] = m_pRoot->m_pChildren[2];
		m_pRoot->m_pChildren[2] = NULL;
		if (pNewRight->m_pChildren[0])
		{
			pNewRight->m_pChildren[0]->m_pParent = pNewRight;
		}
		// 3 번째 자식을 새로운 오른쪽 자식에게 양도한다.
		pNewRight->m_pChildren[1] = m_pRoot->m_pChildren[3];
		m_pRoot->m_pChildren[3] = NULL;
		if (pNewRight->m_pChildren[1])
		{
			pNewRight->m_pChildren[1]->m_pParent = pNewRight;
		}

		// 뿌리 의 키 정리
		m_pRoot->m_pKeys[0] = m_pRoot->m_pKeys[1];
		m_pRoot->m_pKeys[1] = NULL;	// pNewLeft  에게 양도함
		m_pRoot->m_pKeys[2] = NULL;	// pNewRight 에게 양도함
		// 뿌리 의 자식 정리
		m_pRoot->m_pChildren[0] = pNewLeft;
		m_pRoot->m_pChildren[1] = pNewRight;
		
		// 루트는 키가 하나인 상태이다.
		if (0<m_pRoot->m_pKeys[0]->compare(p_key))
		{
			return this->insert(m_pRoot->m_pChildren[0], p_key, p_data, len, p_object);
		}
		return this->insert(m_pRoot->m_pChildren[1], p_key, p_data, len, p_object);
	}

	if (!m_pRoot->m_pChildren[0])	// leaf node 일 경우
	{
		// 동일키 검사
		uint32 key_count = m_pRoot->get_key_count();
		for (uint32 i=0;i<key_count;++i)
		{
			if (0 == m_pRoot->m_pKeys[i]->compare(p_key))
			{
				return false;
			}
		}

		// 새로운 키를 추가한다.
		XTreeKey* pNewKey = new (std::nothrow) XTreeKey;
		if (!pNewKey)
		{
			return false;
		}
		if (false == pNewKey->create(p_key, p_data, len, p_object))
		{
			return false;
		}

		// leaf 노드를 찾는 동안 분할 했기 때문에 leaf 노드의 키는 최대 2개 이다.
		m_pRoot->m_pKeys[2] = m_pRoot->m_pKeys[1];
		m_pRoot->m_pKeys[1] = m_pRoot->m_pKeys[0];
		m_pRoot->m_pKeys[0] = pNewKey;
		m_pRoot->sort_keys();
		// leaf 노드 이므로 자식이 없다.
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
	// 현재 노드가 4 노드일 경우 분할 한다.
	if (pNode->m_pKeys[2])
	{
		if (!this->split(pNode))
		{
			return false;
		}

		// 분할 했기 때문에 부모의 자식 배열이 변경되었다.
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

	if (!pNode->m_pChildren[0])	// leaf node 일 경우
	{
		uint32 key_count = pNode->get_key_count();
		for (uint32 i=0;i<key_count;++i)
		{
			if (0 == pNode->m_pKeys[i]->compare(p_key))
			{
				return false;
			}			
		}

		// 새로운 키를 추가한다.
		XTreeKey* pNewKey = new (std::nothrow) XTreeKey;
		if (!pNewKey)
		{
			return false;
		}
		if (false == pNewKey->create(p_key, p_data, len, p_object))
		{
			return false;
		}
		// leaf 노드를 찾는 동안 분할 했기 때문에 leaf 노드의 키는 최대 2개 이다.
		pNode->m_pKeys[2] = pNode->m_pKeys[1];
		pNode->m_pKeys[1] = pNode->m_pKeys[0];
		pNode->m_pKeys[0] = pNewKey;
		pNode->sort_keys();
		// leaf 노드 이므로 자식이 없다.
		++m_key_count;
		return true;
	}

	// leaf 노드를 찾을때 까지 자식을 순회한다.
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

// pNode 는 항상 4 노드이다. 부모노드는 항상 존재한다. 부모노드는 항상 4 노드가 아니다.
bool XTree::split (XNode234* pNode)
{
	XNode234* pParent = pNode->m_pParent;

	// 현재 노드의 가운데 키를 부모노드로 이동시키고, 부모노드 키 정렬.
	pParent->m_pKeys[2] = pParent->m_pKeys[1];
	pParent->m_pKeys[1] = pParent->m_pKeys[0];
	pParent->m_pKeys[0] = pNode->m_pKeys[1];
	pParent->sort_keys();

	// 형제 노드가 새로 생성된다.
	XNode234* pNewRight = new (std::nothrow) XNode234;
	if (!pNewRight)
	{
		return false;
	}
	pNewRight->m_pParent = pNode->m_pParent;
	pNewRight->m_pKeys[0] = pNode->m_pKeys[2];
	pNode->m_pKeys[1] = NULL;	// 부모에게 양도 했으므로
	pNode->m_pKeys[2] = NULL;	// 형제에게 양도 했으므로

	// 자식들을 나누어 가진다.
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

	// 부모의 자식들을 모두 모아서 == 나의 형제를 모두 모아서 정렬
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
	if (!pEraseNode->m_pChildren[0])	// leaf 노드일 경우
	{
		return this->erase(m_pRoot, p_key);	
	}
	// 삭제해야할 키가 중간에 위치해 있다. 대신 삭제할 leaf node 를 찾는다.
	XNode234* pLeafNode = find_delete_leaf_node(pEraseNode->m_pChildren[key_offset+1]);
	XString LeafKey;
	if (false == LeafKey.create(pLeafNode->m_pKeys[0]->get_key()))
	{
		return false;
	}

	// 키는 그대로 두고, 키와 맵핑되어있는 데이터만 교환한다.
	change_data(pEraseNode->m_pKeys[key_offset], pLeafNode->m_pKeys[0]);

	// 대신 삭제하는 키를 삭제한다. 이때 삭제되어야 하는 데이터는 정상적으로 삭제된다.
	this->erase(m_pRoot, LeafKey.c_str());
	
	// 교환된 데이터와 맵핑되어 있던 원래의 키로 복원한다.  key -> leaf_key 변환
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

	// 뿌리노드가 아니고 2 노드인 경우 3 노드로 변환한다.
	if (pNode->m_pParent && !pNode->m_pKeys[1])
	{
		pNode = this->merge(pNode);
	}

	uint32 key_count = pNode->get_key_count();
	if (!pNode->m_pChildren[0])	// 현재 노드가 마지막 노드일 경우 삭제해야 할 키를 찾는 시도를 한다.
	{
		for (uint32 i=0;i<key_count; ++i)
		{
			if (0 == pNode->m_pKeys[i]->compare(p_key))	// 삭제해야 할 키를 찾았다.
			{
				if (m_erase_trigger)
				{
					m_erase_trigger(pNode->m_pKeys[i]->m_p_data, pNode->m_pKeys[i]->m_len, pNode->m_pKeys[i]->m_p_object);
				}
				
				delete pNode->m_pKeys[i];				// 해당 키를 삭제함
				pNode->m_pKeys[i] = NULL;
				for (;;)								// 키의 빈곳을 기준으로 오른쪽에 있는 키들을 왼쪽으로 한칸씩 이동
				{
					pNode->m_pKeys[i] = pNode->m_pKeys[i+1];
					++i;
					if (i == 3)
					{
						pNode->m_pKeys[i] = NULL;
						if (!pNode->m_pKeys[0])	// root 노드이고 키가 없는 경우 root 노드를 삭제한다.
						{
							delete m_pRoot;
							m_pRoot = NULL;
						}
						// 마지막 노드이기 때문에 자식이 없다. 자식 정렬 불필요
						--m_key_count;
						return true;
					}
				}
			}
		}
		return false;	// 3 개의 키중 일치하는 키가 없다.
	}

	// 삭제해야할 키 찾지 못한 경우 자식들을 순회한다.
	for (uint32 i=0; i<key_count; ++i)
	{
		int compare_result = pNode->m_pKeys[i]->compare(p_key);
		if (0 == compare_result)	// 항상 마지막 노드에서 삭제할 키를 찾는다.
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
	
	if (0 == merge_type)	// 형제와 병합 불가능
	{
		if (0 < my_offset)	// my_offset 은 1, 2, 3 셋 중에 하나이다.
		{
			// 부모의 키중 현재 노드보다 작은키와 왼쪽 형제를 병합한다. 왼쪽 형제가 기준이다. 나자신은 삭제된다.
			XNode234* pLeftBrother = pNode->m_pParent->m_pChildren[my_offset-1];
			uint32 parent_key_offset = my_offset-1;
			pLeftBrother->m_pKeys[1] = pNode->m_pParent->m_pKeys[parent_key_offset];
			// parent 의 키를 재정렬, parent_key_offset 부터 왼쪽으로 한칸 이동
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
			// 자식 2개를 왼쪽 형제에게 넘긴다.
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
			// 나자신이 삭제되기 때문에 나의 부모의 자식을 정열한다.
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
			delete pNode;	// 나 자신은 삭제된다.
			// 나의 부모가 루트이고 부모의 키가 존재하지 않는경우 내가 루트가 된다.
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
		// 자신이 가장 왼쪽에 있기때문에 (0 == my_offset) 부모키중 가장 작은 키와 오른쪽 형제를 병합한다. 오른쪽 형제를 메모리 해제하고 자신을 리턴한다.
		XNode234* pRightBrother = pNode->m_pParent->m_pChildren[my_offset+1];
		uint32 parent_key_offset = 0;
		pNode->m_pKeys[1] = pNode->m_pParent->m_pKeys[parent_key_offset];
		// parent 의 키를 재정렬, parent_key_offset 부터 왼쪽으로 한칸 이동
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
		// 오른쪽 형제의 자식 2 개를 받는다.
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
		// parent 의 자식을 정열한다.
		uint32 order_offset = my_offset+1;	// ((my_offset+1) == 나의 오른쪽 형제 이다. 나의 오른쪽 형제가 사라진다.
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
		delete pRightBrother;	// 오른쪽 형제는 삭제된다.
		// 나의 부모가 루트이고 키가 존재하지 않는경우 내가 루트가 된다.
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

	// 형제와 병합가능, 삭제되는 노드 없음
	if (0>merge_type)	// 왼쪽 형제와 병합
	{
		// 부모의 키를 나의 가장 왼쪽키로 복사한다.
		pNode->m_pKeys[1] = pNode->m_pKeys[0];
		pNode->m_pKeys[0] = pNode->m_pParent->m_pKeys[my_offset-1];

		// 왼쪽 형제의 가장 오른쪽 키를 부모에게 복사한다.
		XNode234* pLeftBrother = pNode->m_pParent->m_pChildren[my_offset-1];
		uint32 last_offset = pLeftBrother->get_key_count()-1;
		pNode->m_pParent->m_pKeys[my_offset-1] = pLeftBrother->m_pKeys[last_offset];
		pLeftBrother->m_pKeys[last_offset] = NULL;
		
		// 왼쪽 형제의 가장 오른쪽 자식을 나의 가장 왼쪽 자식으로 둔다.
		// 현재 키 1개 자식이 2개인 상태에서 키2개 자식 3개로 변경되는 상태이다.
		pNode->m_pChildren[2] = pNode->m_pChildren[1];	// 자식을 복사하기 위해 공간을 만든다.
		pNode->m_pChildren[1] = pNode->m_pChildren[0];
		// 가장 오른쪽 키의 오른쪽에 있는 자식 옵셋, 오른쪽 자식의 옵셋은 항상 키의 옵셋보다 1 크다.
		++last_offset;	
		pNode->m_pChildren[0] = pLeftBrother->m_pChildren[last_offset];
		pLeftBrother->m_pChildren[last_offset] = NULL;
		if (pNode->m_pChildren[0])
		{
			pNode->m_pChildren[0]->m_pParent = pNode;	
		}
		// 가장 오른쪽 자식이 없어진 상태, 자식정렬 불필요
		return pNode;
	}
	// (0 < merge_type) 오른쪽 형제와 병합 가능
	// 부모의 키를 나의 가장 오른쪽키로 복사한다. 키가 1개일 때만 merge 가 발생한다. 가장 오른쪽은 1 번째
	// my_offset 은 0, 1, 2 셋중에 하나이다.
	pNode->m_pKeys[1] = pNode->m_pParent->m_pKeys[my_offset];

	// 오른쪽 형제의 가장 왼쪽 키를 부모에게 복사한다.
	XNode234* pRightBrother = pNode->m_pParent->m_pChildren[my_offset+1];
	pNode->m_pParent->m_pKeys[my_offset] = pRightBrother->m_pKeys[0];	// 오른쪽 형제의 가장 왼쪽 키 -> 부모에게 복사
	// 키를 좌로 한칸씩 이동
	pRightBrother->m_pKeys[0] = pRightBrother->m_pKeys[1];
	pRightBrother->m_pKeys[1] = pRightBrother->m_pKeys[2];
	pRightBrother->m_pKeys[2] = NULL;
	// 오른쪽 형제의 가장 왼쪽 자식을 나의 가장 오른쪽 자식(세번째 자식)으로 둔다.
	pNode->m_pChildren[2] = pRightBrother->m_pChildren[0];
	if (pNode->m_pChildren[2])
	{
		pNode->m_pChildren[2]->m_pParent = pNode;
	}
	// 가장 왼쪽 자식이 없어졌기 때문에 자식들을 정렬하는 것이 필요, 자식들을 좌로 한칸씩 이동
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
	// 나는 몇번째 인가?
	for (uint32 i=0;i<brother_count;++i)
	{
		if (pNode == pNode->m_pParent->m_pChildren[i])
		{
			*p_my_offset = i;
			break;
		}
	}

	// 형제의 수와 나의 옵셋을 알아냈다.
	if (0 == *p_my_offset)	// 둘째와 병합을 시도한다.
	{
		if (1 < pNode->m_pParent->m_pChildren[1]->get_key_count())
		{
			return +1;	// 오른쪽과 병합 가능
		}
		return 0;		// 병합 가능한 형제 없음
	}

	if (*p_my_offset == brother_count-1)	// 가장 오른쪽에 있는 경우
	{
		if (1 < pNode->m_pParent->m_pChildren[brother_count-2]->get_key_count())
		{
			return -1;	// 왼쪽과 병합 가능
		}
		return 0;		// 병합 가능한 형제 없음
	}

	// 중간에 있는 경우
	if (1 < pNode->m_pParent->m_pChildren[(*p_my_offset)-1]->get_key_count())	// 왼쪽 형제
	{
		return -1;	// 왼쪽과 병합 가능
	}

	if (1 < pNode->m_pParent->m_pChildren[(*p_my_offset)+1]->get_key_count())	// 오른쪽 형제
	{
		return +1;	// 오른쪽과 병합 가능
	}
	return 0;		// 병합 가능한 형제 없음
}

XNode234* XTree::find_delete_leaf_node (XNode234* pNode)
{
	// 오류: 현재 노드가 2 노드인 경우 3 노드로 변환한다.
	// 이유: 이과정에서 병합하는 코드가 추가되어 있으면 안된다. 찾고자 하는 leaf 노드를 찾을수가 없다.
	//if (!pNode->m_pKeys[1])
	//{
	//	pNode = this->merge(pNode);
	//}
	
	if (!pNode->m_pChildren[0])	// 마지막 노드를 찾았다.
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
	// 자신의 키를 복사하고
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

	// 자식들의 키를 복사한다.
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

// 키를 uint32 타입도 지원함 0 ~ 4,294,967,295		10 자리수 4294967295
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


// 디버깅용
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
//	ppNode[m_copy_count] = pNode;	// 나자신을 복사하고
//	++(*p_count);
//	uint32 child_count = pNode->get_child_count();	// 자식들을 복사한다.
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
//		// 나는 몇번째 인가?
//		for (uint32 i=0;i<brother_count;++i)
//		{
//			if (m_pDebugNode == m_pDebugNode->m_pParent->m_pChildren[i])
//			{
//				my_offset = i;
//				break;
//			}
//		}
//
//		if (my_offset == brother_count-1)	// 막내일 경우
//		{
//			return NULL;
//		}
//		m_pDebugNode = m_pDebugNode->m_pParent->m_pChildren[my_offset+1];
//		return m_pDebugNode;
//	}
//	return NULL;
//}

