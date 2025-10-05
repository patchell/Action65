#pragma once

class CChainMisc : public CChain
{
public:
	CChainMisc() : CChain(CChain::ChainType::CHAINMISC) {}
	virtual ~CChainMisc() {}
	bool Create() { return CChain::Create(); }
	virtual void Copy(CChain* pC) { CChain::Copy(pC); }
};

