#pragma once

class CAct65SECTION : public CAstNode
{
	CSection* m_pSection;
	inline static const char* m_pNodeTyypeName = "SECTION";
public:
	CAct65SECTION();
	virtual ~CAct65SECTION();
	virtual bool Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec);
	virtual CValue* Process(SAuxEmitInfo* pAuxInfo);
	virtual int Print(char* s, int l, int Indent, const char* pAuxStr, bool* pbNextFlag);
	virtual void PrintNode(FILE* pOut, int Indent, bool* pbNextFlag);
	virtual CValue* Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo);
	virtual const char* GetNodeName() { 
		return m_pNodeTyypeName; 
	}
	//void SetSection(CSection* pSection) {
	//	m_pSection = pSection;
	//}
	//CSection* GetSection() { 
	//	return m_pSection; 
	//}
};

