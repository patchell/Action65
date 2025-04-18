#include "pch.h"


CActionApp ActApp;

CActionApp* Act()
{
	return &ActApp;
}

int main(int argc, char* argv[])
{
	fprintf(stderr, "The Action! Compiler for the 6502 Ver 0.0.3\nApril 8, 2025\n");
	if(ActApp.Create(argc, argv))
		ActApp.Run();
	fprintf(stderr, "Done\n");
}

//--------------------------------------
// Action App Class
//--------------------------------------

CActionApp::CActionApp()
{
	m_pSourceFile = 0;
	m_pLogFile = 0;
	m_pObjectFile = 0;
	m_pBinaryFile = 0;
	m_pAsmSrcOut = 0;
	m_pSettingsFile = 0;
	m_pLinkerScript = 0;
	m_pfSrc = 0;
	m_pfLog = 0;
	m_pfBin = 0;
	m_pfObj = 0;
	m_pfSettings = 0;
	m_pfLinkerScript = 0;
	DO_OD_Stack.Create("DO STACK");
	FOR_Stack.Create("FOR STACK");
	WHILE_Stack.Create("WHILE STACI");
	IF_FI_Stack.Create("IF STACK");
	IFF_FFI_Stack.Create("IFF STACK");
}

CActionApp::~CActionApp()
{
	if (m_pSourceFile)delete[]m_pSourceFile;
	if (m_pLogFile)delete[]m_pLogFile;
	if (m_pObjectFile)delete[]m_pObjectFile;
	if (m_pBinaryFile)delete[]m_pBinaryFile;
	if (m_pAsmSrcOut) delete[] m_pAsmSrcOut;
	if (m_pSettingsFile) delete[]m_pSettingsFile;
	if (m_pLinkerScript) delete[]m_pLinkerScript;
	CloseAll();
}

bool CActionApp::Create(int argc, char* argv[])
{
	//--------------------------------
	// parse the command line
	// 
	// Command line format:
	// action65 <source file> options
	// 
	// options:
	// -L <log file name>
	// -O <object file output
	// -B <binary file output for burning rom>
	// -s <asm file output>
	// -C <Parser Settings File>
	// -K <Linker Script File>
	//--------------------------------

	int i;
	int l=0;
	bool bOpps = false;

	if (argc == 1)
	{
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "action65 <source file> options\n");
		fprintf(stderr, "Options:\n");
		fprintf(stderr, "-L <Log File Name>\n");
		fprintf(stderr, "-B <Binary File Name>\n");
		fprintf(stderr, "-O <Object File Name>\n");
		fprintf(stderr, "-s <Assembly Source File Output>\n");
		fprintf(stderr, "-C <Compiler Settings>\n");
		fprintf(stderr, "-K <Linker Script File>\n");
	}
	else
	{
		for (i = 1; i < argc; ++i)
		{
			if (i == 1)
			{
				//---------------------
				// this had better be
				// the input source
				// file
				//---------------------
				l = (int)strlen(argv[i]) + 1;
				m_pSourceFile = new char[l];
				strcpy_s(m_pSourceFile, l, argv[i]);
			}
			else if (argv[i][0] == '-')
			{
				switch (argv[i][1])
				{
				case 'L':
					++i;
					l = (int)strlen(argv[i]) + 1;
					m_pLogFile = new char[l];
					strcpy_s(m_pLogFile, l, argv[i]);
					break;
				case 'O':
					++i;
					l = (int)strlen(argv[i]) + 1;
					m_pObjectFile = new char[l];
					strcpy_s(m_pObjectFile, l, argv[i]);
					break;
				case 'B':
					++i;
					l = (int)strlen(argv[i]) + 1;
					m_pBinaryFile = new char[l];
					strncpy_s(m_pBinaryFile, l, argv[i], l);
					break;
				case 's':
					++i;
					l = (int)strlen(argv[i]) + 1;
					m_pAsmSrcOut = new char[l];
					strncpy_s(m_pAsmSrcOut, l, argv[i], l);
					break;
				case 'C':
					++i;
					l = (int)strlen(argv[i]) + 1;
					m_pSettingsFile = new char[l];
					strncpy_s(m_pSettingsFile,l,argv[i], l);
					break;
				case 'K':	// linker script
					++i;
					l = (int)strlen(argv[i]) + 1;
					m_pLinkerScript = new char[l];
					strncpy_s(m_pLinkerScript, l, argv[i],l);
					break;
				}
			}
		}
		bOpps = true;
	}
	return bOpps;
}

bool CActionApp::Run()
{
	bool rV = false;
	OpenLog();		// Open Log file
	m_ActParse.Create();	//Create parser
	m_ActParse.Run();		// Run Parser
	return rV;
}

bool CActionApp::OpenSource()
{
	bool rV = false;
	errno_t err;

	if (m_pSourceFile)
	{
		err = fopen_s(&m_pfSrc, m_pSourceFile, "r");
		if (err == 0)
			rV = true;
		else
		{
			fprintf(stderr, "Could not open %s\n", m_pSourceFile);
			CloseAll();
			Act()->Exit(1);
		}
	}
	return rV;
}

bool CActionApp::OpenLog()
{
	bool rV = false;
	errno_t err;

	if (m_pLogFile)
	{
		err = fopen_s(&m_pfLog, m_pLogFile, "w");
		if (err == 0)
			rV = true;
		else
		{
			fprintf(stderr, "Unable to open %s as the log file\n", m_pLogFile);
		}
	}
	return rV;
}

bool CActionApp::OpenObj()
{
	bool rV = false;
	errno_t err;

	if (m_pObjectFile)
	{
		err = fopen_s(&m_pfObj, m_pObjectFile, "wb");
		if (err == 0)
			rV = true;
	}
	return rV;
}

bool CActionApp::OpenBin()
{
	bool rV = false;
	errno_t err;

	if (m_pBinaryFile)
	{
		err = fopen_s(&m_pfBin, m_pBinaryFile, "wb");
		if (err == 0)
			rV = true;
	}
	return rV;
}

bool CActionApp::OpenSettings()
{
	bool rV = false;
	errno_t err;

	if (m_pSettingsFile)
	{
		err = fopen_s(&m_pfSettings, m_pSettingsFile, "r");
		if (err == 0)
			rV = true;
	}
	return rV;
}

bool CActionApp::OpenLinkerScript()
{
	bool rV = false;
	errno_t err;

	if (m_pLinkerScript)
	{
		err = fopen_s(&m_pfLinkerScript, m_pLinkerScript, "r");
		if (err == 0)
			rV = true;
	}
	return rV;
}

void CActionApp::CloseSource()
{
	if(m_pfSrc)
		fclose(m_pfSrc);
	m_pfSrc = 0;
}

void CActionApp::CloseLog()
{
	if(m_pfLog)
		fclose(m_pfLog);
	m_pfLog = 0;
}

void CActionApp::CloseObj()
{
	if(m_pfObj)
		fclose(m_pfObj);
	m_pfObj = 0;
}

void CActionApp::CloseBin()
{
	if(m_pfBin)
		fclose(m_pfBin);
	m_pfBin = 0;
}

void CActionApp::CloseSettings()
{
	if (m_pfSettings) fclose(m_pfSettings);
	m_pfSettings = 0;
}

void CActionApp::CloseLinkerScript()
{
	if (m_pfLinkerScript)
		fclose(m_pfLinkerScript);
	m_pfLinkerScript = 0;
}

void CActionApp::CloseAll()
{
	CloseSource();
	CloseLog();
	CloseBin();
	CloseObj();
	CloseSettings();
	CloseLinkerScript();
}

char* CActionApp::IndentString(char* s, int Indent, int c)
{
	int i;

	for (i = 0; i < Indent;++i)
		s[i] = c;
	s[i] = 0;
	return s;
}


void CActionApp::Dump(
	FILE* pOut, 
	char* pMem, 
	unsigned StartAddress, 
	unsigned Size
)
{
	unsigned Loc = 0;
	bool bDone = false;
	char* s = new char[256];
	char strPad[64];
	int Pad;
	char Ascii[10];
	int l, i, c, n;

	Ascii[8] = 0;
	while (!bDone)
	{
		l = 0;
		l = sprintf_s(s, 256, "%04X ", Loc + StartAddress);
		if ((Size - Loc) <= 0)
		{
			bDone = true;
		}
		else
		{
			if ((Size - Loc) < 8)
				n = Size - Loc;
			else
				n = 8;
			for (i = 0; i < n; ++i)
			{
				l += sprintf_s(&s[l], 256 - l,
					"%02X ", unsigned(pMem[Loc + i]) & 0x0ff);
			}
			if (n < 8)
			{
				Pad = (8 - n) * 3;
				Act()->IndentString(strPad, Pad, ' ');
				l += sprintf_s(&s[l], 256 - l, "%s", strPad);
			}
			for (i = 0; i < n; ++i)
			{
				c = pMem[Loc + i];
				if (c < 0x20)
					c = '.';
				else if (c > 0x7f)
					c = '.';
				Ascii[i] = c;
			}
			fprintf(pOut, "%s %s\n", s, Ascii);
			Loc += 8;
		}
	}
	delete[] s;
}

void CActionApp::Exit(int Err)
{
	if (Err == 2)
		fprintf(stderr, "Internal Error:See Log File\n");
	CloseAll();
	exit(Err);
}

char* CActionApp::CreateIndentString(char* s, int n, int Indent, int c)
{
	int i;

	for (i = 0; (i < Indent) && (i < (n - 1)); ++i)
		s[i] = c;
	s[i] = 0;
	return s;
}
