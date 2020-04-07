## LTAC Assembly

### Intro
LTAC Assembly provides a simple way to interface with the middle and backend. This Assembly is a pseudo-Assembly language. The instructions and overall syntax are similar to other Assembly languages, but it is designed to be as cross-architecture as possible. You can use the asm program to see the LTAC output, and the ltasm program to compile it to Assembly code for your platform.

### Instructions
section {data, code}&nbsp;&nbsp;&nbsp;&nbsp;    Specifies the section we are in (data for variables, code for instructions).   
func {name}&nbsp;&nbsp;&nbsp;&nbsp;    Declare a function   
pusharg {type} {value}&nbsp;&nbsp;&nbsp;&nbsp;    Prepare a variable or a value to be used as a function argument   
call {name}&nbsp;&nbsp;&nbsp;&nbsp;    Call a function   
var {name} {type} {value}&nbsp;&nbsp;&nbsp;&nbsp;    Declare a new variable and set its initial value   
ret {?value}&nbsp;&nbsp;&nbsp;&nbsp;    Return from a function with an optional value   

### Types
int&nbsp;&nbsp;&nbsp;&nbsp;    Integers   
string&nbsp;&nbsp;&nbsp;&nbsp;    Strings   

