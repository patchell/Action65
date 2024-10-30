#pragma once

class CAccessModeStackItem : public CStackItem
{
	CSection::Mode m_AccessMode;
public:
	CAccessModeStackItem();
	virtual ~CAccessModeStackItem();
	bool Create();
	CSection::Mode GetAccessMode() { return m_AccessMode; }
	void SetAccessMode(CSection::Mode AM) { m_AccessMode = AM; }
};

