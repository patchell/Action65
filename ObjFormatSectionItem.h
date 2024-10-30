#pragma once

class CObjFormatSectionItem

{
public:
	enum class FormatType {
		NONE,
		CODE,
		DATA,
		RELOCATIONTABEL,
		SYMBOLS,
		SYMBOL
	};
private:
	struct FormatTypeLUT {
		FormatType m_LutType;
		const char* m_pLutString;
	};
	inline static const FormatTypeLUT FormatStringLUT[] = {
		{FormatType::NONE, "NONE"},
		{FormatType::CODE, "CODE"},
		{FormatType::DATA, "DATA"},
		{FormatType::RELOCATIONTABEL, "RELOCATION TABEL"},
		{FormatType::SYMBOLS, "SYMBOL Table"},
		{FormatType::SYMBOL, "SYMBOL"},
		{FormatType(-1), NULL},
	};
	FormatType m_Type;
	CObjFormatSectionItem* m_pNext;
	CObjFormatSectionItem* m_pPrev;
public:
	CObjFormatSectionItem();
	virtual ~CObjFormatSectionItem();
	bool Create(FormatType Type);
	CObjFormatSectionItem* GetNext() { return m_pNext; }
	void SetNext(CObjFormatSectionItem* pN) { m_pNext = pN; }
	CObjFormatSectionItem* GetPrev() { return m_pPrev; }
	void SetPrev(CObjFormatSectionItem* pP) { m_pPrev = pP; }
	virtual void Out(FILE* pOut);
	virtual void Print(FILE* pO, int Indent);
	static const char* LookupFormatSectionType(FormatType Type);
	static FormatType LookupFormatSectionType(const char* pName);
};

