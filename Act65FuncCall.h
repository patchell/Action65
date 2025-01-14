#pragma once

class CAct65FuncCall : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "FUNC-CALL";
public:
	CAct65FuncCall();
	virtual ~CAct65FuncCall();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

