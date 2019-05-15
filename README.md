# Simplang
This is a programming language compiler(interpreter for now) that I'm writing.

.sl files are simplang files


Basic Types:

- `num`       Integer or Floating Point. Autocasting. compiler flag to force it to always check
- `string`
- `bool`

Specific Types
- `num8`      8 bits
- `num16`     16 bits
- `num32`     32 bits
- `num64`     64 bits
- `num128`    128 bits - support is questionable(use at your own risk!)
- `ptr`        A blank pointer. can be used like `void* x;` or `pointer number x;` 


Notes:

https://en.wikipedia.org/wiki/Operator-precedence_parser
https://llvm.org/docs/tutorial/index.html