#pragma once

class CObjFormatSection
{
	struct SObjSectionData {
		//-----------------------------------------------
		//	‘SECTION’, \0        8 Bytes
		//	< Section Name>    32 bytes, 0 terminated
		//	< Attributes        2 bytes
		//	< Section Token>    2 bytes
		//	< File Offset From 0 of data>    4 bytes
		//-----------------------------------------------
		char m_BlockTypeName[8];
		char m_SectionName[32];
		unsigned short m_Attributes;
		unsigned short m_SectionToken;
		unsigned m_FileOffset;
		SObjSectionData() {
			int i;

			m_Attributes = 0;
			m_SectionToken = 0;
			m_FileOffset = 0;
			strcpy_s(m_BlockTypeName, 8, "SECTION");
			for (i = 0; i < 32; ++i)
				m_SectionName[i] = 0;
		}
	};
	CSection* m_pSection;
	CObjFormatSection* m_pNext;
	CObjFormatSection* m_pPrev;
	CObjFormatSectionItem* m_pHead;
	CObjFormatSectionItem* m_pTail;
public:
	CObjFormatSection();
	virtual ~CObjFormatSection();
	bool Create(CSection* pSection);
	CSection* GetSection() { return m_pSection; }
	//--------------------------------------------------
	CObjFormatSectionItem* GetHead() { return m_pHead; }
	void SetHead(CObjFormatSectionItem* pH) { m_pHead = pH; }
	CObjFormatSectionItem* GetTail() { return m_pTail; }
	void SetTail(CObjFormatSectionItem* pT) { m_pTail = pT; }
	//---------------------------------------------------
	void AddItem(CObjFormatSectionItem* pItem);
	//-------------------------------------------------
	virtual void Print(FILE* pO, int Indent);
	virtual void Out(FILE* pO);
	//------------------------------------------------
	void SetNext(CObjFormatSection* pN) { m_pNext = pN; }
	CObjFormatSection* GetNext() { return m_pNext; }
	void SetPrev(CObjFormatSection* pP) { m_pPrev = pP; }
	CObjFormatSection* GetPrev() { return m_pPrev; }
};

