#pragma once

class CAddressSizeStackItem : public CStackItem
{
	CSection::AddressSize m_OpRandAdrSize;
public:
	CAddressSizeStackItem();
	virtual ~CAddressSizeStackItem();
	bool Create();
	CSection::AddressSize GetOprandSize() { return m_OpRandAdrSize; }
	void SetOprandSize(CSection::AddressSize OpSize) { m_OpRandAdrSize = OpSize; }
};

