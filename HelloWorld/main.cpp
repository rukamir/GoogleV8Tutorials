#include <v8.h>
#include <iostream>

using namespace v8;

int main(int argc, char* argv[])
{
	//Get the default Isolate created
	Isolate* isolate = Isolate::GetCurrent();

	//Create a stack-allocated handle scope
	HandleScope handle_scope(isolate);

	//Create a context
	Handle<Context> context = Context::New(isolate);

	//Example : Create a persistent handle
	Persistent<Context> persistent_context(isolate, context);

	//Use the created context to compile and run the script
	Context::Scope context_scope(context);

	//Create a string containing the JavaScript source code
	Handle<String> source = String::New("'Hello, World!'");

	//Compile the source code 
	Handle<Script> script = Script::Compile(source);

	//Run the script
	Handle<Value> result = script->Run();

	//Release the persistent handle
	persistent_context.Dispose();

	//Convert the result to string and print
	String::AsciiValue ascii(result);
	printf("%s\n", *ascii);

	std::cin.get();

	return 0;
}