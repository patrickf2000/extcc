#pragma once

//There are LTAC commands to manage the registers
// These represent the registers and the order they should
// be used.

//The following registers are resevered:
// rbp and rsp -> Memory operations
// r8 and r9-> Internal operations

//64-bit registers
static std::string registers[] = {
	"r10",
	"r11",
	"r12",
	"r13",
	"r14",
	"r15",
	"rax",
	"rbx",
	"rcx",
	"rdx",
	"rsi",
	"rdi"
};

//32-bit registers
static std::string registers32[] = {
	"r10d",
	"r11d",
	"r12d",
	"r13d",
	"r14d",
	"r15d",
	"eax",
	"ebx",
	"ecx",
	"edx",
	"esi",
	"edi"
};

//Floating point registers
static std::string float_registers[] = {
	"xmm8",
	"xmm9",
	"xmm10",
	"xmm11",
	"xmm12",
	"xmm13",
	"xmm14",
	"xmm15",
	"xmm0",
	"xmm1",
	"xmm2",
	"xmm3",
	"xmm4",
	"xmm5",
	"xmm6",
	"xmm7"
};

//Vector registers
static std::string vector_registers[] = {
	"ymm0",
	"ymm1",
	"ymm2",
	"ymm3",
	"ymm4",
	"ymm5",
	"ymm6",
	"ymm7",
	"ymm8",
	"ymm9",
	"ymm10",
	"ymm11",
	"ymm12",
	"ymm13",
	"ymm14",
	"ymm15"
};


