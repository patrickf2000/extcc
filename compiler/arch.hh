#pragma once

#ifdef __amd64__
#define x64
#elif defined __amd64
#define x64
#elif defined __x86_64__
#define x64
#elif defined __x86_64
#define x64
#else
#error Unknown CPU architecture
#endif
