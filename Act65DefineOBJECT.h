#pragma once

class CAct65DefineOBJECT : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "DEF";
public:
	CAct65DefineOBJECT();
	virtual ~CAct65DefineOBJECT();
	virtual bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0, CBin* pSym = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent);
};

