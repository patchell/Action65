#pragma once

class CAct65LogicalOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "L-OR";
public:
	CAct65LogicalOR();
	virtual ~CAct65LogicalOR();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

