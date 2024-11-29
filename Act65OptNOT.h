#pragma once

class CAct65OptNOT : public CAstNode
{
public:
	CAct65OptNOT();
	virtual ~CAct65OptNOT();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

