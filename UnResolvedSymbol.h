#pragma once

class CUnResolvedSymbol: public CBin
{
	unsigned m_Address;
	CSection* m_pSection;
	CWhereSymbolIsUsed::UnResolvedType m_UnResRefType;
public:
	CUnResolvedSymbol();
	virtual ~CUnResolvedSymbol();
	bool Create();
	void SetAddress(unsigned a);
};

