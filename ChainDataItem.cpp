#include "pch.h"

CChainDataItem::CChainDataItem() : CChainItem(ChainItemType::DATA)
{
	m_DataSize = 0;
	m_pData = 0;
	m_pLabel = 0;
}

CChainDataItem::~CChainDataItem()
{
}

bool CChainDataItem::Create()
{
	bool rV = true;
	if (m_DataSize > 0)
	{
		m_pData = new char[m_DataSize];
		if (!m_pData)
			rV = false;
	}
	return rV;
}

void CChainDataItem::Copy(CChainItem* pCI)
{
	CChainDataItem* pCDI = (CChainDataItem*)pCI;
	if (pCI)
	{
		if(pCDI->GetLabelValue())
			m_pLabel = pCDI->GetLabelValue();
		m_DataSize = pCDI->GetDataSize();
		if (m_DataSize > 0)
		{
			m_pData = new char[m_DataSize];
			if (m_pData && pCDI->m_pData)
			{
				memcpy(m_pData, pCDI->m_pData, m_DataSize);
			}
		}
	}
}

bool CChainDataItem::Compare(const char* pName)
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

int CChainDataItem::Print(char* pSO, int l, int Indent, const char* s)
{
	int rV = 0;
	int n = 0;
	if (s)
	{
		n += snprintf(pSO + n, l - n, "%*sData Item: %s\n", Indent, "", s);
	}
	else
	{
		n += snprintf(pSO + n, l - n, "%*sData Item:\n", Indent, "");
	}
	if (m_pLabel)
	{
		char LabelStr[256];
		m_pLabel->Print(LabelStr, sizeof(LabelStr), Indent + 2, "Label");
		n += snprintf(pSO + n, l - n, "%s", LabelStr);
	}
	n += snprintf(pSO + n, l - n, "%*sData Size: %d\n", Indent + 2, "", m_DataSize);
	rV = n;
	return rV;
}

void CChainDataItem::Emit(CSection* pSec)
{
	if (pSec && m_pData && m_DataSize > 0)
	{
		pSec->AddData(m_DataSize, m_pData, m_pLabel);
	}
}

void CChainDataItem::EmitListing(CSection* pSec)
{
	if (m_pLabel)
	{
		char LabelStr[256];
		m_pLabel->Print(LabelStr, sizeof(LabelStr), 0, "Label");
		fprintf_s(Act()->GetListingFile(), "%s", LabelStr);
	}
	fprintf_s(Act()->GetListingFile(), "%04X %*sData %d bytes\n", pSec->GetLocationCounter(), 4, "", m_DataSize);
}
