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
	void SetAddress(int a);
	int GetAddress() { return m_Address; }
	void SetSection(CSection* pS) { 
		m_pSection = pS; 
	}
	CSection* GetSection() { 
		return m_pSection; 
	}
	void SetUnResType(UnResolvedType type) { m_UnResRefType = type; }
	UnResolvedType GetUnResType() { return m_UnResRefType; }
};

