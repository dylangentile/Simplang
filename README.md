# Simplang
This is a programming language compiler(interpreter for now) that I'm writing.

.sl files are simplang files

### Types:

Basic Types:

- `num`       Depends on value. If initialized with integer, it will be a `num64`. If intialized with floating point, it'll be a `numfp`.
- `string`
- `bool`

Specific Types
- `num8`      8 bits
- `num16`     16 bits
- `num32`     32 bits
- `num64`     64 bits
- `numfp`     Equivalent to C++ `long double` on your machine.
- `ptr`       A blank pointer. can be used like C's `void* x;`(ie `ptr x`) or `ptr num x;`, however it's a little fancier(I hope). See Notes::Pointers for my reasons behind doing it like this.
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
- `@` Pointer indirection (what's @ this location?). Similar to C's derefrence, ie `@x` is the same as C's `*x`.
- `&` What is the location of this.
- `-` make it negative (note: `+` cannot be used as unary operator instead use `abs()`)
- `!` Logical Negation. NOT (`!true == false`, `!false == true`, etc...)


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

(these don't behave like in other languages. or exist)
- `[>` inbetween. instead of writing `x > 0 && x < 9` write `x [> 0 & 9`
- `<>` `[>` or equal to bounds. `x <> 0 & 9` is the same as `x [> 0 & 9 && x == 0 | 9`
- `&` instead of writing `x >= 0 && x <= 9` write `x <> 0 & 9`
- `|` instead of writing `x == 1 || x == 2` write `x == 1 | 2`

##### Bitwise Operations:
- `compl` same as C's `~`. Bitwise NOT
- `bitand` same as C's `&`. Bitwise AND
- `bitor` same as C's `|` . Bitwise OR
- `shiftl` same as C's `<<`. Bitwise left shift
- `shiftr` same as C's `>>`. Bitwise right shift


### Notes:

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

##### On dynamic typing:
I'm debating implementing a `var` type. I really don't want to. I really can't think of any cases except for lazyness where you need such loose typing(if you can even call it that). It would also cause issues with compilation, since I've already got my hands full implementing dynamic functions, and at that point it would just end up becoming a bytecode language. (which I detest. 'Modern' languages don't compile to machine code so much anymore, and I'd like to be a modern, higher-level language, that can compromise, with pointer manipulation, and compilation.)

##### On Pointers:
Why do pointers with the keyword `ptr`, and use `@` for pointer indirection. On the former:I think that the C-style way of doing it is fine, but I like how this reads better, and I'd also like to treat pointers more like a specific type. That is, in C, when I write `int* x, y, z;`, `x` will be a pointer but `y` and `z` will not. I'd rather have `int*` be the type, and therefore `x`,`y`, and `z` all become pointers. Therefore in Simplang `ptr num32 x, y, z;` will make `x`,`y`, and `z` all pointers. I also think that using the ptr keyword disambiguates the definiton of `*` by having it not do double duty. Which leads right into my next point: `*` means both multiplication, and pointer indirection. I'd rather it just mean one. I also think that the `@` sign is the perfect replacement, since we all call it the 'at' sign, learning and understanding the code will be easier, since one can say, 'what's <i>at</i>i> this memory location'. Also, the at symbol isn't used in any other useful programming languages, therefore there's no need to worry about a prexisting meaning. (unless you come from Objective-C. I pity you then.)

##### On Garbage:
This language will be garbage collected. If it's to be high level and modern, it needs to cleanup after itself. This is to decrease developer suicide rates due to memory leaks, and give the Valgrind team a break. Maybe I'll just have the target platforms be [missiles](https://groups.google.com/forum/message/raw?msg=comp.lang.ada/E9bNCvDQ12k/1tezW24ZxdAJ). The first step is for me to learn how to best pick up the garbage. Luckily I've got the [authoritative handbook](http://gchandbook.org/).

##### Including files:
I have zero ideas. Perhaps header files that declare header files. Therefore you declare things twice...? ;)

#####  On Expression Evaluation:
Operator Precedence Parsing converts from infix notation to a pseudo postfix(reverse polish) notation.
This notation is evaluated by going from left to right, until you hit the first binary operator.
Then you perform that operator upon the previous two terms. Then you keep going. 


### Formatting:
All formating should be in the ALLMAN format. Emphasis on <i>should</i>. I'm a little bit of a hypocrite so hopefully most of my code follows it. Please don't submit pull requests to fix it. I'll worry about that later.