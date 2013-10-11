#include <v8.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace v8;
using namespace std;

Handle<String> ReadFile(const char* name);

class Point
{
public: 
	Point(int x, int y) : x_(x), y_(y) {}
	int x_, y_;
};

void GetPointX(Local<String> property,
						const PropertyCallbackInfo<Value>& info)
{
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	int value = static_cast<Point*>(ptr)->x_;
	info.GetReturnValue().Set(Number::New(value));
}

void SetPointX(Local<String> property, 
			   Local<Value> value,
			   const PropertyCallbackInfo<Value>& info)
{
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	static_cast<Point*>(ptr)->x_ = value->Int32Value();
}

void GetPointY(Local<String> property,
						const PropertyCallbackInfo<Value>& info)
{
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	int value = static_cast<Point*>(ptr)->y_;
	info.GetReturnValue().Set(Number::New(value));
}

void SetPointY(Local<String> property, 
			   Local<Value> value,
			   const PropertyCallbackInfo<Value>& info)
{
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	static_cast<Point*>(ptr)->y_ = value->Int32Value();
}

int main()
{
	//Create the v8 environment
	Isolate* isolate = Isolate::GetCurrent();

	HandleScope handle_scope(isolate);

	Handle<ObjectTemplate> global_template = ObjectTemplate::New();

	Local<Context> context = Context::New(isolate, NULL, global_template);

	Context::Scope context_scope(context);

	/**
	 * Set up the template
	 */
	Handle<ObjectTemplate> point_template = ObjectTemplate::New();

	//The JavaScript point object only has 1 C++ object
	point_template->SetInternalFieldCount(1);

	//Set the accessors
	point_template->SetAccessor(String::New("x"), 
		(AccessorGetterCallback)GetPointX,
		(AccessorSetterCallback)SetPointX);

	point_template->SetAccessor(String::New("y"),
		(AccessorGetterCallback)GetPointY,
		(AccessorSetterCallback)SetPointY);


	/**
	 * Wrap a point object
	 */
	Point* p = new Point(0, 0);
	Local<Object> obj = point_template->NewInstance();
	obj->SetInternalField(0, External::New(p));

	/**
	 * Set up the movement scripts
	 */

	//Up
	Handle<String> source = ReadFile("up.js");

	if (source.IsEmpty())
	{
		cout << "Error reading file" << endl;
		cout << "Press enter to quit" << endl;
		cin.get();
		return 0;
	}

	Handle<Script> upScript = Script::Compile(source);

	//Left
	source = ReadFile("left.js");

	if (source.IsEmpty())
	{
		cout << "Error reading file" << endl;
		cout << "Press enter to quit" << endl;
		cin.get();
		return 0;
	}

	Handle<Script> leftScript = Script::Compile(source);

	//Down
	source = ReadFile("down.js");

	if (source.IsEmpty())
	{
		cout << "Error reading file" << endl;
		cout << "Press enter to quit" << endl;
		cin.get();
		return 0;
	}

	Handle<Script> downScript = Script::Compile(source);

	//Right
	source = ReadFile("right.js");

	if (source.IsEmpty())
	{
		cout << "Error reading file" << endl;
		cout << "Press enter to quit" << endl;
		cin.get();
		return 0;
	}

	Handle<Script> rightScript = Script::Compile(source);

	/**
	 * Move a the point using WASD
	 */

	string input;

	cout << "Use W, A, S, D to move the point from the origin" << endl;

	cin >> input;

	while (input != "exit")
	{
		if (input == "w" || input == "W")
		{
			cout << "Move forward" << endl;
			upScript->Run();
		}

		if (input == "a" || input == "A")
		{
			cout << "Move left" << endl;
			leftScript->Run();
		}

		if (input == "s" || input == "S")
		{
			cout << "Move right" << endl;
			rightScript->Run();
		}

		if (input == "d" || input == "D")
		{
			cout << "Move back" << endl;
			downScript->Run();
		}

		cout << "Point : x = " << p->x_ << " , y = " << p->y_ << endl;

		cin >> input;
	}

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
