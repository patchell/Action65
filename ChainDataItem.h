#pragma once

class CChainDataItem : public CChainItem
{
	int m_DataSize;
	char* m_pData;
	CValue* m_pLabel;
public:
	CChainDataItem();
	~CChainDataItem();
	virtual bool Create();
	virtual void Copy(CChainItem* pI);
	virtual bool Compare(const char* pName);
	virtual int Print(char* pSO, int l, int Indent, const char* s = 0);
	virtual void Emit(CSection* pSec);
	virtual void EmitListing(CSection* pSec);
	void SetDataSize(int S) { m_DataSize = S; }
	int GetDataSize() const { return m_DataSize; }
	void AddData(const char* pD, int S);
	void SetLabelValue(CValue* pL) { m_pLabel = pL; }
	CValue* GetLabelValue() { return m_pLabel; }
};

