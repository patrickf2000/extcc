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
