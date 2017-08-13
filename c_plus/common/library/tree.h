#ifndef __TREE_H__
#define __TREE_H__

#include "data_type.h"

enum tree234_key_type {tree234_key_type_number, tree234_key_type_string};

typedef void (*erase_trigger_t) (void* p_data, size_t len, void* p_object);

class XString;
class XTree;
class XNode234;

class XTreeKey
{
friend XTree;
friend XNode234;

public:
	XTreeKey();
	~XTreeKey();

	void*	m_p_object;
	void*	m_p_data;
	size_t	m_len;
	TCHAR*	get_key ();
	bool	set_key (const TCHAR* p_key);
	//uint32 m_key;

private:
	bool create(const TCHAR* p_key, void* p_data, size_t len, void* p_object);
	int compare (const TCHAR* p_key);
	int compare (XNode234* pNode);
	int compare (XTreeKey* pTreeKey);
	//int compare (uint32 key);

	XTreeKey* copy ();
	XTreeKey* copy (XTreeKey* pTreeKey);

	XString* m_pKey;
	//int	m_type;
};
class XNode234
{

public:
	XNode234();
	~XNode234();

//private:
	int compare (XNode234* pNode);
	uint32 get_key_count ();
	uint32 get_child_count ();
	void sort_keys ();
	void sort_children ();

	XNode234* m_pChildren[4];
	XNode234* m_pParent;

	XTreeKey* m_pKeys[3];
	bool m_copy_flag;	// 모든 노드를 복사 할때 
	// 
};

class XTree
{
public:
	XTree();
	~XTree();

	void create (size_t data_len, erase_trigger_t erase_trigger);
	void clear ();
	bool insert (const TCHAR* p_key, void* p_data, size_t len);
	void* search (const TCHAR* key, void* p_data, size_t in_len);
	bool update (TCHAR* p_key, void* p_data, size_t len);
	bool erase (const TCHAR* p_key);
	void erase_all ();
	bool get_root_key (TCHAR* p_root_key, size_t len);
#if defined (__WINDOWS)
	bool erase (const char* p_key);
	bool insert (const char* pKey, void* p_data);
	void* search (const char* pKey);
#endif
	bool copy_key (XTreeKey* pKey, uint32 in_key_count, uint32* p_out_key_count);

	// void* 
	bool insert (const TCHAR* pKey, void* p_data);
	void* search (const TCHAR* pKey);
	bool update (TCHAR* pKey, void* p_data);

	uint32 get_key_count ();
	
	// 키를 uint32 타입도 지원함
	bool insert (uint32 key, void* p_data, size_t len);
	bool insert (void* p_data, size_t len);
	void* search (uint32 key, void* p_data, size_t in_len);
	bool update (uint32 key, void* p_data, size_t len);
	bool erase (uint32 key);
	// void* 
	bool insert (uint32 key, void* p_data);
	bool insert (void* p_object);
	void* search (uint32 key);
	bool update (uint32 key, void* p_data);



	// test
	XNode234* get_root_node ();
	uint32 get_depth ();
	void calc_depth (XNode234* pNode, uint32* p_depth);
	bool merge (TCHAR* p_key);
private:
	
	//bool erase (uint32 key);
	bool split (XNode234* pNode234);
	bool erase (XNode234* pNode, const TCHAR* p_key);
	bool insert (const TCHAR* p_key, void* p_data, size_t len, void* p_object);
	bool insert (XNode234* pNode234, const TCHAR* p_key, void* p_data, size_t len, void* p_object);
	bool update (TCHAR* p_key, void* p_data, size_t len, void* p_object);
	XNode234* merge (XNode234* pNode);
	bool brother_merge (XNode234* pNode);
	int check_brother_merge (XNode234* pNode, uint32* p_my_offset);
	XNode234* find_delete_leaf_node(XNode234* pNode);
	XNode234* search (XNode234* pNode, const TCHAR* p_key, uint32* p_key_offset);
	void change_data (XTreeKey* pTreeKeyA, XTreeKey* pTreeKeyB);
	bool swap_key (const TCHAR* p_key, const TCHAR* p_leaf_key);
	bool copy_key (XNode234* pNode, uint32* p_out_key_count);

	XNode234* m_pRoot;
    int m_key_type;	// 0: 키타입이 숫자, 1: 문자열
	uint32 m_key_count;
	size_t m_data_len;
	erase_trigger_t m_erase_trigger;

	//uint32 m_copy_count;
	XTreeKey* m_pCopyKey;
	// for debugging
	XNode234* m_pDebugNode;
};

#endif
