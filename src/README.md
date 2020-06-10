# How this all works

This document exists to document how the simplang compiler works and allows me to keep my sanity.



## The Pipeline

1. Tokenize into Tokens. These tokens have a category and type (lexer.cpp)
2. Parse the Tokens. The tokens are parsed for structure! It doesn't matter what tokens are being used, as long as they follow the structure of the language. The [parser](/src/parser.cpp) takes the tokens and generates an [Abstract Syntax Tree](/src/statement.cpp) along with some [type information](/src/typing.cpp). 
3. Then we perform the [analysis](/src/analyze.cpp). First we will take the type information generated by the parser, and generate our llvm types. Then we generate the [Simplang IR](#).
4. Then we convert the Simplang IR to LLVM IR or our own handrolled thing. 
5. Generate .o files.


## The Compiler

### lexer.cpp
[lexer.cpp](/src/lexer.cpp) implements the Lexer class defined within [lexer.h](/include/lexer.h). The Lexer class generates a std::vector<Token\*> from a src file, const char\*. It utilizes [tokenenums.h](/include/tokenenums.h) to define multiple hashmaps, allowing simple token generation. Deleteing the lexer deletes the the Token\*'s generated by it, so we delete the lexer at the same time as the parser.

### parser.cpp
#### Overview
[parser.cpp](/src/parser.cpp) implements the Parser class defined within [parser.h](/include/parser.h). The Parser class takes a std::vector<Token\*> (generated by Lexer), and transforms it into a Stage1Program\*.   This stage one program contains a list of all of the types needed by it, and an abstract syntax tree, whose nodes are defined in [statement.h](/include/statement.h). The structure of the types is defined in [typing.h](/include/typing.h). Deleteing the Parser doesn't delete the Stage1Program (the types and AST). The Stage1Program doesn't contain any Token\*'s, so the Lexer can now be safely deleted. By deleteing the base Scope* node of the AST, all of the other nodes are deleted (*it is a tree!*).    

#### Detailed look
A Parser object is a simple state-machine. The class is made up of a 2 functions of interest: `Token* Parser::fetchToken()`, and `Scope* Parser::currentScope()`. These contain the main functionality of the state-machine. `fetchToken()` iterates the token vector, and sets the `Token* Parser::currentToken` member to the *current token*. If we reach the end of the token vector, instead of returning `nullptr`, which we would need to test all of the time (there is no safe-navigation in c++), it instead returns a bogus "null token", whose category and type are both NULL. The `currentScope()` function returns the topmost Scope within the scope deque. With these two mechanisms we know where to get information and append new statements.


## Development Policies
My rules for programming are somewhat arbitrary and medieval, but they get the job done. 

Also note that the older code doesn't strictly conform to this, and that I'm actively working to fix that. I was learning to program as I was developing this project, but I can safely say I've made it past consious incompetence into concious competence (this relates to rule 22).

In no particular order:
1. camelCase for variables and functions, PascalCase for classes.
2. pass by `const T&` or whenever you can.
3. Explictly write out constructors and destructors, no `= default` crap!
4. Function implementations, (not declarations), must have a newline after the type, before the function name.
5. NEVER `using namespace` anywhere! zero tolerance!
6. Explicitly dynamic_cast<>()! ie; Do not return a `Scope*` in a `Statement*` function!
7. BSD/Allman style brackets. The Left Curly should always get its own line, as should the right curly. No exceptions!
8. For  simpleifs without an else, you can omit the curly-braces.
9. All enums prefixed with `k` followed by the enum's *type*, followed by an underscore and the name. 
10. use ranged based loops when possible so my screen doesn't get shit on.
11. an `int` is 2 bytes. AKA: use the `<cstdint>` always. 
12. Absolutely no macros! (The error handler is an exception to this rule)
13. No global or static variables. (error handler is an exception)
14. No templates!
15. Avoid inheritance. Just because C++ has OOP doesn't mean we need JAVA insanity up in here. Only use it where it is necessary.
16. Avoid `auto`. The only place where I will be caught using auto is when I'm working with the stdlib, because writing std::vector<Statement\*>::iterator is a major hassle!
17. Structs do not have member functions!
18. Avoid asserts, use proper error handling. This leads into 19:
19. No throwing exceptions. Feel free to try/catch exceptions, but do not throw them! Use the ErrorManager mechanism!
20. No `malloc()`/`calloc()`/`free()`, we use `new` and `delete`!
21. No shared/unique pointers! Just be smart about cleaning up your shit, don't expect mommy to wipe your ass for you!
22. Don't write code when you're tired, otherwise it will look like mine!
23. Use `#pragma once`. If you're using a compiler that isn't supporting this then I banish you back into the 80s. The `#ifndef` guards are for C-land. This is C++!