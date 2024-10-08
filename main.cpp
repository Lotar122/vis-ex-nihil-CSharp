#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include "Classes/Engine/Engine.hpp"
#include "Classes/Handle/Handle.hpp"

#include "Classes/Graphics/Engine_CS.hpp"
#include "Classes/Memory/Engine/Engine_Mem.hpp"
#include "Structs/Example.hpp"

int add(int x, int y)
{
    return x + y;
}

nihil::interop::Engine_Mem* memoryEngine = NULL;

uint64_t allocateNativeObject_CS_global(uint64_t type)
{
    return memoryEngine->allocateNativeObject_CS(type);
}
void freeNativeObject_CS_global(uint64_t name)
{
    memoryEngine->freeNativeObject_CS(name);
}

uint64_t allocateNativeVector_CS_global(uint64_t elementType)
{
    return memoryEngine->allocateNativeVector_CS(elementType);
}
void freeNativeVector_CS_global(uint64_t name)
{
    memoryEngine->freeNativeVector_CS(name);
}

uint64_t getPtr_global(uint64_t name, bool isVector)
{
    return memoryEngine->getPtr(name, isVector);
}
uint64_t getType_global(uint64_t name, bool isVector, bool elementType)
{
    return memoryEngine->getType(name, isVector, elementType);
}

int addHandles(uint64_t handle1Name, uint64_t handle2Name)
{
    uint64_t handle1Type = memoryEngine->getType(handle1Name);
    uint64_t handle2Type = memoryEngine->getType(handle2Name);
    if (!(handle1Type == (uint64_t)nihil::interop::Types::INT && handle2Type == (uint64_t)nihil::interop::Types::INT)) return 0;

    int* num1 = reinterpret_cast<int*>(memoryEngine->getPtr(handle1Name));
    int* num2 = reinterpret_cast<int*>(memoryEngine->getPtr(handle2Name));

    return *num1 + *num2;
}

template<typename T>
void assignGenericHandle(T val, uint64_t name)
{
    //if you get a write access violation to 0x00 just remember to add the types allocation case to the memory engine
    uint64_t ptr = memoryEngine->getPtr(name);
    T* tar = reinterpret_cast<T*>(ptr);
    *tar = val;
    std::cout << *tar << std::endl;
}

template<typename T>
void assignNonGenericType(T* val, uint64_t name)
{
    T* tar = reinterpret_cast<T*>(memoryEngine->getPtr(name));
    *tar = *val;
}

using nihil::interop::charToUint;

int main(int argc, char* argv[]) {
    nihil::scripting::Engine engine = nihil::scripting::Engine();
    engine.addNativeCall("Script.Native.Example", "add", "(int,int)", &add);

    nihil::interop::graphics::Engine_CS engineGraphics(NULL);
    memoryEngine = new nihil::interop::Engine_Mem();

#pragma region Memory related native calls
    engine.addNativeCall("Script.Native.Interop.Memory", "allocateNativeObject_CXX", "(ulong)", &allocateNativeObject_CS_global);
    engine.addNativeCall("Script.Native.Interop.Memory", "freeNativeObject_CXX", "(ulong)", &freeNativeObject_CS_global);

    engine.addNativeCall("Script.Native.Interop.Memory", "allocateNativeVector_CXX", "(ulong)", &allocateNativeVector_CS_global);
    engine.addNativeCall("Script.Native.Interop.Memory", "freeNativeVector_CXX", "(ulong)", &freeNativeVector_CS_global);

    engine.addNativeCall("Script.Native.Interop.Memory", "getPtr_CXX", "(ulong,bool)", &getPtr_global);
    engine.addNativeCall("Script.Native.Interop.Memory", "getType_CXX", "(ulong,bool,bool)", &getType_global);

    engine.addNativeCall("Script.Native.Example", "handleTest", "(ulong,ulong)", &addHandles);
#pragma endregion

    //handle passing structures from c# to c++ and vice versa
    //remember, no spaces in (ulong,ulong) mono is stupidly pedantic (the engine.addNativeCall function removes them now)
#pragma region Generic types assignment
    engine.addNativeCall("Script.Native.Interop.Memory", "assign_INT_CXX", "(int,ulong)", &assignGenericHandle<int>);
    engine.addNativeCall("Script.Native.Interop.Memory", "assign_INT16_CXX", "", &assignGenericHandle<int16_t>);
    engine.addNativeCall("Script.Native.Interop.Memory", "assign_INT32_CXX", "(int,ulong)", &assignGenericHandle<int32_t>);
    engine.addNativeCall("Script.Native.Interop.Memory", "assign_INT64_CXX", "(long,ulong)", &assignGenericHandle<int64_t>);
    engine.addNativeCall("Script.Native.Interop.Memory", "assign_UINT16_CXX", "", &assignGenericHandle<uint16_t>);
    engine.addNativeCall("Script.Native.Interop.Memory", "assign_UINT32_CXX", "(uint,ulong)", &assignGenericHandle<uint32_t>);
    engine.addNativeCall("Script.Native.Interop.Memory", "assign_UINT64_CXX", "(ulong,ulong)", &assignGenericHandle<uint64_t>);
    //for some weird reason adding the signature makes the code not work
    engine.addNativeCall("Script.Native.Interop.Memory", "assign_FLOAT_CXX", "", &assignGenericHandle<float>);
    engine.addNativeCall("Script.Native.Interop.Memory", "assign_DOUBLE_CXX", "(double,ulong)", &assignGenericHandle<double>);
    engine.addNativeCall("Script.Native.Interop.Memory", "assign_BOOL_CXX", "(bool,ulong)", &assignGenericHandle<bool>);
    engine.addNativeCall("Script.Native.Interop.Memory", "assign_CHAR_CXX", "(byte,ulong)", &assignGenericHandle<unsigned char>);
#pragma endregion

    engine.addNativeCall("Script.Native.Interop.Memory", "assign_Example_CXX", "", &assignNonGenericType<nihil::interop::structs::Example>);

    engine.Start();

    engine.invokeScriptMethod("physicsTick", engine.ScriptObject, 0, NULL);
    engine.invokeScriptMethod("graphicsTick", engine.ScriptObject, 0, NULL);

    delete memoryEngine;
    return 0;
}