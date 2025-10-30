#pragma once

class CChainStorageItem : public CChainItem
{
	int m_StorageSize;
	CValue* m_pLabel;
public:
	CChainStorageItem();
	~CChainStorageItem();
	virtual void Copy(CChainItem* pI);
	virtual bool Compare(const char* pName);
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
	virtual void Emit(CSection* pSec);
	virtual void EmitListing(CSection* pSec);
	void SetStorageSize(int S) { m_StorageSize = S; }
	int GetStorageSize() const { return m_StorageSize; }
	void SetLabelValue(CValue* pL) { m_pLabel = pL; }
	CValue* GetLabelValue() { return m_pLabel; }
};

