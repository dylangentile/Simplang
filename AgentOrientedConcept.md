# Simplang Language Specification v0.1.0-alpha AGENT ORIENTED CONCEPTs

Simplang is a agent-oriented programming language that uses a structure ownership model.


## Struct Ownership Model


## Language Constructs

### Functions

A basic function declaraction takes the form of:
```
type function_name(type arg1, arg2) steals type as name
{
	function body
	return type;
}

```

Functions in Simplang do not have side-effects except on the variable they steal. Variables/Structs are always passed by copy, or const refrence(c++ `const int& x`) depending on what the compiler deduces.

Functions can also steal an object from an agent, its caller. (the main function is also technically an agent, but it cannot be stolen from, instead you must write an explicit steal)


#### Explicit steals



### Structs




## EXAMPLE SRC CODE

```Simplang
struct ProgramData 
{
	int8 x;
	int16 y;
	str z;
};


int8 add() steals ProgramData as data //by defualt it steals as 'this'.
{
	data.y = data.x + data.y;
	data.z = (str)data.y;
	return data.x;
}


agent ProgDataAgent
{
	uses add; //the functions it uses
	owns ProgramData; //the struct it uses
	waits std; //how it recieves messages (standard in this case)
};



int8 main()
{
	ProgramData theData;
	theData.x = 5;
	theData.y = 6;
	theData.str = "asdf";

	ProgramDataAgent myAgent = create(ProgDataAgent); //create an agent
	myAgent.steals(theData); //give it theData, passes by move, theData popped off of stack


	myAgent.signal(add); // signal/message the agent to perform the add function.
	theData = myAgent.returns(ProgramData);
	myAgent.exit(); //quits the agent.

	int8 ans = steal(theData)->add(); //an explicit steal

	return 0;
}
```