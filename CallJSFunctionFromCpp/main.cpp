/**
 * COMPILE foo.js AT THE FIRST COMMAND PROMPT TO RUN foo.js
 */


#include <v8.h>
#include <iostream>
#include <fstream>
#include <string>


using namespace v8;
using namespace std;

v8::Handle<v8::String> ReadFile(const char* name);

//*******************************
//        My helpers
//*******************************
/**
* Add[DataType]ToArguments(string/double/bool, Handle<Value>, UINT)
* / [datatype] value to assign to argument list
* / pass in arguments handle
* / position in argument list to
* This function will eaily convert and set the values for an argument list
* to easily pass into a JS function you are calling from C++
* JSEx: Func(arg[0], arg[1], ..)
**/
void AddStringToArguments(std::string str, Handle<Value> argList[], unsigned int argPos){
	argList[argPos] = v8::String::New(str.c_str());
}
void AddNumberToArguments(double num, Handle<Value> argList[], unsigned int argPos){
	argList[argPos] = v8::Number::New(num);
}
void AddBooleanToArguments(bool value, Handle<Value> argList[], unsigned int argPos){
	argList[argPos] = v8::Boolean::New(value);
}
// Examples of these pass in the Isolite instead of global and create global within shell.cc for example line 99
/**
* CallJSFunction(Handle<v8::Object>, string, Handle<Value>, UINT)
* / Handle of the global that is running the script with desired function
* / Title of the JS fuction
* / List of arguments for function
* / Number of agrguments
* Returns the return value of the JS function
**/
Handle<v8::Value> CallJSFunction(Handle<v8::Object> global, std::string funcName, Handle<Value> argList[], unsigned int argCount){
	// Create value for the return of the JS function
	Handle<Value> js_result;
	// Grab JS function out of file
	Handle<v8::Value> value = global->Get(String::New(funcName.c_str()));
	// Cast value to v8::Function
	Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
	// Call function with all set values
	js_result = func->Call(global, argCount, argList);
	// Return value from function
	return js_result;
}

int main()
{
	//Get the default Isolate
	Isolate* isolate = Isolate::GetCurrent();

	//Create a stack allocated handle scope
	HandleScope handle_scope(isolate);

	//Handle<Value> init = Integer::New(x);

	//Create the global template
	Handle<ObjectTemplate> global_template = ObjectTemplate::New();

	//Create a context
	Local<Context> context = Context::New(isolate, NULL, global_template);

	//Set the context scope
	Context::Scope context_scope(context);

	Handle<v8::Object> global = context->Global();

	string file = "foo.js";

	while(true){
		cout << "How many times do you want to run the script? \n" << endl;

		int n; 

		cin >> n;

		cout << "" << endl;

		std::cin.get();

		Handle<String> source = ReadFile(file.c_str());

		if(source.IsEmpty())
		{
			cout << "Error reading file" << endl;
			cout << "Press enter to quit" << endl;
			cin.get();
			return 0;
		}

		//Compile
		Handle<Script> script = Script::Compile(source);
	
		//Run the script and print 
		Handle<Value> result;

		result = script->Run();

		// Once script has ran lets call some Functions!!******************
		// Create handle for arguements
		Handle<Value> args[2];

		// Create arguments to be passed into JS function
		AddStringToArguments("BOSS", args, 0);
		AddNumberToArguments(5.0, args, 1);

		// Call the JS function
		Handle<Value> js_result = CallJSFunction(global, "JSrepeat", args, 2);
		String::AsciiValue ascii2(js_result);
		printf("JSrepeat() returned: %s\n", ascii2);

		// Lets try another JS fucntion call!!*****************************
		// This function returns the name "Jimmy" with no parameters
		js_result = CallJSFunction(global, "WhatsMyName", NULL, 0);
		String::AsciiValue ascii3(js_result);
		printf("WhatsMyName() returned: %s\n", ascii3);


		String::AsciiValue ascii(result);
		cout << "Script result : " ;
		printf("%s\n", *ascii);
	} // End of while

	//Exit program
	cout << "\nTest completed.  Press enter to exit program. \n" << endl;
	std::cin.get();
	
	return 0;
}

v8::Handle<String> ReadFile(const char* name)
{
	//Open the file
	FILE* file;
	fopen_s(&file, name, "rb");

	//If there is no file, return an empty string
	if (file == NULL) return v8::Handle<v8::String>();

	//Set the pointer to the end of the file
	fseek(file, 0, SEEK_END);

	//Get the size of file
	int size = ftell(file);

	//Rewind the pointer to the beginning of the stream
	rewind(file);

	//Set up and read into the buffer
	char* chars = new char[size + 1];
	chars[size] = '\0';
	for (int i = 0 ; i < size;)
	{
		int read = static_cast<int>(fread(&chars[i], 1, size - i, file));
		i += read;
	}

	//Close file
	fclose(file);

	v8::Handle<v8::String> result = v8::String::New(chars, size);
	delete[] chars;
	return result;
}
