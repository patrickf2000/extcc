## extcc

This is the Extensible Compiler. This is designed to make creating a compiler frontend and backend for small languages easily. There are three parts: the frontend parsers, the intermediate representations, and the backend code generator.

A substantial part of this program is reused from my Quik compiler. The entire backend code generator, the LTAC IR generator, and the AST itself was all copied over to this project. Originally, I thought about turning Quik into an extensible compiler, but I decided that would probably be more difficult than starting over from scratch. At the same time though, I was very happy with my design for the AST, the LTAC IR, and the backend for Quik, and because it was modular enough to do so, I decided to reuse those portions.
