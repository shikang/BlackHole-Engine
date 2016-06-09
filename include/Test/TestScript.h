#ifndef TEST_SCRIPT_H
#define TEST_SCRIPT_H

#include "Script/Script.h"
#include "Script/ScriptObject.h"
#include "Script/ScriptComponent.h"
#include "Script/ScriptComponentTypeCreator.h"

#include "Platform/ConsoleColor.h"
#include "Core/CoreEssential.h"

#include "Core/XMLSerialiser.h"

#define COUT_BOOL(exp)\
	{\
		bool condition = exp; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

ObjectManager script_objManager;

void TestScript()
{
	std::cout << ConsoleYellow << "--------------------- C#Script Test ------------------" << std::endl << ConsoleWhite;

	// Test Compile
	BH::Script::AddScript("TestDependencies.cs");
	BH::Script::AddScript("Test.cs");
	BH::Script::BuildScripts( ENTITY_MANAGER.Types );

	if ( !ENTITY_MANAGER.Types.Get( TypeCName<ScriptComponent>() ) )
		TypeCreator<ScriptComponent>::RegisterType();

	BH::ScriptObject testclass = BH::Script::CreateScriptObject("TestClass", "testclass");
	BH::ScriptObject testdep = BH::Script::CreateScriptObject("TestDependencies", "testdep");
	COUT_BOOL(testclass.GetType() == BH::String("TestClass")) << " - Create Script Object Test 1" << std::endl;
	COUT_BOOL(testclass.GetName() == BH::String("testclass")) << " - Create Script Object Test 2" << std::endl;
	COUT_BOOL(testdep.GetType() == BH::String("TestDependencies")) << " - Create Script Object Test 3" << std::endl;
	COUT_BOOL(testdep.GetName() == BH::String("testdep")) << " - Create Script Object Test 4" << std::endl;

	testclass.CallMethod("TestMethod");
	testdep.CallMethod("TestDependenciesMethod", &testclass);

	BH::Script::DestroyScriptObject(testclass);
	BH::Script::DestroyScriptObject(testdep);
	COUT_BOOL(testclass.GetType() == BH::String("[InvalidType]")) << " - Destroy Script Object Test 1" << std::endl;
	COUT_BOOL(testclass.GetName() == BH::String("[InvalidName]")) << " - Destroy Script Object Test 2" << std::endl;
	COUT_BOOL(testdep.GetType() == BH::String("[InvalidType]")) << " - Destroy Script Object Test 3" << std::endl;
	COUT_BOOL(testdep.GetName() == BH::String("[InvalidName]")) << " - Destroy Script Object Test 4" << std::endl;

	COUT_BOOL(ENTITY_MANAGER.Types.Get("TestClass") != nullptr) << " - Reflection Script Object Test 1" << std::endl;
	COUT_BOOL(ENTITY_MANAGER.Types.Get("TestDependencies") != nullptr) << " - Reflection Script Object Test 2" << std::endl;

	// Script Component Test
	ScriptComponent::SetTypeDB( &ENTITY_MANAGER.Types );
	Object * o = script_objManager.Create("TestScripts", false);
	o->AddComponent<ScriptComponent>();
	o->AddComponent<ScriptComponent>();

	const Type * st = ENTITY_MANAGER.Types.Get<ScriptComponent>();
	std::vector<ScriptComponent *> scs = o->GetAllComponent<ScriptComponent>();
	ScriptComponent * sc = scs[0];
	String * var = reinterpret_cast<String *>(st->GetField("ScriptType")->GetPtr(sc));
	*var = "TestClass";
	ScriptComponent * sc2 = scs[1];
	var = reinterpret_cast<String *>(st->GetField("ScriptType")->GetPtr(sc2));
	*var = "TestDependencies";

	script_objManager.InitObject(o);

	if (FileSystem::FileCheckExists("TestScriptComponent.xml"))
		FileSystem::FileDelete("", "TestScriptComponent.xml");

	XMLSerialiser serialiser;
	serialiser.TraverseStream("TestScriptComponent.xml");
	serialiser.Serialise<Object>(*o);

	script_objManager.ForceFullPurge();

	std::cout << std::endl << std::endl;
}

#undef COUT_BOOL

#endif