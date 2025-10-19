#pragma once

class KeyWord
{
public:
	enum class KeyWordType {
		NONE,
		ASSEMBLER_DIRECTIVE,
		COMPILER_DIRECTIVE,
		DATA_TYPE,
		STATEMENT,
		FUNCTION,
		OPCODE,
		OPCODE_65C02,
		OPCODE_65C816,
		LABEL_TYPE,
		REGISTER,
		STATUS_FLAG,
		OPERATOR,
		END_OF_KEYWORD_TYPES
	};
	struct KeyWordTypeItem {
		KeyWordType m_Type;
		const char* m_pName;
		KeyWordTypeItem() {
			m_Type = KeyWordType::NONE;
			m_pName = 0;
		}
		KeyWordTypeItem(KeyWordType T, const char* pN) {
			m_Type = T;
			m_pName = pN;
		}
		static const char* KeyWordTypeToString(KeyWordType T);
		static KeyWordType StringToKeyWordType(const char* pN);
	};
private:
	static inline KeyWordTypeItem KeyWordTypesLUT[] = {
		{KeyWordType::NONE,"NONE"},
		{KeyWordType::ASSEMBLER_DIRECTIVE,"ASSEMBLER_DIRECTIVE"},
		{KeyWordType::COMPILER_DIRECTIVE,"COMPILER_DIRECTIVE"},
		{KeyWordType::DATA_TYPE,"DATA_TYPE"},
		{KeyWordType::STATEMENT,"STATEMENT"},
		{KeyWordType::FUNCTION,"FUNCTION"},
		{KeyWordType::OPCODE,"OPCODE"},
		{KeyWordType::OPCODE_65C02,"OPCODE_65C02"},
		{KeyWordType::OPCODE_65C816,"OPCODE_65C816"},
		{KeyWordType::LABEL_TYPE,"LABEL_TYPE"},
		{KeyWordType::REGISTER,"REGISTER"},
		{KeyWordType::STATUS_FLAG,"STATUS_FLAG"},
		{KeyWordType::OPERATOR,"OPERATOR"},
		{KeyWordType::END_OF_KEYWORD_TYPES,NULL}
	};
public:
	Token m_TokenID;	//token value
	const char* m_Name;	//token name
	KeyWordType m_KeyWordType; //what type of keyword
	int m_MaxBytes;			//Maximum number of bytes for instruction
	int m_OpCode;			//base opcode
	Processor m_Processor;	//for which processor
	AdressModeLUT* m_pAddresModeLUT;
	int NumOfAdrModes;
	//------ Methods ------
	KeyWord() {
		m_TokenID = Token(0);
		m_Name = 0;
		m_KeyWordType = KeyWordType::NONE;
		m_MaxBytes = 0;
		m_OpCode = 0;
		m_Processor = Processor::ALL;
		m_pAddresModeLUT = 0;
		NumOfAdrModes = 0;
	}
	KeyWord(
		Token T, 
		const char* pN, 
		KeyWordType KT,
		int MaxB,	//max bytes for instruction 
		int OpC,	//base opcode
		Processor P,	//for which processor
		AdressModeLUT* pLUT,	//addressing mode lookup table
		int NumAdrModes			//number of addressing modes for this opcode
	) {
		m_TokenID = T;
		m_Name = pN;
		m_KeyWordType = KT;
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
	static const char* KeyWordTypeToString(KeyWordType T) { return KeyWordTypeItem().KeyWordTypeToString(T); }
	static KeyWordType StringToKeyWordType(const char* pN) { return KeyWordTypeItem().StringToKeyWordType(pN); }
};

