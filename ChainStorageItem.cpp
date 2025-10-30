#include "pch.h"

CChainStorageItem::CChainStorageItem() :CChainItem(ChainItemType::STORAGE)
{
	m_StorageSize = 0;
	m_pLabel = 0;
}

CChainStorageItem::~CChainStorageItem()
{
}

void CChainStorageItem::Copy(CChainItem* pCI)
{
	CChainStorageItem* pCSI = (CChainStorageItem*)pCI;
	if (pCI)
	{
		if(pCSI->GetLabelValue())
			m_pLabel = pCSI->GetLabelValue();
		m_StorageSize = pCSI->GetStorageSize();
	}
}

bool CChainStorageItem::Compare(const char* pName)
{
	bool rV = false;
	if (m_pLabel && pName)
	{
		char* pLName = m_pLabel->GetName();
		if (pLName)
		{
			rV = strcmp(pLName, pName) == 0;
		}
	}
	return rV;
}

int CChainStorageItem::Print(char* pSO, int l, int Indent, const char* s)
{
	int rV = 0;
	int n = 0;
	if (s)
	{
		n += snprintf(pSO + n, l - n, "%*sStorage Item: %s\n", Indent, "", s);
	}
	else
	{
		n += snprintf(pSO + n, l - n, "%*sStorage Item:\n", Indent, "");
	}
	if (m_pLabel)
	{
		char LabelStr[256];
		m_pLabel->Print(LabelStr, sizeof(LabelStr), Indent + 2, "Label");
		n += snprintf(pSO + n, l - n, "%s", LabelStr);
	}
	n += snprintf(pSO + n, l - n, "%*sStorage Size: %d\n", Indent + 2, "", m_StorageSize);
	rV = n;
	return rV;
}

void CChainStorageItem::Emit(CSection* pSec)
{
	//if (m_pLabel)
	//{
	//	m_pLabel->SetAddress(pSec->GetLocationCounter());
	//}
	//pSec->IncrementLocationCounterBy(m_StorageSize);
}

void CChainStorageItem::EmitListing(CSection* pSec)
{
	int l = 16000;
	char* pLabelStr = new char[l];
	int ls = 0;

	memset(pLabelStr, 0, l);
	if (m_pLabel)
	{
//		ls += m_pLabel->Print(pLabelStr, l, 0, "Label:");
	}
	fprintf_s(Act()->GetListingFile(), "$%04X %s Storage %d bytes\n", 
		pSec->GetLocationCounter(), 
		pLabelStr, 
		m_StorageSize
	);
	delete[] pLabelStr;
}


