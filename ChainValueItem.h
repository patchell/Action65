#pragma once

class CChainValueItem : public CChainItem
{
	CValue* m_pValue;
	int m_Location;	// Location in Section, not the ADDRESS
public:
	CChainValueItem();
	virtual ~CChainValueItem();
	virtual bool Create() { return true; }
	virtual void Copy(CChainItem* pI);
	virtual bool Compare(const char* pName);
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
	void SetValue(CValue* pVal) { m_pValue = pVal; }
	CValue* GetValue() { return m_pValue; }
	void SetLocation(int L) { m_Location = L; }
	int GetLocation() const { return m_Location; }
	virtual void Emit(CSection* pSec) {}
	virtual void EmitListing(CSection* pSec);
};

