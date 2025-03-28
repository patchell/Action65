#pragma once

class CLexer
{

private:
	//alu access modes
	static inline AdressModeItem Order8[8] = {
		{AdrModeType::INDEXED_INDIRECT_X_ADR, 0},
		{AdrModeType::ZERO_PAGE_ADR, 4},
		{AdrModeType::IMMEDIATE_ADR, 8},
		{AdrModeType::ABSOLUTE_ADR, 12},
		{AdrModeType::INDIRECT_INDEXED_Y_ADR, 16},
		{AdrModeType::ZERO_PAGE_X_ADR, 20},
		{AdrModeType::ABSOLUTE_Y_ADR, 24},
		{AdrModeType::ABSOLUTE_X_ADR, 28}
	};
	static inline AdressModeLUT LUT_Order8 = { 8, Order8 };

	// sta address modes
	static inline AdressModeItem Order7[7] = {
		{AdrModeType::INDEXED_INDIRECT_X_ADR, 0},
		{AdrModeType::ZERO_PAGE_ADR, 4},
		{AdrModeType::ABSOLUTE_ADR, 12},
		{AdrModeType::INDIRECT_INDEXED_Y_ADR, 16},
		{AdrModeType::ZERO_PAGE_X_ADR, 20},
		{AdrModeType::ABSOLUTE_Y_ADR, 24},
		{AdrModeType::ABSOLUTE_X_ADR, 28}
	};
	static inline AdressModeLUT LUT_Order7 = { 7, Order7 };
	//---------------------------------------
	// ASL, LSR, ROR, ROL
	//---------------------------------------
	static inline AdressModeItem Order5[5] = {
		{AdrModeType::ZERO_PAGE_ADR, 0},
		{AdrModeType::ACCUMULATOR,4},
		{AdrModeType::ABSOLUTE_ADR, 8},
		{AdrModeType::ZERO_PAGE_X_ADR, 16},
		{AdrModeType::ABSOLUTE_X_ADR, 24}
	};
	static inline AdressModeLUT LUT_Order5 = { 5, Order5 };
	//----------------------------------------------
	static inline AdressModeItem Order5_LDY[5] = {
		{AdrModeType::IMMEDIATE_ADR,0},
		{AdrModeType::ZERO_PAGE_ADR, 4},
		{AdrModeType::ABSOLUTE_ADR, 12},
		{AdrModeType::ZERO_PAGE_X_ADR, 20},
		{AdrModeType::ABSOLUTE_X_ADR, 28}
	};
	static inline AdressModeLUT LUT_Order5_LDY = { 5, Order5_LDY };
	//---------------------------------------------
	static inline AdressModeItem Order5_LDX[5] = {
		{AdrModeType::IMMEDIATE_ADR,0},
		{AdrModeType::ZERO_PAGE_ADR, 4},
		{AdrModeType::ABSOLUTE_ADR, 12},
		{AdrModeType::ZERO_PAGE_Y_ADR, 20},
		{AdrModeType::ABSOLUTE_Y_ADR, 28}
	};
	static inline AdressModeLUT LUT_Order5_LDX = { 5, Order5_LDX };
	//---------------------------------------------
	static inline AdressModeItem Order4[4] = {
		{AdrModeType::ZERO_PAGE_ADR, 0},
		{AdrModeType::ABSOLUTE_ADR, 8},
		{AdrModeType::ZERO_PAGE_X_ADR, 16},
		{AdrModeType::ABSOLUTE_X_ADR, 24}
	};
	static inline AdressModeLUT LUT_Order4 = { 4, Order4 };
	//---------------------------------------------
	static inline AdressModeItem Order3_STX[3] = {	//STX
		{AdrModeType::ZERO_PAGE_ADR, 0},
		{AdrModeType::ZERO_PAGE_Y_ADR,16},
		{AdrModeType::ABSOLUTE_ADR, 8},
	};
	static inline AdressModeLUT LUT_Order3_STX = { 3, Order3_STX };
	//---------------------------------------------
	static inline AdressModeItem Order3_STY[3] = {	//STY
		{AdrModeType::ZERO_PAGE_ADR, 0},
		{AdrModeType::ZERO_PAGE_X_ADR,16},
		{AdrModeType::ABSOLUTE_ADR, 8},
	};
	static inline AdressModeLUT LUT_Order3_STY = { 3, Order3_STY };
	//---------------------------------------------
	static inline AdressModeItem Order3_CPY_CPX[3] = {	//CPY/CPX
		{AdrModeType::IMMEDIATE_ADR,0},
		{AdrModeType::ZERO_PAGE_ADR, 4},
		{AdrModeType::ABSOLUTE_ADR, 12}
	};
	static inline AdressModeLUT LUT_Order3__CPY_CPX = { 3, Order3_CPY_CPX };
	//---------------------------------------------
	static inline AdressModeItem Order2[2] = {
		{AdrModeType::ABSOLUTE_ADR,0 },
		{AdrModeType::INDIRECT_ADR,32 }
	};
	static inline AdressModeLUT LUT_Order2 = { 2, Order2 };
	//---------------------------------------------
	static inline AdressModeItem Order2_BIT[2] = {
		{AdrModeType::ZERO_PAGE_ADR,0 },
		{AdrModeType::ABSOLUTE_ADR,8 }
	};
	static inline AdressModeLUT LUT_Order2_BIT = { 2, Order2_BIT };
	//---------------------------------------------
	static inline AdressModeItem Order1[1] = {
		{AdrModeType::ABSOLUTE_ADR,0 }
	};
	static inline AdressModeLUT LUT_Order1 = {1, Order1};
	//-------------------------------------------------
	static inline AdressModeItem Order_BRANCH[1] = {
		{AdrModeType::RELATIVE, 0}
	};
	static inline AdressModeLUT LUT_Relative = { 1, Order_BRANCH };
	//------------------------------------------------00
	static inline AdressModeItem Implied[1] = {
		{AdrModeType::IMPLIED, 0}
	};
	static inline AdressModeLUT LUT_Implied = { 1, Implied };
	//------------------------------------------------00
	static inline KeyWord KeyWords[] = {
		{Token::ENDOFFILE,"End Of File"},		//1
		{Token::IDENT," IDENT"},					//2
		{Token::NUMBER,"NUMBER"},				//3
		{Token::CUR_LOC, "Current Location"},
		{Token::MOD,"MOD"},
		{Token::DEFINED_IDENT, "DEFINED IDENT"},
		//--------- Assignment -------------
		{ Token::ASSIGN_ADD,"==+", 0 },
		{ Token::ASSIGN_SUB,"==-", 0 },
		{ Token::ASSIGN_MUL,"==*", 0 },
		{ Token::ASSIGN_DIV,"==/", 0 },
		{ Token::ASSIGN_MOD,"==MOD", 0 },
		{ Token::ASSIGN_AND,"==&", 0 },
		{ Token::ASSIGN_OR,"==%", 0 },
		{ Token::ASSIGN_XOR,"==XOR", 0 },
		{ Token::ASSIGN_RSH,"==RSH", 0 },
		{ Token::ASSIGN_LSH,"==LSH", 0 },
		//---------- Operators ------------
		{Token::LSH,"LSH"},						
		{Token::RSH,"RSH"},						
		{Token::OR,"OR"},						
		{Token::XOR, "XOR"},	
		{Token::MOD,"MOD", 0 },
		{Token::GTEQ,">=", 0 },
		{Token::LTEQ,"<=", 0 },
		//--------------- Functions --------------
		{Token::PROC,"PROC"},	// Proceedure Declaration
		{Token::FUNC,"FUNC"},	//  Function Declaration
		{Token::INTERRUPT,"INTERRUPT"},
		{Token::PROC_IDENT,"PROC Idnet"},	//  Proceedure Declaration
		{Token::FUNC_IDENT,"FUNC Ident"},	//  Function Declaration
		{Token::INTERRUPT_IDENT,"INTERRUPT Indent"},
		{Token::DECLARE,"DECLARE"},
		//------ Data Types -----------------
		{Token::BOOL,"BOOL"},
		{Token::BYTE,"BYTE"},
		{Token::CHAR,"CHAR"},
		{Token::INT,"INT"},
		{Token::CARD,"CARD"},
		{Token::TYPE, "TYPE"},
		{Token::RECORDTYPE,"Record Type (TYPE)"},
		{Token::POINTER,"POINTER"},
		{Token::CONST,"CONST"},
		{Token::ARRAY,"ARRAY"},
		{Token::VAR_GLOBAL,"VAR_GLOBAL"},
		{Token::VAR_LOCAL,"VAR_LOCAL"},
		{Token::VAR_PARAM,"VAR_PARAM"},
	//--------- Statements --------------
		{Token::MODULE,"MODULE"},
		{Token::VECTOR,"VECTOR"},
		{Token::FOR,"FOR"},
		{Token::TO,"TO"},
		{Token::STEP,"STEP"},
		{Token::IF,"IF"},
		{Token::IFF,"IFF"},
		{Token::THEN,"THEN"},
		{Token::ELSEIF,"ELSEIF"},
		{Token::ELSE,"ELSE"},
		{Token::FI,"FI"},
		{Token::FFI,"FFI"},
		{Token::WHILE,"WHILE"},
		{Token::DO,"DO"},
		{Token::OD,"OD"},
		{Token::UNTIL,"UNTIL"},
		{Token::EXIT,"EXIT"},
		{Token::RETURN,"RETURN"},
		{Token::ASM,"ASM"},
		{Token::PUSH,"PUSH"},
		{Token::POP,"POP"},
		{Token::BREAK, "BREAK"},
		{Token::BITS,"BITS"},
		{Token::BEGIN,"BEGIN"},
		{Token::END, "END"},
		//--------- Compiler Directives -----------------
		{Token::DEFINE,"DEFINE"},
		{Token::SET,"SET"},
		{Token::INCLUDE,"INCLUDE"},
		//---------------------------------
		// Assembler Tokens
		//---------------------------------
		//--------- Section ------------
		{Token::SECTION,"SECTION", 0,0, Processor::ALL,NULL,0},
		{Token::SECTION_NAME,"SECTION NAME", 0,0, Processor::ALL,NULL,0},
		{Token::START,"START", 0,0,Processor::ALL,NULL,0},
		{Token::SIZE,"SIZE", 0,0,Processor::ALL,NULL,0},
		{Token::NAME,"NAME", 0,0,Processor::ALL,NULL,0},
		{Token::MODE,"MODE", 0,0,Processor::ALL,NULL,0},
		{Token::READ_WRTE,"READ_WRITE", 0,0,Processor::ALL,NULL,0},
		{Token::READ_ONLY,"READ_ONLY", 0,0,Processor::ALL,NULL,0},
		{Token::ZEROPAGE,"ZEROPAGE", 0},
		{Token::TRUE,"TRUE", 0},
		{Token::FALSE,"FALSE", 0},
		//----- Assembler Declarations
		{Token::ORG,"ORG", 0,0, Processor::ALL,NULL,0},
		{Token::DB,"DB", 0,0, Processor::ALL,NULL,0},
		{Token::DW,"DW", 0,0, Processor::ALL,NULL,0},
		{Token::DL,"DL", 0,0, Processor::ALL,NULL,0},
		{Token::DAS,"DAS", 0,0, Processor::ALL,NULL,0},
		{Token::DCS,"DCS", 0,0, Processor::ALL,NULL,0},
		{Token::DS,"DS", 0,0, Processor::ALL,NULL,0},
		//----------- Opcodes --------------
		{Token::ADC,"ADC", 3 ,0X61, Processor::R6502, &LUT_Order8,8},
		{Token::AND,"AND", 3,0X21, Processor::R6502, &LUT_Order8,8},
		{Token::ASL,"ASL", 3,0X06, Processor::R6502, &LUT_Order5,5},
		{Token::BCC,"BCC", 2,0x90, Processor::R6502, &LUT_Relative,1},
		{Token::BCS,"BCS", 2,0XB0, Processor::R6502, &LUT_Relative,1},
		{Token::BEQ,"BEQ", 2,0XF0, Processor::R6502, &LUT_Relative,1},
		{Token::BMI,"BMI", 2,0x30, Processor::R6502, &LUT_Relative,1},
		{Token::BNE,"BNE", 2,0XD0, Processor::R6502, &LUT_Relative,1},
		{Token::BPL,"BPL", 2,0x10, Processor::R6502, &LUT_Relative,1},
		{Token::BVC,"BVC", 2,0x50, Processor::R6502, &LUT_Relative,1},
		{Token::BVS,"BVS", 2,0x70, Processor::R6502, &LUT_Relative,1},
		{Token::BIT,"BIT", 3,0X24, Processor::R6502, &LUT_Order2_BIT,2},
		{Token::BRK,"BRK", 1,0x00, Processor::R6502, &LUT_Implied,1},
		{Token::CLC,"CLC", 1,0X18, Processor::R6502, &LUT_Implied,1 },
		{Token::CLD,"CLD", 1,0xd8, Processor::R6502, &LUT_Implied,1 },
		{Token::CLI,"CLI", 1,0X58, Processor::R6502, &LUT_Implied,1 },
		{Token::CLV,"CLV", 1,0xb8, Processor::R6502, &LUT_Implied,1 },
		{Token::CMP,"CMP", 3,0xc1, Processor::R6502, &LUT_Order8,8},
		{Token::CPX,"CPX", 3,0XE0, Processor::R6502, &LUT_Order3__CPY_CPX,3},
		{Token::CPY,"CPY", 3,0XC0, Processor::R6502, &LUT_Order3__CPY_CPX,3},
		{Token::DEC,"DEC", 3,0xc6, Processor::R6502, &LUT_Order4,4},
		{Token::DEX,"DEX", 1,0xca, Processor::R6502, &LUT_Implied,1 },
		{Token::DEY,"DEY", 1,0x88, Processor::R6502, &LUT_Implied,1 },
		{Token::EOR,"EOR", 3,0X41, Processor::R6502, &LUT_Order8,8},
		{Token::INC,"INC", 3,0xe6, Processor::R6502, &LUT_Order4,4},
		{Token::INX,"INX", 1,0xe8, Processor::R6502, &LUT_Implied,1 },
		{Token::INY,"INY", 1,0xc8, Processor::R6502, &LUT_Implied,1 },
		{Token::JMP,"JMP", 3,0X4C, Processor::R6502, &LUT_Order2,2},
		{Token::JSR,"JSR", 3,0x20, Processor::R6502, &LUT_Order1,1},
		{Token::LDA, "LDA", 3, 0xa1, Processor::R6502, & LUT_Order8, 8},
		{ Token::LDX,"LDX", 3,0xA2, Processor::R6502, &LUT_Order5_LDX,5 },
		{ Token::LDY,"LDY", 3,0xa0, Processor::R6502, &LUT_Order5_LDY,5 },
		{ Token::LSR,"LSR", 3,0X46, Processor::R6502, &LUT_Order5,5 },
		{ Token::NOP,"NOP", 1,0xea, Processor::R6502, &LUT_Implied,1 },
		{ Token::ORA,"ORA", 3,0X01, Processor::R6502, &LUT_Order8,8 },
		{ Token::PHA,"PHA", 1,0X48, Processor::R6502, &LUT_Implied,1 },
		{ Token::PLA,"PLA", 1,0x68, Processor::R6502, &LUT_Implied,1 },
		{ Token::PHP,"PHP", 1,0X08, Processor::R6502, &LUT_Implied,1 },
		{ Token::PLP,"PLP", 1,0X28, Processor::R6502, &LUT_Implied,1 },
		{ Token::ROL,"ROL",3,0X26, Processor::R6502, &LUT_Order5,5 },
		{ Token::ROR,"ROR",3,0X66, Processor::R6502, &LUT_Order5,5 },
		{ Token::RTI,"RTI", 1,0x40, Processor::R6502, &LUT_Implied,1 },
		{ Token::RTS,"RTS", 1,0x60, Processor::R6502, &LUT_Implied,1 },
		{ Token::SBC,"SBC",3,0xe1, Processor::R6502, &LUT_Order8,8 },
		{ Token::STA,"STA",3,0x81, Processor::R6502, &LUT_Order7,7 },
		{ Token::SEC,"SEC", 1,0x38, Processor::R6502, &LUT_Implied,1 },
		{ Token::SED,"SED", 1,0Xf8, Processor::R6502, &LUT_Implied,1 },
		{ Token::SEI,"SEI", 1,0x78, Processor::R6502, &LUT_Implied,1 },
		{ Token::STX,"STX",3,0x86, Processor::R6502, &LUT_Order3_STX,3 },
		{ Token::STY,"STY",3,0x84, Processor::R6502, &LUT_Order3_STY,3 },
		{ Token::TAX,"TAX", 1,0xaa, Processor::R6502, &LUT_Implied,1 },
		{ Token::TAY,"TAY", 1,0xa8, Processor::R6502,&LUT_Implied,1 },
		{ Token::TXA,"TXA", 1,0x8a, Processor::R6502, &LUT_Implied,1 },
		{ Token::TYA,"TYA", 1,0x98, Processor::R6502, &LUT_Implied,1 },
		{ Token::TXS,"TXS", 1,0x9a, Processor::R6502, &LUT_Implied,1 },
		{ Token::TSX,"TSX", 1,0xba, Processor::R6502, &LUT_Implied,1 },
		//--------------------------------------------
		// W65C02 Opcodes
		//--------------------------------------------
		{ Token::BRA,"BRA", 2,0xba, Processor::W65C02, NULL,0 },	//  Branch Always
		{ Token::PHX,"PHX", 1,0xba, Processor::W65C02, NULL,0 },	// push index reg X
		{ Token::PHY,"PHY", 1,0xba, Processor::W65C02, NULL,0 },	// push Y register
		{ Token::PLX,"PLX", 1,0xba, Processor::W65C02, NULL,0 },	// pop stack to X
		{ Token::PLY,"PLY", 1,0xba, Processor::W65C02, NULL,0 },	// pop stack to Y
		{ Token::STZ,"STZ", 3,0xba, Processor::W65C02, NULL,0 },	// Store Zero to Memory
		{ Token::TRB,"TRB", 3,0xba, Processor::W65C02, NULL,0 },	// Test and reset bits
		{ Token::TSB,"TSB", 3,0xba, Processor::W65C02, NULL,0 },	// Test and Set bits
		//--------------------------------------------
		// Lower Case Opcodes
		//--------------------------------------------
		{ Token::ADC,"adc", 3 ,0X61, Processor::R6502, &LUT_Order8,8 },
		{ Token::AND,"and", 3,0X21, Processor::R6502, &LUT_Order8,8 },
		{ Token::ASL,"asl", 3,0X06, Processor::R6502, &LUT_Order5,5 },
		{ Token::BCC,"bcc", 2,0x90, Processor::R6502, &LUT_Relative,1 },
		{ Token::BCS,"bcs", 2,0XB0, Processor::R6502, &LUT_Relative,1 },
		{ Token::BEQ,"beq", 2,0XF0, Processor::R6502, &LUT_Relative,1 },
		{ Token::BMI,"bmi", 2,0x30, Processor::R6502, &LUT_Relative,1 },
		{ Token::BNE,"bne", 2,0XD0, Processor::R6502, &LUT_Relative,1 },
		{ Token::BPL,"bpl", 2,0x10, Processor::R6502, &LUT_Relative,1 },
		{ Token::BVC,"bvc", 2,0x50, Processor::R6502, &LUT_Relative,1 },
		{ Token::BVS,"bvs", 2,0x70, Processor::R6502, &LUT_Relative,1 },
		{ Token::BIT,"bit", 3,0X24, Processor::R6502, &LUT_Order2_BIT,2 },
		{ Token::BRK,"brk", 1,0x00, Processor::R6502, &LUT_Implied,1 },
		{ Token::CLC,"clc", 1,0X18, Processor::R6502, &LUT_Implied,1 },
		{ Token::CLD,"cld", 1,0xd8, Processor::R6502,&LUT_Implied,1 },
		{ Token::CLI,"cli", 1,0X58, Processor::R6502,&LUT_Implied,1 },
		{ Token::CLV,"clv", 1,0xb8, Processor::R6502, &LUT_Implied,1 },
		{ Token::CMP,"cmp", 3,0xc1, Processor::R6502, &LUT_Order8,8 },
		{ Token::CPX,"cpx", 3,0XE0, Processor::R6502, &LUT_Order3__CPY_CPX,3 },
		{ Token::CPY,"cpy", 3,0XC0, Processor::R6502, &LUT_Order3__CPY_CPX,3 },
		{ Token::DEC,"dec", 3,0xc6, Processor::R6502, &LUT_Order4,4 },
		{ Token::DEX,"dex", 1,0xca, Processor::R6502, &LUT_Implied,1 },
		{ Token::DEY,"dey", 1,0x88, Processor::R6502, &LUT_Implied,1 },
		{ Token::EOR,"eor", 3,0X41, Processor::R6502, &LUT_Order8,8 },
		{ Token::INC,"inc", 3,0xe6, Processor::R6502, &LUT_Order4,4 },
		{ Token::INX,"inx", 1,0xe8, Processor::R6502, &LUT_Implied,1 },
		{ Token::INY,"iny", 1,0xc8, Processor::R6502, &LUT_Implied,1 },
		{ Token::JMP,"jmp", 3,0X4C, Processor::R6502, &LUT_Order2,2 },
		{ Token::JSR,"jsr", 3,0x20, Processor::R6502, &LUT_Order1,1 },
		{ Token::LDA,"lda", 3,0xa1, Processor::R6502, &LUT_Order8,8 },
		{ Token::LDX,"ldx", 3,0xA2, Processor::R6502, &LUT_Order5_LDX,5 },
		{ Token::LDY,"ldy", 3,0xa0, Processor::R6502, &LUT_Order5_LDY,5 },
		{ Token::LSR,"lsr", 3,0X46, Processor::R6502, &LUT_Order5,5 },
		{ Token::NOP,"nop", 1,0xea, Processor::R6502, &LUT_Implied,1 },
		{ Token::ORA,"ora", 3,0X01, Processor::R6502, &LUT_Order8,8 },
		{ Token::PHA,"pha", 1,0X48, Processor::R6502, &LUT_Implied,1 },
		{ Token::PLA,"pla", 1,0x68, Processor::R6502, &LUT_Implied,1 },
		{ Token::PHP,"php", 1,0X08, Processor::R6502, &LUT_Implied,1 },
		{ Token::PLP,"plp", 1,0X28, Processor::R6502, &LUT_Implied,1 },
		{ Token::ROL,"rol",3,0X26, Processor::R6502, &LUT_Order5,5 },
		{ Token::ROR,"ror",3,0X66, Processor::R6502, &LUT_Order5,5 },
		{ Token::RTI,"rti", 1,0x40, Processor::R6502, &LUT_Implied,1 },
		{ Token::RTS,"rts", 1,0x60, Processor::R6502, &LUT_Implied,1 },
		{ Token::SBC,"sbc",3,0xe1, Processor::R6502, &LUT_Order8,8 },
		{ Token::STA,"sta",3,0x81, Processor::R6502, &LUT_Order7,7 },
		{ Token::SEC,"sec", 1,0x38, Processor::R6502, &LUT_Implied,1 },
		{ Token::SED,"sed", 1,0Xf8, Processor::R6502, &LUT_Implied,1 },
		{ Token::SEI,"sei", 1,0x78, Processor::R6502, &LUT_Implied,1 },
		{ Token::STX,"stx",3,0x86, Processor::R6502, &LUT_Order3_STX,3 },
		{ Token::STY,"sty",3,0x84, Processor::R6502, &LUT_Order3_STY,3 },
		{ Token::TAX,"tax", 1,0xaa, Processor::R6502, &LUT_Implied,1 },
		{ Token::TAY,"tay", 1,0xa8, Processor::R6502, &LUT_Implied,1 },
		{ Token::TXA,"txa", 1,0x8a, Processor::R6502, &LUT_Implied,1 },
		{ Token::TYA,"tya", 1,0x98, Processor::R6502, &LUT_Implied,1 },
		{ Token::TXS,"txs", 1,0x9a, Processor::R6502, &LUT_Implied,1 },
		{ Token::TSX,"tsx", 1,0xba, Processor::R6502, &LUT_Implied,1 },
		//--------------------------------------------
		// W65C02 Opcodes
		//--------------------------------------------
		{ Token::BRA,"bra", 2,0xba, Processor::W65C02, &LUT_Relative,1 },	//  Branch Always
		{ Token::PHX,"phx", 1,0xba, Processor::W65C02, &LUT_Implied,1 },	// push index reg X
		{ Token::PHY,"phy", 1,0xba, Processor::W65C02, &LUT_Implied,1 },	// push Y register
		{ Token::PLX,"plx", 1,0xba, Processor::W65C02, &LUT_Implied,1 },	// pop stack to X
		{ Token::PLY,"ply", 1,0xba, Processor::W65C02, &LUT_Implied,1 },	// pop stack to Y
		{ Token::STZ,"stz", 3,0xba, Processor::W65C02, NULL,0 },	// Store Zero to Memory
		{ Token::TRB,"trb", 3,0xba, Processor::W65C02, NULL,0 },	// Test and reset bits
		{ Token::TSB,"tsb", 3,0xba, Processor::W65C02, NULL,0 },	// Test and Set bits
		{ Token::ENDOFOPCODES,"EOO", 0, 0, Processor::ALL,NULL,0 },
		//------------------ Registers -------------------
		{ Token::AREG,".A", 0,0, Processor::R6502, NULL,0 },	// Accumulator
		{ Token::XREG,".X", 0,0, Processor::R6502, NULL,0 },	// X Index Register
		{ Token::YREG,".Y", 0,0, Processor::R6502, NULL,0 },	// Y Index Register
		{ Token::SPREG,".S", 0,0, Processor::R6502, NULL,0 },	// Stack Pointer
		{ Token::PSREG,".P", 0,0, Processor::R6502, NULL,0 },	// Processor Status Register
		{ Token::LOCAL_LABEL,"Local Lable", 0,0,Processor::ALL,NULL,0 },
		{ Token::GLOBAL_LABEL,"Global Lable", 0,0,Processor::ALL,NULL,0 },
		{ Token::LABEL,"LABEL", 0,0,Processor::ALL,NULL,0 },
		//-------- Processor Selection -------
		{ Token::PROCESSOR,"PROCESSOR", 0,0,Processor::ALL,NULL,0 },
		{ Token::R6502,"R6502", 0,0, Processor::ALL,NULL,0 },
		{ Token::W65C02,"WD65C02", 0,0, Processor::ALL ,NULL,0 },
		{ Token::W65C816,"WD65C816", 0,0, Processor::ALL,NULL,0 },
		//----------------------------------
		// Misc
		//----------------------------------
		{ Token::EPROC,"EPROC", 0 },
		{Token::CHAR_CONSTANT,"Character Constant"},
		{ Token('('),"(", 0 },
		{ Token(')'),")", 0 },
		{ Token('{'),"{", 0 },
		{ Token('}'),"}", 0 },
		{ Token('['),"[", 0 },
		{ Token(']'),"]", 0 },
		{ Token('<'),"<", 0 },
		{ Token('>'),">", 0 },
		{ Token('!'),"!", 0 },
		{ Token('@'),"@", 0 },
		{ Token('#'),"#", 0 },
		{ Token('$'),"$", 0 },
		{ Token('%'),"%", 0 },
		{ Token('^'),"^", 0 },
		{ Token('&'),"&", 0 },
		{ Token('*'),"*", 0 },
		{ Token('-'),"-", 0 },
		{ Token('='),"=", 0 },
		{ Token('+'),"+", 0 },
		{ Token(','),",", 0 },
		{ Token('.'),".", 0 },
		{ Token('/'),"/", 0 },
		{ Token('?'),"?", 0 },
		{ Token('~'),"~", 0 },
		{Token::ENDOFTOKENS,NULL}
	};
	CSymTab m_SymbolTable;
	int m_UngetBuffer;
	char m_aLexBuff[256];	// Buffer for the current token string
	int m_LexBuffIndex;		// index into ^
	int m_Line;
	int m_Col;
	int m_LexValue;
	CSymbol* m_pLexSymbol;
	char* m_pFileBuffeer;	// Buffer the source file is stored in
	int m_InFileSize;	// Size of the buffer
	int m_FileIndex;	// Current index into file buffer
	bool m_bAsmMode;
public:
	CLexer();
	virtual ~CLexer();
	bool Create();
	FILE* LogFile();
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
	const char* LookupToName(Token Toke);
	KeyWord* GetKeyWords() { return KeyWords; }
	//--------------------------------------
	// Symbol Methods
	//--------------------------------------
	CSymbol* GetLexSymbol() { return m_pLexSymbol; }
	CSection* GetLexSection() { return (CSection*)m_pLexSymbol; }
	int GetLexValue() { return m_LexValue; }
	char* GetLexBuffer() { return m_aLexBuff; }
	int GetLineNumber() { return m_Line; }
	int GetColunm() { return m_Col; }
};

