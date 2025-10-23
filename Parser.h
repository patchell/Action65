#pragma once

//-------------------------------------
// Allowed Addressing Modes
//-------------------------------------

constexpr auto ADR_MODE_ABSOLUTE = 0x0001;
constexpr auto ADR_MODE_ABSOLUTE_X = 0x0002;
constexpr auto ADR_MODE_ABSOLUTE_Y = 0x0041;
constexpr auto ADR_MODE_ZEROPAGE = 0x0008;
constexpr auto ADR_MODE_ZEROPAGE_X = 0x0010;
constexpr auto ADR_MODE_ZEROPAGE_Y = 0x0020;
constexpr auto ADR_MODE_IMMEDIATE = 0x0040;
constexpr auto ADR_MODE_INDIRECT_Y = 0x0080;
constexpr auto ADR_MODE_INDIRECT_X = 0x0100;
constexpr auto ADR_MODE_INDIRECT = 0x0200;
constexpr auto ADR_MODE_RELATIVE = 0x0400;
constexpr auto ADR_MODE_ACCUMULATOR = 0x0800;
constexpr auto ADR_MODE_IMPLIED = 0x1000;

constexpr auto PHASE_LUT_DIM = 5;

class CParser
{
	Token m_LookAheadToken;
//------------------------------
// Lexer Stuff
//------------------------------
public:
	class CLexer
	{
		//---------------------------------------
		// we are all friends here
		//---------------------------------------
		friend class SubParser;
		friend class SubLexer;
		friend class LexSubParser;
		friend class CParser;
		//alu access modes
		static inline AdressModeItem Order8[8] = {
			{AdrModeType::INDEXED_INDIRECT_X_ADR, 0, 2},
			{AdrModeType::ZERO_PAGE_ADR, 4, 2},
			{AdrModeType::IMMEDIATE_ADR, 8, 2},
			{AdrModeType::ABSOLUTE_ADR, 12, 3},
			{AdrModeType::INDIRECT_INDEXED_Y_ADR, 16, 2},
			{AdrModeType::ZERO_PAGE_X_ADR, 20, 2},
			{AdrModeType::ABSOLUTE_Y_ADR, 24, 3},
			{AdrModeType::ABSOLUTE_X_ADR, 28, 3}
		};
		static inline AdressModeLUT LUT_Order8 = { 8, Order8 };

		// sta address modes
		static inline AdressModeItem Order7[7] = {
			{AdrModeType::INDEXED_INDIRECT_X_ADR, 0,2},
			{AdrModeType::ZERO_PAGE_ADR, 4,2},
			{AdrModeType::ABSOLUTE_ADR, 12,3},
			{AdrModeType::INDIRECT_INDEXED_Y_ADR, 16,2},
			{AdrModeType::ZERO_PAGE_X_ADR, 20,2},
			{AdrModeType::ABSOLUTE_Y_ADR, 24,3},
			{AdrModeType::ABSOLUTE_X_ADR, 28,3}
		};
		static inline AdressModeLUT LUT_Order7 = { 7, Order7 };
		//---------------------------------------
		// ASL, LSR, ROR, ROL
		//---------------------------------------
		static inline AdressModeItem Order5[5] = {
			{AdrModeType::ZERO_PAGE_ADR, 0,2},
			{AdrModeType::ACCUMULATOR,4,1},
			{AdrModeType::ABSOLUTE_ADR, 8,3},
			{AdrModeType::ZERO_PAGE_X_ADR, 16,2},
			{AdrModeType::ABSOLUTE_X_ADR, 24,3}
		};
		static inline AdressModeLUT LUT_Order5 = { 5, Order5 };
		//----------------------------------------------
		static inline AdressModeItem Order5_LDY[5] = {
			{AdrModeType::IMMEDIATE_ADR,0,2},
			{AdrModeType::ZERO_PAGE_ADR, 4,2},
			{AdrModeType::ABSOLUTE_ADR, 12,3},
			{AdrModeType::ZERO_PAGE_X_ADR, 20,2},
			{AdrModeType::ABSOLUTE_X_ADR, 28,3}
		};
		static inline AdressModeLUT LUT_Order5_LDY = { 5, Order5_LDY };
		//---------------------------------------------
		static inline AdressModeItem Order5_LDX[5] = {
			{AdrModeType::IMMEDIATE_ADR,0,2},
			{AdrModeType::ZERO_PAGE_ADR, 4,2},
			{AdrModeType::ABSOLUTE_ADR, 12,3},
			{AdrModeType::ZERO_PAGE_Y_ADR, 20,2},
			{AdrModeType::ABSOLUTE_Y_ADR, 28,3}
		};
		static inline AdressModeLUT LUT_Order5_LDX = { 5, Order5_LDX };
		//---------------------------------------------
		static inline AdressModeItem Order4[4] = {
			{AdrModeType::ZERO_PAGE_ADR, 0,2},
			{AdrModeType::ABSOLUTE_ADR, 8,3},
			{AdrModeType::ZERO_PAGE_X_ADR, 16,2},
			{AdrModeType::ABSOLUTE_X_ADR, 24,3}
		};
		static inline AdressModeLUT LUT_Order4 = { 4, Order4 };
		//---------------------------------------------
		static inline AdressModeItem Order3_STX[3] = {	//STX
			{AdrModeType::ZERO_PAGE_ADR, 0,2},
			{AdrModeType::ZERO_PAGE_Y_ADR,16,2},
			{AdrModeType::ABSOLUTE_ADR, 8,3},
		};
		static inline AdressModeLUT LUT_Order3_STX = { 3, Order3_STX };
		//---------------------------------------------
		static inline AdressModeItem Order3_STY[3] = {	//STY
			{AdrModeType::ZERO_PAGE_ADR, 0, 2},
			{AdrModeType::ZERO_PAGE_X_ADR, 16, 2},
			{AdrModeType::ABSOLUTE_ADR, 8, 3},
		};
		static inline AdressModeLUT LUT_Order3_STY = { 3, Order3_STY };
		//---------------------------------------------
		static inline AdressModeItem Order3_CPY_CPX[3] = {	//CPY/CPX
			{AdrModeType::IMMEDIATE_ADR,0,2},
			{AdrModeType::ZERO_PAGE_ADR, 4,2},
			{AdrModeType::ABSOLUTE_ADR, 12,3}
		};
		static inline AdressModeLUT LUT_Order3__CPY_CPX = { 3, Order3_CPY_CPX };
		//---------------------------------------------
		static inline AdressModeItem Order2[2] = {
			{AdrModeType::ABSOLUTE_ADR,0,3 },
			{AdrModeType::INDIRECT_ADR,32,3 }
		};
		static inline AdressModeLUT LUT_Order2 = { 2, Order2 };
		//---------------------------------------------
		static inline AdressModeItem Order2_BIT[2] = {
			{AdrModeType::ZERO_PAGE_ADR,0,2 },
			{AdrModeType::ABSOLUTE_ADR,8,3 }
		};
		static inline AdressModeLUT LUT_Order2_BIT = { 2, Order2_BIT };
		//---------------------------------------------
		static inline AdressModeItem Order1[1] = {
			{AdrModeType::ABSOLUTE_ADR,0 ,3}
		};
		static inline AdressModeLUT LUT_Order1 = { 1, Order1 };
		//-------------------------------------------------
		static inline AdressModeItem Order_BRANCH[1] = {
			{AdrModeType::RELATIVE, 0, 2}
		};
		static inline AdressModeLUT LUT_Relative = { 1, Order_BRANCH };
		//------------------------------------------------00
		static inline AdressModeItem Implied[1] = {
			{AdrModeType::IMPLIED, 0, 1}
		};
		static inline AdressModeLUT LUT_Implied = { 1, Implied };
		//------------------------------------------------00
		static inline KeyWord KeyWordsLUT[] = {
			{Token::ENDOFFILE,"End Of File",KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			{Token::IDENT," IDENT",KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},					//2
			{Token::NUMBER,"NUMBER",KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},				//3
			{Token::CUR_LOC, "Current Location",KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			{Token::MOD,"MOD",KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			{Token::DEFINED_IDENT, "DEFINED IDENT",KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			//--------- Assignment -------------
			{ Token::ASSIGN_ADD,"==+", KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			{ Token::ASSIGN_SUB,"==-", KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			{ Token::ASSIGN_MUL,"==*", KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			{ Token::ASSIGN_DIV,"==/", KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			{ Token::ASSIGN_MOD,"==MOD", KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			{ Token::ASSIGN_AND,"==&", KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			{ Token::ASSIGN_OR,"==%", KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			{ Token::ASSIGN_XOR,"==XOR", KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			{ Token::ASSIGN_RSH,"==RSH", KeyWord::KeyWordType::NONE,0,0, Processor::ALL, NULL, 0},
			{ Token::ASSIGN_LSH,"==LSH", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL, NULL, 0},
			//---------- Operators ------------
			{Token::LSH,"LSH", KeyWord::KeyWordType::OPERATOR, 0 ,0, Processor::ALL, NULL, 0},
			{Token::RSH,"RSH", KeyWord::KeyWordType::OPERATOR, 0 ,0, Processor::ALL, NULL, 0},
			{Token::OR,"OR", KeyWord::KeyWordType::OPERATOR, 0 ,0, Processor::ALL, NULL, 0},
			{Token::XOR, "XOR", KeyWord::KeyWordType::OPERATOR, 0 ,0, Processor::ALL, NULL, 0},
			{Token::MOD,"MOD", KeyWord::KeyWordType::OPERATOR, 0 ,0, Processor::ALL, NULL, 0},
			{Token::GTEQ,">=", KeyWord::KeyWordType::OPERATOR, 0 ,0, Processor::ALL, NULL, 0},
			{Token::LTEQ,"<=", KeyWord::KeyWordType::OPERATOR, 0 ,0, Processor::ALL, NULL, 0},
			//--------------- Functions --------------
			{Token::PROC,"PROC", KeyWord::KeyWordType::FUNCTION, 0 ,0, Processor::ALL, NULL, 0},	// Proceedure Declaration
			{Token::FUNC,"FUNC", KeyWord::KeyWordType::FUNCTION, 0 ,0, Processor::ALL, NULL, 0},	//  Function Declaration
			{Token::INTERRUPT,"INTERRUPT", KeyWord::KeyWordType::FUNCTION, 0 ,0, Processor::ALL, NULL, 0},
			{Token::PROC_IDENT,"PROC Idnet", KeyWord::KeyWordType::FUNCTION, 0 ,0, Processor::ALL, NULL, 0},	//  Proceedure Declaration
			{Token::FUNC_IDENT,"FUNC Ident", KeyWord::KeyWordType::FUNCTION, 0 ,0, Processor::ALL, NULL, 0},	//  Function Declaration
			{Token::INTERRUPT_IDENT,"INTERRUPT Indent", KeyWord::KeyWordType::FUNCTION, 0 ,0, Processor::ALL, NULL, 0},
			{Token::DECLARE,"DECLARE", KeyWord::KeyWordType::FUNCTION, 0 ,0, Processor::ALL, NULL, 0},
			//------ Data Types -----------------
			{Token::BOOL,"BOOL", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			{Token::BYTE,"BYTE", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			{Token::CHAR,"CHAR", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			{Token::INT,"INT", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			{Token::CARD,"CARD", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			{Token::TYPE, "TYPE", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			{Token::RECORDTYPE,"Record Type (TYPE)", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			{Token::POINTER,"POINTER", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			{Token::CONST,"CONST", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			{Token::ARRAY,"ARRAY", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			{Token::VAR_GLOBAL,"VAR_GLOBAL", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			{Token::VAR_LOCAL,"VAR_LOCAL", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			{Token::VAR_PARAM,"VAR_PARAM", KeyWord::KeyWordType::DATA_TYPE, 0 ,0, Processor::ALL, NULL, 0},
			//--------- Statements --------------
			{Token::MODULE,"MODULE", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::ASMMODULE,"ASMMODULE", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::VECTOR,"VECTOR", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::FOR,"FOR", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::TO,"TO", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::STEP,"STEP", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::IF,"IF", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::IFF,"IFF", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::THEN,"THEN", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::ELSEIF,"ELSEIF", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::ELSE,"ELSE", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::FI,"FI", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::FFI,"FFI", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::WHILE,"WHILE", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::DO,"DO", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::OD,"OD", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::UNTIL,"UNTIL", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::EXIT,"EXIT", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::RETURN,"RETURN", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::ASM,"ASM", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::PUSH,"PUSH", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::POP,"POP", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::BREAK, "BREAK", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::BITS,"BITS", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::BEGIN,"BEGIN", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			{Token::END, "END", KeyWord::KeyWordType::STATEMENT, 0 ,0, Processor::ALL, NULL, 0},
			//---------------------------------
			// Assembler Tokens
			//---------------------------------
			//----- Assembler Declarations
			{Token::ORG,"ORG", KeyWord::KeyWordType::ASSEMBLER_DIRECTIVE, 0,0, Processor::ALL,NULL,0},
			{Token::DB,"DB", KeyWord::KeyWordType::ASSEMBLER_DIRECTIVE, 0,0, Processor::ALL,NULL,0},
			{Token::DW,"DW", KeyWord::KeyWordType::ASSEMBLER_DIRECTIVE, 0,0, Processor::ALL,NULL,0},
			{Token::DL,"DL", KeyWord::KeyWordType::ASSEMBLER_DIRECTIVE, 0,0, Processor::ALL,NULL,0},
			{Token::DAS,"DAS", KeyWord::KeyWordType::ASSEMBLER_DIRECTIVE, 0,0, Processor::ALL,NULL,0},
			{Token::DCS,"DCS", KeyWord::KeyWordType::ASSEMBLER_DIRECTIVE, 0,0, Processor::ALL,NULL,0},
			{Token::DS,"DS", KeyWord::KeyWordType::ASSEMBLER_DIRECTIVE, 0,0, Processor::ALL,NULL,0},
			//----------- Opcodes --------------
			{Token::ADC,"ADC", KeyWord::KeyWordType::OPCODE, 3 ,0X61, Processor::R6502, &LUT_Order8,8},
			{Token::AND,"AND", KeyWord::KeyWordType::OPCODE, 3,0X21, Processor::R6502, &LUT_Order8,8},
			{Token::ASL,"ASL", KeyWord::KeyWordType::OPCODE, 3,0X06, Processor::R6502, &LUT_Order5,5},
			{Token::BCC,"BCC", KeyWord::KeyWordType::OPCODE, 2,0x90, Processor::R6502, &LUT_Relative,1},
			{Token::BCS,"BCS", KeyWord::KeyWordType::OPCODE, 2,0XB0, Processor::R6502, &LUT_Relative,1},
			{Token::BEQ,"BEQ", KeyWord::KeyWordType::OPCODE, 2,0XF0, Processor::R6502, &LUT_Relative,1},
			{Token::BMI,"BMI", KeyWord::KeyWordType::OPCODE, 2,0x30, Processor::R6502, &LUT_Relative,1},
			{Token::BNE,"BNE", KeyWord::KeyWordType::OPCODE, 2,0XD0, Processor::R6502, &LUT_Relative,1},
			{Token::BPL,"BPL", KeyWord::KeyWordType::OPCODE, 2,0x10, Processor::R6502, &LUT_Relative,1},
			{Token::BVC,"BVC", KeyWord::KeyWordType::OPCODE, 2,0x50, Processor::R6502, &LUT_Relative,1},
			{Token::BVS,"BVS", KeyWord::KeyWordType::OPCODE, 2,0x70, Processor::R6502, &LUT_Relative,1},
			{Token::BIT,"BIT", KeyWord::KeyWordType::OPCODE, 3,0X24, Processor::R6502, &LUT_Order2_BIT,2},
			{Token::BRK,"BRK", KeyWord::KeyWordType::OPCODE, 1,0x00, Processor::R6502, &LUT_Implied,1},
			{Token::CLC,"CLC", KeyWord::KeyWordType::OPCODE, 1,0X18, Processor::R6502, &LUT_Implied,1 },
			{Token::CLD,"CLD", KeyWord::KeyWordType::OPCODE, 1,0xd8, Processor::R6502, &LUT_Implied,1 },
			{Token::CLI,"CLI", KeyWord::KeyWordType::OPCODE, 1,0X58, Processor::R6502, &LUT_Implied,1 },
			{Token::CLV,"CLV", KeyWord::KeyWordType::OPCODE, 1,0xb8, Processor::R6502, &LUT_Implied,1 },
			{Token::CMP,"CMP", KeyWord::KeyWordType::OPCODE, 3,0xc1, Processor::R6502, &LUT_Order8,8},
			{Token::CPX,"CPX", KeyWord::KeyWordType::OPCODE, 3,0XE0, Processor::R6502, &LUT_Order3__CPY_CPX,3},
			{Token::CPY,"CPY", KeyWord::KeyWordType::OPCODE, 3,0XC0, Processor::R6502, &LUT_Order3__CPY_CPX,3},
			{Token::DEC,"DEC", KeyWord::KeyWordType::OPCODE,	 3,0xc6, Processor::R6502, &LUT_Order4,4},
			{Token::DEX,"DEX", KeyWord::KeyWordType::OPCODE, 1,0xca, Processor::R6502, &LUT_Implied,1 },
			{Token::DEY,"DEY", KeyWord::KeyWordType::OPCODE, 1,0x88, Processor::R6502, &LUT_Implied,1 },
			{Token::EOR,"EOR", KeyWord::KeyWordType::OPCODE, 3,0X41, Processor::R6502, &LUT_Order8,8},
			{Token::INC,"INC", KeyWord::KeyWordType::OPCODE, 3,0xe6, Processor::R6502, &LUT_Order4,4},
			{Token::INX,"INX", KeyWord::KeyWordType::OPCODE, 1,0xe8, Processor::R6502, &LUT_Implied,1 },
			{Token::INY,"INY", KeyWord::KeyWordType::OPCODE, 1,0xc8, Processor::R6502, &LUT_Implied,1 },
			{Token::JMP,"JMP", KeyWord::KeyWordType::OPCODE, 3,0X4C, Processor::R6502, &LUT_Order2,2},
			{Token::JSR,"JSR", KeyWord::KeyWordType::OPCODE, 3,0x20, Processor::R6502, &LUT_Order1,1},
			{Token::LDA, "LDA", KeyWord::KeyWordType::OPCODE, 3, 0xa1, Processor::R6502, &LUT_Order8, 8},
			{Token::LDX,"LDX", KeyWord::KeyWordType::OPCODE, 3,0xA2, Processor::R6502, &LUT_Order5_LDX,5 },
			{Token::LDY,"LDY", KeyWord::KeyWordType::OPCODE, 3,0xa0, Processor::R6502, &LUT_Order5_LDY,5 },
			{ Token::LSR,"LSR", KeyWord::KeyWordType::OPCODE, 3,0X46, Processor::R6502, &LUT_Order5,5 },
			{ Token::NOP,"NOP", KeyWord::KeyWordType::OPCODE, 1,0xea, Processor::R6502, &LUT_Implied,1 },
			{ Token::ORA,"ORA", KeyWord::KeyWordType::OPCODE, 3,0X01, Processor::R6502, &LUT_Order8,8 },
			{ Token::PHA,"PHA", KeyWord::KeyWordType::OPCODE, 1,0X48, Processor::R6502, &LUT_Implied,1 },
			{ Token::PLA,"PLA", KeyWord::KeyWordType::OPCODE, 1,0x68, Processor::R6502, &LUT_Implied,1 },
			{ Token::PHP,"PHP", KeyWord::KeyWordType::OPCODE, 1,0X08, Processor::R6502, &LUT_Implied,1 },
			{ Token::PLP,"PLP", KeyWord::KeyWordType::OPCODE, 1,0X28, Processor::R6502, &LUT_Implied,1 },
			{ Token::ROL,"ROL", KeyWord::KeyWordType::OPCODE, 3,0X26, Processor::R6502, &LUT_Order5,5 },
			{ Token::ROR,"ROR", KeyWord::KeyWordType::OPCODE, 3,0X66, Processor::R6502, &LUT_Order5,5 },
			{ Token::RTI,"RTI", KeyWord::KeyWordType::OPCODE, 1,0x40, Processor::R6502, &LUT_Implied,1 },
			{ Token::RTS,"RTS", KeyWord::KeyWordType::OPCODE, 1,0x60, Processor::R6502, &LUT_Implied,1 },
			{ Token::SBC,"SBC", KeyWord::KeyWordType::OPCODE, 3,0xe1, Processor::R6502, &LUT_Order8,8 },
			{ Token::STA,"STA", KeyWord::KeyWordType::OPCODE, 3,0x81, Processor::R6502, &LUT_Order7,7 },
			{ Token::SEC,"SEC", KeyWord::KeyWordType::OPCODE, 1,0x38, Processor::R6502, &LUT_Implied,1 },
			{ Token::SED,"SED", KeyWord::KeyWordType::OPCODE, 1,0Xf8, Processor::R6502, &LUT_Implied,1 },
			{ Token::SEI,"SEI", KeyWord::KeyWordType::OPCODE, 1,0x78, Processor::R6502, &LUT_Implied,1 },
			{ Token::STX,"STX", KeyWord::KeyWordType::OPCODE, 3,0x86, Processor::R6502, &LUT_Order3_STX,3 },
			{ Token::STY,"STY", KeyWord::KeyWordType::OPCODE, 3,0x84, Processor::R6502, &LUT_Order3_STY,3 },
			{ Token::TAX,"TAX", KeyWord::KeyWordType::OPCODE, 1,0xaa, Processor::R6502, &LUT_Implied,1 },
			{ Token::TAY,"TAY", KeyWord::KeyWordType::OPCODE, 1,0xa8, Processor::R6502,&LUT_Implied,1 },
			{ Token::TXA,"TXA", KeyWord::KeyWordType::OPCODE, 1,0x8a, Processor::R6502, &LUT_Implied,1 },
			{ Token::TYA,"TYA", KeyWord::KeyWordType::OPCODE, 1,0x98, Processor::R6502, &LUT_Implied,1 },
			{ Token::TXS,"TXS", KeyWord::KeyWordType::OPCODE, 1,0x9a, Processor::R6502, &LUT_Implied,1 },
			{ Token::TSX,"TSX", KeyWord::KeyWordType::OPCODE, 1,0xba, Processor::R6502, &LUT_Implied,1 },
			//--------------------------------------------
			// W65C02 Opcodes
			//--------------------------------------------
			{ Token::BRA,"BRA", KeyWord::KeyWordType::OPCODE_65C02, 2,0xba, Processor::W65C02, NULL,0 },	//  Branch Always
			{ Token::PHX,"PHX", KeyWord::KeyWordType::OPCODE_65C02, 1,0xba, Processor::W65C02, NULL,0 },	// push index reg X
			{ Token::PHY,"PHY", KeyWord::KeyWordType::OPCODE_65C02, 1,0xba, Processor::W65C02, NULL,0 },	// push Y register
			{ Token::PLX,"PLX", KeyWord::KeyWordType::OPCODE_65C02, 1,0xba, Processor::W65C02, NULL,0 },	// pop stack to X
			{ Token::PLY,"PLY", KeyWord::KeyWordType::OPCODE_65C02, 1,0xba, Processor::W65C02, NULL,0 },	// pop stack to Y
			{ Token::STZ,"STZ", KeyWord::KeyWordType::OPCODE_65C02, 3,0xba, Processor::W65C02, NULL,0 },	// Store Zero to Memory
			{ Token::TRB,"TRB", KeyWord::KeyWordType::OPCODE_65C02, 3,0xba, Processor::W65C02, NULL,0 },	// Test and reset bits
			{ Token::TSB,"TSB", KeyWord::KeyWordType::OPCODE_65C02, 3,0xba, Processor::W65C02, NULL,0 },	// Test and Set bits
			//--------------------------------------------
			// Lower Case Opcodes
			//--------------------------------------------
			{ Token::ADC,"adc", KeyWord::KeyWordType::OPCODE, 3 ,0X61, Processor::R6502, &LUT_Order8,8 },
			{ Token::AND,"and", KeyWord::KeyWordType::OPCODE, 3,0X21, Processor::R6502, &LUT_Order8,8 },
			{ Token::ASL,"asl", KeyWord::KeyWordType::OPCODE, 3,0X06, Processor::R6502, &LUT_Order5,5 },
			{ Token::BCC,"bcc", KeyWord::KeyWordType::OPCODE, 2,0x90, Processor::R6502, &LUT_Relative,1 },
			{ Token::BCS,"bcs", KeyWord::KeyWordType::OPCODE, 2,0XB0, Processor::R6502, &LUT_Relative,1 },
			{ Token::BEQ,"beq", KeyWord::KeyWordType::OPCODE, 2,0XF0, Processor::R6502, &LUT_Relative,1 },
			{ Token::BMI,"bmi", KeyWord::KeyWordType::OPCODE, 2,0x30, Processor::R6502, &LUT_Relative,1 },
			{ Token::BNE,"bne", KeyWord::KeyWordType::OPCODE, 2,0XD0, Processor::R6502, &LUT_Relative,1 },
			{ Token::BPL,"bpl", KeyWord::KeyWordType::OPCODE, 2,0x10, Processor::R6502, &LUT_Relative,1 },
			{ Token::BVC,"bvc", KeyWord::KeyWordType::OPCODE, 2,0x50, Processor::R6502, &LUT_Relative,1 },
			{ Token::BVS,"bvs", KeyWord::KeyWordType::OPCODE, 2,0x70, Processor::R6502, &LUT_Relative,1 },
			{ Token::BIT,"bit", KeyWord::KeyWordType::OPCODE, 3,0X24, Processor::R6502, &LUT_Order2_BIT,2 },
			{ Token::BRK,"brk", KeyWord::KeyWordType::OPCODE, 1,0x00, Processor::R6502, &LUT_Implied,1 },
			{ Token::CLC,"clc", KeyWord::KeyWordType::OPCODE, 1,0X18, Processor::R6502, &LUT_Implied,1 },
			{ Token::CLD,"cld", KeyWord::KeyWordType::OPCODE, 1,0xd8, Processor::R6502,&LUT_Implied,1 },
			{ Token::CLI,"cli", KeyWord::KeyWordType::OPCODE, 1,0X58, Processor::R6502,&LUT_Implied,1 },
			{ Token::CLV,"clv", KeyWord::KeyWordType::OPCODE, 1,0xb8, Processor::R6502, &LUT_Implied,1 },
			{ Token::CMP,"cmp", KeyWord::KeyWordType::OPCODE, 3,0xc1, Processor::R6502, &LUT_Order8,8 },
			{ Token::CPX,"cpx", KeyWord::KeyWordType::OPCODE, 3,0XE0, Processor::R6502, &LUT_Order3__CPY_CPX,3 },
			{ Token::CPY,"cpy", KeyWord::KeyWordType::OPCODE, 3,0XC0, Processor::R6502, &LUT_Order3__CPY_CPX,3 },
			{ Token::DEC,"dec", KeyWord::KeyWordType::OPCODE, 3,0xc6, Processor::R6502, &LUT_Order4,4 },
			{ Token::DEX,"dex", KeyWord::KeyWordType::OPCODE, 1,0xca, Processor::R6502, &LUT_Implied,1 },
			{ Token::DEY,"dey", KeyWord::KeyWordType::OPCODE, 1,0x88, Processor::R6502, &LUT_Implied,1 },
			{ Token::EOR,"eor", KeyWord::KeyWordType::OPCODE, 3,0X41, Processor::R6502, &LUT_Order8,8 },
			{ Token::INC,"inc", KeyWord::KeyWordType::OPCODE, 3,0xe6, Processor::R6502, &LUT_Order4,4 },
			{ Token::INX,"inx", KeyWord::KeyWordType::OPCODE, 1,0xe8, Processor::R6502, &LUT_Implied,1 },
			{ Token::INY,"iny", KeyWord::KeyWordType::OPCODE, 1,0xc8, Processor::R6502, &LUT_Implied,1 },
			{ Token::JMP,"jmp", KeyWord::KeyWordType::OPCODE, 3,0X4C, Processor::R6502, &LUT_Order2,2 },
			{ Token::JSR,"jsr", KeyWord::KeyWordType::OPCODE, 3,0x20, Processor::R6502, &LUT_Order1,1 },
			{ Token::LDA,"lda", KeyWord::KeyWordType::OPCODE, 3,0xa1, Processor::R6502, &LUT_Order8,8 },
			{ Token::LDX,"ldx", KeyWord::KeyWordType::OPCODE, 3,0xA2, Processor::R6502, &LUT_Order5_LDX,5 },
			{ Token::LDY,"ldy", KeyWord::KeyWordType::OPCODE, 3,0xa0, Processor::R6502, &LUT_Order5_LDY,5 },
			{ Token::LSR,"lsr", KeyWord::KeyWordType::OPCODE, 3,0X46, Processor::R6502, &LUT_Order5,5 },
			{ Token::NOP,"nop", KeyWord::KeyWordType::OPCODE, 1,0xea, Processor::R6502, &LUT_Implied,1 },
			{ Token::ORA,"ora", KeyWord::KeyWordType::OPCODE, 3,0X01, Processor::R6502, &LUT_Order8,8 },
			{ Token::PHA,"pha", KeyWord::KeyWordType::OPCODE, 1,0X48, Processor::R6502, &LUT_Implied,1 },
			{ Token::PLA,"pla", KeyWord::KeyWordType::OPCODE, 1,0x68, Processor::R6502, &LUT_Implied,1 },
			{ Token::PHP,"php", KeyWord::KeyWordType::OPCODE, 1,0X08, Processor::R6502, &LUT_Implied,1 },
			{ Token::PLP,"plp", KeyWord::KeyWordType::OPCODE, 1,0X28, Processor::R6502, &LUT_Implied,1 },
			{ Token::ROL,"rol", KeyWord::KeyWordType::OPCODE, 3,0X26, Processor::R6502, &LUT_Order5,5 },
			{ Token::ROR,"ror", KeyWord::KeyWordType::OPCODE, 3,0X66, Processor::R6502, &LUT_Order5,5 },
			{ Token::RTI,"rti", KeyWord::KeyWordType::OPCODE, 1,0x40, Processor::R6502, &LUT_Implied,1 },
			{ Token::RTS,"rts", KeyWord::KeyWordType::OPCODE, 1,0x60, Processor::R6502, &LUT_Implied,1 },
			{ Token::SBC,"sbc", KeyWord::KeyWordType::OPCODE, 3,0xe1, Processor::R6502, &LUT_Order8,8 },
			{ Token::STA,"sta", KeyWord::KeyWordType::OPCODE, 3,0x81, Processor::R6502, &LUT_Order7,7 },
			{ Token::SEC,"sec", KeyWord::KeyWordType::OPCODE, 1,0x38, Processor::R6502, &LUT_Implied,1 },
			{ Token::SED,"sed", KeyWord::KeyWordType::OPCODE, 1,0Xf8, Processor::R6502, &LUT_Implied,1 },
			{ Token::SEI,"sei", KeyWord::KeyWordType::OPCODE, 1,0x78, Processor::R6502, &LUT_Implied,1 },
			{ Token::STX,"stx", KeyWord::KeyWordType::OPCODE, 3,0x86, Processor::R6502, &LUT_Order3_STX,3 },
			{ Token::STY,"sty", KeyWord::KeyWordType::OPCODE, 3,0x84, Processor::R6502, &LUT_Order3_STY,3 },
			{ Token::TAX,"tax", KeyWord::KeyWordType::OPCODE, 1,0xaa, Processor::R6502, &LUT_Implied,1 },
			{ Token::TAY,"tay", KeyWord::KeyWordType::OPCODE, 1,0xa8, Processor::R6502, &LUT_Implied,1 },
			{ Token::TXA,"txa", KeyWord::KeyWordType::OPCODE, 1,0x8a, Processor::R6502, &LUT_Implied,1 },
			{ Token::TYA,"tya", KeyWord::KeyWordType::OPCODE, 1,0x98, Processor::R6502, &LUT_Implied,1 },
			{ Token::TXS,"txs", KeyWord::KeyWordType::OPCODE, 1,0x9a, Processor::R6502, &LUT_Implied,1 },
			{ Token::TSX,"tsx", KeyWord::KeyWordType::OPCODE, 1,0xba, Processor::R6502, &LUT_Implied,1 },
			//--------------------------------------------
			// W65C02 Opcodes
			//--------------------------------------------
			{ Token::BRA,"bra", KeyWord::KeyWordType::OPCODE_65C02, 2,0xba, Processor::W65C02, &LUT_Relative,1 },	//  Branch Always
			{ Token::PHX,"phx", KeyWord::KeyWordType::OPCODE_65C02, 1,0xba, Processor::W65C02, &LUT_Implied,1 },	// push index reg X
			{ Token::PHY,"phy", KeyWord::KeyWordType::OPCODE_65C02, 1,0xba, Processor::W65C02, &LUT_Implied,1 },	// push Y register
			{ Token::PLX,"plx", KeyWord::KeyWordType::OPCODE_65C02, 1,0xba, Processor::W65C02, &LUT_Implied,1 },	// pop stack to X
			{ Token::PLY,"ply", KeyWord::KeyWordType::OPCODE_65C02, 1,0xba, Processor::W65C02, &LUT_Implied,1 },	// pop stack to Y
			{ Token::STZ,"stz", KeyWord::KeyWordType::OPCODE_65C02, 3,0xba, Processor::W65C02, NULL,0 },	// Store Zero to Memory
			{ Token::TRB,"trb", KeyWord::KeyWordType::OPCODE_65C02, 3,0xba, Processor::W65C02, NULL,0 },	// Test and reset bits
			{ Token::TSB,"tsb", KeyWord::KeyWordType::OPCODE_65C02, 3,0xba, Processor::W65C02, NULL,0 },	// Test and Set bits
			{ Token::ENDOFOPCODES,"EOO", KeyWord::KeyWordType::NONE, 0, 0, Processor::ALL,NULL,0 },
			//------------------ Registers -------------------
			{ Token::AREG,".A", KeyWord::KeyWordType::REGISTER,  0,0, Processor::R6502, NULL,0 },	// Accumulator
			{ Token::XREG,".X", KeyWord::KeyWordType::REGISTER, 0,0, Processor::R6502, NULL,0 },	// X Index Register
			{ Token::YREG,".Y", KeyWord::KeyWordType::REGISTER, 0,0, Processor::R6502, NULL,0 },	// Y Index Register
			{ Token::SPREG,".S", KeyWord::KeyWordType::REGISTER, 0,0, Processor::R6502, NULL,0 },	// Stack Pointer
			{ Token::PSREG,".P", KeyWord::KeyWordType::REGISTER, 0,0, Processor::R6502, NULL,0 },	// Processor Status Register
			{ Token::LOCAL_LABEL,"Local Lable", KeyWord::KeyWordType::NONE, 0,0,Processor::ALL,NULL,0 },
			{ Token::GLOBAL_LABEL,"Global Lable", KeyWord::KeyWordType::NONE, 0,0,Processor::ALL,NULL,0 },
			{ Token::LABEL,"LABEL", KeyWord::KeyWordType::NONE, 0,0,Processor::ALL,NULL,0 },
			//----------------------------------
			// Misc
			//----------------------------------
			{ Token::EPROC,"EPROC", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{Token::CHAR_CONSTANT,"Character Constant", KeyWord::KeyWordType::NONE, 0,0, Processor::ALL,NULL,0 },
			{ Token('('),"(", KeyWord::KeyWordType::NONE, 0,0, Processor::ALL,NULL,0 },
			{ Token(')'),")", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('{'),"{", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('}'),"}", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('['),"[", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token(']'),"]", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('<'),"<", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('>'),">", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('!'),"!", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('@'),"@", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('#'),"#", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('$'),"$", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('%'),"%", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('^'),"^", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('&'),"&", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('*'),"*", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('-'),"-", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('='),"=", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('+'),"+", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token(','),",", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('.'),".", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('/'),"/", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('?'),"?", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			{ Token('~'),"~", KeyWord::KeyWordType::NONE, 0 ,0, Processor::ALL,NULL,0 },
			//----------- Compiler Directives --------------
			{ Token::INCLUDE,"INCLUDE", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::SET,"SET", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::DEFINE, "DEFINE", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::SECTION, "SECTION", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::SECTION_NAME, "SECTION NAME", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL, NULL,0},
			{ Token::START, "START", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::SIZE,"SIZE", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::NAME, "NAME", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::MODE, "MODE", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::READ_WRTE,"READ WRTE", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::READ_ONLY, "READ ONLY", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::PAGEZERO, "PAGEZERO", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::TRUE,"TRUE", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::FALSE,"FALSE", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::PROCESSOR,"PROCESSOR", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::W65C02,"W65C02", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::R6502,"R6502", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::W65C816,"W65C816", KeyWord::KeyWordType::COMPILER_DIRECTIVE, 0,0, Processor::ALL,NULL,0 },
			{ Token::ENDOFTOKENS,(const char*)NULL, KeyWord::KeyWordType::NONE, 0,0, Processor::ALL,NULL,0 }
		};
		class LexSubParser
		{
			friend class CLexer;
			//-------------------------------
			// SubParse class Definition
			//-------------------------------
			class SubParse
			{
				friend class LexSubParser;
				friend class SubLex;
				friend class CLexer;
			public:
				//-------------------------------
				// SubLex class Definition
				//-------------------------------
				class SubLex {
					friend class SubParse;
					friend class LexSubParser;
					friend class CLexer;
					LexSubParser* m_pGrandParent;
					SubParse* m_pParent;
					CLexer* m_pLexer;
				public:
					SubLex();
					~SubLex();
					bool Create(LexSubParser* pLSP, CLexer::LexSubParser::SubParse* pSP);
					Token Lex();
					LexSubParser* GetGrandParent() { return m_pGrandParent; }
					SubParse* GetParent() { return m_pParent; }
					int GetLineNumber() const { return m_pGrandParent->m_pParent->GetLineNumber(); }
					int GetColumn() const { return m_pLexer->GetColumn(); }
					int LexGet() { return m_pLexer->LexGet(); }
					int LexLook(int index) { return m_pLexer->LexLook(index); }
					void LexUnGet(int c) { m_pLexer->LexUnGet(c); }
					bool IsValidNumber(int c) { return m_pLexer->IsValidNumber(c); }
					bool IsValidHexNumber(int c) { return m_pLexer->IsValidHexNumber(c); }
				} m_SubLexer;
				//--------------------------------
				// End of SubLex class
				//--------------------------------
				// SibParse class data members
				//-------------------------------
			private:
				LexSubParser* m_pLexSubParserParent;
				//-------------------------------
				// SubParse class methods
				//-------------------------------
			public:
				SubParse();
				~SubParse();
				bool Create(LexSubParser* pLSP);
				bool Accept(Token ExpectedToken);
				void Expect(Token ExpectedToken);
				bool Parse(Token TokenVal);
				Token LookAheadToken() { return m_pLexSubParserParent->LookAheadToken(); }
				void SetLookAheadToken(Token t) {
					m_pLexSubParserParent->SetLookAheadToken(t);
				}
			private:
				//-------------------------------
				// Grammar Rule Methods
				//-------------------------------
				void Set();
				void SetObjects();
				void ProcessorType();
				void Define();
				void DefObjects();
				void DefList();
				void Def();
				void SectionName();
				void SectionAttributes();
				void Modes();
				void AddressSize();
			public:
				SubLex* GetSubLexer() { return &m_SubLexer; }
				LexSubParser* GetParent() { return m_pLexSubParserParent; }
			}m_SubParser;
			//-------------------------------
			// End of SubParse class
			//-------------------------------
			// LexSubParser class methods
			//-------------------------------
			CLexer* m_pParent;
		public:
			LexSubParser();
			~LexSubParser();
			bool Create(CLexer* pParent);
			bool Parse(Token TokenVal)
			{
				return m_SubParser.Parse(TokenVal);
			}
			SubParse::SubLex* GetSubLexer() { return m_SubParser.GetSubLexer(); }
			SubParse* GetSubParser() { return &m_SubParser; }
			Token LookAheadToken() {
				return m_pParent->LookAheadToken();
			}
			void SetLookAheadToken(Token t) {
				m_pParent->SetLookAheadToken(t);
			}
		} m_LexSubParser;

		//--------------------------------
		// Begining of the CLexer class
		//--------------------------------
		// Clexer data members
		//-------------------------------
		// The one and only symbol table
		//-------------------------------
		CSymTab m_SymbolTable;
		//-------------------------------
		// Source File Buffer
		//-------------------------------
		char* m_pFileBuffeer;	// Buffer the source file is stored in
		int m_InFileSize;	// Size of the buffer
		int m_FileIndex;	// Current index into file buffer
		//-------------------------------
		// Lexing Buffer
		//-------------------------------
		int m_UngetBuffer;
		char m_aLexBuff[256];	// Buffer for the current token string
		int m_LexBuffIndex;		// index into ^
		//-------------------------------
		//  Source file statistics
		//-------------------------------
		int m_Line;
		int m_Col;
		int m_LexValue;
		CSymbol* m_pLexSymbol;
		bool m_bAsmMode;
		//-------------------------------
		char* m_pCurrentLine;
		char* m_pCurrentComment;
		CParser* m_pCompilerParser;
	public:
		CLexer();
		virtual ~CLexer();
		bool Create(CParser* pParse);
		//	FILE* LogFile() {
		//		return Act()->LogFile();
		//	}
		bool IsAsmMode()const { return m_bAsmMode; }
		void SetActionMode() { m_bAsmMode = false; }
		void SetAsmMode() { m_bAsmMode = true; }
		int LexGet();
		int LexLook(int index);
		void LexUnGet(int c);
		bool IsValidHexNumber(int c);
		bool IsValidNumber(int c);
		bool IsValidNameFirstChar(int c);
		bool IsValidNameChar(int c);
		bool IsWhiteSpace(int c);
		bool IsValidAssignmentOperator(int c);
		Token Lex();
		//------------------------------------------
		// Symbol Table Methods
		//------------------------------------------
		CBin* LookupSymbol(const char* pName);
		CSymTab* GetSymTab() { return &m_SymbolTable; }
		//------------------------------------------
		// Keyword Lookup Methods
		//------------------------------------------
		Token LookupKeyword(const char* pKeyword);
		KeyWord* FindKeyword(Token KeywordToken);
		Processor LookupProcessor(Token KeywordToken);
		int LookupOpcode(Token OpcodeToken);
		int MakeOpcode(Token OpCodeToken, AdrModeType AddressMode);
		int GetOpcode(Token OpCodeToken);
		static const char* LookupTokenToString(Token Toke)
		{
			return KeyWord::LookupToString(Toke);
		}
		static Token LookupToToken(const char* pName)
		{
			return KeyWord::LookupToToken(pName);
		}
		static KeyWord* GetKeyWords() { return KeyWordsLUT; }
		//--------------------------------------
		// Symbol Methods
		//--------------------------------------
		CSymbol* GetLexSymbol() { return m_pLexSymbol; }
		CSection* GetLexSection() { return (CSection*)m_pLexSymbol; }
		int GetLexValue() { return m_LexValue; }
		char* GetLexBuffer() {
			return m_aLexBuff;
		}
		int GetLineNumber() const { 
			return m_Line; 
		}
		int GetColumn() const { return m_Col; }
		int IncFileIndex(int inc = 1) { return m_FileIndex += inc; }
		Token LookAheadToken();
		void SetLookAheadToken(Token t);
		CParser* GetMainParser() { return m_pCompilerParser; }
		CSymbol* CheckForLocalVariable(const char* pName);
		CSymbol* CheckForParameterVariable(const char* pName);
	}m_MainLexer;;

//------------------------------
// Parser Stuff
//------------------------------
public:
	enum class PHASE {
		NONE,
		GENERATE_AST,
		OPTIMAZE_AST,
		CODE_GENERATION,
		PROGRAM_LISTING,
	};
private:
	struct PASS {
		PHASE m_Phase;
		const char* m_pName;
		PASS() {
			m_Phase = PHASE::NONE;
			m_pName = 0;
		}
		PASS(PHASE ph, const char* pN) {
			m_Phase = ph;
			m_pName = pN;
		}
		const char* NextPass();
	};
	static inline PASS ParsePhase[PHASE_LUT_DIM] = {
		{PHASE::NONE,"NONE"},
		{PHASE::GENERATE_AST,"Pass 1:AST Generation"},
		{PHASE::OPTIMAZE_AST,"Pass 2:AST Optimization"},
		{PHASE::CODE_GENERATION,"Pass 3:CODE Generation"},
		{PHASE::PROGRAM_LISTING,"Pass 4:Program Listing Ouput File"},
	};
	PASS m_Pass;			// Phase of the compiler operation
	Processor m_Processor;
	int m_Recursion;
	int m_Bump;
	CActionAstTree m_AstTree;
	//------------------------------
	// Default Memory Sections
	//------------------------------
	CSection* m_pHead;
	CSection* m_pTail;
	//------------------------------
	CSection* m_pCurrentSection;
	CLinker* m_pLinkerScript;
	CSymbol* m_pCurrentProceedure;
public:
	CParser();
	virtual ~CParser();
	bool Create();
	//----------------- Symbol Generator -------------------
	CSymbol* GenerateSymbol(const char* pPrefix);
	//------------------------------------------------------
	Token LookAheadToken() const {
		return m_LookAheadToken;
	}
	void SetLookAheadToken(Token t) {
		m_LookAheadToken = t;
	}
	void SetCurrentProc(CSymbol* pProcSym) { 
		m_pCurrentProceedure = pProcSym; 
	}
	CSymbol* GetCurrentProc() {
		return m_pCurrentProceedure;
	}
	void SetCurrentSection(CSection* pSection);
	CSection* GetCurrentSection() {
		return m_pCurrentSection;
	}
	CLexer* GetLexer() { return &m_MainLexer; }
	CActionAstTree* GetAstTree() { return &m_AstTree; }
	FILE* LogFile();
	CAstNode* Run();
	//----------------------------------
	// Manage Memory Sections
	//---------------------------------
	CSection* GetSectionHead() { return m_pHead; }
	void SetSectionHead(CSection* pH) { m_pHead = pH; }
	CSection* GetSectionTail() { return m_pTail;}
	void SetSectionTail(CSection* pT) { m_pTail = pT; }
	CSection* FindSection(const char* pSectionName);
	void AddSection(CSection* pSection);
	void PrintSections();
	//---------------------------------
	void Expect(Token Expected);
	bool Accept(Token Expected);
private:
	//---------------------------------
	// Parsing Methods
	//---------------------------------
	// Program Structure
	//---------------------------------
	CAstNode*  Action65();
	CAstNode*  Modules();
	//------------------------------------------
	// Statements
	//------------------------------------------
	CAstNode*  Statements();
	//------------------------------------------
	// Statements
	//------------------------------------------
	CAstNode*  Call();	//ProcCal
	CAstNode*  ProcParams();
	CAstNode*  ProcParamsEnd();
	//----FOR
	CAstNode*  ForStmt();
	CAstNode* ForDOendOD();
	CAstNode* STEPoption();
	CAstNode* ForTO();
	CAstNode* Itterrator();
	//--- IF
	CAstNode*  IfStmt();
	CAstNode* EndIF();
	CAstNode* ElsePart();
	CAstNode* ElseIfPart();
	CAstNode* ThenPart();
	//----WHILE
	CAstNode*  WhileStmt();
	CAstNode*  WhileDO();
	//----DO
	CAstNode*  DoStmt();
	CAstNode*  DoEND();
	//----EXIT
	CAstNode*  EXITstmt();
	//----RETURN
	CAstNode*  RetStmt();
	//----ASM
	CAstNode*  InlineAssembly();
	CAstNode*  InlineAssBlock();
	//----CODE BLOCK
	CAstNode*  CodeBlock();
	//---- UNTIL
	CAstNode*  UntillStmt();
	//---- BREAK
	CAstNode*  Break();
	//---- RTI
	CAstNode*  Rti();
	//---- Assigmemt
	CAstNode*  Assignment();
	//--------------------------------------
	// Logical Expressions
	//--------------------------------------
	CAstNode*  RelOperation();
	CAstNode*  LogicalOR();
	CAstNode*  LogicalAND();
	//--------------------------------------
	// Arithmetic Expressions
	//--------------------------------------
	CAstNode*  ArithExpr();	//bitwise OR
	CAstNode*  BitwiseAND();
	CAstNode*  BitwiseXOR();
	CAstNode*  AddExpr();
	CAstNode*  ShifExpr();
	CAstNode*  MultExpr();
	CAstNode*  Unary();
	//----------------------------------
	//Variable References
	//Memory References
	//----------------------------------
	CAstNode*  ValueList();
	CAstNode*  Value();
	CAstNode*  AddressOf();
	CAstNode*  MemContentsType();
	CAstNode*  MemContents();
	CAstNode*  ArrayIndex();
	CAstNode*  Factor();
	CAstNode* MemoryValue();
	CAstNode* MemValLocation();
	//-------------------------------------------
	// Declarations
	//-------------------------------------------
	// VECTOR
	//-------------------------------------------
	CAstNode*  SysDecl();	//Vectpr
	CAstNode*  VectorEnd();
	CAstNode*  AddressEnd();
	CAstNode*  VectorAddress();
	//--------------------------------------
	// TYPEdef Definition
	//--------------------------------------
	CAstNode*  TypeDefDecl();
	CAstNode*  TypeDef();
	//----------------------------------------
	// FUNCtion/PROCedure/INTERRUPT
	// Declarations/Function Prototypes
	CAstNode*  Declare();
	void DECLAREnd();
	void DECLAREParamList();
	void DECLAREParamTypeSpec(CChainTypeSpec* pTypeChain);
	void DECLAREParamIdentList(CChainTypeSpec* pTypeChain);
	void DECLAREParamIdent(CChainTypeSpec* pTypeChain);
	//-------------------
	void DECLAREFuncType(void);
	void DECLAREFuncTypeSpec(CChainTypeSpec* pTypeChain);
	void  DECLAREfunction(CChainTypeSpec* pTypeChain);
	void DECLAREFuncName(CChainTypeSpec* pTypeChain);
	// ---------------------------------------
	// Fundamental Declarations
	//----------------------------------------
	CAstNode* FundamentalDecl();
	CAstNode*  FundTypeSpec(CChainTypeSpec* pTypeChain);
	//----------------------------------
	// Identifiers
	//----------------------------------
	CAstNode*  IdentList(CChainTypeSpec* pTypeChain);
	void MakeIdent(CChainTypeSpec* pTypeChain);
	CAstNode*  Ident( CChainTypeSpec* pTypeChain);
	CAstNode*  IdentInitType();
	CAstNode*  InitData();
	//------------------
	CAstNode*  IrqDecl(CChainTypeSpec* pTypeChain);
	CAstNode*  IrqDeclParams();
	CAstNode*  IrqBody();
	//------------------
	CAstNode*  ProcDecl(CChainTypeSpec* pTypeChain);
	CAstNode*  ProcDeclParams(CSymbol* pFuncSym);
	CAstNode*  ProcBody();
	//------------------
	CAstNode*  FuncDecl(CChainTypeSpec* pTypeChain);
	CAstNode*  FuncDeclParams(CSymbol* pFuncSym);
	CAstNode*  FuncBody();
	//------------------
	CAstNode*  OptInit();
	//-------------------------------------------
	// Parameter Declarations
	//-------------------------------------------
	CAstNode*  ParamList(CSymbol* pFuncSym);
	CAstNode*  ParamTypeSpec(CChainTypeSpec* pTypeChain, CSymbol* pFuncSym);
	CAstNode*  DefineParamIdentList(CChainTypeSpec* pTypeChain, CSymbol* pFuncSym);
	CAstNode*  DefineParamIdent(CChainTypeSpec* pTypeChain, CSymbol* pFuncSym);
	//-----------------------------------------------
	// TYPE field members
	//-----------------------------------------------
	CAstNode* TypeField(CSymbol* pTypeSym);
	//-----------------------------------------------
	// Local Variableas
	//-----------------------------------------------
	CAstNode*  LocalDecls();
	CAstNode*  LocalVarDecls();
	CAstNode*  LocalTypeSpec(CChainTypeSpec* pTypeChain);
	//-------------------------------
	// Compiler Constants
	//-------------------------------
	CAstNode*  ConstListEnd();
	CAstNode*  CompConst();
	CAstNode*  BaseCompConst();
	//****************************************
	//----------------------------------------
	//  Inline assembly code
	//----------------------------------------
	//	Statements
	//----------------------------------------
	//****************************************
	//-----------------------------------------
	// Code
	//		These statements are what actually
	// do something that concerns the machine
	// code that is executed
	//-----------------------------------------
	//----------------------------
	// Code Statements
	//----------------------------
	//	CAstNode* AsmStatements();
	//----IFF-------------
	// AsmStmt is the start of
	// IFFend
	//------------------------------
	CAstNode* IFFend();
	CAstNode* IFFthenpart();;
	CAstNode* IffRelOper();;
	CAstNode* IffRegister();;
	CAstNode* Bits();;
	CAstNode* BitValue();;
	CAstNode* StatusFlags();;
	CAstNode* OptNot();;
	//---- POP
	CAstNode* PopDestList();
	CAstNode* PopDest();
	//---- PUSH
	CAstNode* PushSourceList();
	CAstNode* PushSource();
	//-------------------------------------
	// Assembly Proceedure
	//-------------------------------------
	CAstNode*  AsmProcEnd();
	CAstNode*  AsmProcBody();
	CAstNode*  AsmProcName();
	//--------------------------------------
	// AsmStatements
	//--------------------------------------
	CAstNode* AsmStatements();
	CAstNode* OptLabel();
	CAstNode* Operand(Token OpCodeToken, CAstNode* pLabel);
	//------------------------------------------
	// Branch Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  RelAddressingMode(Token  OpCodeToken, CAstNode* pLabel);
	//------------------------------------------
	// JMP Instructions Addressing Mode
	//------------------------------------------
	CAstNode*  JumpAddressingModes(Token OpCodeToken, CAstNode* pLabel);
	//---------------------------------------------
	//	Optional Index Registers
	//---------------------------------------------
	CReg::RegType  OptIndexReg();
	//---------------------------------------
	// Assembler Constants
	//---------------------------------------
	CAstNode* AsmConstList();
	CAstNode* AsmConstList_1();
	CValue* AsmConstant();
	CValue* AsmConstUpLow();
	CValue* BaseAsmConstant();
	//------------------------------------------------
	CAstNode*  Indirect(Token  OpCodeToken, CAstNode* pLabel);
	CAstNode*  Immediate(Token OpCodeToken, CAstNode* pLabel);
	CAstNode*  Absolute(Token  OpCodeToken, CAstNode* pLabel);
	CAstNode* Accumulator(Token OpCodeToken, CAstNode* pLabel);
	//---------------- Debug Utillity ----------------------
public:
	void NextPass();
	void DebugTraverse(
		CAstNode* pN,		// head of tree
		const char* pTitle, // title of dump
		int MaxRecursions,	// Maximum number of recursions (safety limit)
		int MaxLoops,		// ,axo,i, number of loops (safety limit)
		int Misc = 0,
		const char* pSmiscLabel = 0
	);
	//---------------------------------
	void ProgramListing();
};

