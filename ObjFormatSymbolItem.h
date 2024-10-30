#pragma once

class CObjFormatSymbolItem : public CObjFormatSectionItem
{
	char m_strName[MAX_SYMBOL_NAME_LEN];
	unsigned m_DefinedAddress;
public:
	CObjFormatSymbolItem();
	virtual ~CObjFormatSymbolItem();
	bool Create(CSymbol* pSym);
	virtual void Out(FILE* pOut);
	virtual void Print(FILE* pO);
};

