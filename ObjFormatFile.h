#pragma once

class CObjFormatFile
{
	char m_strName[512];
	CObjFormatSection* m_pHead;
	CObjFormatSection* m_pTail;
public:
	CObjFormatFile();
	virtual ~CObjFormatFile();
	bool Create(const char* pName);
	CSymTab* GetSymTab();
	char* GetObjectName() { return m_strName; }
	//-------------------------------------------------
	CObjFormatSection* GetHead() { return m_pHead; }
	void SetHead(CObjFormatSection* pHead) { m_pHead = pHead; }
	CObjFormatSection* GetTail() { return m_pTail; }
	void SetTail(CObjFormatSection* pT) { m_pTail = pT; };
	//-------------------------------------------------
	virtual void Print(FILE* pO);
	virtual void Out(FILE* pO);
	//-------------------------------------------------
	void AddSection(CSection* pSection);
	void AddObjFormatSection(CObjFormatSection* pAOFS);
};

