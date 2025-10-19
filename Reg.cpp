#include "pch.h"


const char* CReg::RegTypeItem::GetRegName(RegType Reg)
{
	return RegTypeLUT[int(Reg)].m_pName;
}


bool CReg::CreateTypeChain(CChainType* pTC)
{
	m_pTypeChain = new CChainType;
	if (pTC)
	{
		//--------------------------------------
		// Move the type chain objects into the
		// new type chain
		//--------------------------------------
		CChainTypeItem* pTCobj = (CChainTypeItem*)pTC->GetHead();
		while (pTCobj)
		{
			m_pTypeChain->AddToTail(pTCobj);
			pTC->Unlink(pTCobj);
			pTCobj = (CChainTypeItem*)pTC->GetHead();
		}
	}
	return true;
}
