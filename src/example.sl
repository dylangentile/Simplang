
struct MyStruct
{
	uint8 id;
	int16 value;

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

int main(string[] argv)
{

	x, y := returnsTwoInts();

	for(arg : argv)
	{

	}

	for(arg, it : argv)
	{

	}

	for(int i : 1..argv.size()) //inclusive range with ..=
	{

	}

	for(int i = 0; i < argv.size(); i++)
	{

	}

	string hello = "hello world";

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



