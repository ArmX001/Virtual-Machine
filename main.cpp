#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <math.h>

enum class EOperation : char { INVALID, ADD, SUB, MUL, DIV , MOVI , MOVE , SWAP , TEST , CMP , JMP , CALL , RET , PUSH , POP , PUSHSF , 
						POPSF , JEQ , JNE , JAB , JAE , JBL , JBE , JGR , JGE , JLO , JLE , JOF , JNO , JSN , JNS };

enum class EType : char { REG , NUM };
 
enum class EOpSize : char { BYTE , WORD , DWORD , QWORD };

struct SToken
{
	std::string sWord;
};

struct  SOperand
{
	int		nValue = 0; // 4 byte
	EType	eType = EType::REG; // 1 byte
}; // 8 byte

struct  SCommand
{
	SOperand    tOp1;
	SOperand    tOp2;
	EOperation  eOpCode = EOperation::INVALID;
	EOpSize opndSize = EOpSize::DWORD;
}; // 20 byte

struct SData
{
	int value;
	EOpSize opndSize = EOpSize::DWORD;
};

struct Flag
{
	bool CF = 0;
	bool ZF = 0;
	bool SF = 0;
	bool OF = 0;
};

struct CPUContext
{
	unsigned char RegPool[64];
	Flag FlagReg;

	unsigned int& IP = *reinterpret_cast<unsigned int*>(&RegPool[0]);
	unsigned int& RIP = *reinterpret_cast<unsigned int*>(&RegPool[4]);
	unsigned int& SP = *reinterpret_cast<unsigned int*>(&RegPool[8]);
	unsigned int& SF = *reinterpret_cast<unsigned int*>(&RegPool[12]);

	SCommand IR;
};

struct DecCommand;
struct SMachine;

using PFuncExecute = void(*)(DecCommand&, SMachine&);

struct DecCommand
{
	PFuncExecute pFunc;
	void* Arg1;
	void* Arg2;
};

struct SProgramCode
{
	std::vector<SCommand>	vCom;
};

struct SMachine
{
	CPUContext 	tCont;
	unsigned char* RAM;
};

struct SProgramHeader
{
	int nStackSize;
};

struct SProgramData
{
	std::vector<SData> aData;
};

struct SymbolTable
{
	std::map<std::string,int> stable;
};

struct SProgram
{	
	SymbolTable		tstable;
	SProgramHeader	tHeader;
	SProgramCode	tCode;
	SProgramData	tData;
	unsigned start;
};

void AddByte(DecCommand& cm, SMachine& cpu)
{
	unsigned char& nOp1 = *reinterpret_cast<unsigned char*>(cm.Arg1);
	unsigned char& nOp2 = *reinterpret_cast<unsigned char*>(cm.Arg2);
	unsigned char tOp1 = nOp1;
	nOp1 += nOp2;
	
	if (nOp1 == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (((tOp1 >> 7 == 1) && (nOp2 >> 7 == 1)) || (((tOp1 >> 7) ^ (nOp2 >> 7) == 1 ) && (nOp1 >> 7) == 0)) {
		cpu.tCont.FlagReg.CF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
	}
	
	if (nOp1 < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}
	 
	if ((tOp1 > 0 && nOp2 > 0 && nOp1 < 0) || (tOp1 < 0 && nOp2 < 0 && nOp1 > 0)) {
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.OF = 0;
	}
}
void AddWord(DecCommand& cm, SMachine& cpu)
{
	unsigned short& nOp1 = *reinterpret_cast<unsigned short*>(cm.Arg1);
	unsigned short& nOp2 = *reinterpret_cast<unsigned short*>(cm.Arg2);
	unsigned short tOp1 = nOp1;
	nOp1 += nOp2;

	if (nOp1 == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (((tOp1 >> 15 == 1) && (nOp2 >> 15 == 1)) || (((tOp1 >> 15) ^ (nOp2 >> 15) == 1) && (nOp1 >> 15) == 0)) {
		cpu.tCont.FlagReg.CF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
	}

	if (nOp1 < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}

	if ((tOp1 > 0 && nOp2 > 0 && nOp1 < 0) || (tOp1 < 0 && nOp2 < 0 && nOp1 > 0)) {
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.OF = 0;
	}
}
void AddDWord(DecCommand& cm, SMachine& cpu)
{
	unsigned int& nOp1 = *reinterpret_cast<unsigned int*>(cm.Arg1);
	unsigned int& nOp2 = *reinterpret_cast<unsigned int*>(cm.Arg2);
	unsigned int tOp1 = nOp1;
	nOp1 += nOp2;

	if (nOp1 == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (((tOp1 >> 31 == 1) && (nOp2 >> 31 == 1)) || (((tOp1 >> 31) ^ (nOp2 >> 31) == 1) && (nOp1 >> 31) == 0)) {
		cpu.tCont.FlagReg.CF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
	}

	if (nOp1 < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}

	if ((tOp1 > 0 && nOp2 > 0 && nOp1 < 0) || (tOp1 < 0 && nOp2 < 0 && nOp1 > 0)) {
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.OF = 0;
	}
}
void AddQWord(DecCommand& cm, SMachine& cpu)
{
	unsigned long long& nOp1 = *reinterpret_cast<unsigned long long*>(cm.Arg1);
	unsigned long long& nOp2 = *reinterpret_cast<unsigned long long*>(cm.Arg2);
	unsigned long long tOp1 = nOp1;
	nOp1 += nOp2;

	if (nOp1 == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	  }

	if (((tOp1 >> 63 == 1) && (nOp2 >> 63 == 1)) || (((tOp1 >> 63) ^ (nOp2 >> 63) == 1) && (nOp1 >> 63) == 0)) {
		cpu.tCont.FlagReg.CF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
	}

	if (nOp1 < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}

	if ((tOp1 > 0 && nOp2 > 0 && nOp1 < 0) || (tOp1 < 0 && nOp2 < 0 && nOp1 > 0)) {
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.OF = 0;
	}
}
void SubByte(DecCommand& cm, SMachine& cpu)
{
	unsigned char& nOp1 = *reinterpret_cast<unsigned char*>(cm.Arg1);
	unsigned char& nOp2 = *reinterpret_cast<unsigned char*>(cm.Arg2);
	unsigned char tOp1 = nOp1;
	nOp1 -= nOp2;

	if (nOp1 == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (((tOp1 >> 7 == 1) && (nOp2 >> 7 == 1)) || (((tOp1 >> 7) ^ (nOp2 >> 7) == 1) && (nOp1 >> 7) == 0)) {
		cpu.tCont.FlagReg.CF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
	}

	if (nOp1 < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}

	if ((tOp1 > 0 && nOp2 > 0 && nOp1 < 0) || (tOp1 < 0 && nOp2 < 0 && nOp1 > 0)) {
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.OF = 0;
	}
}
void SubWord(DecCommand& cm, SMachine& cpu)
{
	unsigned short& nOp1 = *reinterpret_cast<unsigned short*>(cm.Arg1);
	unsigned short& nOp2 = *reinterpret_cast<unsigned short*>(cm.Arg2);
	unsigned short tOp1 = nOp1;
	nOp1 -= nOp2;

	if (nOp1 == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (((tOp1 >> 15 == 1) && (nOp2 >> 15 == 1)) || (((tOp1 >> 15) ^ (nOp2 >> 15) == 1) && (nOp1 >> 15) == 0)) {
		cpu.tCont.FlagReg.CF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
	}

	if (nOp1 < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}

	if ((tOp1 > 0 && nOp2 > 0 && nOp1 < 0) || (tOp1 < 0 && nOp2 < 0 && nOp1 > 0)) {
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.OF = 0;
	}
}
void SubDWord(DecCommand& cm, SMachine& cpu)
{
	unsigned int& nOp1 = *reinterpret_cast<unsigned int*>(cm.Arg1);
	unsigned int& nOp2 = *reinterpret_cast<unsigned int*>(cm.Arg2);
	unsigned int tOp1 = nOp1;
	nOp1 -= nOp2;

	if (nOp1 == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (((tOp1 >> 31 == 1) && (nOp2 >> 31 == 1)) || (((tOp1 >> 31) ^ (nOp2 >> 31) == 1) && (nOp1 >> 31) == 0)) {
		cpu.tCont.FlagReg.CF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
	}

	if (nOp1 < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}

	if ((tOp1 > 0 && nOp2 > 0 && nOp1 < 0) || (tOp1 < 0 && nOp2 < 0 && nOp1 > 0)) {
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.OF = 0;
	}
}
void SubQWord(DecCommand& cm, SMachine& cpu)
{
	unsigned long long& nOp1 = *reinterpret_cast<unsigned long long*>(cm.Arg1);
	unsigned long long& nOp2 = *reinterpret_cast<unsigned long long*>(cm.Arg2);
	unsigned long long tOp1 = nOp1;
	nOp1 -= nOp2;

	if (nOp1 == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (((tOp1 >> 63 == 1) && (nOp2 >> 63 == 1)) || (((tOp1 >>  63) ^ (nOp2 >> 63) == 1) && (nOp1 >> 63) == 0)) {
		cpu.tCont.FlagReg.CF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
	}

	if (nOp1 < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}

	if ((tOp1 > 0 && nOp2 > 0 && nOp1 < 0) || (tOp1 < 0 && nOp2 < 0 && nOp1 > 0)) {
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.OF = 0;
	}
}
void MulByte(DecCommand& cm, SMachine& cpu)
{
	unsigned char& nOp1 = *reinterpret_cast<unsigned char*>(cm.Arg1);
	unsigned char& nOp2 = *reinterpret_cast<unsigned char*>(cm.Arg2);
	unsigned short res = static_cast<unsigned short>(nOp1) * static_cast<unsigned short>(nOp2);
	nOp1 *= nOp2;

	if (res > std::pow(2,8)-1) {
		cpu.tCont.FlagReg.CF = 1;
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
		cpu.tCont.FlagReg.OF = 0;
	}
}
void MulWord(DecCommand& cm, SMachine& cpu)
{
	unsigned short& nOp1 = *reinterpret_cast<unsigned short*>(cm.Arg1);
	unsigned short& nOp2 = *reinterpret_cast<unsigned short*>(cm.Arg2);
	unsigned int res = static_cast<unsigned int>(nOp1) * static_cast<unsigned int>(nOp2);
	nOp1 *= nOp2;

	if (res > std::pow(2,16)-1) {
		cpu.tCont.FlagReg.CF = 1;
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
		cpu.tCont.FlagReg.OF = 0;
	}
}
void MulDWord(DecCommand& cm, SMachine& cpu)
{
	unsigned int& nOp1 = *reinterpret_cast<unsigned int*>(cm.Arg1);
	unsigned int& nOp2 = *reinterpret_cast<unsigned int*>(cm.Arg2);
	unsigned long long res = static_cast<unsigned long long>(nOp1) * static_cast<unsigned long long>(nOp2);
	nOp1 *= nOp2;

	if (res > std::pow(2,32)-1) {
		cpu.tCont.FlagReg.CF = 1;
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
		cpu.tCont.FlagReg.OF = 0;
	}
}
void MulQWord(DecCommand& cm, SMachine& cpu)
{
	unsigned long long& nOp1 = *reinterpret_cast<unsigned long long*>(cm.Arg1);
	unsigned long long& nOp2 = *reinterpret_cast<unsigned long long*>(cm.Arg2);
	nOp1 *= nOp2;
	/*
	if () {
		cpu.tCont.FlagReg.CF = 1;
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
		cpu.tCont.FlagReg.OF = 0;
	}
	*/
}
void DivByte(DecCommand& cm, SMachine& cpu)
{
	unsigned char& nOp1 = *reinterpret_cast<unsigned char*>(cm.Arg1);
	unsigned char& nOp2 = *reinterpret_cast<unsigned char*>(cm.Arg2);
	nOp1 /= nOp2;
}
void DivWord(DecCommand& cm, SMachine& cpu)
{
	unsigned short& nOp1 = *reinterpret_cast<unsigned short*>(cm.Arg1);
	unsigned short& nOp2 = *reinterpret_cast<unsigned short*>(cm.Arg2);
	nOp1 /= nOp2;
}
void DivDWord(DecCommand& cm, SMachine& cpu)
{
	unsigned int& nOp1 = *reinterpret_cast<unsigned int*>(cm.Arg1);
	unsigned int& nOp2 = *reinterpret_cast<unsigned int*>(cm.Arg2);
	nOp1 /= nOp2;
}
void DivQWord(DecCommand& cm, SMachine& cpu)
{
	unsigned long long& nOp1 = *reinterpret_cast<unsigned long long*>(cm.Arg1);
	unsigned long long& nOp2 = *reinterpret_cast<unsigned long long*>(cm.Arg2);
	nOp1 /= nOp2;
}
void MoveByte(DecCommand& cm, SMachine& cpu)
{
	unsigned char& nOp1 = *reinterpret_cast<unsigned char*>(cm.Arg1);
	unsigned char& nOp2 = *reinterpret_cast<unsigned char*>(cm.Arg2);
	nOp1 = nOp2;
}
void MoveWord(DecCommand& cm, SMachine& cpu)
{
	unsigned short& nOp1 = *reinterpret_cast<unsigned short*>(cm.Arg1);
	unsigned short& nOp2 = *reinterpret_cast<unsigned short*>(cm.Arg2);
	nOp1 = nOp2;
}
void MoveDWord(DecCommand& cm, SMachine& cpu)
{
	unsigned int& nOp1 = *reinterpret_cast<unsigned int*>(cm.Arg1);
	unsigned int& nOp2 = *reinterpret_cast<unsigned int*>(cm.Arg2);
	nOp1 = nOp2;
}
void MoveQWord(DecCommand& cm, SMachine& cpu)
{
	unsigned long long& nOp1 = *reinterpret_cast<unsigned long long*>(cm.Arg1);
	unsigned long long& nOp2 = *reinterpret_cast<unsigned long long*>(cm.Arg2);
	nOp1 = nOp2;
}
void MoviByte(DecCommand& cm, SMachine& cpu)
{
	unsigned char& nOp1 = *reinterpret_cast<unsigned char*>(cm.Arg1);
	unsigned char& nOp2 = *reinterpret_cast<unsigned char*>(cm.Arg2);
	nOp1 = nOp2;
}
void MoviWord(DecCommand& cm, SMachine& cpu)
{
	unsigned short& nOp1 = *reinterpret_cast<unsigned short*>(cm.Arg1);
	unsigned short& nOp2 = *reinterpret_cast<unsigned short*>(cm.Arg2);
	nOp1 = nOp2;
}
void MoviDWord(DecCommand& cm, SMachine& cpu)
{
	unsigned int& nOp1 = *reinterpret_cast<unsigned int*>(cm.Arg1);
	unsigned int& nOp2 = *reinterpret_cast<unsigned int*>(cm.Arg2);
	nOp1 = nOp2;
}
void MoviQWord(DecCommand& cm, SMachine& cpu)
{
	unsigned long long& nOp1 = *reinterpret_cast<unsigned long long*>(cm.Arg1);
	unsigned long long& nOp2 = *reinterpret_cast<unsigned long long*>(cm.Arg2);
	nOp1 = nOp2;
}
void SwapByte(DecCommand& cm, SMachine& cpu)
{
	unsigned char& nOp1 = *reinterpret_cast<unsigned char*>(cm.Arg1);
	unsigned char& nOp2 = *reinterpret_cast<unsigned char*>(cm.Arg2);
	unsigned char temp = nOp1;
	nOp1 = nOp2;
	nOp2 = temp;
}
void SwapWord(DecCommand& cm, SMachine& cpu)
{
	unsigned short& nOp1 = *reinterpret_cast<unsigned short*>(cm.Arg1);
	unsigned short& nOp2 = *reinterpret_cast<unsigned short*>(cm.Arg2);
	unsigned short temp = nOp1;
	nOp1 = nOp2;
	nOp2 = temp;
}
void SwapDWord(DecCommand& cm, SMachine& cpu)
{
	unsigned int& nOp1 = *reinterpret_cast<unsigned int*>(cm.Arg1);
	unsigned int& nOp2 = *reinterpret_cast<unsigned int*>(cm.Arg2);
	unsigned int temp = nOp1;
	nOp1 = nOp2;
	nOp2 = temp;
}
void SwapQWord(DecCommand& cm, SMachine& cpu)
{
	unsigned long long& nOp1 = *reinterpret_cast<unsigned long long*>(cm.Arg1);
	unsigned long long& nOp2 = *reinterpret_cast<unsigned long long*>(cm.Arg2);
	unsigned long long temp = nOp1;
	nOp1 = nOp2;
	nOp2 = temp;
}
void TestByte(DecCommand& cm, SMachine& cpu)
{
	unsigned char& nOp1 = *reinterpret_cast<unsigned char*>(cm.Arg1);
	unsigned char& nOp2 = *reinterpret_cast<unsigned char*>(cm.Arg2);
	unsigned char res = nOp1 & nOp2;

	cpu.tCont.FlagReg.CF = 0;
	cpu.tCont.FlagReg.OF = 0;
	if (res == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (res < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}
}
void TestWord(DecCommand& cm, SMachine& cpu)
{
	unsigned short& nOp1 = *reinterpret_cast<unsigned short*>(cm.Arg1);
	unsigned short& nOp2 = *reinterpret_cast<unsigned short*>(cm.Arg2);
	unsigned short res = nOp1 & nOp2;

	cpu.tCont.FlagReg.CF = 0;
	cpu.tCont.FlagReg.OF = 0;
	if (res == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (res < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}
}
void TestDWord(DecCommand& cm, SMachine& cpu)
{
	unsigned int& nOp1 = *reinterpret_cast<unsigned int*>(cm.Arg1);
	unsigned int& nOp2 = *reinterpret_cast<unsigned int*>(cm.Arg2);
	unsigned int res = nOp1 & nOp2;

	cpu.tCont.FlagReg.CF = 0;
	cpu.tCont.FlagReg.OF = 0;
	if (res == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (res < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}
}
void TestQWord(DecCommand& cm, SMachine& cpu)
{
	unsigned long long& nOp1 = *reinterpret_cast<unsigned long long*>(cm.Arg1);
	unsigned long long& nOp2 = *reinterpret_cast<unsigned long long*>(cm.Arg2);
	unsigned long long res = nOp1 & nOp2;

	cpu.tCont.FlagReg.CF = 0;
	cpu.tCont.FlagReg.OF = 0;
	if (res == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (res < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}
}
void CmpByte(DecCommand& cm, SMachine& cpu)
{
	unsigned char& nOp1 = *reinterpret_cast<unsigned char*>(cm.Arg1);
	unsigned char& nOp2 = *reinterpret_cast<unsigned char*>(cm.Arg2);
	unsigned char res = nOp1 - nOp2;

	if (res == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (((nOp1 >> 7 == 1) && (nOp2 >> 7 == 1)) || (((nOp1 >> 7) ^ (nOp2 >> 7) == 1) && (res >> 7) == 0)) {
		cpu.tCont.FlagReg.CF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
	}

	if (res < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}

	if ((nOp1 > 0 && nOp2 > 0 && res < 0) || (nOp1 < 0 && nOp2 < 0 && res > 0)) {
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.OF = 0;
	}
}
void CmpWord(DecCommand& cm, SMachine& cpu)
{
	unsigned short& nOp1 = *reinterpret_cast<unsigned short*>(cm.Arg1);
	unsigned short& nOp2 = *reinterpret_cast<unsigned short*>(cm.Arg2);
	unsigned short res = nOp1 - nOp2;

	if (res == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (((nOp1 >> 15 == 1) && (nOp2 >> 15 == 1)) || (((nOp1 >> 15) ^ (nOp2 >> 15) == 1) && (res >> 15) == 0)) {
		cpu.tCont.FlagReg.CF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
	}

	if (res < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}

	if ((nOp1 > 0 && nOp2 > 0 && res < 0) || (nOp1 < 0 && nOp2 < 0 && res > 0)) {
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.OF = 0;
	}
}
void CmpDWord(DecCommand& cm, SMachine& cpu)
{
	unsigned int& nOp1 = *reinterpret_cast<unsigned int*>(cm.Arg1);
	unsigned int& nOp2 = *reinterpret_cast<unsigned int*>(cm.Arg2);
	unsigned int res = nOp1 - nOp2;

	if (res == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (((nOp1 >> 31 == 1) && (nOp2 >> 31 == 1)) || (((nOp1 >> 31) ^ (nOp2 >> 31) == 1) && (res >> 31) == 0)) {
		cpu.tCont.FlagReg.CF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
	}

	if (res < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}

	if ((nOp1 > 0 && nOp2 > 0 && res < 0) || (nOp1 < 0 && nOp2 < 0 && res > 0)) {
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.OF = 0;
	}
}
void CmpQWord(DecCommand& cm, SMachine& cpu)
{
	unsigned long long& nOp1 = *reinterpret_cast<unsigned long long*>(cm.Arg1);
	unsigned long long& nOp2 = *reinterpret_cast<unsigned long long*>(cm.Arg2);
	unsigned long long res = nOp1 - nOp2;

	if (res == 0) {
		cpu.tCont.FlagReg.ZF = 1;
	}
	else {
		cpu.tCont.FlagReg.ZF = 0;
	}

	if (((nOp1 >> 63 == 1) && (nOp2 >> 63 == 1)) || (((nOp1 >> 63) ^ (nOp2 >> 63) == 1) && (res >> 63) == 0)) {
		cpu.tCont.FlagReg.CF = 1;
	}
	else {
		cpu.tCont.FlagReg.CF = 0;
	}

	if (res < 0) {
		cpu.tCont.FlagReg.SF = 1;
	}
	else {
		cpu.tCont.FlagReg.SF = 0;
	}

	if ((nOp1 > 0 && nOp2 > 0 && res < 0) || (nOp1 < 0 && nOp2 < 0 && res > 0)) {
		cpu.tCont.FlagReg.OF = 1;
	}
	else {
		cpu.tCont.FlagReg.OF = 0;
	}
}
void Jump(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	cpu.tCont.IP += x*20;
}
void Call(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	cpu.tCont.RIP = cpu.tCont.IP;
	cpu.tCont.IP += x*20;
}
void Ret(DecCommand& cm, SMachine& cpu)
{
	cpu.tCont.IP = cpu.tCont.RIP;
	cpu.tCont.RIP = 0;
}
void PushByte(DecCommand& cm, SMachine& cpu)
{
	cpu.RAM[cpu.tCont.SP] = *reinterpret_cast<unsigned char*>(&cm.Arg1);
	cpu.tCont.SP += 1;
}
void PushWord(DecCommand& cm, SMachine& cpu)
{
	cpu.RAM[cpu.tCont.SP] = *reinterpret_cast<unsigned char*>(&cm.Arg1);
	cpu.tCont.SP += 2;
}
void PushDWord(DecCommand& cm, SMachine& cpu)
{
	cpu.RAM[cpu.tCont.SP] = *reinterpret_cast<unsigned char*>(&cm.Arg1);
	cpu.tCont.SP += 4;
}
void PushQWord(DecCommand& cm, SMachine& cpu)
{
	cpu.RAM[cpu.tCont.SP] = *reinterpret_cast<unsigned char*>(&cm.Arg1);
	cpu.tCont.SP += 8;
}
void PopByte(DecCommand& cm, SMachine& cpu)
{
	unsigned char& dest = *reinterpret_cast<unsigned char*>(&cm.Arg1);
	cpu.tCont.SP -= 1;
	dest = cpu.RAM[cpu.tCont.SP];
}
void PopWord(DecCommand& cm, SMachine& cpu)
{
	unsigned char& dest = *reinterpret_cast<unsigned char*>(&cm.Arg1);
	cpu.tCont.SP -= 2;
	dest = cpu.RAM[cpu.tCont.SP];
}
void PopDWord(DecCommand& cm, SMachine& cpu)
{
	unsigned char& dest = *reinterpret_cast<unsigned char*>(&cm.Arg1);
	cpu.tCont.SP -= 4;
	dest = cpu.RAM[cpu.tCont.SP];
}
void PopQWord(DecCommand& cm, SMachine& cpu)
{
	unsigned char& dest = *reinterpret_cast<unsigned char*>(&cm.Arg1);
	cpu.tCont.SP -= 8;
	dest = cpu.RAM[cpu.tCont.SP];
}
void Pushsf(DecCommand& cm, SMachine& cpu)
{
	unsigned int& off = *reinterpret_cast<unsigned int*>(&cm.Arg1);
	reinterpret_cast<unsigned int&>(cpu.RAM[cpu.tCont.SP]) = cpu.tCont.RIP;
	cpu.tCont.SP += 4;
	reinterpret_cast<unsigned int&>(cpu.RAM[cpu.tCont.SP]) = cpu.tCont.SF;
	cpu.tCont.SP += 4;
	cpu.tCont.SF = cpu.tCont.SP;
	cpu.tCont.SP += off;
}
void Popsf(DecCommand& cm, SMachine& cpu)
{
	unsigned int& off = *reinterpret_cast<unsigned int*>(&cm.Arg1);
	cpu.tCont.SP = cpu.tCont.SF;
	cpu.tCont.SP -= 4;
	cpu.tCont.SF = *reinterpret_cast<unsigned int*>(&cpu.RAM[cpu.tCont.SP]);
	cpu.tCont.SP -= 4;
	cpu.tCont.RIP = *reinterpret_cast<unsigned int*>(&cpu.RAM[cpu.tCont.SP]);
	cpu.tCont.SP -= off;
}
void Jeq(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if(cpu.tCont.FlagReg.ZF == 1)
		cpu.tCont.IP += (x*20);
}
void Jne(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if (cpu.tCont.FlagReg.ZF == 0)
		cpu.tCont.IP += (x*20);
}
void Jab(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if (cpu.tCont.FlagReg.ZF == 0 && cpu.tCont.FlagReg.CF == 0)
		cpu.tCont.IP += (x * 20);
}
void Jae(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if (cpu.tCont.FlagReg.CF == 0)
		cpu.tCont.IP += (x * 20);
}
void Jbl(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if (cpu.tCont.FlagReg.CF == 1)
		cpu.tCont.IP += (x * 20);
}
void Jbe(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if (cpu.tCont.FlagReg.ZF == 1 || cpu.tCont.FlagReg.CF == 1)
		cpu.tCont.IP += (x * 20);
}
void Jgr(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if (cpu.tCont.FlagReg.ZF == 0 && cpu.tCont.FlagReg.SF == cpu.tCont.FlagReg.OF)
		cpu.tCont.IP += (x * 20);
}
void Jge(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if (cpu.tCont.FlagReg.SF == cpu.tCont.FlagReg.OF)
		cpu.tCont.IP += (x * 20);
}
void Jlo(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if(cpu.tCont.FlagReg.SF != cpu.tCont.FlagReg.OF)
		cpu.tCont.IP += (x * 20);
}
void Jle(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if (cpu.tCont.FlagReg.ZF == 1 || cpu.tCont.FlagReg.SF != cpu.tCont.FlagReg.OF)
		cpu.tCont.IP += (x * 20);
}
void Jof(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if (cpu.tCont.FlagReg.OF == 1)
		cpu.tCont.IP += (x * 20);
}
void Jno(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if (cpu.tCont.FlagReg.OF == 0)
		cpu.tCont.IP += (x * 20);
}
void Jns(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if (cpu.tCont.FlagReg.SF == 0)
		cpu.tCont.IP += (x * 20);
}
void Jsn(DecCommand& cm, SMachine& cpu)
{
	int& x = *reinterpret_cast<int*>(cm.Arg1);
	if (cpu.tCont.FlagReg.SF == 1)
		cpu.tCont.IP += (x * 20);
}

class CParserError : public std::exception
{
public:
	std::string err;
	CParserError(std::string o)
		: err(o)
	{}
};

bool isSpace(const char ch)
{
	switch (ch) {
	case  ' ':
	case '\n':
	case '\t':
		return true;
	default:
		break;
	}
	return false;
}

bool isDelimeter(const char ch)
{
	return (ch < 'a' || ch > 'z') && (ch < 'A' || ch > 'Z') && ch != '_' && ch != ':' && ch != '.' && (ch < '0' || ch > '9');
}

bool isReg(SOperand tOp) {
	return tOp.eType == EType::REG;
}

void NextLine(std::istream& oInput)
{
	if (!oInput.eof()) {
		char ch;
		while (oInput.peek() != '\n') {
			oInput.get(ch);
			if (ch != ' ' && ch != '\t') {
				throw CParserError("Syntax Error");
			}
		}
		oInput.get(ch);
	}
}

SToken parseToken(std::istream& oInput)
{  
	SToken oToken;
	char ch;
	do {
		oInput.get(ch);
	} while (isSpace(ch));

	oToken.sWord.append(1, ch);
	if (!isDelimeter(ch)) {
		while (!isDelimeter(oInput.peek())) {
			oInput.get(ch);
			oToken.sWord.append(1, ch);
		}
	}
	return oToken;
}

EOperation parseOperation(SToken oToken)
{
	EOperation ret;

	if (oToken.sWord == "ADD") {
		ret = EOperation::ADD;
	}else if(oToken.sWord == "SUB") {
		ret = EOperation::SUB;
	}
	else if (oToken.sWord == "MUL") {
		ret = EOperation::MUL;
	}
	else if (oToken.sWord == "DIV") {
		ret = EOperation::DIV;
	}
	else if (oToken.sWord == "MOVI") {
		ret = EOperation::MOVI;
	}
	else if (oToken.sWord == "MOVE") {
		ret = EOperation::MOVE;
	}
	else if (oToken.sWord == "SWAP") {
		ret = EOperation::SWAP;
	}
	else if (oToken.sWord == "TEST") {
		ret = EOperation::TEST;
	}
	else if (oToken.sWord == "CMP") {
		ret = EOperation::CMP;
	}
	else if (oToken.sWord == "JMP") {
		ret = EOperation::JMP;
	}
	else if (oToken.sWord == "CALL") {
		ret = EOperation::CALL;
	}
	else if (oToken.sWord == "RET") {
		ret = EOperation::RET;
	}
	else if (oToken.sWord == "PUSH") {
		ret = EOperation::PUSH;
	}
	else if (oToken.sWord == "POP") {
		ret = EOperation::POP;
	}
	else if (oToken.sWord == "PUSHSF") {
		ret = EOperation::PUSHSF;
	}
	else if (oToken.sWord == "POPSF") {
		ret = EOperation::POPSF;
	}
	else if (oToken.sWord == "JEQ") {
		ret = EOperation::JEQ;
	}
	else if (oToken.sWord == "JAB") {
		ret = EOperation::JAB;
	}
	else if (oToken.sWord == "JNE") {
		ret = EOperation::JNE;
	}
	else if (oToken.sWord == "JAE") {
		ret = EOperation::JAE;
	}
	else if (oToken.sWord == "JBL") {
		ret = EOperation::JBL;
	}
	else if (oToken.sWord == "JBE") {
		ret = EOperation::JBE;
	}
	else if (oToken.sWord == "JGR") {
		ret = EOperation::JGR;
	}
	else if (oToken.sWord == "JGE") {
		ret = EOperation::JGE;
	}
	else if (oToken.sWord == "JLO") {
		ret = EOperation::JLO;
	}
	else if (oToken.sWord == "JLE") {
		ret = EOperation::JLE;
	}
	else if (oToken.sWord == "JOF") {
		ret = EOperation::JOF;
	}
	else if (oToken.sWord == "JNO") {
		ret = EOperation::JNO;
	}
	else if (oToken.sWord == "JSN") {
		ret = EOperation::JSN;
	}
	else if (oToken.sWord == "JNS") {
		ret = EOperation::JNS;
	}
	else {
		throw CParserError("Invalid operation");
	}
	
	return ret;
}
 
SOperand parseOperand(SToken oToken)
{
	SOperand name;
	
	if (oToken.sWord[0] == 'R') {
		name.eType = EType::REG;
		if (std::stoi(oToken.sWord.substr(1)) >= 0 && std::stoi(oToken.sWord.substr(1)) <= 63) {
			name.nValue = std::stoi(oToken.sWord.substr(1));
		}
		else {
			throw CParserError("Invalid Reg number");
		}
	}
	else if (std::stoi(oToken.sWord)) {
		name.eType = EType::NUM;
		name.nValue = std::stoi(oToken.sWord);
	}
	else {
		throw CParserError("Invalid Operand input");
	}

	return name;
}

EOpSize parseOpSize(SToken oToken) {
	EOpSize tOpSize;
	
	if (oToken.sWord == "BYTE") {
		tOpSize = EOpSize::BYTE;
	}else if (oToken.sWord == "WORD") {
		tOpSize = EOpSize::WORD;
	}
	else if (oToken.sWord == "DWORD") {
		tOpSize = EOpSize::DWORD;
	}
	else if (oToken.sWord == "QWORD") {
		tOpSize = EOpSize::QWORD;
	}

	return tOpSize;
}

SProgram compiler(std::istream& oInput)
{
	std::map<std::string, int> lMp;

	SProgram cm;
	char block = 's';

	if (block == 's') {
		SToken t0 = parseToken(oInput);
		SToken t1 = parseToken(oInput);
		if (t0.sWord == ".STACK" && t1.sWord == "=") {
			SToken t2 = parseToken(oInput);
			SOperand op = parseOperand(t2);
			cm.tHeader.nStackSize = op.nValue;
		}
		else {
			throw CParserError("Syntax Error in .STACK");
		}
		NextLine(oInput);
		block = 'd';
	}

	if (block == 'd') {
		SToken t0 = parseToken(oInput);
		if (t0.sWord == ".DATA") {
			while (block == 'd') {
				SToken t1 = parseToken(oInput);				
				if (t1.sWord == "BYTE" || t1.sWord == "WORD" || t1.sWord == "DWORD" || t1.sWord == "QWORD") {
					SData oData;
					oData.opndSize = parseOpSize(t1);
					SToken t2 = parseToken(oInput);
					SToken t3 = parseToken(oInput);
					if(t3.sWord == "="){
						SToken t4 = parseToken(oInput);
						oData.value = std::stoi(t4.sWord);
						cm.tData.aData.push_back(oData);
						cm.tstable.stable.insert(std::pair<std::string, int>(t2.sWord, oData.value));
					}
					else {
						throw CParserError("Syntax Error : Data must be initialized!");
					}
				}
				else if (t1.sWord == ".CODE") {
					block = 'c';
				}
				else {
					throw CParserError("Syntax Error in .DATA");
				}
				NextLine(oInput);
			}
		}
		else {
			throw CParserError("Syntax Error in .DATA");
		}
	}
	
	if (block == 'c') {
			while (block == 'c') {
				SToken t1 = parseToken(oInput);
				if (t1.sWord == "Func") {
					SToken t2 = parseToken(oInput);
					if(t2.sWord.back() == ':'){
						lMp.insert(std::pair<std::string, int>(t2.sWord.substr(0, t2.sWord.size() - 1), cm.tCode.vCom.size()));
					}
					else {
						throw CParserError("Syntax Error in .CODE");
					}
				}
				else if (t1.sWord.back() == ':')
				{
					auto it = lMp.find(t1.sWord.substr(0, t1.sWord.size() - 1));
					if (it == lMp.end()) {
						lMp.insert(std::pair<std::string, int>(t1.sWord.substr(0, t1.sWord.size() - 1), cm.tCode.vCom.size()));
					}
					else {
						cm.tCode.vCom[it->second].tOp1.nValue = cm.tCode.vCom.size() - it->second - 1;
					}
				}
				else if (t1.sWord == "EndF")
				{
					//End of Function -> Do nothing
				}
				else if (t1.sWord == ".MAIN") {
					block = 'm';
				}
				else
				{
					SCommand tCmd;
					tCmd.eOpCode = parseOperation(t1);
					if (tCmd.eOpCode == EOperation::CALL || tCmd.eOpCode == EOperation::JMP || tCmd.eOpCode == EOperation::JEQ || tCmd.eOpCode == EOperation::JNE || tCmd.eOpCode == EOperation::JAB || tCmd.eOpCode == EOperation::JAE || tCmd.eOpCode == EOperation::JBL || tCmd.eOpCode == EOperation::JBE || tCmd.eOpCode == EOperation::JGR || tCmd.eOpCode == EOperation::JGE || tCmd.eOpCode == EOperation::JLE || tCmd.eOpCode == EOperation::JLO || tCmd.eOpCode == EOperation::JOF || tCmd.eOpCode == EOperation::JNO || tCmd.eOpCode == EOperation::JNS || tCmd.eOpCode == EOperation::JSN)
					{
						SToken j = parseToken(oInput);
						if (j.sWord.back() == ':') {
							auto it = lMp.find(j.sWord.substr(0, j.sWord.size() - 1));
							if (it == lMp.end()) {
								lMp.insert(std::pair<std::string, int>(j.sWord.substr(0, j.sWord.size() - 1), cm.tCode.vCom.size()));
							}
							else {
								tCmd.tOp1.nValue = it->second - cm.tCode.vCom.size() - 1;
							}
						}
						else {
							throw CParserError("Invalid Operation (JMP) Argument");
						}
					}
					else if (tCmd.eOpCode == EOperation::PUSHSF) {
						SToken off = parseToken(oInput);
						tCmd.tOp1 = parseOperand(off);
					}
					else if (tCmd.eOpCode == EOperation::POPSF) {
						SToken off = parseToken(oInput);
						tCmd.tOp1 = parseOperand(off);
					}
					else if (tCmd.eOpCode == EOperation::RET) {
						//RET must be empty
					}
					else if (tCmd.eOpCode == EOperation::PUSH || tCmd.eOpCode == EOperation::POP) {
						SToken t2 = parseToken(oInput);
						if (t2.sWord == "BYTE" || t2.sWord == "WORD" || t2.sWord == "DWORD" || t2.sWord == "QWORD")
						{
							tCmd.opndSize = parseOpSize(t2);
							SToken t3 = parseToken(oInput);
							tCmd.tOp1 = parseOperand(t3);
						}
						else {
							tCmd.tOp1 = parseOperand(t2);
						}
					}
					else
					{
						SToken t2 = parseToken(oInput);
						if (t2.sWord == "BYTE" || t2.sWord == "WORD" || t2.sWord == "DWORD" || t2.sWord == "QWORD")
						{
							tCmd.opndSize = parseOpSize(t2);
							SToken t3 = parseToken(oInput);
							tCmd.tOp1 = parseOperand(t3);
							SToken t4 = parseToken(oInput);
							if (tCmd.eOpCode == EOperation::MOVI && cm.tstable.stable[t4.sWord]) {
								tCmd.tOp2.eType = EType::NUM;
								tCmd.tOp2.nValue = cm.tstable.stable[t4.sWord];
							}
							else {
								tCmd.tOp2 = parseOperand(t4);
							}
						}
						else
						{
							tCmd.tOp1 = parseOperand(t2);
							SToken t3 = parseToken(oInput);
							if (tCmd.eOpCode == EOperation::MOVI && cm.tstable.stable[t3.sWord]) {
								tCmd.tOp2.eType = EType::NUM;
								tCmd.tOp2.nValue = cm.tstable.stable[t3.sWord];
							}
							else {
								tCmd.tOp2 = parseOperand(t3);
							}
						}
					}
					cm.tCode.vCom.push_back(tCmd);
					NextLine(oInput);
				}
			}
	}
	
	if (block == 'm') {
		SToken t1 = parseToken(oInput);
		if (t1.sWord == "=") {
			SToken t2 = parseToken(oInput);
			auto it = lMp.find(t2.sWord);
			cm.start = it->second;
		}
	}

	return cm;
}

void fetch(SMachine& vm) {
	vm.tCont.IR = *reinterpret_cast<SCommand*>(&vm.RAM[vm.tCont.IP]);
}
	
DecCommand decode(CPUContext& cpu) {
	DecCommand tDecCom;

	switch(cpu.IR.eOpCode){
		case EOperation::ADD:
			if (cpu.IR.opndSize == EOpSize::BYTE) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &AddByte;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}else if (cpu.IR.opndSize == EOpSize::WORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &AddWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::DWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &AddDWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::QWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &AddQWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			break;
		case EOperation::SUB:
			if (cpu.IR.opndSize == EOpSize::BYTE) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &SubByte;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::WORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &SubWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::DWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &SubDWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::QWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &SubQWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			break;
		case EOperation::MUL:
			if (cpu.IR.opndSize == EOpSize::BYTE) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &MulByte;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::WORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &MulWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::DWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &MulDWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::QWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &MulQWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			break;
		case EOperation::DIV:
			if (cpu.IR.opndSize == EOpSize::BYTE) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &DivByte;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::WORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &DivWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::DWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &DivDWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::QWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &DivQWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			break;
		case EOperation::MOVE:
			if (cpu.IR.opndSize == EOpSize::BYTE) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &MoveByte;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::WORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &MoveWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::DWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &MoveDWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::QWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &MoveQWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			break;
		case EOperation::MOVI:
			if (cpu.IR.opndSize == EOpSize::BYTE) {
				if (isReg(cpu.IR.tOp1) && !isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &MoviByte;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.IR.tOp2.nValue);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::WORD) {
				if (isReg(cpu.IR.tOp1) && !isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &MoviWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.IR.tOp2.nValue);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::DWORD) {
				if (isReg(cpu.IR.tOp1) && !isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &MoviDWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.IR.tOp2.nValue);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::QWORD) {
				if (isReg(cpu.IR.tOp1) && !isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &MoviQWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.IR.tOp2.nValue);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			break;
		case EOperation::SWAP:
			if (cpu.IR.opndSize == EOpSize::BYTE) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &SwapByte;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::WORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &SwapWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::DWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &SwapDWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::QWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &SwapQWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			break;
		case EOperation::TEST:
			if (cpu.IR.opndSize == EOpSize::BYTE) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &TestByte;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::WORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &TestWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::DWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &TestDWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::QWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &TestQWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			break;
		case EOperation::CMP:
			if (cpu.IR.opndSize == EOpSize::BYTE) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &CmpByte;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::WORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &CmpWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::DWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &CmpDWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::QWORD) {
				if (isReg(cpu.IR.tOp1) && isReg(cpu.IR.tOp2)) {
					tDecCom.pFunc = &CmpQWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
					tDecCom.Arg2 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp2.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			break;
		case EOperation::PUSH:
			if (cpu.IR.opndSize == EOpSize::BYTE) {
				if (isReg(cpu.IR.tOp1)) {
					tDecCom.pFunc = &PushByte;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::WORD) {
				if (isReg(cpu.IR.tOp1)) {
					tDecCom.pFunc = &PushWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::DWORD) {
				if (isReg(cpu.IR.tOp1)) {
					tDecCom.pFunc = &PushDWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			else if (cpu.IR.opndSize == EOpSize::QWORD) {
				if (isReg(cpu.IR.tOp1)) {
					tDecCom.pFunc = &PushQWord;
					tDecCom.Arg1 = reinterpret_cast<void *>(&cpu.RegPool[cpu.IR.tOp1.nValue]);
				}
				else {
					throw CParserError("Invalid Operation Operand");
				}
			}
			break;
		case EOperation::JMP:
			tDecCom.pFunc = &Jump;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::CALL:
			tDecCom.pFunc = &Call;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::RET:
			tDecCom.pFunc = &Ret;
			break;
		case EOperation::PUSHSF:
			tDecCom.pFunc = &Pushsf;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::POPSF:
			tDecCom.pFunc = &Popsf;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JEQ:
			tDecCom.pFunc = &Jeq;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JNE:
			tDecCom.pFunc = &Jne;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JAB:
			tDecCom.pFunc = &Jab;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JAE:
			tDecCom.pFunc = &Jae;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JBL:
			tDecCom.pFunc = &Jbl;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JBE:
			tDecCom.pFunc = &Jbe;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JGR:
			tDecCom.pFunc = &Jgr;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JGE:
			tDecCom.pFunc = &Jge;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JLO:
			tDecCom.pFunc = &Jlo;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JLE:
			tDecCom.pFunc = &Jle;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JOF:
			tDecCom.pFunc = &Jof;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
		case EOperation::JNO:
			tDecCom.pFunc = &Jno;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JSN:
			tDecCom.pFunc = &Jsn;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
		case EOperation::JNS:
			tDecCom.pFunc = &Jns;
			tDecCom.Arg1 = reinterpret_cast<void*>(&cpu.IR.tOp1.nValue);
			break;
	}

	return tDecCom;
}

void execute(DecCommand& decCom , SMachine& sm)
{
	decCom.pFunc(decCom, sm);
}
 
SMachine loader(SProgram& oProgram , SMachine& vm)
{
	int dsize = oProgram.tData.aData.size() * sizeof(oProgram.tData.aData[0]);
	int csize = oProgram.tCode.vCom.size() * sizeof(oProgram.tCode.vCom[0]);

	std::size_t size = dsize + csize + oProgram.tHeader.nStackSize;
	vm.RAM = new unsigned char[size];
	
	int ram_counter = 0;

	for (int i = 0; i < oProgram.tData.aData.size(); i++) {
		std::memcpy(&vm.RAM[ram_counter], &oProgram.tData.aData[i], 8);
		ram_counter += 8;
	}

	for (int i = 0; i < oProgram.tCode.vCom.size(); i++) {
		std::memcpy(&vm.RAM[ram_counter], &oProgram.tCode.vCom[i], 20);
		ram_counter += 20;
	}

	vm.tCont.SP = dsize + csize;
	vm.tCont.SF = dsize + csize;
	vm.tCont.IP = dsize + oProgram.start * sizeof(oProgram.tCode.vCom[0]);
}

void startMachine(SProgram& oProgram)
{
	SMachine oMachine;
	loader(oProgram,oMachine);
	
	while (oMachine.tCont.IP < oMachine.tCont.SP) {
		fetch(oMachine);
		DecCommand cmd = decode(oMachine.tCont);
		oMachine.tCont.IP += 20;
		execute(cmd, oMachine);
	}

	std::cout << static_cast<int>(oMachine.tCont.RegPool[20]) << std::endl << static_cast<int>(oMachine.tCont.RegPool[25]) << std::endl << static_cast<int>(oMachine.tCont.RegPool[30]) << std::endl; 
}

int main() {
	
	std::ifstream file("input.txt", std::ios::in);
	
	try {
		SProgram program = compiler(file);
		startMachine(program);
	}catch(CParserError& e) {
		std::cerr << e.err << std::endl;
	}

	system("pause > bin");
	return 0;
 } 