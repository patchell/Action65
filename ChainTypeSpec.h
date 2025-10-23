#pragma once

class CChainTypeSpec : public CChain
{
public:
	CChainTypeSpec();
	virtual ~CChainTypeSpec() {}
	bool Create();
	virtual void CopyTypeChain(CChain* pC) { CChain::Copy(pC); }
	bool Is(CChainTypeSpecItem::Spec Type);
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
		return Is(CChainTypeSpecItem::Spec::BYTE) || 
			Is(CChainTypeSpecItem::Spec::CHAR) ||
			Is(CChainTypeSpecItem::Spec::BOOL
			);
	}
	bool IsWord() {
		return Is(CChainTypeSpecItem::Spec::INT) || Is(CChainTypeSpecItem::Spec::CARD) || Is(CChainTypeSpecItem::Spec::BOOL);
	}
	CChainTypeSpecItem::Spec GetFundSpec();
};