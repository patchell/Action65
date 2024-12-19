#pragma once

class CAct65FlagCARRY : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".C";
public:
	CAct65FlagCARRY();
	virtual ~CAct65FlagCARRY();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};
