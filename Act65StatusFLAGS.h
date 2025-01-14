#pragma once

class CAct65StatusFLAGS : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "SR FLAGS";
public:
	CAct65StatusFLAGS();
	virtual ~CAct65StatusFLAGS();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

