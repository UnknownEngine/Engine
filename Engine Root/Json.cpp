#include "Json.h"
#include "JSON/parson.h"
#include "JSON/parson.c"
#include "Globals.h"

JsonObj::JsonObj(const char* buffer) : obj(nullptr)
{
	val = json_parse_string(buffer);
	
	if (val != NULL)
	{
		obj = json_value_get_object(val);
		LOG("File Loaded");
	}
	else
	{
		LOG("Error loading config");
	}
}

JsonObj::JsonObj()
{
	val = json_value_init_object();
	obj = json_value_get_object(val);
}

JsonObj::JsonObj(JSON_Object* obj)
{
	this->obj = obj;
}

JsonObj::~JsonObj()
{
	obj = nullptr;
	val = nullptr;
}

JSON_Object* JsonObj::GetObj()
{
	return obj;
}

char** JsonObj::GetObjNames()
{
	return obj->names;
}


JSON_Value* JsonObj::GetVal()
{
	return val;
}

JSON_Array* JsonObj::GetArray(const char* name)
{
	if (json_object_has_value(obj, name) == 1)
		return json_object_get_array(obj, name);
	else
		return nullptr;
}

uint JsonObj::Save(char** buffer)
{
	uint size = json_serialization_size_pretty(val);
	*buffer = new char[size];
	json_serialize_to_buffer_pretty(val, *buffer, size);

	return size;
}



int JsonObj::GetInt(const char* name)
{
	if (json_object_has_value(obj, name) == 1)
		return json_object_get_number(obj, name);
	else
		return 0;
}

float JsonObj::GetFloat(const char* name)
{
	if (json_object_has_value(obj, name) == 1)
		return json_object_get_number(obj, name);
	else
		return 0.f;
}

bool JsonObj::GetBool(const char* name)
{
	if (json_object_has_value(obj, name) == 1)
		return json_object_get_boolean(obj, name);
	else
		return false;
}

const char* JsonObj::GetString(const char* name)
{
	if (json_object_has_value(obj, name) == 1)
		return json_object_get_string(obj, name);
	else
		return "null";
}

void JsonObj::AddInt(const char* name, int num)
{
	json_object_set_number(obj, name, num);
}

void JsonObj::AddFloat(const char* name, float num)
{
	json_object_set_number(obj, name, num);
}

void JsonObj::AddBool(const char* name, bool b)
{
	json_object_set_boolean(obj, name, b);
}

void JsonObj::AddString(const char* name, const char* string)
{
	json_object_set_string(obj, name, string);
}

JsonArray JsonObj::AddArray(const char* name)
{
	JSON_Value* value = json_value_init_array();
	JSON_Status status = json_object_set_value(obj, name, value);

	if (status == JSONFailure)
	{
		json_value_free(value);
	}
	else
	{
		return JsonArray(json_object_get_array(obj, name));
	}
}


JsonArray::JsonArray(){
	this->array = nullptr;
	val = json_value_init_array();
	json_value_get_array(val);
}

JsonArray::JsonArray(JSON_Array* array)
{
	this->array = array;
}

JsonArray::JsonArray(JSON_Array* array, JSON_Object* obj)
{
	this->array = array;
}

JsonArray::JsonArray(const char* name)
{
	JSON_Value* value = json_value_init_array();
	json_value_get_array(value);
}

JsonArray::~JsonArray()
{	
	array = nullptr;
}

int JsonArray::Size()
{
	return json_array_get_count(array);
}

JsonObj JsonArray::GetObjectInArray(const char* name)
{
	for (size_t i = 0; i < json_array_get_count(array); i++)
	{
		JSON_Object* object = json_array_get_object(array, i);
		const char* object_name = json_object_get_string(object, "name");

		if (strcmp(name, object_name) == 0)
			return JsonObj(object);
	}

	LOG("JSON object %s not found", name);
}

JsonObj JsonArray::GetObjectAt(int index)
{
	JSON_Object* object = json_array_get_object(array, index);
	return JsonObj(object);
}

double JsonArray::GetDouble(int index)
{
	return json_array_get_number(array, index);
}

int JsonArray::GetInt(int index)
{
	return json_array_get_number(array, index);
}

float3 JsonArray::GetFloat3(int index)
{
	float3 ret;

	ret.x = GetDouble(index);
	ret.y = GetDouble(index+1);
	ret.z = GetDouble(index+2);

	return ret;
}

Quat JsonArray::GetQuaternion(int index)
{
	Quat ret;

	ret.w= GetDouble(index);
	ret.x= GetDouble(index+1);
	ret.y= GetDouble(index+2);
	ret.z= GetDouble(index+3);

	return ret;

}

std::vector<int> JsonArray::GetUIDs(int index)
{
	std::vector<int> UIDs;
	for (index; index < elements; index++)
	{
		UIDs.push_back(GetInt(index));
	}
	return UIDs;
}

void JsonArray::AddObject(JsonObj& object)
{
	json_array_append_value(array, object.GetVal());
	elements++;
}

void JsonArray::AddInt(int num)
{
	json_array_append_number(array, num);
	elements++;
}

void JsonArray::AddFloat(float num)
{
	json_array_append_number(array, num);
	elements++;
}

void JsonArray::AddString(const char* string)
{
	json_array_append_string(array, string);
	elements++;
}

void JsonArray::AddQuaternion(float a, float x, float y, float z)
{
	json_array_append_number(array, a);
	json_array_append_number(array, x);
	json_array_append_number(array, y);
	json_array_append_number(array, z);
	elements += 4;
}

void JsonArray::AddFloat3(float x, float y, float z)
{
	json_array_append_number(array, x);
	json_array_append_number(array, y);
	json_array_append_number(array, z);
	elements +=3;
}
