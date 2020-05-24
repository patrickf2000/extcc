## extcc

## Notice
This is the new iteration of extcc. For the old working version, please visit the "old" branch. This is not as feature-complete as the original since I am migrating to a new middle-end IR (which is still under development). For right now, the old code is in the legacy folder (but will not be built).

### Introduction
This is my new compiler project. It is called the extensible C compiler. Its name is very much of what it implies. It is designed to be a modular C compiler, both easy to port to new platforms, and easy to extend for other languages.

This project was originally forked from my Quik compiler, and for a while I continued it with a similar code base. However, I recently decided to completely redesign the middle end, and as a result, the backend. All the original code still exists (for the working version, please see the "old" branch, otherwise you can browse the code in the legacy folder). Because the front-end was a rewrite from the Quik compiler, the project is becoming essentially its own thing.

I will add documentation later on once I get a little more done.

For now, it only generates code for the Intel x86-64 platform. Arm 64-bit support (AArch8) will likely be next. You will need the GNU Assembler to assemble the code extcc generates. Currently, only non-PIC executables are generated.
