/*

My Macros
UPDATED: 2025-07-17

includes:

DECLARE_PROPERTY(TYPE, NAME)
DECLARE_PROPERTY_DEFAULT(TYPE, NAME, DEFAULT)

CREATE_GETTER_SETTER(CLASS, TYPE, NAME)
CREATE_VAR_BINDINGS(CLASS, TYPE, NAME)  
CREATE_CLASS_BINDINGS(CLASS, TYPE, NAME)  

DECLARE_PROPERTY_SINGLE_FILE(TYPE, NAME)
DECLARE_PROPERTY_SINGLE_FILE_DEFAULT(TYPE, NAME, DEFAULT)

DECLARE_PROPERTY_TYPED_ARRAY(Type, property_name)
CREATE_GETTER_SETTER_TYPED_ARRAY(ClassName, Type, property_name)
CREATE_TYPED_ARRAY_BINDINGS(ClassName, Type, property_name)
CREATE_TYPED_ARRAY_STRING_BINDINGS(ClassName, ElementVariantType, ElementCppType, property_name)


*/
#pragma once

// declare property in the header file (warning no default value)
//
// examples:
// DECLARE_PROPERTY(int, test_int) // declare an int
//
// DECLARE_PROPERTY(Ref<Texture2D>, texture2d) // decare a texture 2D
//
#define DECLARE_PROPERTY(TYPE, NAME)      \
                                          \
public:                                   \
	void set_##NAME(const TYPE p_##NAME); \
	TYPE get_##NAME() const;              \
                                          \
private:                                  \
	TYPE NAME;

// same with default value
// eg:
// DECLARE_PROPERTY(int, test_int, 123)
//
#define DECLARE_PROPERTY_DEFAULT(TYPE, NAME, DEFAULT) \
                                                      \
public:                                               \
	void set_##NAME(const TYPE p_##NAME);             \
	TYPE get_##NAME() const;                          \
                                                      \
private:                                              \
	TYPE NAME = DEFAULT;

// cpp file macro, create the get/set functions for @export
//
// no semicolons are required for this as it's on the cpp body and these are the functions
//
#define CREATE_GETTER_SETTER(CLASS, TYPE, NAME)     \
	TYPE CLASS::get_##NAME() const { return NAME; } \
	void CLASS::set_##NAME(const TYPE p_##NAME) { NAME = p_##NAME; }

// COPY
// bindings macro
// run inside _bind_methods on the cpp file
// works only for variants and needs you to find a special constant for each type (like BOOL, FLOAT, VECTOR2I)
//
// Examples:
// CREATE_CLASSDB_BINDINGS(MeshGenerator, Variant::BOOL, enabled)
// CREATE_CLASSDB_BINDINGS(MeshGenerator, Variant::FLOAT, height)
// CREATE_CLASSDB_BINDINGS(MeshGenerator, Variant::VECTOR2I, grid_size)
//
#define CREATE_VAR_BINDINGS(CLASS, TYPE, NAME)                                    \
	ClassDB::bind_method(D_METHOD("get_" #NAME), &CLASS::get_##NAME);             \
	ClassDB::bind_method(D_METHOD("set_" #NAME, "p_" #NAME), &CLASS::set_##NAME); \
	ADD_PROPERTY(PropertyInfo(Variant::TYPE, #NAME), "set_" #NAME, "get_" #NAME);

// this one works for clases, like Texture2D etc
//
// CREATE_CLASS_BINDINGS(MeshGenerator, "Texture2D", texture2d);
//
// i have only tested Texture2D so far
//
//
#define CREATE_CLASS_BINDINGS(CLASS, TYPE, NAME)                            \
	ClassDB::bind_method(D_METHOD("set_" #NAME, TYPE), &CLASS::set_##NAME); \
	ClassDB::bind_method(D_METHOD("get_" #NAME), &CLASS::get_##NAME);       \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #NAME, PROPERTY_HINT_RESOURCE_TYPE, TYPE), "set_" #NAME, "get_" #NAME);

// EXPERIMENTAL (putting the functions on the header file, breaks!!!)
// // header file macro, to declare a property (supports only variants)
// // this all in one macro changes the public/private scope

//
//
// i think const causes problem!!

// note ending has no ;
// allows:
//
// DECLARE_PROPERTY_SINGLE_FILE(float, pulse_width) = 0.5;
//
#define DECLARE_PROPERTY_SINGLE_FILE(TYPE, NAME)               \
public:                                                        \
	TYPE get_##NAME() const { return NAME; };                        \
	void set_##NAME(const TYPE p_##NAME) { NAME = p_##NAME; }; \
                                                               \
private:                                                       \
	TYPE NAME;

// the same as the single file template, but allowing setting a default value (cleaner)
// we can use this alone with:
// CREATE_VAR_BINDINGS and CREATE_CLASS_BINDINGS
#define DECLARE_PROPERTY_SINGLE_FILE_DEFAULT(TYPE, NAME, DEFAULT) \
public:                                                           \
	TYPE get_##NAME() const { return NAME; };                           \
	void set_##NAME(const TYPE p_##NAME) { NAME = p_##NAME; };    \
                                                                  \
private:                                                          \
	TYPE NAME = DEFAULT;

// MACRO to create typed array property
// DECLARE_PROPERTY_TYPED_ARRAY(Texture2D, textures)
#define DECLARE_PROPERTY_TYPED_ARRAY(Type, property_name)                \
public:                                                                  \
	void set_##property_name(const TypedArray<Type> &p_##property_name); \
	TypedArray<Type> get_##property_name() const;                        \
                                                                         \
private:                                                                 \
	TypedArray<Type> property_name;

// macro to create get/set
// example: CREATE_GETTER_SETTER_TYPED_ARRAY(Template, Texture2D, textures)
#define CREATE_GETTER_SETTER_TYPED_ARRAY(ClassName, Type, property_name)              \
	TypedArray<Type> ClassName::get_##property_name() const { return property_name; } \
	void ClassName::set_##property_name(const TypedArray<Type> &p_##property_name) { property_name = p_##property_name; }

// CREATE_TYPED_ARRAY_BINDINGS

// This macro should be defined in your .cpp file, or a common header included by it.
// It assumes you have already included:
// #include <godot_cpp/core/class_db.hpp>
// #include <godot_cpp/variant/utility_functions.hpp> // For String::num()

#define CREATE_TYPED_ARRAY_BINDINGS(ClassName, Type, property_name)                                                  \
	ClassDB::bind_method(D_METHOD("set_" #property_name, #property_name), &ClassName::set_##property_name);          \
	ClassDB::bind_method(D_METHOD("get_" #property_name), &ClassName::get_##property_name);                          \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #property_name, PROPERTY_HINT_TYPE_STRING,                             \
						 String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":" #Type), \
			"set_" #property_name, "get_" #property_name);



// emulates Array[String] or Array[int] (Array of value types)
// usage:
// CREATE_TYPED_ARRAY_STRING_BINDINGS(Template, Variant::INT, int, numbers)
//
// concider instead using a PackedIntArray or other PackedArray
//
#define CREATE_TYPED_ARRAY_STRING_BINDINGS(ClassName, ElementVariantType, ElementCppType, property_name)    \
	ClassDB::bind_method(D_METHOD("set_" #property_name, #property_name), &ClassName::set_##property_name); \
	ClassDB::bind_method(D_METHOD("get_" #property_name), &ClassName::get_##property_name);                 \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #property_name, PROPERTY_HINT_TYPE_STRING,                    \
						 String::num(ElementVariantType)),                                                  \
			"set_" #property_name, "get_" #property_name);
