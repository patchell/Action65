#pragma once

class KeyWord
{
public:
	Token m_TokenID;	//token value
	const char* m_Name;	//token name
	int m_MaxBytes;			//Maximum number of bytes for instruction
	int m_OpCode;			//base opcode
	Processor m_Processor;	//for which processor
	AdressModeLUT* m_pAddresModeLUT;
	int NumOfAdrModes;
	//------ Methods ------
	KeyWord();
	KeyWord(
		Token T, 
		const char* pN, 
		int MaxB, 
		int OpC, 
		Processor P, 
		AdressModeLUT* pLUT, 
		int NumAdrModes
	) {
		m_TokenID = T;
		m_Name = pN;
		m_MaxBytes = MaxB;
		m_OpCode = OpC;
		m_Processor = P;
		m_pAddresModeLUT = pLUT;
		NumOfAdrModes = NumAdrModes;
	}
	~KeyWord() {}
	int FindInc(AdrModeType AdrMode);
	int GetNumberOfBytes(AdrModeType AdrMode);
	static Token LookupToToken(const char* pName);
	static const char* LookupToString(Token T);
};

