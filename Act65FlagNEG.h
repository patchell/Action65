#pragma once

class CAct65FlagNEG : public CAstNode
{
	inline static const char* m_pNodeTyypeName = ".N";
public:
	CAct65FlagNEG();
	virtual ~CAct65FlagNEG();
	bool Create(CAstNode* pChild = 0, CAstNode* pNext = 0);
	virtual CValue* Process();
	virtual void Print(FILE* pOut, int Indent, char* s);
};

