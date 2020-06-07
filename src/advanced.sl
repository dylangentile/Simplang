//this document showcases some of the more advanced features and proposals of simplang.
// some of these features may not be included in the final language, and will be implemented last


//templates - inspired by 'D'
//think of this as a namespace, where the template params are specified
template Foo(T, U)
{
	T, T dupe(T : x)
	{
		return x, x;
	}

	struct Pair
	{
		T, U : first, second; 
	};


};



int main()
{
	Foo!(int32, string).Pair myPair;


}




//proposal 

class MyClass
{
//public by default, unlike c++

	private
	{
		MyClass()
		{

		}
		
		~MyClass()
		{

		}
	}


	int32 x;
	//getters and setters?


};