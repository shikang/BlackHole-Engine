#ifndef TEST_XMLSERIALISER_H
#define TEST_XMLSERIALISER_H

#include "Core/CoreEssential.h"
#include "Core/Typing.h"
#include "Core/EntityManager.h"
#include "Core/Type.h"
#include "Core/Field.h"
#include "Core/TypeRegistrar.h"
#include "Core/ObjectComponent.h"
#include "Core/ObjectManager.h"

#include "Core/XMLSerialiser.h"
#include "Core/XMLDeserialiser.h"

#include "Platform/FileSystem.h"

using namespace BH;

#define COUT_BOOL(exp)\
	{\
		bool condition = exp; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

namespace BH
{
	struct DummyAccessXML {};
	template<>
	DummyAccessXML* EntityManager::New<DummyAccessXML>() const
	{
		this->EntityManager::~EntityManager();
		this->EntityManager::EntityManager();
		//TypeRegistrar::RegisterTypes();
		return nullptr;
	}
}

void ForceReconstructEntityManager_XML()
{
	EntityManager* em = const_cast<EntityManager*>(EntityManager::InstancePtr());
	em->New<DummyAccessXML>();
}

class TestSerialiseObject
{
public:
	TestSerialiseObject() {}

	TestSerialiseObject(const String & name, const std::vector<Vector2f> & vectors)
		: mName(name), mVectors(vectors) {}

	~TestSerialiseObject(){}

public:
	std::vector<Vector2f> mVectors;
	Hash mName;
	
};

DECLARE_TYPE(TestSerialiseObject, TestSerialiseObject);

class TestPositionComponent : public BH::ObjectComponent
{
public:
	virtual ~TestPositionComponent() {}

	TestPositionComponent() : mX(0.0f), mY(0.0f) {}

protected:
	virtual void Initialise() {}

	virtual void Shutdown() {}

	BH_OBJECT_COMPONENT_FRIENDS;

public:
	f32 mX;
	f32 mY;

};

class Test3DPositionComponent : public TestPositionComponent
{
public:
	virtual ~Test3DPositionComponent() {}

	Test3DPositionComponent() : mZ(0.0f) {}

protected:
	virtual void Initialise() {}

	virtual void Shutdown() {}

	BH_OBJECT_COMPONENT_FRIENDS;

public:
	f32 mZ;

};

class TestMeshComponent : public BH::ObjectComponent
{
public:
	virtual ~TestMeshComponent() {}

	TestMeshComponent() : mMesh("Mesh") {}

protected:
	virtual void Initialise() {}

	virtual void Shutdown() {}

	BH_OBJECT_COMPONENT_FRIENDS;

public:
	String mMesh;
	std::vector<String> mAnimations;
};

DECLARE_TYPE(TestPositionComponent, TestPositionComponent);
DECLARE_TYPE(Test3DPositionComponent, Test3DPositionComponent);
DECLARE_TYPE(TestMeshComponent, TestMeshComponent);

// Test XML Serialiser
ObjectManager objManager;

void XMLSerialiseObjectTest()
{
	TypeDB& typedb = ENTITY_MANAGER.Types;

	typedb.Create<TestMeshComponent>()
		.FIELDM(Mesh, TestMeshComponent)
		.FIELDM(Animations, TestMeshComponent)
		.Base<BH::ObjectComponent>();
	typedb.Create<TestPositionComponent>()
		.FIELDM(X, TestPositionComponent)
		.FIELDM(Y, TestPositionComponent)
		.Base<BH::ObjectComponent>();
	typedb.Create<Test3DPositionComponent>()
		.FIELDM(Z, Test3DPositionComponent)
		.Base<TestPositionComponent>();

	Object * o = objManager.Create("TestXMLSerialiser");
	BH_ASSERT_TRUE(o != nullptr);

	o->AddComponent<Test3DPositionComponent>();
	o->AddComponent<TestMeshComponent>();
	o->GetComponent<Test3DPositionComponent>()->mX = 1.0f;
	o->GetComponent<Test3DPositionComponent>()->mY = 1.0f;
	o->GetComponent<Test3DPositionComponent>()->mZ = 1.0f;
	o->GetComponent<TestMeshComponent>()->mMesh = "TestMesh";
	o->GetComponent<TestMeshComponent>()->mAnimations.push_back("Eat");
	o->GetComponent<TestMeshComponent>()->mAnimations.push_back("Sleep");
	o->GetComponent<TestMeshComponent>()->mAnimations.push_back("Play");

	Object * o2 = objManager.Create("TestXMLSerialiser");
	o2->AddComponent<Test3DPositionComponent>();
	o2->AddComponent<TestMeshComponent>();
	o2->GetComponent<Test3DPositionComponent>()->mX = 0.0f;
	o2->GetComponent<Test3DPositionComponent>()->mY = 0.0f;
	o2->GetComponent<Test3DPositionComponent>()->mZ = 0.0f;
	o2->GetComponent<TestMeshComponent>()->mMesh = "TestMesh2";
	o2->GetComponent<TestMeshComponent>()->mAnimations.push_back("Work");
	o2->GetComponent<TestMeshComponent>()->mAnimations.push_back("Study");
	o2->GetComponent<TestMeshComponent>()->mAnimations.push_back("Die");

	XMLSerialiser serialiser;
	serialiser.TraverseStream("TestXMLSerialiser.xml");
	serialiser.Serialise<Object>(*o);
	serialiser.Serialise<Object>(*o2);

	objManager.ForceFullPurge();
	ForceReconstructEntityManager_XML();
}

void XMLDeserialiserObjectTest()
{
	TypeDB& typedb = ENTITY_MANAGER.Types;

	typedb.Create<TestMeshComponent>()
		.FIELDM(Mesh, TestMeshComponent)
		.FIELDM(Animations, TestMeshComponent)
		.Base<BH::ObjectComponent>();
	typedb.Create<TestPositionComponent>()
		.FIELDM(X, TestPositionComponent)
		.FIELDM(Y, TestPositionComponent)
		.Base<BH::ObjectComponent>();
	typedb.Create<Test3DPositionComponent>()
		.FIELDM(Z, Test3DPositionComponent)
		.Base<TestPositionComponent>();

	Object * o = nullptr;
	Object * o2 = nullptr;
	XMLDeserialiser deserialiser;
	deserialiser.TraverseStream("TestXMLSerialiser.xml");
	deserialiser.DeserialisePtr<Object>(o);

	COUT_BOOL(o->GetComponent<TestMeshComponent>()->mMesh == "TestMesh") << " - XML Object Deserialise Test 1" << std::endl;
	COUT_BOOL(o->GetComponent<TestMeshComponent>()->mAnimations[0] == "Eat") << " - XML Object Deserialise Test 2" << std::endl;
	COUT_BOOL(o->GetComponent<TestMeshComponent>()->mAnimations[1] == "Sleep") << " - XML Object Deserialise Test 3" << std::endl;
	COUT_BOOL(o->GetComponent<TestMeshComponent>()->mAnimations[2] == "Play") << " - XML Object Deserialise Test 4" << std::endl;
	COUT_BOOL(o->GetComponent<Test3DPositionComponent>()->mX == 1.0f) << " - XML Object Deserialise Test 5" << std::endl;
	COUT_BOOL(o->GetComponent<Test3DPositionComponent>()->mY == 1.0f) << " - XML Object Deserialise Test 6" << std::endl;
	COUT_BOOL(o->GetComponent<Test3DPositionComponent>()->mZ == 1.0f) << " - XML Object Deserialise Test 7" << std::endl;

	deserialiser.DeserialisePtr<Object>(o2);

	COUT_BOOL(o2->GetComponent<TestMeshComponent>()->mMesh == "TestMesh2") << " - XML Object Deserialise Test 8" << std::endl;
	COUT_BOOL(o2->GetComponent<TestMeshComponent>()->mAnimations[0] == "Work") << " - XML Object Deserialise Test 9" << std::endl;
	COUT_BOOL(o2->GetComponent<TestMeshComponent>()->mAnimations[1] == "Study") << " - XML Object Deserialise Test 10" << std::endl;
	COUT_BOOL(o2->GetComponent<TestMeshComponent>()->mAnimations[2] == "Die") << " - XML Object Deserialise Test 11" << std::endl;
	COUT_BOOL(o2->GetComponent<Test3DPositionComponent>()->mX == 0.0f) << " - XML Object Deserialise Test 12" << std::endl;
	COUT_BOOL(o2->GetComponent<Test3DPositionComponent>()->mY == 0.0f) << " - XML Object Deserialise Test 13" << std::endl;
	COUT_BOOL(o2->GetComponent<Test3DPositionComponent>()->mZ == 0.0f) << " - XML Object Deserialise Test 14" << std::endl;

	objManager.ForceFullPurge();
	ForceReconstructEntityManager_XML();

	FileSystem::FileDelete("", "TestXMLSerialiser.xml");
}

void ObjManagerXMLSerialiserTest()
{
	TypeDB& typedb = ENTITY_MANAGER.Types;

	typedb.Create<TestMeshComponent>()
		.FIELDM(Mesh, TestMeshComponent)
		.FIELDM(Animations, TestMeshComponent)
		.Base<BH::ObjectComponent>();
	typedb.Create<TestPositionComponent>()
		.FIELDM(X, TestPositionComponent)
		.FIELDM(Y, TestPositionComponent)
		.Base<BH::ObjectComponent>();
	typedb.Create<Test3DPositionComponent>()
		.FIELDM(Z, Test3DPositionComponent)
		.Base<TestPositionComponent>();

	Object * o = objManager.Create("TestXMLSerialiser");
	BH_ASSERT_TRUE(o != nullptr);

	o->AddComponent<Test3DPositionComponent>();
	o->AddComponent<TestMeshComponent>();
	o->GetComponent<Test3DPositionComponent>()->mX = 1.0f;
	o->GetComponent<Test3DPositionComponent>()->mY = 1.0f;
	o->GetComponent<Test3DPositionComponent>()->mZ = 1.0f;
	o->GetComponent<TestMeshComponent>()->mMesh = "TestMesh";
	o->GetComponent<TestMeshComponent>()->mAnimations.push_back("Eat");
	o->GetComponent<TestMeshComponent>()->mAnimations.push_back("Sleep");
	o->GetComponent<TestMeshComponent>()->mAnimations.push_back("Play");

	Object * o2 = objManager.Create("TestXMLSerialiser");
	o2->AddComponent<Test3DPositionComponent>();
	o2->AddComponent<TestMeshComponent>();
	o2->GetComponent<Test3DPositionComponent>()->mX = 0.0f;
	o2->GetComponent<Test3DPositionComponent>()->mY = 0.0f;
	o2->GetComponent<Test3DPositionComponent>()->mZ = 0.0f;
	o2->GetComponent<TestMeshComponent>()->mMesh = "TestMesh2";
	o2->GetComponent<TestMeshComponent>()->mAnimations.push_back("Work");
	o2->GetComponent<TestMeshComponent>()->mAnimations.push_back("Study");
	o2->GetComponent<TestMeshComponent>()->mAnimations.push_back("Die");

	objManager.SaveAllObjectsToFile<XMLSerialiser>("TestXMLSerialiser.xml");

	objManager.ForceFullPurge();
	ForceReconstructEntityManager_XML();
}

void ObjManagerXMLDeserialiserTest()
{
	TypeDB& typedb = ENTITY_MANAGER.Types;

	typedb.Create<TestMeshComponent>()
		.FIELDM(Mesh, TestMeshComponent)
		.FIELDM(Animations, TestMeshComponent)
		.Base<BH::ObjectComponent>();
	typedb.Create<TestPositionComponent>()
		.FIELDM(X, TestPositionComponent)
		.FIELDM(Y, TestPositionComponent)
		.Base<BH::ObjectComponent>();
	typedb.Create<Test3DPositionComponent>()
		.FIELDM(Z, Test3DPositionComponent)
		.Base<TestPositionComponent>();

	Object * o = nullptr;
	Object * o2 = nullptr;
	objManager.LoadAllObjectFromFile<XMLDeserialiser>("TestXMLSerialiser.xml");

	o = objManager.Get("TestXMLSerialiser");

	COUT_BOOL(o->GetComponent<TestMeshComponent>()->mMesh == "TestMesh") << " - Obj Manager XML Deserialiser Test 1" << std::endl;
	COUT_BOOL(o->GetComponent<TestMeshComponent>()->mAnimations[0] == "Eat") << " - Obj Manager XML Deserialiser Test 2" << std::endl;
	COUT_BOOL(o->GetComponent<TestMeshComponent>()->mAnimations[1] == "Sleep") << " - Obj Manager XML Deserialiser Test 3" << std::endl;
	COUT_BOOL(o->GetComponent<TestMeshComponent>()->mAnimations[2] == "Play") << " - Obj Manager XML Deserialiser Test 4" << std::endl;
	COUT_BOOL(o->GetComponent<Test3DPositionComponent>()->mX == 1.0f) << " - Obj Manager XML Deserialiser Test 5" << std::endl;
	COUT_BOOL(o->GetComponent<Test3DPositionComponent>()->mY == 1.0f) << " - Obj Manager XML Deserialiser Test 6" << std::endl;
	COUT_BOOL(o->GetComponent<Test3DPositionComponent>()->mZ == 1.0f) << " - Obj Manager XML Deserialiser Test 7" << std::endl;

	o2 = objManager.Get("TestXMLSerialiser0");

	COUT_BOOL(o2->GetComponent<TestMeshComponent>()->mMesh == "TestMesh2") << " - Obj Manager XML Deserialiser Test 8" << std::endl;
	COUT_BOOL(o2->GetComponent<TestMeshComponent>()->mAnimations[0] == "Work") << " - Obj Manager XML Deserialiser Test 9" << std::endl;
	COUT_BOOL(o2->GetComponent<TestMeshComponent>()->mAnimations[1] == "Study") << " - Obj Manager XML Deserialiser Test 10" << std::endl;
	COUT_BOOL(o2->GetComponent<TestMeshComponent>()->mAnimations[2] == "Die") << " - Obj Manager XML Deserialiser Test 11" << std::endl;
	COUT_BOOL(o2->GetComponent<Test3DPositionComponent>()->mX == 0.0f) << " - Obj Manager XML Deserialiser Test 12" << std::endl;
	COUT_BOOL(o2->GetComponent<Test3DPositionComponent>()->mY == 0.0f) << " - Obj Manager XML Deserialiser Test 13" << std::endl;
	COUT_BOOL(o2->GetComponent<Test3DPositionComponent>()->mZ == 0.0f) << " - Obj Manager XML Deserialiser Test 14" << std::endl;

	objManager.ForceFullPurge();
	ForceReconstructEntityManager_XML();

	FileSystem::FileDelete("", "TestXMLSerialiser.xml");
}

void XMLSerialiseTest()
{
	TypeDB& typedb = ENTITY_MANAGER.Types;

	typedb.Create<TestSerialiseObject>()
		  .FIELDM(Name, TestSerialiseObject)
		  .FIELDM(Vectors, TestSerialiseObject);

	std::vector<Vector2f> v = { Vector2f::ZERO, Vector2f::UNIT_X, Vector2f::UNIT_Y, Vector2f::ONE };
	TestSerialiseObject t("TestXmlSerialiser", v);

	XMLSerialiser serialiser;
	serialiser.TraverseStream("TestXMLSerialiser.xml");
	serialiser.Serialise<TestSerialiseObject>(t);

	ForceReconstructEntityManager_XML();
}

void XMLDeserialiserTest()
{
	TypeDB& typedb = ENTITY_MANAGER.Types;

	typedb.Create<TestSerialiseObject>()
		.FIELDM(Name, TestSerialiseObject)
		.FIELDM(Vectors, TestSerialiseObject);

	TestSerialiseObject * t = nullptr;
	XMLDeserialiser deserialiser;
	deserialiser.TraverseStream("TestXMLSerialiser.xml");
	deserialiser.DeserialisePtr<TestSerialiseObject>(t);

	COUT_BOOL(t->mName == Hash("TestXmlSerialiser")) << " - XML Deserialise Test 1" << std::endl;
	COUT_BOOL(t->mVectors[0] == Vector2f::ZERO) << " - XML Deserialise Test 2" << std::endl;
	COUT_BOOL(t->mVectors[1] == Vector2f::UNIT_X) << " - XML Deserialise Test 3" << std::endl;
	COUT_BOOL(t->mVectors[2] == Vector2f::UNIT_Y) << " - XML Deserialise Test 4" << std::endl;
	COUT_BOOL(t->mVectors[3] == Vector2f::ONE) << " - XML Deserialise Test 5" << std::endl;

	ForceReconstructEntityManager_XML();

	FileSystem::FileDelete("", "TestXMLSerialiser.xml");
}

void ObjManagerXMLSerialiserTest2()
{
	TypeDB& typedb = ENTITY_MANAGER.Types;

	typedb.Create<TestMeshComponent>()
		.FIELDM(Mesh, TestMeshComponent)
		.FIELDM(Animations, TestMeshComponent)
		.Base<BH::ObjectComponent>();
	typedb.Create<TestPositionComponent>()
		.FIELDM(X, TestPositionComponent)
		.FIELDM(Y, TestPositionComponent)
		.Base<BH::ObjectComponent>();
	typedb.Create<Test3DPositionComponent>()
		.FIELDM(Z, Test3DPositionComponent)
		.Base<TestPositionComponent>();

	Object * o = objManager.CreateArcheType("TestArcheType");
	BH_ASSERT_TRUE(o != nullptr);

	o->AddComponent<Test3DPositionComponent>();
	o->AddComponent<TestMeshComponent>();

	objManager.SaveArcheTypes<XMLSerialiser>("TestXMLSerialiser.xml");

	objManager.ForceFullPurge();
	objManager.DeleteArcheType("TestArcheType");
	ForceReconstructEntityManager_XML();
}

void ObjManagerXMLDeserialiserTest2()
{
	TypeDB& typedb = ENTITY_MANAGER.Types;

	typedb.Create<TestMeshComponent>()
		.FIELDM(Mesh, TestMeshComponent)
		.FIELDM(Animations, TestMeshComponent)
		.Base<BH::ObjectComponent>();
	typedb.Create<TestPositionComponent>()
		.FIELDM(X, TestPositionComponent)
		.FIELDM(Y, TestPositionComponent)
		.Base<BH::ObjectComponent>();
	typedb.Create<Test3DPositionComponent>()
		.FIELDM(Z, Test3DPositionComponent)
		.Base<TestPositionComponent>();

	Object * o = nullptr;
	objManager.LoadArcheTypes<XMLDeserialiser>("TestXMLSerialiser.xml");

	o = objManager.CreateFromArcheType( "TestArcheType", "TestXMLSerialiser" );

	COUT_BOOL(o->GetComponent<TestMeshComponent>() != nullptr) << " - Obj Manager Archetype XML Deserialiser Test 1" << std::endl;
	COUT_BOOL(o->GetComponent<Test3DPositionComponent>() != nullptr) << " - Obj Manager Archetype XML Deserialiser Test 2" << std::endl;
	COUT_BOOL(o->GetComponent<TestPositionComponent>() == nullptr) << " - Obj Manager Archetype XML Deserialiser Test 3" << std::endl;

	objManager.ForceFullPurge();
	objManager.DeleteArcheType("TestArcheType");
	ForceReconstructEntityManager_XML();

	FileSystem::FileDelete("", "TestXMLSerialiser.xml");
}

void TestXML()
{
	std::cout << ConsoleYellow << "--------------------- XML Serialiser Test ------------------" << std::endl;

	if (FileSystem::FileCheckExists("TestXMLSerialiser.xml"))
		FileSystem::FileDelete("", "TestXMLSerialiser.xml");

	XMLSerialiseTest();
	XMLDeserialiserTest();
	XMLSerialiseObjectTest();
	XMLDeserialiserObjectTest();
	ObjManagerXMLSerialiserTest();
	ObjManagerXMLDeserialiserTest();
	ObjManagerXMLSerialiserTest2();
	ObjManagerXMLDeserialiserTest2();

	std::cout << std::endl << std::endl;
}


#undef COUT_BOOL

#endif