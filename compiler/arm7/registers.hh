#pragma once

#include <string>

namespace Arm7 {

static std::string registers[] = {
	"r4",
	"r5",
	"r6",
	"r7",
	"r8",
	"r9",
	"r10"
};

static std::string call_regs[] = {
	"r0",
	"r1",
	"r2",
	"r3"
};

static int call_index = 0;

};
