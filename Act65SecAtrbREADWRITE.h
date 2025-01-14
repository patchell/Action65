#pragma once

class CAct65SecAtrbREADWRITE : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "R/W";
public:
	CAct65SecAtrbREADWRITE();
	virtual ~CAct65SecAtrbREADWRITE();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s, int l);
};

