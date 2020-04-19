
struct MyStruct
{
	uint8, int16 : id, value;
};


void myStructEditor(MyStruct* thingy)
{
	thingy.id = 5;
	thingy.value = 12;
}

int, int returnsTwoInts()
{
	return 3, 4;
}

int, string returnsTwoTypes()
{
	return 3, "hello!";
}

int main(string[] argv)
{
	//two ways to declare variables:
	//	c-style
	int i1, i2 = 4, i3;
	//	variable-type style
	int, int, int : i4, i5 = 4, i6;
	//the above is useful in the examples below

	//can assign to standard var-declaration list
	int d1, x1, y1 = returnsTwoInts(); //assigns to x1, y1
	//int w1, string s1 = returnsTwoTypes(); //error string cannot be used as variable name... etc
	int, string : w1, s1 = returnsTwoTypes(); //handling multiple types
	x2, y2 := returnsTwoInts(); // the type is inferred
	x1, y1 = returnsTwoInts(); // purely assignment, no declaration
	//x1, y1 := returnsTwoInts(); //error! variable redeclaration!



	for(arg : argv)
	{

	}

	//this will also give you an iterator obj
	for(arg, it : argv)
	{

	}

	for(int i : 0..argv.size()) //inclusive range with ..=
	{

	}

	//c-style (declaration; halting condition; increment)
	for(int i = 0; i < argv.size(); i++)
	{

	}

	string hello = "hello world";
	hello2 := "hello world"; //essentially auto hello2 = string('hello world')
	//note that string immediates in simplang are treated like the string type, not a byte array!

	//array
	byte[] strbytes = hello.bytes();

	//for const char* folks:
	byte* g = &byte[0];  //byte* x 


	//the three types of pointer and allocations
	//byte* x = new byte(0x12);
	byte* x = make(byte(0x12));
	
	//std::unique_ptr<byte> y = std::make_unique<byte>(0xAB);
	unique byte y = make(byte(0xAB)); 
	
	//std::shared_ptr<byte> z = make_shared<byte>(0xFF);
	shared byte z = make(byte(0xFF)); 


	destroy(x);

	//c-style array;
	//note that this is not a simplang array,
	x = make(byte)[25];

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
		5: "it's five!",
		2 || 3 || 5 || 7 || 11: "it's prime!",
		_: "Default"
	};

	return 0;
}



