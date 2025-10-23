#include "pch.h"


const char* CReg::RegTypeItem::GetRegName(RegType Reg)
{
	return RegTypeLUT[int(Reg)].m_pName;
}


bool CReg::CreateTypeChain(CChainTypeSpec* pTC)
{
	m_pTypeChain = new CChainTypeSpec;
	if (pTC)
	{
		//--------------------------------------
		// Move the type chain objects into the
		// new type chain
		//--------------------------------------
		CChainTypeSpecItem* pTCobj = (CChainTypeSpecItem*)pTC->GetHead();
		while (pTCobj)
		{
			m_pTypeChain->AddToTail(pTCobj);
			pTC->Unlink(pTCobj);
			pTCobj = (CChainTypeSpecItem*)pTC->GetHead();
		}
	}
	return true;
}
