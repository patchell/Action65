#pragma once

class CDataSizeStackItem : public CStackItem
{
	int m_ObjectSize;
public:
	CDataSizeStackItem();
	virtual ~CDataSizeStackItem();
	bool Create();
	void SetSize(int size) { m_ObjectSize = size; }
	int GetSize() { return m_ObjectSize; }
};

