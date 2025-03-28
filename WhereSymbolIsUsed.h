#pragma once

class CWhereSymbolIsUsed : public CBin
{
public:
	enum UnResolvedType {
		ABSOLUTE_REFERENCE,
		RELATIVE_REFERENCE
	};
private:
	struct UnResolvedSymItem {
		UnResolvedType m_Type;
		const char* m_pName;
		UnResolvedSymItem() {
			m_Type = ABSOLUTE_REFERENCE;
			m_pName = 0;
		}
		UnResolvedSymItem(UnResolvedType t, const char* pS) {
			m_Type = t;
			m_pName = pS;
		}
	};
	inline static UnResolvedSymItem UnResolvedTypeLUTLUT[2] = {
		{ABSOLUTE_REFERENCE,"ABSOLUTE REFERENCE"},
		{RELATIVE_REFERENCE,"RELATIVE REFERENCE"}
	};
	int m_Address;	//address location where found
	CSection* m_pSection;	// section block where found
	UnResolvedType m_UnResRefType;	//type of reference
public:
	CWhereSymbolIsUsed();
	virtual ~CWhereSymbolIsUsed();
	bool Create();
	virtual int Print(char* pSO, int l, const char* s);
	virtual bool Compare(const char* name, int scope);
	void SetAddress(int a);
	virtual unsigned GetAddress() const { return m_Address; }
	void SetSection(CSection* pS) { 
		m_pSection = pS; 
	}
	CSection* GetSection() { 
		return m_pSection; 
	}
	void SetUnResType(UnResolvedType type) { m_UnResRefType = type; }
	UnResolvedType GetUnResType() const { return m_UnResRefType; }
};

