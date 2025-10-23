#include "pch.h"

CChainLocalItem::CChainLocalItem() :CChainItem(CChainItem::ChainItemType::LOCAL_SYMBOL)
{
	m_pValue = nullptr;
}

CChainLocalItem::~CChainLocalItem()
{
	
}

bool CChainLocalItem::Create(CSymbol* pSym)
{
	if (m_pValue)
	{
		m_pValue->SetSymbol(pSym);
	}
	else
	{
		m_pValue = new CValue;
		m_pValue->Create(pSym);
	}
	return true;
}

void CChainLocalItem::Copy(CChainItem* pI)
{
	if (pI->Is(CChainItem::ChainItemType::LOCAL_SYMBOL))
	{
		CChainLocalItem* pLI = static_cast<CChainLocalItem*>(pI);
		if (pLI->m_pValue)
		{
			if (m_pValue)
				delete m_pValue;
			m_pValue = new CValue;
			m_pValue->SetSymbol(pLI->m_pValue->GetSymbol());
		}
	}
}
bool CChainLocalItem::Is(const char* pName)
{
	bool rV = false;

	if (GetName())
	{
		if (strcmp(pName, GetName()) == 0)
			rV = true;
	}
	return rV;
}

bool CChainLocalItem::Compare(const char* pName)
{
	return Is(pName);
}

CSymbol* CChainLocalItem::GetSymbol()
{
	CSymbol* rV = nullptr;

	if (m_pValue)
	{
		rV = m_pValue->GetSymbol();
	}
	return rV;
}

char* CChainLocalItem::GetName()
{
	char* rV = nullptr;

	if (m_pValue)
	{
		rV = m_pValue->GetName();
	}
	return rV;
}

int CChainLocalItem::Print(char* pSO, int l, int Indent, const char* s)
{
	int rV = 0;

	if (m_pValue)
	{
		rV = m_pValue->Print(pSO, l, Indent, s);
	}
	return rV;
}
