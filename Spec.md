# Simplang Language Specification v0.0.1-alpha

This document will change and will introduce breaking behaviour from one version to the next. This project loosely follows semantic versioning, and as such until we reach v1.0.0 there will be no backward compatiblity guarentees. From v1.0.0 onward there should be complete backward compatibility unless a subsequent v2.0.0 is developed. (Sorta like Python2 vs Python3, etc, as opposed to C++11 vs C++17).


## Type System
Simplang is statically typed. The basic types are:
	
	`byte`, `int`, `float` `bool`, `string`

Along with the pointer types (where `T` is a type)

	`T*`, `unique T*`, `shared T*`, `fancy T*`

The unique pointer and shared pointer behave as you might expect. By default all types imply mutability. (except certain restrictions from unique and shared pointers) `const<T>` can be used to specify immutability.

Simplang allows user defined types in the form of structs:

```
struct MyStruct
{
	int x;
	string y;
};
```

Simplang structs do not inherit, they are not polymorphic, and cannot have any member functions.
All members of structs are public w/i the module.

That is, `struct MyStruct : private` means that the struct is private to other modules, but its members all still public


### Detailed types:
Note that the parser always substitutes `int` with `int32`

- int8, int16, int32, int64
- uint8, uint16, uint32, uint64
- float, double
- string
- byte, char
- bool
- void


### On const

const<T\*> x;
const<T>*  z;
const_twice<T\*> y; //const T const* 

## Variables
In Simplang variables are defined by a type, and a symbol, in the form of:

	`T S;` `byte theByte;` `MyStruct mine;`...etc
or in the advanced form

	`T1, T2 : S1, S2;`

with n types and n symbols

If there are less types than symbols, all of the later symbols will get the last type. This is not reccomended usage, execpt for when there is one type.

	`T1, T2, T3 : S1, S2, S3, S4, S5;`

For now valid variable symbols include A-Z, a-z, 0-9, and \_. I plan on extending this to unicode but for now this is fine.


### Variable Initialization
Every Basic type has a defined default intialization, and uninitialized variables are non-conforming to the standard since the performance benefits are negligible. (That doesn't mean that you can't pass `--no-init` to the compiler if you value that speed)

The default initializations are as follows

For any numeric type (int8-int64, uint8-uint64, float, double, byte), it must default intialize to zero, `0`.
Char is by default U+0000 or '\0'.

string is `""`.
bool is `false`
raw pointers are `nullptr` //however the compiler will give a warning for uninitialized pointers! Just because you can doesn't mean you should
fancy, shared, and unique pointers must be explicitly intialized!

#### Struct Intialization
In a struct you can intialize in place:
```
struct MyStruct
{
	int x = 5;
};
```
or use a constructor:
```
struct MyStruct
{
	Mystruct()
	{
		x = 5;
	}

	int x = 7; //assignment ignored
};
```

If the struct has a constructor with arguements defined, and the constructor is tagged `only`:
```
struct MyStruct
{
	Mystruct(int z) only
	{
		x = z;
	}

	int x = 7; //assignment will be ignored, no performance loss
};
```
Then it cannot be default intialized, since the `only` way to do so is with the constructor. If you drop the `only` tag then the default intialization is allowed and will be attempted, following the rules of assignments within the struct ie:
```
struct MyStruct
{
	Mystruct(int z)
	{
		x = z;
	}

	int x = 7; //assignment will be ignored, no performance loss
};

int main()
{
	MyStruct(12) : a;
	MyStruct : b;
	a.x == 12;//true
	b.x == 7; //true
	return 0;
}
```


You are not allowed to override functions in Simplang, and as such you cannot override constructors of structs. The language also supplies move and copy semantics for structs so unlike c++ you don't write move and copy constructors this way (it's different, and most of the time you don't have to).

Do note that you can have two constructors following the example from above by using the assignment intialization inline as the default, and the actual constructor for passing certain parameters.


If your struct contains a unique pointer or a smart pointer you must supply a constructor tagged `only` that initializes them.


## Operators
### Assignment
- '='  '=:'


### Arithmetic
- '+'  '-'  '\*'  '/'  '%'
- '+=' '-=' '\*=' '/=' '%='
- '++' '--'

### Logic
- '==' '<'  '>'  '!'
- '!=' '<=' '>='
- '&&' '||' 



## Scopes
Although Simplang doesn't allow overriding of functions within a single scope, you can override/declare functions, structs, and variables in each scope:

```
int add(int, int : x, y)
{
	return x + y;
}

int main()
{
	int : k = 5;
	

	k = add(5, 10); //resolves to int = int add(int, int)

	{
		double add(double, double : x, y)
		{
			return x + y;
		}
		//"overrides" k from outer scope
		double : k = add(7.9 + 3.4); //resolves to double double add(double, double) from this scope
	}
}

```


Basically, there can only exist one use of a symbol per scope, and a symbol can override a symbol of an outer scope. This comes with a caveat that functions can only override functions, variables can only override variables, and structs can only override structs, as to minimize headaches and confusion.

The way the compiler finds the appropriate override is by hiking back up the stack. Each time it enters a new scope, it pushes the scope onto the stack, so if the symbol isn't in the current scope, the compiler will look in the previous, and keep working backward until it reaches the global scope.


## Memory
It is reccomended to use `unique T*` and `shared T*` wherever possible, since they clean themselves up using simple refrence counting (Meaning there are still possible bugs...). Simplang doesn't implement garbage collection, nor is it planned to, so proceed at your own risk. 


### "Fancy Pointers"
One class "owns" the fancy pointer, and this class is responsible for destroying it. Other classes get a fancy "ref" wrapper. "ref" wrappers cannot be free-ed, and will be converted to null when the fancy pointer is free-ed. The `&` and `.` operators cannot be used with fancy refs. Instead use `@` and `?.`


## In Contention

### Active Proposals
- `defer`/`panic` from golang
- char - utf8 support

### Active debates
- struct method functions vs golang interfaces? (definitely not Rust traits(those suck!))
- private members of struct and friend functions?



### "typename" and "dynamic"
These keywords are reserved. I do not plan on implementing `dynamic` for now, but it's a possibility. `typename` will exist for the Generics system, and for the reflection system.






















