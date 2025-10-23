#pragma once

class CActionApp
{
	//-----------------------------------------
	// File Names and File Pointers
	//-----------------------------------------
	char* m_pSourceFile;	// Source file
	char* m_pLogFile;		// Log file
	char* m_pObjectFile;	// Object file
	char* m_pBinaryFile;	// Binary file
	char* m_pAsmSrcOut;		// ASM source output
	char* m_pSettingsFile;	// Settings file
	char* m_pLinkerScript;	// Linker script
	char* m_pListingFile;	// Listing file
	FILE* m_pfSrc;
	FILE* m_pfLog;
	FILE* m_pfBin;
	FILE* m_pfObj;
	FILE* m_pfSettings;
	FILE* m_pfLinkerScript;
	FILE* m_pfListing;
	//-----------------------------------------
	CCodeGeneration m_CodeGenerationUtils;
	CParser m_ActParse;
	CAstTree m_AstTree;
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
	bool OpenListing();

	void CloseSource();
	void CloseLog();
	void CloseObj();
	void CloseBin();
	void CloseSettings();
	void CloseLinkerScript();
	void CloseListing();
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
	virtual FILE* GetListingFile() {
		return m_pfListing;
	}
	virtual CParser* GetParser()
	{
		return &m_ActParse;
	}

	char* IndentString(char* s, int StringLength, int Indent, int c);
	static void Dump(
		FILE* pOut, 
		char* pMem, 
		unsigned StartAddress, 
		int Size
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
	//-----------------------------------------
	char* CreateIndentString(char* s, int n, int Indent, int c = ' ');
	CParser::CLexer* GetLexer() { return m_ActParse.GetLexer(); }
	CSymTab* GetSymTab() { return m_ActParse.GetLexer()->GetSymTab(); }
	CCodeGeneration* GetCodeGen() { return &m_CodeGenerationUtils; }
};

extern CActionApp* Act();
