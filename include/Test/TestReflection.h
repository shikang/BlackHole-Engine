#ifndef TEST_REFLECTION_H
#define TEST_REFLECTION_H

#include "Core/CoreEssential.h"
#include "Core/Typing.h"
#include "Core/EntityManager.h"
#include "Core/Type.h"
#include "Core/Field.h"
#include "Core/TypeRegistrar.h"
#include "Core/TypeCreator.h"
#include "Core/TypeStringifier.h"

#include "Platform/Typedefs.h"
#include "Platform/ConsoleColor.h"
#include "Platform/Vector3f.h"

using namespace BH;

#define COUT_BOOL(exp)\
	{\
		bool condition = exp; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

// Test reflection
namespace BH
{
	struct DummyAccess {};
	template<>
	DummyAccess* EntityManager::New<DummyAccess>() const
	{
		this->EntityManager::~EntityManager();
		this->EntityManager::EntityManager();
		//TypeRegistrar::RegisterTypes();
		return nullptr;
	}
}

void ForceReconstructEntityManager()
{
	EntityManager* em = const_cast<EntityManager*>(EntityManager::InstancePtr());
	em->New<DummyAccess>();
}

struct ReflectTest :
	public Entity
{
	int x;
	int y;

	virtual Entity::OPTIONS OnPreChanged(void) { return Entity::O_NORMAL; }
	virtual void OnChanged(void) {}
	virtual Entity::OPTIONS OnPreSaved(void) const { return Entity::O_NORMAL; }
	virtual void OnSaved(void) const {}
};

DECLARE_TYPE(ReflectTest, ReflectTest);
BEGIN_DEFINE_TYPE(ReflectTest)
	ENTITY_MANAGER.Types.Create<ReflectTest>()
				  .FIELD(x, ReflectTest);
END_DEFINE_TYPE(ReflectTest)

class TestType {};

namespace _tt
{
	class TestType {};

	namespace _tt
	{
		struct TestType{};
	}
}

class InheritedType : public TestType
{
};

DECLARE_TYPE(TestType, TestType);
DECLARE_TYPE(InheritedType, InheritedType);
DECLARE_TYPE(_tt::TestType, _ttTestType);
DECLARE_TYPE(_tt::_tt::TestType, _tt_ttTestType);

void TestTypeCName()
{
	COUT_BOOL(TypeCName<TestType>().Text == BH::String("TestType")) << " - TypeCName Test 1" << std::endl;
	COUT_BOOL(TypeCName<_tt::TestType>().Text == BH::String("_ttTestType")) << " - TypeCName Test 2" << std::endl;
	COUT_BOOL(TypeCName<_tt::_tt::TestType>().Text == BH::String("_tt_ttTestType")) << " - TypeCName Test 3" << std::endl;
}

void TestTypeBase()
{
	ENTITY_MANAGER.Types.Create<TestType>();
	ENTITY_MANAGER.Types.Create<InheritedType>()
						.Base<TestType>();			//!< Declare base type
	ENTITY_MANAGER.Types.Create<_tt::TestType>();

	COUT_BOOL(ENTITY_MANAGER.Types.Get<TestType>() != nullptr) << " - TypeBase Test 1" << std::endl;
	COUT_BOOL(ENTITY_MANAGER.Types.Get<InheritedType>() != nullptr) << " - TypeBase Test 2" << std::endl;
	COUT_BOOL(ENTITY_MANAGER.Types.Get<_tt::TestType>() != nullptr) << " - TypeBase Test 3" << std::endl;
	COUT_BOOL(ENTITY_MANAGER.Types.Get<InheritedType>()->IsType<TestType>()) << " - TypeBase Test 4" << std::endl;
	COUT_BOOL(!(ENTITY_MANAGER.Types.Get<InheritedType>()->IsType<_tt::TestType>())) << " - TypeBase Test 5" << std::endl;

	//ForceReconstructEntityManager();
}

void TestEntityInheritance()
{
	//const Type* entType = ENTITY_MANAGER.Types.Get<Object>();
	//COUT_BOOL((entType != nullptr) && entType->IsEntity) << " - Entity Inheritance Test 1" << std::endl;
	const Type* nonEnt = ENTITY_MANAGER.Types.Get<s8>();
	COUT_BOOL((nonEnt != nullptr) && !nonEnt->IsEntity) << " - Entity Inheritance Test 2" << std::endl;

	s8* newInt = ENTITY_MANAGER.New<s8>();
	*newInt = 10;

	COUT_BOOL(*newInt == 10) << " - Get Int Test" << std::endl;
	ENTITY_MANAGER.Delete(newInt);
	//delete(newInt);
}

void TestStringify()
{
	// Test Int8 values.
	{
		s8 value = 10;

		const Type* type = ENTITY_MANAGER.Types.Get<s8>();
		String testStringify;
		TYPE_STRINGIFIER.GetToString(type)(testStringify, type, &value, nullptr);
		COUT_BOOL(testStringify == "10") << " - To String Test (signed char)" << std::endl;
		testStringify = "50";
		TYPE_STRINGIFIER.GetFromString(type)(testStringify.c_str(), type, &value, nullptr);
		COUT_BOOL(value == 50) << " - From String Test (signed char)" << std::endl;
	}

	// Test bool values.
	{
		bool value = false;

		const Type* type = ENTITY_MANAGER.Types.Get<bool>();
		String testStringify;
		TYPE_STRINGIFIER.GetToString(type)(testStringify, type, &value, nullptr);
		COUT_BOOL(testStringify == "false") << " - To String Test (bool)" << std::endl;
		value = true;
		TYPE_STRINGIFIER.GetToString(type)(testStringify, type, &value, nullptr);
		COUT_BOOL(testStringify == "true") << " - From String Test 1 (bool)" << std::endl;

		TYPE_STRINGIFIER.GetFromString(type)("FaLsE", type, &value, nullptr);
		COUT_BOOL(!value) << " - From String Test 2 (bool)" << std::endl;
		TYPE_STRINGIFIER.GetFromString(type)("TruE", type, &value, nullptr);
		COUT_BOOL(value) << " - From String Test 3 (bool)" << std::endl;
		TYPE_STRINGIFIER.GetFromString(type)("0", type, &value, nullptr);
		COUT_BOOL(!value) << " - From String Test 4 (bool)" << std::endl;
		TYPE_STRINGIFIER.GetFromString(type)("1", type, &value, nullptr);
		COUT_BOOL(value) << " - From String Test 5 (bool)" << std::endl;
	}
}

void TestEntityCreateDelete()
{
	//ENTITY_MANAGER.Types.Create<ReflectTest>()
	//					.FIELD(x, ReflectTest);

	ReflectTest* tpl_rt = ENTITY_MANAGER.New<ReflectTest>();
	ReflectTest* nam_rt = (ReflectTest*)ENTITY_MANAGER.New("ReflectTest");

	COUT_BOOL(tpl_rt != nullptr) << " - Create Entity Test 1" << std::endl;
	COUT_BOOL(nam_rt != nullptr) << " - Create Entity Test 2" << std::endl;

	ENTITY_MANAGER.Delete(tpl_rt);
	ENTITY_MANAGER.Delete(nam_rt);

	nam_rt = (ReflectTest*)ENTITY_MANAGER.New(BH::TypeCName<ReflectTest>());
	COUT_BOOL(nam_rt != nullptr) << " - Create Entity Test 3" << std::endl;

	//nam_rt->x = 5;
	const Type* type = ENTITY_MANAGER.Types.Get<ReflectTest>();
	const Field* x = type->GetField("x");
	COUT_BOOL(x != nullptr) << " - Create Field Test 1" << std::endl;
	*reinterpret_cast<int*>(x->GetPtr(nam_rt)) = 5;
	nam_rt->y = 10;

	COUT_BOOL(nam_rt->x == 5) << " - Create Entity Test 4" << std::endl;
	COUT_BOOL(nam_rt->y == 10) << " - Create Entity Test 5" << std::endl;

	ENTITY_MANAGER.Delete(nam_rt);

	ForceReconstructEntityManager();
}

void TestField()
{
	Vector3f vec(1.0f, 2.0f, 3.0f);

	const Type* type = ENTITY_MANAGER.Types.Get<Vector3f>();

	const Field* x = type->GetField("x");
	const Field* y = type->GetField("y");
	const Field* z = type->GetField("z");

	COUT_BOOL(x && y && z) << " - Field Test 1" << std::endl;
	COUT_BOOL(type->GetField("Bad") == nullptr) << " - Field Test 2" << std::endl;

	COUT_BOOL(std::fabs(*(BH::f32*)(x->GetPtr(&vec)) - 1.0f) <= FLT_EPSILON) << " - Field Test 3" << std::endl;
	COUT_BOOL(std::fabs(*(BH::f32*)(y->GetPtr(&vec)) - 2.0f) <= FLT_EPSILON) << " - Field Test 4" << std::endl;
	COUT_BOOL(std::fabs(*(BH::f32*)(z->GetPtr(&vec)) - 3.0f) <= FLT_EPSILON) << " - Field Test 5" << std::endl;
}

namespace TestEnum
{
	enum Options
	{
		First = 1 << 0,
		Second = 1 << 1,
		Third = 1 << 2,
		Fourth = 1 << 3,
	};
}

DECLARE_TYPE(TestEnum::Options, TestEnum);

void TestFieldEnum()
{
	ENTITY_MANAGER.Types.Create<TestEnum::Options>()
		.FIELD_ENUM(First, TestEnum)
		.FIELD_ENUM(Second, TestEnum)
		.FIELD_ENUM(Third, TestEnum)
		.FIELD_ENUM(Fourth, TestEnum);

	TestEnum::Options value = TestEnum::First;

	const Type* type = ENTITY_MANAGER.Types.Get<TestEnum::Options>();
	String testStringify;
	TYPE_STRINGIFIER.GetToString(type)(testStringify, type, &value, nullptr);
	COUT_BOOL(testStringify == "First") << " - To String Test 1 (Enum)" << std::endl;
	value = TestEnum::Second;
	TYPE_STRINGIFIER.GetToString(type)(testStringify, type, &value, nullptr);
	COUT_BOOL(testStringify == "Second") << " - To String Test 2 (Enum)" << std::endl;

	TYPE_STRINGIFIER.GetFromString(type)("Third", type, &value, nullptr);
	COUT_BOOL(value == TestEnum::Third) << " - From String Test 1 (Enum)" << std::endl;
	TYPE_STRINGIFIER.GetFromString(type)("Fourth", type, &value, nullptr);
	COUT_BOOL(value == TestEnum::Fourth) << " - From String Test 2 (Enum)" << std::endl;

	ForceReconstructEntityManager();
}

struct ContainerTest
{
	s32										Array[15];
	std::vector<s32>						Vector;
	std::set<s32>							Set;
	std::unordered_set<s32>					USet;
	std::map<s32, std::string>				Map;
	std::unordered_map<s32, std::string>	UMap;
	std::deque<s32>							Deque;

	s32 UnorderedTest;

	ContainerTest() : UnorderedTest(0)
	{
		for (int i = 0; i < 15; ++i)
		{
			Array[i] = i;
			Vector.push_back(i);
			Set.insert(i);
			USet.insert(i);
			Deque.push_back(i);

			std::stringstream num;
			num << i;
			Map.insert(std::make_pair(i, num.str()));
			UMap.insert(std::make_pair(i, num.str()));

			UnorderedTest += i;
		}
	}
};
DECLARE_TYPE(ContainerTest, ContainerTest);

class ReflectionFieldContainerTestClass
{
public:
	void SetUp()
	{
		ENTITY_MANAGER.Types.Create<ContainerTest>()
			.FIELD(Array, ContainerTest)
			.FIELD(Vector, ContainerTest)
			.FIELD(Set, ContainerTest)
			.FIELD(USet, ContainerTest)
			.FIELD(Map, ContainerTest)
			.FIELD(UMap, ContainerTest)
			.FIELD(Deque, ContainerTest);

		ContType = ENTITY_MANAGER.Types.Get<ContainerTest>();

		BH_ASSERT_TRUE(ContType != nullptr);
		BH_ASSERT_TRUE(ContType->GetFields().size() == 7);
	}

	void TearDown()
	{
		ForceReconstructEntityManager();
	}

public:
	const Type* ContType;

};

void TestArrayContainer()
{
	ReflectionFieldContainerTestClass test;
	test.SetUp();
	const Field* field = test.ContType->GetField("Array");
	COUT_BOOL(field != nullptr) << " - Field Array Test 1" << std::endl;
	COUT_BOOL(field->Container != nullptr) << " - Field Array Test 2" << std::endl;
	COUT_BOOL(field->Container->KeyType == nullptr) << " - Field Array Test 3" << std::endl;
	COUT_BOOL(field->Container->ValueType->Name == BH::TypeName<s32>()) << " - Field Array Test 4" << std::endl;

	ContainerTest cont;

	s32 t = 0;
	FOREACH_READ_FIELD_BEGIN(field->Container, it, &cont.Array)
	{
		COUT_BOOL(*(s32*)it->GetValue() == t++) << " - Field Array Value Test " << t << std::endl;
	}
	FOREACH_FIELD_END(field->Container, it);

	test.TearDown();
}

void TestVectorContainer()
{
	ReflectionFieldContainerTestClass test;
	test.SetUp();
	const Field* field = test.ContType->GetField("Vector");
	COUT_BOOL(field != nullptr) << " - Field Vector Test 1" << std::endl;
	COUT_BOOL(field->Container != nullptr) << " - Field Vector Test 2" << std::endl;
	COUT_BOOL(field->Container->KeyType == nullptr) << " - Field Vector Test 3" << std::endl;
	COUT_BOOL(field->Container->ValueType->Name == BH::TypeName<s32>()) << " - Field Vector Test 4" << std::endl;

	ContainerTest cont;

	s32 t = 0;
	FOREACH_READ_FIELD_BEGIN(field->Container, it, &cont.Vector)
	{
		COUT_BOOL(*(s32*)it->GetValue() == t++) << " - Field Vector Value Test " << t << std::endl;
	}
	FOREACH_FIELD_END(field->Container, it);

	test.TearDown();
}

void TestSetContainer()
{
	ReflectionFieldContainerTestClass test;
	test.SetUp();
	const Field* field = test.ContType->GetField("Set");
	COUT_BOOL(field != nullptr) << " - Field Set Test 1" << std::endl;
	COUT_BOOL(field->Container != nullptr) << " - Field Set Test 2" << std::endl;
	COUT_BOOL(field->Container->KeyType == nullptr) << " - Field Set Test 3" << std::endl;
	COUT_BOOL(field->Container->ValueType->Name == BH::TypeName<s32>()) << " - Field Set Test 4" << std::endl;

	ContainerTest cont;

	s32 t = 0;
	FOREACH_READ_FIELD_BEGIN(field->Container, it, &cont.Set)
	{
		COUT_BOOL(*(s32*)it->GetValue() == t++) << " - Field Set Value Test " << t << std::endl;
	}
	FOREACH_FIELD_END(field->Container, it);

	test.TearDown();
}

void TestUnorderedSetContainer()
{
	ReflectionFieldContainerTestClass test;
	test.SetUp();
	const Field* field = test.ContType->GetField("USet");
	COUT_BOOL(field != nullptr) << " - Field Unordered Set Test 1" << std::endl;
	COUT_BOOL(field->Container != nullptr) << " - Field Unordered Set Test 2" << std::endl;
	COUT_BOOL(field->Container->KeyType == nullptr) << " - Field Unordered Set Test 3" << std::endl;
	COUT_BOOL(field->Container->ValueType->Name == BH::TypeName<s32>()) << " - Field Unordered Set Test 4" << std::endl;

	ContainerTest cont;

	s32 t = 0;
	FOREACH_READ_FIELD_BEGIN(field->Container, it, &cont.USet)
	{
		t += *(s32*)it->GetValue();
	}
	FOREACH_FIELD_END(field->Container, it);

	COUT_BOOL(t == cont.UnorderedTest) << " - Field Unordered Set Value Test" << std::endl;

	test.TearDown();
}

void TestMapContainer()
{
	ReflectionFieldContainerTestClass test;
	test.SetUp();
	const Field* field = test.ContType->GetField("Map");
	COUT_BOOL(field != nullptr) << " - Field Map Test 1" << std::endl;
	COUT_BOOL(field->Container != nullptr) << " - Field Map Test 2" << std::endl;
	COUT_BOOL(field->Container->KeyType->Name == BH::TypeName<s32>()) << " - Field Map Test 3" << std::endl;
	COUT_BOOL(field->Container->ValueType->Name == BH::TypeName<std::string>()) << " - Field Map Test 4" << std::endl;

	ContainerTest cont;

	s32 t = 0;
	FOREACH_READ_FIELD_BEGIN(field->Container, it, &cont.Map)
	{
		std::stringstream num;
		num << t;
		COUT_BOOL(*(String*)it->GetValue() == num.str()) << " - Field Map Value Test " << t + 1 << std::endl;
		COUT_BOOL(*(s32*)it->GetKey() == t++) << " - Field Map Key Test " << t << std::endl;
	}
	FOREACH_FIELD_END(field->Container, it);

	test.TearDown();
}

void TestUnorderedMapContainer()
{
	ReflectionFieldContainerTestClass test;
	test.SetUp();
	const Field* field = test.ContType->GetField("UMap");
	COUT_BOOL(field != nullptr) << " - Field Unordered Map Test 1" << std::endl;
	COUT_BOOL(field->Container != nullptr) << " - Field Unordered Map Test 2" << std::endl;
	COUT_BOOL(field->Container->KeyType->Name == BH::TypeName<s32>()) << " - Field Unordered Map Test 3" << std::endl;
	COUT_BOOL(field->Container->ValueType->Name == BH::TypeName<std::string>()) << " - Field Unordered Map Test 4" << std::endl;

	ContainerTest cont;

	s32 t = 0;
	FOREACH_READ_FIELD_BEGIN(field->Container, it, &cont.UMap)
	{
		t += *(s32*)it->GetKey();
	}
	FOREACH_FIELD_END(field->Container, it);

	COUT_BOOL(t == cont.UnorderedTest) << " - Field Unordered Map Value Test" << std::endl;

	test.TearDown();
}

void TestReflection()
{
	std::cout << ConsoleYellow << "--------------------- Relection Test ------------------" << std::endl;

	TestTypeCName();
	TestTypeBase();
	TestEntityInheritance();
	TestStringify();
	TestEntityCreateDelete();
	TestField();
	TestFieldEnum();
	TestArrayContainer();
	TestVectorContainer();
	TestSetContainer();
	TestUnorderedSetContainer();
	TestMapContainer();
	TestUnorderedMapContainer();

	std::cout << std::endl << std::endl;
}

#undef COUT_BOOL

#endif