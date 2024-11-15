#pragma once

class CActionApp
{
	char* m_pSourceFile;
	char* m_pLogFile;
	char* m_pObjectFile;
	char* m_pBinaryFile;
	FILE* m_pfSrc;
	FILE* m_pfLog;
	FILE* m_pfBin;
	FILE* m_pfObj;
	CParser m_ActParse;
public:
	CActionApp();
	virtual ~CActionApp();
	virtual bool Create(int argc, char* argv[]);
	virtual bool Run();
	bool OpenSource();
	bool OpenLog();
	bool OpenObj();
	bool OpenBin();

	void CloseSource();
	void CloseLog();
	void CloseObj();
	void CloseBin();
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
	virtual CParser* GetParser()
	{
		return &m_ActParse;
	}
	char* IndentString(char* s, int Indent, int c);
	static void Dump(
		FILE* pOut, 
		char* pMem, 
		unsigned StartAddress, 
		unsigned Size
	);
	//-----------------------------------------
	char* CreateIndentString(char* s, int n, int Indent, int c = ' ');
};

extern CActionApp* Act();
