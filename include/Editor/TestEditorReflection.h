#ifndef BH_TEST_EDITOR_REFLECTION_H
#define BH_TEST_EDITOR_REFLECTION_H

#include "Core/CoreEssential.h"
#include "Core/TypeCreator.h"
#include "Core/ObjectComponent.h"

// @test
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
	BH::f32 mX;
	BH::f32 mY;

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
	BH::f32 mZ;

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
	BH::String mMesh;
	std::vector<BH::String> mAnimations;
};

DECLARE_TYPE(TestPositionComponent, TestPositionComponent);
BEGIN_DEFINE_TYPE(TestPositionComponent)
	ENTITY_MANAGER.Types.Create<TestPositionComponent>()
						.FIELDM(X, TestPositionComponent)
						.FIELDM(Y, TestPositionComponent)
						.Base<ObjectComponent>();
END_DEFINE_TYPE(TestPositionComponent)

DECLARE_TYPE(Test3DPositionComponent, Test3DPositionComponent);
BEGIN_DEFINE_TYPE(Test3DPositionComponent)
	ENTITY_MANAGER.Types.Create<Test3DPositionComponent>()
						.FIELDM(Z, Test3DPositionComponent)
						.Base<TestPositionComponent>();
END_DEFINE_TYPE(Test3DPositionComponent)

DECLARE_TYPE(TestMeshComponent, TestMeshComponent);
BEGIN_DEFINE_TYPE(TestMeshComponent)
	ENTITY_MANAGER.Types.Create<TestMeshComponent>()
						.FIELDM(Mesh, TestMeshComponent)
						.FIELDM(Animations, TestMeshComponent)
						.Base<ObjectComponent>();
END_DEFINE_TYPE(TestMeshComponent)
// @end

#endif