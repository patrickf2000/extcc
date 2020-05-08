## extcc

### Introduction
This is my new compiler project. Its called the extensible compiler, meaning that it is intended to provide common utilities to allow making a compiler for multiple languages using as many common parts as possible. So far- and indeed, this is the goal of the project- the everything from the abstract syntax tree (AST) download is unified across languages. That means you only need a frontend parser to generate an AST, which can be sent to the middle and backend for code generation. I am trying to make the frontend as unified as possible, and to provide unified tools wherever possible. So far, I have a universal lexical analyzer, syntax reporting tool, and parser interface.

The backend is also designed to be extensible to new platforms. The middle end IR- called LTAC IR- is designed to translate the AST into pseudo-machine code. This pseudo-machine code is saved as a tree (though its much more linear than the AST), and then easily translated into platform-specific machine code. So far, Intel x86-64 is the most complete. There is also basic support for Arm and LLVM IR. Intel i386 and Java VM is planned (though net definite).

### Note on Origins
A substantial part of this program is reused from my Quik compiler. The entire backend code generator, the LTAC IR generator, and the AST itself were all copied over to this project (with minor modifications for now). Originally, I thought about turning Quik into an extensible compiler, but I decided that would probably be more difficult than starting over from scratch. At the same time though, I was very happy with my design for the AST, the LTAC IR, and the backend for Quik, and because it was modular enough to do so, I decided to reuse those portions.

### Supported Languages
Currently, I have frontends in progress for Quik and C. The C frontend (which is stored in the clang folder) is the most complete, and I'm going to try to finish that first. There is no pre-processor at this point, though I do plan to provide an interface for that.

In addition to C, I have started a new frontend that interfaces directly with the LTAC IR. It is called LTAC Assembly. It is basically pseudo-assembly, so it is probably useful in cases where you may want a cross-platform Assembly language. The frontend transforms it from text to an LTAC tree, from which it is passed directly to the appropriate backend (the compiler is the ltasm program). There is a direct correlation between the LTAC Assembly statements and the abstract LTAC tree nodes.

Note on Quik: If you are interested in the Quik language, then the original Quik compiler provides the most complete implementation. However, this compiler (along with the AST, LTAC IR, and backend) is my new focus, so if you are interested in those portions of the project, then you should reference this.
