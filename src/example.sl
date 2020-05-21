unit main is private; //this is the "main" 'unit' of our 'program', and its members are 'private'
import std.io;


public struct 
MyStruct
{
	uint8, int16 : id, value;
};


enum 
Color
{
	Red,
	White,
	Blue
};


void 
myStructEditor(MyStruct* thingy)
{	
	thingy.id = 5;
	thingy.value = 12;
}

int, int 
returnsTwoInts()
{
	return 3, 4;
}

int, string 
returnsTwoTypes()
{
	return 3, "hello!";
}

int 
main(string[] argv)
{
	//Two ways to declare variables
  //int    : i1 = 5; // option 1
	i1 := 5;

	//there is also the single declaration
	//   int i = 7;
	//but you are only allowed to declare one variable
	// hence
	// 	int i = 6, x = 12; //won't compile!


	//	declare multiple vars of same type
	int(8) : i4, i5 = 4, i6; //the i5 = 4 overrides the default 8
	//the above is useful in the examples below

	//can assign to standard var-declaration list
	int : d1, x1, y1 = returnsTwoInts(); //assigns to x1, y1
	int, string : w1, s1 = returnsTwoTypes(); //handling multiple types
	x2, y2 := returnsTwoInts(); // the type is inferred
	x1, y1  = returnsTwoInts(); // purely assignment, no declaration
	//x1, y1 := returnsTwoInts(); //error! variable redeclaration!



	for(arg : argv)
	{

	}

	//this will also give you an iterator obj
	for(arg, it : argv) //alternatively: for(_, it : argv)
	{

	}

	//note that you can do this with enums if that's your kind of thing
	for(i : 0..argv.size()) //inclusive range with ..=
	{

	}



	string hello = "hello world";
	hello2 := "hello world";
	//note that string immediates in simplang are treated like the string type, not a byte array!

	//array - note that this will break the utf-32 characters up into bytes
	// similar to std::string::c_str()
	byte[] strbytes = hello.bytes();

	//for const char* folks:
	byte* g = &byte[0]; 




	//the three types of pointer
	//byte* x = new byte(0x12);
	byte* x = new byte(0x12);
	
	//std::unique_ptr<byte> y = std::make_unique<byte>(0xAB);
	unique byte y = new byte(0xAB); 
	
	//std::shared_ptr<byte> z = std::make_shared<byte>(0xFF);
	shared byte z = new byte(0xFF); 

	//alternatively you can use calloc()

	//allocations made with calloc vs new can both be freed with destroy
	destroy(x);

	//c-style array;
	//note that this is not a simplang array,
	x = new byte[25];

	//you can also access your c-style array like in c
	x[5] = 0xFF;
	//however you can do this on any arbitray pointer
	//	there is no safety, unlike a simplang array
	//	which functions more like a std::vector
	


	byte[] byteArr(x, 25); //if you are wrong about the size this is undefined behaviour, obviously

	destroy(x);

	int num;

	string msg = match(num)
	{
		2 || 3 || 5 || 7 || 11: "it's prime!" continue +=; //proposal
		5: "it's five!";
		_: "Default"
	};



	MyStruct* k = nullptr;
	w := k?.id; //safe navigation operator '?.'
	if(isNull(w)) //isNull operator
	{
		w = 42;
	}

	// in the above example 'w' is not a uint8, it is a uint8& hence:
		// uint8 w = k?.id; //will fail to compile
	//So then what is w? The := unwraps it into the following:

	uint8& b = @k?.id;

	// So why is it this way? The uint8 type is just a single byte in memory. Just like in C
	// 	there are no wrappers or objects to be found here. So there is no way for us to set it
	// 	to a null value.  But the refrence type adds a little overhead that takes care of this.
	//  as does the @ operator add some overhead over the & operator.
	
	// if you are willing to pay for it, use the refrence type and the @ operator.
	// the @ operator is null(ptr) safe @null = null. &null is undefined behavior.
	// remember, you only pay for what you use.

	if(isnull(b))
	{
		b = 69;
	}



















	std.print("Hello World!\n");

	return 0;
}






