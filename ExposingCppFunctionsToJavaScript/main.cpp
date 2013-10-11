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

/*
* JS Function: print(string, ...)
* /a string value to be printed
*/
void Print(const v8::FunctionCallbackInfo<v8::Value>& args) {
	std::cout << "Print worked= ";
	
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope handle_scope(args.GetIsolate());
		v8::String::Utf8Value str(args[i]);
		string myStr = *(str);
		const char* cstr = myStr.c_str();
		printf("%s", cstr);
	}
	printf("\n", NULL);
}

/*
* JS Function: repeat(string, number)
* /a string value to be repeated
* /number of times you want to repeat
*/
void Repeat(const v8::FunctionCallbackInfo<v8::Value>& args) {
	std::cout << "Repeat worked= ";
	string myStr;
	//const char* cstr;
	int count = args[1]->Int32Value();

	for (int i = 0; i < count; i++) {
		v8::HandleScope handle_scope(args.GetIsolate());
		v8::String::Utf8Value str(args[0]);
		myStr += *(str);
	}
	const char* cstr = myStr.c_str();
	printf("%s", cstr);
	printf("\n", NULL);
}

/*
* JS Function: myadd(number, ...)
* /a number value to be added
* Calculates the sum of all numbers entered
*/
void Add(const v8::FunctionCallbackInfo<v8::Value>& args) {
	std::cout << "Add worked= ";
	int myVal = 0;
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope handle_scope(args.GetIsolate());
		myVal += args[i]->Int32Value();
	}
	printf("%i\n", myVal);
	args.GetReturnValue().Set(v8::Number::New(myVal));
}

//*******************************
//        My helpers
//*******************************
/**
* Collection of C++ functions created in the JS enviornment
**/
void CreateFunctionsForJS(Handle<v8::Object> global){
	global->Set(v8::String::New("print"), v8::FunctionTemplate::New(Print)->GetFunction());
	global->Set(v8::String::New("repeat"), v8::FunctionTemplate::New(Repeat)->GetFunction());
	global->Set(v8::String::New("myadd"), v8::FunctionTemplate::New(Add)->GetFunction());
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
CreateFunctionsForJS(global);

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
	
	//Run the script 3 times and print 
	Handle<Value> result;

	//for ( int i = 0 ; i < n ; i++)
	//{
		result = script->Run();
		String::AsciiValue ascii(result);
		cout << "Script result : " ;
		printf("%s\n", *ascii);
		//std::cin.get();
	//}
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