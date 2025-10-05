#pragma once

class CChainType : public CChain
{
public:
	CChainType();
	virtual ~CChainType() {}
	bool Create();
	virtual void CopyTypeChain(CChain* pC) { CChain::Copy(pC); }
	bool Is(CChainTypeObject::Spec Type);
	bool IsFundamentalType();
	bool MoveTypeChain(CChain* pC) {
		if (pC) {
			while (pC->GetHead()) {
				CChainItem* pItem = pC->GetHead();
				pC->Unlink(pItem);
				AddToTail(pItem);
			}
			return true;
		}
		return false;
	}
	bool IsByte() {
		return Is(CChainTypeObject::Spec::BYTE) || 
			Is(CChainTypeObject::Spec::CHAR) ||
			Is(CChainTypeObject::Spec::BOOL
			);
	}
	bool IsWord() {
		return Is(CChainTypeObject::Spec::INT) || Is(CChainTypeObject::Spec::CARD) || Is(CChainTypeObject::Spec::BOOL);
	}
	CChainTypeObject::Spec GetFundSpec();
};