#pragma once

class CAct65AddrOfINTERRUPT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ADDRESS OF ISR";
public:
	CAct65AddrOfINTERRUPT();
	virtual ~CAct65AddrOfINTERRUPT();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

