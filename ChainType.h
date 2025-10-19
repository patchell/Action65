#pragma once

class CChainType : public CChain
{
public:
	CChainType();
	virtual ~CChainType() {}
	bool Create();
	virtual void CopyTypeChain(CChain* pC) { CChain::Copy(pC); }
	bool Is(CChainTypeItem::Spec Type);
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
		return Is(CChainTypeItem::Spec::BYTE) || 
			Is(CChainTypeItem::Spec::CHAR) ||
			Is(CChainTypeItem::Spec::BOOL
			);
	}
	bool IsWord() {
		return Is(CChainTypeItem::Spec::INT) || Is(CChainTypeItem::Spec::CARD) || Is(CChainTypeItem::Spec::BOOL);
	}
	CChainTypeItem::Spec GetFundSpec();
};