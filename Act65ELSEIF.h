#pragma once

class CAct65ELSEIF : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ELSEIF";
public:
	CAct65ELSEIF();
	virtual ~CAct65ELSEIF();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};
