#include "pch.h"


const char* CReg::RegTypeItem::GetRegName(RegType Reg)
{
	return RegTypeLUT[int(Reg)].m_pName;
}


bool CReg::CreateTypeChain(CTypeChain* pTC)
{
	m_pTypeChain = new CTypeChain;
	if (pTC)
	{
		//--------------------------------------
		// Move the type chain objects into the
		// new type chain
		//--------------------------------------
		CObjTypeChain* pTCobj = pTC->GetHead();
		while (pTCobj)
		{
			m_pTypeChain->AddToTail(pTCobj);
			pTC->Unlink(pTCobj);
			pTCobj = pTC->GetHead();
		}
	}
	return true;
}
