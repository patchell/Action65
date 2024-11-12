#pragma once

class CAct65CodeBlock : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "CODE-BLOCK";
public:
	CAct65CodeBlock();
	virtual ~CAct65CodeBlock();
	bool Create(CAstNode* pChild, CAstNode* pNext);
	virtual CValue* Process();
};

