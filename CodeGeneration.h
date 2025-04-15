#pragma once

class CCodeGeneration
{

public:
	class CodeTemplate {
		char* m_pName;
	public:
		CodeTemplate();
		virtual ~CodeTemplate();
	};
	class CodeObject {
		const char* m_pName;
		
	public:
		CodeObject();
		virtual ~CodeObject();
	};
	class CodeTable {
	public:
		CodeTable();
		virtual ~CodeTable();
	};
public:
	CCodeGeneration();
	virtual ~CCodeGeneration();
	//---------------------------
	CValue* EmitBinaryOp(Token Op, CValue* pV1, CValue* pV2, CValue* pV3, CSection* pSection);
	void EmitSource();
};

