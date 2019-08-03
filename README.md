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
- `numfp`     Equivalent to C++ long double.
- `ptr`       A blank pointer. can be used like `void* x;` or `ptr num x;`, however it's a little fancier...
- `void`	  Function that returns nothing
- `typename` You can store types in a typename variable. ie `typename T = numfp` You can then use the typename to create new variables, ie `typename T = numfp; T y = 0.7;`
 - `dynamic`  For functions, it dynamically changes the return type of the function depending on the type of the return value. With regard to variables, it behaves just like a c++ `auto`.
Note: 
It's still up to debate but for now there are no global/static variables.

### Operators:
##### Binary:
- `+` Add
- `-` Subtract
- `*` Multiply
- `/` Divide
- `^` Power
- `%` Modulo

##### Unary

- `++` increment (note: unlike C++ you <i><u>must</u></i> put the ++/-- <b>after</b> the identifier).
- `--` decrement
- `@` Pointer indirection (what's @ this location?)
- `&` What is the location of this.
- `-` make it negative (note: `+` cannot be used as unary operator instead use `abs()`)
- `!` not (`!true == false`, `!false == true`, etc...)


##### Assignment:
- `=` Equals
- `+=`  for instance `x += 2` is the same as `x = x + 2`
- `-=` Same story as `+=`
- `*=`
- `/=`
- `^=` 
- `%=` 

##### Comparison:
- `==` Equality
- `!=` not equal
- `<` less than
- `>` greater than
- `<=` less than or equal to
- `>=` greater than or equal to
- `&&` logical AND
- `||` logical OR

(these don't behave like in other languages. If you're looking for bitwise and/or, do the following `num x = 6 BITAND 1` or `num x = 6 BITOR 1`)
- `<>` between(see next)
- `&` instead of writing `x >= 0 && x <= 9` write `x <> 0 & 9`
- `|` instead of writing `x == 1 || x == 2` write `x == 1 | 2`

##### On Templates/Typing:
If you read up on the types, simplang has a type called `typename`. This is similar to templatization but in a more functional, dynamic way. You can write code such as:
```Simplang
dynamic giveMeZerosRepresentationInTypeX(typename x)
{
	x ans = null;
	if(x == num)
	{
		ans = 0;
	}
	else if(x == bool)
	{
		ans = (bool)0;
	}
	else if(x == string)
	{
		ans = "0";
	}

	return ans;
}
```



### Expression Evaluation:
Operator Precedence Parsing converts from infix notation to a pseudo postfix(reverse polish) notation.
This notation is evaluated by going from left to right, until you hit the first binary operator.
 Then you perform that operator upon the previous two terms. Then you keep going. Use flag `--pf` to print postfix notation of all statements as they're being parsed.


Notes:

https://en.wikipedia.org/wiki/Operator-precedence_parser
https://llvm.org/docs/tutorial/index.html

### Formatting:
All formating should be in the ALLMAN format. Emphasis on <i>should</i>. I'm a little bit of a hypocrite so hopefully most of my code follows it. Please don't submit pull requests to fix it. I'll worry about that later.