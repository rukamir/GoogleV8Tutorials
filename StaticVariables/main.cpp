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

static int x = 0;

/**
 * XGetter - Gets the x value when the script needs to access it
 *
 * Either function body will work
 */
void XGetter (Local<String> property,
					  const PropertyCallbackInfo<Value> &info)
{
	//int* value = static_cast<int*>(Handle<External>::Cast(info.Data())->Value());
	//info.GetReturnValue().Set(Number::New(*value));

	info.GetReturnValue().Set(Number::New(x));
}

/**
 * XSetter - Sets the x value to the x result from the script
 *
 * Either function body will work
 */
void XSetter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info)
{
	//int* field = static_cast<int*>(Handle<External>::Cast(info.Data())->Value());
	//*field = value->Int32Value();

	x = value->Int32Value();
}

void Add(const v8::FunctionCallbackInfo<Value>& args)
{
	float ret = 0;
	for (int i = 0 ; i < 2 ; i++)
	{
		ret += args[i]->NumberValue();
	}
	args.GetReturnValue().Set(v8::Number::New(ret));
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

	global_template->SetAccessor(String::New("x"), (AccessorGetterCallback)XGetter, (AccessorSetterCallback)XSetter); //, External::New(&x));
	global_template->Set(String::New("add"), FunctionTemplate::New(Add));

	//Create a context
	Local<Context> context = Context::New(isolate, NULL, global_template);

	//Set the context scope
	Context::Scope context_scope(context);

	/**
	 * Try running the script
	 */

	/**
	 * Static variable script
	 */
	//Get user input
	cout << "Enter the script you wish to run (ex: foo.js) " << endl;
	string file;
	getline(cin, file);
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

	cout << "C++ origin : " << x << endl;

	cout << "How many times do you want to run the script? \n" << endl;

	int n; 

	cin >> n;

	cout << "" << endl;

	std::cin.get();

	for ( int i = 0 ; i < n ; i++)
	{
		result = script->Run();

		String::AsciiValue ascii(result);
		cout << "Script result : " ;
		printf("%s\n", *ascii);
		cout << "C++ result : " << x << endl;
		std::cin.get();
	}

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
