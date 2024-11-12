#include "pch.h"


CActionApp ActApp;

CActionApp* Act()
{
	return &ActApp;
}

int main(int argc, char* argv[])
{
	printf("The Action! Compiler for the 6502 Ver 1.0.1\n");
	ActApp.Create(argc, argv);
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
	m_pfSrc = 0;
	m_pfLog = 0;
	m_pfBin = 0;
	m_pfObj = 0;
}

CActionApp::~CActionApp()
{
	if (m_pSourceFile)delete[]m_pSourceFile;
	if (m_pLogFile)delete[]m_pLogFile;
	if (m_pObjectFile)delete[]m_pObjectFile;
	if (m_pBinaryFile)delete[]m_pBinaryFile;
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
	//--------------------------------

	int i;
	int l=0;

	if (argc == 1)
	{
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "action65 <source file> options\n");
		fprintf(stderr, "Options:\n");
		fprintf(stderr, "-L <Log File Name>\n");
		fprintf(stderr, "-B <Binary File Name>\n");
		fprintf(stderr, "-O <Object File Name>\n");
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
				l = strlen(argv[i]) + 1;
				m_pSourceFile = new char[l];
				strcpy_s(m_pSourceFile, l, argv[i]);
			}
			else if (argv[i][0] == '-')
			{
				switch (argv[i][1])
				{
				case 'L':
					++i;
					l = strlen(argv[i]) + 1;
					m_pLogFile = new char[l];
					strcpy_s(m_pLogFile, l, argv[i]);
					break;
				case 'O':
					++i;
					l = strlen(argv[i]) + 1;
					m_pObjectFile = new char[l];
					strcpy_s(m_pObjectFile, l, argv[i]);
					break;
				case 'B':
					++i;
					l = strlen(argv[i]) + 1;
					m_pBinaryFile = new char[l];
					strcpy_s(m_pBinaryFile, l, argv[i]);
					break;
				}
			}
		}
	}
	return true;
}

bool CActionApp::Run()
{
	bool rV = false;
	OpenLog();
	m_ActParse.Create();
	m_ActParse.Run();
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
			exit(1);
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

void CActionApp::CloseAll()
{
	CloseSource();
	CloseLog();
	CloseBin();
	CloseObj();
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
