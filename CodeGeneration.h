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
	bool Create(CAstTree* pAstTree);
	bool Run();
	//---------------------------
	void EmitBinary();
	void EmitSource();
	//-------- Code Generation ------
};

