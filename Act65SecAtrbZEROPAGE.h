#pragma once

class CAct65SecAtrbZEROPAGE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ZERO-PAGE";
public:
	CAct65SecAtrbZEROPAGE();
	virtual ~CAct65SecAtrbZEROPAGE();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

