#pragma once

class CAct65AssignXOR : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "=XOR";
public:
	CAct65AssignXOR();
	virtual ~CAct65AssignXOR();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

