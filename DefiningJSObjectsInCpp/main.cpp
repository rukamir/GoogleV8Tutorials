#include <v8.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace v8;
using namespace std;

Handle<String> ReadFile(const char* name);

void MulCallback(const FunctionCallbackInfo<v8::Value>& args){
	// Grab the object calling this function and Get the X property
	// Cast this value to an INT then multiply it by the argument
	// -passed into the first param and return it to product
	int product = args.This()->Get(String::New("x"))->Int32Value() * args[0]->Int32Value();

	// Grab the object calling this function and assign its X property
	// -to the product
	args.This()->Set(String::New("x"), Number::New(product));

	// Grab the object calling this function and Get the Y property
	// Cast this value to an INT then multiply it by the argument
	// -passed into the first param
	product = args.This()->Get(String::New("y"))->Int32Value() * args[0]->Int32Value();

	// Grab the object calling this function and assign its Y property
	// -to the product
	args.This()->Set(String::New("y"), Number::New(product));
}

// Defines a Point() JS Object
void PointConstructor( const FunctionCallbackInfo<v8::Value>& args )
{
    HandleScope scope;

	// Create a new ObjectTemplete
	// This we will use to define our JS object
	Handle<ObjectTemplate> t = v8::ObjectTemplate::New();

	// Create x and y members with starting values of 0
	t->Set(String::New("x"), Number::New(0));
	t->Set(String::New("y"), Number::New(0));

	// Create a mul(number) function that scales the point
	t->Set(String::New("mul"), FunctionTemplate::New(MulCallback));

	// If Point(x, y) ctor was passed in values assign them
	if(!args[0].IsEmpty() && args[0]->IsNumber() &&
		!args[1].IsEmpty() && args[1]->IsNumber()) {
			t->Set(String::New("x"), args[0]);
			t->Set(String::New("y"), args[1]);
	}

	// Create and Return this newly created object
	args.GetReturnValue().Set(t->NewInstance());
}

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

int main()
{
	//Create the v8 environment
	Isolate *isolate = Isolate::GetCurrent();

	HandleScope handle_scope(isolate);

	Handle<ObjectTemplate> global_template = ObjectTemplate::New();

	// Tell global A JS Obj Point() can now be create and instructions on how to set up
	global_template->Set(String::New("Point"), FunctionTemplate::New(PointConstructor));

	Local<Context> context = Context::New(isolate, NULL, global_template);

	Context::Scope context_scope(context);

	Handle<v8::Object> global = context->Global();

	//global->Set(String::New("Point"), t);
	global->Set(v8::String::New("print"), v8::FunctionTemplate::New(Print)->GetFunction());

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
