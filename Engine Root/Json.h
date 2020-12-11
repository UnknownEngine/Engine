#pragma once

#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
class JsonArray;


struct json_array_t;
typedef json_array_t JSON_Array;

struct json_value_t;
typedef json_value_t JSON_Value;

struct json_object_t;
typedef json_object_t JSON_Object;

class JsonObj {
public:
	JsonObj(const char* buffer);
	JsonObj();
	JsonObj(JSON_Object* obj);
	~JsonObj();

	JSON_Object* GetObj();
	char** GetObjNames();
	JSON_Value* GetVal();

	JSON_Array* GetArray(const char* name);

	uint Save(char** buffer);
	
	//Getters
	int GetInt(const char* name);
	float GetFloat(const char* name);
	bool GetBool(const char* name);
	const char* GetString(const char* name);

	//Setters
	void AddInt(const char* name, int num);
	void AddFloat(const char* name, float num);
	void AddBool(const char* name, bool b);
	void AddString(const char* name, const char* string);
	JsonArray AddArray(const char* name);

private:
	JSON_Object* obj;
	JSON_Value* val;
};

class JsonArray {
public:
	JsonArray();
	JsonArray(JSON_Array* array);
	JsonArray(JSON_Array* array, JSON_Object* obj);
	JsonArray(const char* name);
	~JsonArray();

	int Size();
	JsonObj GetObjectInArray(const char* name);
	JsonObj GetObjectAt(int index);
	double GetDouble(int index);
	int GetInt(int index);
	float3 GetFloat3(int index);
	Quat GetQuaternion(int index);
	std::vector<int>GetUIDs(int index);

	void AddObject(JsonObj& object);
	void AddInt(int num);
	void AddFloat(float num);
	void AddString(const char* string);
	void AddQuaternion(float a, float x, float y, float z);
	void AddFloat3(float x, float y, float z);
	uint elements;
private:
	JSON_Array* array;
	JSON_Value* val;
	
};