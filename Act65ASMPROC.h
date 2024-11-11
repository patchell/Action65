#pragma once

class CAct65ASMPROC : public CAstNode
{
	inline static const char* m_pNodeTyypeName = "ASMPROC";
public:
	CAct65ASMPROC();
	virtual ~CAct65ASMPROC();
	bool Create();
};

