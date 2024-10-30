#pragma once

class CSection;

class CStackSectionItem : public CStackItem
{
	CSection* m_pSection;
public:
	CStackSectionItem();
	~CStackSectionItem();
	bool Create(CSection* pSect);
	void SetSection(CSection* pSect) { m_pSection = pSect; }
	CSection* GetSection() { return m_pSection; }
};

