#pragma once

class CActionApp
{
	char* m_pSourceFile;
	char* m_pLogFile;
	char* m_pObjectFile;
	char* m_pBinaryFile;
	char* m_pAsmSrcOut;
	char* m_pSettingsFile;
	char* m_pLinkerScript;
	FILE* m_pfSrc;
	FILE* m_pfLog;
	FILE* m_pfBin;
	FILE* m_pfObj;
	FILE* m_pfSettings;
	FILE* m_pfLinkerScript;
	CParser m_ActParse;
	//-----------------------------------------
	// Stacks used during code generation
	//-----------------------------------------
	CStack DO_OD_Stack;
	CStack FOR_Stack;
	CStack WHILE_Stack;
	CStack IF_FI_Stack;
	CStack IFF_FFI_Stack;
	//-----------------------------------------
	// Code Generation Utilities
	//-----------------------------------------
	CCodeGeneration CodeGenerationUtils;
	CVirtualReg VREG_Pool;
public:
	CActionApp();
	virtual ~CActionApp();
	virtual bool Create(int argc, char* argv[]);
	virtual bool Run();
	bool OpenSource();
	bool OpenLog();
	bool OpenObj();
	bool OpenBin();
	bool OpenSettings();

    bool OpenLinkerScript();

	void CloseSource();
	void CloseLog();
	void CloseObj();
	void CloseBin();
	void CloseSettings();
	void CloseLinkerScript();
	void CloseAll();
	char* GetSourceFileName() {
		return m_pSourceFile;
	}
	char* GetLogFileName() {
		return m_pLogFile;
	}
	char* GetObjFileName() {
		return m_pObjectFile;
	}
	char* GetBinFileName() {
		return m_pBinaryFile;
	}
	char* GetSettingsFileName() {
		return m_pSettingsFile;
	}
	char* GetLinkerScriptName() {
		return m_pLinkerScript;
	}
	virtual FILE* SrcFile() {
		return m_pfSrc;
	}
	virtual FILE* LogFile() {
		return m_pfLog;
	}
	virtual FILE* ObjFile() {
		return m_pfObj;
	}
	virtual FILE* BinFile() {
		return m_pfBin;
	}
	virtual FILE* GetSettingsFile() {
		return m_pfSettings;
	}
	virtual FILE* GetLinkerScriptFile() {
		return m_pfLinkerScript;
	}
	virtual CParser* GetParser()
	{
		return &m_ActParse;
	}
	virtual CVirtualReg* GetVirtualRegPool() {
		return &VREG_Pool;
	}
	char* IndentString(char* s, int Indent, int c);
	static void Dump(
		FILE* pOut, 
		char* pMem, 
		unsigned StartAddress, 
		unsigned Size
	);
	void Exit(int Err);
	//-----------------------------------------
	// Access Stacks
	//-----------------------------------------
	CStack* GetDOODStack() { return &DO_OD_Stack; }
	CStack* GetFORStack() { return &FOR_Stack; }
	CStack* GetWHILEStack() { return &WHILE_Stack; }
	CStack* GetIFFIStack() { return &IF_FI_Stack;}
	CStack* GetIFFFFIStack() { return &IFF_FFI_Stack; }
	CCodeGeneration* GetCodeGenUtils() { return &CodeGenerationUtils; }
	//-----------------------------------------
	char* CreateIndentString(char* s, int n, int Indent, int c = ' ');
};

extern CActionApp* Act();
