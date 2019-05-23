# Simplang
This is a programming language compiler(interpreter for now) that I'm writing.

.sl files are simplang files

### Types:

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
- `ptr`        A blank pointer. can be used like `void* x;` or `ptr num x;` 

###Operators:
#####Binary:
- `+` Add
- `-` Subtract
- `*` Multiply
- `/` Divide
- `^` Power
- `%` Modulo

#####Unary

- `++` increment (note: unlike C++ you <i><u>must</u></i> put the ++/-- <b>after</b> the identifier).
- `--` decrement
- `@` Pointer derefrence(what's @ this location?)
- `&` What is th location of this.
- `-` make it negative (note: `+` cannot be used as unary operator instead use `abs()`)

#####Assignment:
- `=` Equals
- `+=`  for instance `x += 2` is the same as `x = x + 2`
- `-=` Same story as `+=`
- `*=`
- `/=`
- `^=` 
- Note: there is no `%=` because it seems useless/unecessary for most applications.


###Expression Evaluation:
Operator Precedence Parsing converts from infix notation to a pseudo postfix(reverse polish) notation.
This notation is evaluated by going from left to right, until you hit the first binary operator.
 Then you perform that operator upon the previous two terms. Then you keep going. Use flag `--pf` to print postfix notation of all statements as they're being parsed.


Notes:

https://en.wikipedia.org/wiki/Operator-precedence_parser
https://llvm.org/docs/tutorial/index.html

### Formatting:
All formating should be in the ALLMAN format. Emphasis on <i>should</i>. I'm a little bit of a hypocrite so hopefully most of my code follows it. Please don't submit pull requests to fix it. I'll worry about that later.