#pragma once

class CObjFormatSymbols : public CObjFormatSectionItem
{
	CObjFormatSymbolItem* m_pHead;
	CObjFormatSymbolItem* m_pTail;
public:
	CObjFormatSymbols();
	virtual ~CObjFormatSymbols();
	bool Create();
	virtual void Print(FILE* pO, int Indent);
	virtual void Out(FILE* pO);
	virtual void Add(CObjFormatSymbolItem pSym);
};

