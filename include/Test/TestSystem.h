#ifndef TEST_SYSTEM_H
#define TEST_SYSTEM_H

#include "Core/System.h"
#include "Core/SystemManager.h"
#include "Core/IUpdatable.h"
#include "Core/IDrawable.h"
#include "Core/GameComponent.h"
#include "Core/ObjectComponent.h"
#include "Core/Object.h"
#include "Core/ObjectManager.h"
#include "Core/TypeRegistrar.h"
#include "Core/TypeDB.h"
#include "Core/EntityManager.h"

#include "Platform/ConsoleColor.h"

#include <iostream>

#define COUT_BOOL(exp)\
	{\
		bool condition = exp; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

namespace BH
{
	struct DummyAccessSYS {};
	template<>
	DummyAccessSYS* EntityManager::New<DummyAccessSYS>() const
	{
		this->EntityManager::~EntityManager();
		this->EntityManager::EntityManager();
		//TypeRegistrar::RegisterTypes();
		return nullptr;
	}
}

void ForceReconstructEntityManager_SYS()
{
	BH::EntityManager* em = const_cast<BH::EntityManager*>(BH::EntityManager::InstancePtr());
	em->New<BH::DummyAccessSYS>();
}

class FirstGameComponent : public BH::GameComponent, public BH::IUpdatable, public BH::IDrawable
{
public:
	FirstGameComponent()
		: IUpdatable(0, true)
		, IDrawable(0, true) {}

	void Initialise()
	{
		std::cout << ConsoleBlue << "       FirstGameComponent Initialised!" << ConsoleWhite << std::endl;
	}

	void Shutdown()
	{
		std::cout << ConsoleBlue << "       FirstGameComponent Shutdown!" << ConsoleWhite << std::endl;
	}

	void Update(BH::f32 ) override
	{
		std::cout << ConsoleGreen << "PASS" << ConsoleWhite << " - System Updatable Test 1" << std::endl;
		mSystem->SetEnabled(this, false);
	}

	void Draw(BH::f32) override
	{
		std::cout << ConsoleGreen << "PASS" << ConsoleWhite << " - System Drawable Test 1" << std::endl;
		mSystem->SetVisible(this, false);
	}
};

class SecondGameComponent : public BH::GameComponent, public BH::IDrawable
{
public:
	SecondGameComponent()
		: IDrawable(3, false) {}

	void Initialise()
	{
		std::cout << ConsoleBlue << "       SecondGameComponent Initialised!" << ConsoleWhite << std::endl;
	}

	void Shutdown()
	{
		std::cout << ConsoleBlue << "       SecondGameComponent Shutdown!" << ConsoleWhite << std::endl;
	}

	void Draw(BH::f32 ) override
	{
		std::cout << ConsoleRed << "FAIL" << ConsoleWhite << " - System Drawable Test 3" << std::endl;
	}
};

class ThirdGameComponent : public BH::GameComponent, public BH::IUpdatable
{
public:
	ThirdGameComponent()
		: IUpdatable(2, true) {}

	void Initialise()
	{
		std::cout << ConsoleBlue << "       ThirdGameComponent Initialised!" << ConsoleWhite << std::endl;
	}

	void Shutdown()
	{
		std::cout << ConsoleBlue << "       ThirdGameComponent Shutdown!" << ConsoleWhite << std::endl;
	}

	void Update(BH::f32 ) override
	{
		std::cout << ConsoleGreen << "PASS" << ConsoleWhite << " - System Updatable Test 2" << std::endl;
		mSystem->SetEnabled(this, false);
	}
};

class FourthGameComponent : public BH::GameComponent, public BH::IUpdatable, public BH::IDrawable
{
public:
	FourthGameComponent()
		: IUpdatable(3, false)
		, IDrawable(2, true) {}

	void Initialise()
	{
		std::cout << ConsoleBlue << "       FourthGameComponent Initialised!" << ConsoleWhite << std::endl;
	}

	void Shutdown()
	{
		std::cout << ConsoleBlue << "       FourthGameComponent Shutdown!" << ConsoleWhite << std::endl;
	}

	void Update(BH::f32) override
	{
		std::cout << ConsoleRed << "FAIL" << ConsoleWhite << " - System Updatable Test 4" << std::endl;
	}

	void Draw(BH::f32) override
	{
		std::cout << ConsoleGreen << "PASS" << ConsoleWhite << " - System Drawable Test 2" << std::endl;
		mSystem->SetVisible(this, false);
	}
};

class FifthGameComponent : public BH::GameComponent
{
public:
	void Initialise()
	{
		std::cout << ConsoleBlue << "       FifthGameComponent Initialised!" << ConsoleWhite << std::endl;
	}

	void Shutdown()
	{
		std::cout << ConsoleBlue << "       FifthGameComponent Shutdown!" << ConsoleWhite << std::endl;
	}
};

#include "Core/Typing.h"
SYSTEM_ASSIGN_GAMECOMPONENT(FirstGameComponent, TSystem);
SYSTEM_ASSIGN_GAMECOMPONENT(SecondGameComponent, TSystem);
SYSTEM_ASSIGN_GAMECOMPONENT(ThirdGameComponent, TSystem);
SYSTEM_ASSIGN_GAMECOMPONENT(FourthGameComponent, TSystem);
SYSTEM_ASSIGN_GAMECOMPONENT(FifthGameComponent, TSystem);

class FirstObjectComponent : public BH::ObjectComponent, public BH::IUpdatable
{
public:
	// Constructor
	FirstObjectComponent()
		: IUpdatable(3, true)
		, mFirstVar(243.0f) {}

	void Update(BH::f32) override
	{
		std::cout << ConsoleGreen << "PASS" << ConsoleWhite << " - System Updatable Test 3" << std::endl;
		mSystem->SetEnabled(this, false);
	}
	
	GETSET_AUTO(BH::f32, FirstVar);

private:
	BH::f32 mFirstVar;
};

DECLARE_TYPE(FirstObjectComponent, FirstObjectComponent);
SYSTEM_ASSIGN_OBJCOMPONENT(FirstObjectComponent, TSystem);

class TSystem : public BH::System
{
public:
	TSystem( const BH::Char * name ) : System( name ) {}

	void Initialise() { BH::System::Initialise(); }

	void Work() { BH::System::Work(); }

	void Shutdown() { BH::System::Shutdown(); }

};

BH::ObjectManager sysObjManager;

class InitialiseDependency
{
public:
	InitialiseDependency()
	{
		SYSTEM_MANAGER.AddSystem<TSystem>("TSystem");

		SYSTEM_MANAGER.AddGameComponentToSystem<FourthGameComponent>();
		SYSTEM_MANAGER.AddGameComponentToSystem<FifthGameComponent>();
		SYSTEM_MANAGER.AddGameComponentToSystem<ThirdGameComponent>();
		SYSTEM_MANAGER.AddGameComponentToSystem<FirstGameComponent>();
		SYSTEM_MANAGER.AddGameComponentToSystem<SecondGameComponent>();

		SYSTEM_MANAGER.GameComponentDependency<ThirdGameComponent, FirstGameComponent>();
		SYSTEM_MANAGER.GameComponentDependency<ThirdGameComponent, SecondGameComponent>();
		SYSTEM_MANAGER.GameComponentDependency<FifthGameComponent, FirstGameComponent>();
		SYSTEM_MANAGER.GameComponentDependency<FifthGameComponent, ThirdGameComponent>();
		SYSTEM_MANAGER.GameComponentDependency<FourthGameComponent, ThirdGameComponent>();

		//testSystem.AddGameComponent<FourthGameComponent>("FourthGameComponent");
		//testSystem.AddGameComponent<FifthGameComponent>("FifthGameComponent");
		//testSystem.AddGameComponent<ThirdGameComponent>("ThirdGameComponent");
		//testSystem.AddGameComponent<FirstGameComponent>("FirstGameComponent");
		//testSystem.AddGameComponent<SecondGameComponent>("SecondGameComponent");

		//testSystem.Dependency("ThirdGameComponent", "FirstGameComponent");
		//testSystem.Dependency("ThirdGameComponent", "SecondGameComponent");
		//testSystem.Dependency("FifthGameComponent", "FirstGameComponent");
		//testSystem.Dependency("FifthGameComponent", "ThirdGameComponent");
		//testSystem.Dependency("FourthGameComponent", "ThirdGameComponent");
	}
};

InitialiseDependency initialiseDependency;

void SystemPrint1()
{
	std::cout << ConsoleGreen << "PASS" << ConsoleWhite << " - System Job No Parameters Test" << std::endl;
}

void SystemPrint2(const BH::Char * p1)
{
	std::cout << ConsoleGreen << "PASS" << ConsoleWhite << p1 << std::endl;
}

void SystemPrint3(const BH::Char * p1, const BH::Char * p2, const BH::Char * p3, const BH::Char * p4, const BH::Char * p5)
{
	std::cout << ConsoleGreen << "PASS" << ConsoleWhite << p1 << p2 << p3 << p4 << p5 << std::endl;
}

void TestSystem()
{
	std::cout << ConsoleYellow << "--------------------- System Test ------------------" << std::endl;

	SYSTEM_MANAGER.Initialise();

	BH::GameComponent * gettest = nullptr;
	gettest = SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")->GetGameComponent<FirstGameComponent>("FirstGameComponent");
	COUT_BOOL(gettest != nullptr) << " - System Get Component Test 1" << std::endl;
	gettest = SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")->GetGameComponent<ThirdGameComponent>("SecondGameComponent");
	COUT_BOOL(gettest == nullptr) << " - System Get Component Test 2" << std::endl;

	// Object Component test
	BH::TypeDB& typedb = ENTITY_MANAGER.Types;
	typedb.Create<FirstObjectComponent>();

	BH::Object * o = sysObjManager.Create("TestSystem");
	o->AddComponent<FirstObjectComponent>();
	//SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")->AddObjectComponent(o->GetComponent<FirstObjectComponent>());

	// Job test
	SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")->AddJob(1, 
															BH::Delegate<void(const BH::Char *, const BH::Char *, const BH::Char *, const BH::Char *, const BH::Char *)>::FromFunction<&SystemPrint3>(), 
															" - System ", "Job ", "Five ", "Parameters ", "Test");
	SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")->AddJob(0, 
															BH::Delegate<void()>::FromFunction<&SystemPrint1>());
	SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")->AddJob(0, 
															BH::Delegate<void(const BH::Char *)>::FromFunction<&SystemPrint2>(), 
															" - System Job One Parameters Test");
	//testSystem.Work();

	SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")->AddJob(0,
															BH::Delegate<void(BH::f32, bool)>::FromMethod<BH::System, &BH::System::UpdateUpdatables>(SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")),
															1.0f, true);

	while ( !SYSTEM_MANAGER.IsAllIdling() )
		BH::Thread::ThreadYield();

	SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")->AddJob(0,
															BH::Delegate<void(BH::f32, bool)>::FromMethod<BH::System, &BH::System::UpdateUpdatables>(SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")),
															1.0f, true);

	// Update test
	//testSystem.UpdateUpdatables(1.0f, true);
	//testSystem.Work();
	//testSystem.UpdateUpdatables(1.0f, true);
	//testSystem.Work();
	//testSystem.DrawDrawables(1.0f);
	//testSystem.DrawDrawables(1.0f);
	SYSTEM_MANAGER.Update(1.0f);

	SYSTEM_MANAGER.Draw(1.0f);
	SYSTEM_MANAGER.Draw(1.0f);

	// Request test
	while ( !SYSTEM_MANAGER.IsAllIdling() )
		BH::Thread::ThreadYield();
	BH::f32 fv = SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")->RequestJob(0, &FirstObjectComponent::GetFirstVar);
	COUT_BOOL(fv == 243.0f) << " - System Request Test 1" << std::endl;
	SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")->RequestJob<const BH::f32 &>(0, &FirstObjectComponent::SetFirstVar, 567.0f);
	fv = SYSTEM_MANAGER.GetSystem<BH::System>("TSystem")->RequestJob(0, &FirstObjectComponent::GetFirstVar);
	COUT_BOOL(fv == 567.0f) << " - System Request Test 2" << std::endl;

	sysObjManager.ForceFullPurge();
	ForceReconstructEntityManager_SYS();
	SYSTEM_MANAGER.Shutdown();

	std::cout << std::endl << std::endl;
}

#undef COUT_BOOL

#endif