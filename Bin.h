#pragma once

constexpr auto MAX_NAME_LEN = 64;
class CBin
{
	CBin* m_pNext;
	CBin* m_pPrev;
	char* m_pName;
public:
	CBin() {
		m_pNext = 0;
		m_pPrev = 0;
		m_pName = new char[MAX_NAME_LEN];
	}
	virtual ~CBin() {
		if (m_pName)
			delete[]m_pName;
	};
	CBin* GetNext() { return m_pNext; }
	void SetNext(CBin* pB) { m_pNext = pB; }
	CBin* GetPrev() { return m_pPrev; }
	void SetPrev(CBin* pB) { m_pPrev = pB; }
	 virtual char* GetName(void) { return m_pName; }
	 virtual void SetName(const char* pName) {
		int l = (int)strlen(pName) + 1;
		if (m_pName) delete m_pName;
		m_pName = new char[l];
		strcpy_s(m_pName, l, pName);
	}
	 virtual BOOL Compare(const char *name, int aux = 0) = 0;
//	virtual BOOL Compare(const char* name) {
//		BOOL rV = FALSE;
//		if (strcmp(m_pName, name) == 0) rV = TRUE;
//		return rV;
	//}
	virtual void Print(FILE* pOut, const char* s = 0);
};

