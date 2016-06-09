#ifndef TEST_VECTOR2F_H
#define TEST_VECTOR2F_H

#include "Platform/Vector2f.h"
#include "Platform/ConsoleColor.h"

#include <iostream>

#define COUT_BOOL(exp)\
	{\
		bool condition = exp; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

#define COUT_NEAR(num1, num2, ep)\
	{ \
		bool condition = fabs((num1) - (num2)) <= ep; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

void Vector2fOperationsTest()
{
	const BH::Real tolerance = 0.00001f;

	// Assert ZERO values.
	COUT_BOOL(BH::Vector2f::ZERO.x == 0.0f) << " - Vector2f ZERO Test 1" << std::endl;
	COUT_BOOL(BH::Vector2f::ZERO.y == 0.0f) << " - Vector2f ZERO Test 1" << std::endl;

	// Assert UNIT_X values.
	COUT_BOOL(BH::Vector2f::UNIT_X.x == 1.0f) << " - Vector2f UNIT x Test 1" << std::endl;
	COUT_BOOL(BH::Vector2f::UNIT_X.y == 0.0f) << " - Vector2f UNIT x Test 1" << std::endl;

	// Assert UNIT_Y values.
	COUT_BOOL(BH::Vector2f::UNIT_Y.x == 0.0f) << " - Vector2f UNIT y Test 1" << std::endl;
	COUT_BOOL(BH::Vector2f::UNIT_Y.y == 1.0f) << " - Vector2f UNIT y Test 1" << std::endl;

	BH::Vector2f vecTest(3.0f, 4.0f);

	// Length test.
	COUT_NEAR(vecTest.LengthSq(), 25.0f, tolerance) << " - Vector2f Squared Length Test" << std::endl;
	COUT_NEAR(vecTest.Length(), 5.0f, tolerance) << " - Vector2f Length Test" << std::endl;

	// Normalise test.
	COUT_NEAR(BH::Vector2f::Normalize(vecTest).Length(), 1.0f, tolerance) << " - Vector2f Normalize Test 1" << std::endl;

	vecTest.Normalize();
	COUT_NEAR(vecTest.Length(), 1.0f, tolerance) << " - Vector2f Normalize Test 2" << std::endl;

	// Distance test.
	BH::Vector2f dist1(10.0f, 16.0f);
	BH::Vector2f dist2(13.0f, 12.0f);

	COUT_NEAR(BH::Vector2f::DistanceSquared(dist1, dist2), 25.0f, tolerance) << " - Vector2f Squared Distance Test 2" << std::endl;
	COUT_NEAR(BH::Vector2f::Distance(dist1, dist2), 5.0f, tolerance) << " - Vector2f Distance Test 2" << std::endl;

	// Dot product test.
	BH::Vector2f dotProd(25.0f, 10.0f);
	BH::Vector2f dotProd2(1.0f, 13.5f);

	COUT_NEAR(dotProd.Dot(dotProd2), 25.0f + 135.0f, tolerance) << " - Vector2f Dot Product Test" << std::endl;

	// Cross product test.
	BH::Vector2f crossProd(15.0f, 12.0f);
	BH::Vector2f crossProd2(51.0f, 17.1f);

	COUT_NEAR(crossProd.ScalarTripleProduct(crossProd2), 15.0f * 17.1f - 12.0f * 51.0f, tolerance) << " - Vector2f Scalar Triple Product Test" << std::endl;

	// Vector operations test.
	BH::Vector2f vec1(3.0f, 12.5f);
	BH::Vector2f vec2(5.76f, 1.15f);
	BH::Vector2f compTest(vec1);

	COUT_BOOL(vec1 == compTest) << " - Vector2f == Test" << std::endl;
	COUT_BOOL(vec2 != compTest) << " - Vector2f != Test" << std::endl;

	BH::Vector2f addResult = vec1 + vec2;
	COUT_NEAR(addResult.x, 8.76f, tolerance) << " - Vector2f + Test 1" << std::endl;
	COUT_NEAR(addResult.y, 13.65f, tolerance) << " - Vector2f + Test 2" << std::endl;

	BH::Vector2f minusResult = vec1 - vec2;
	COUT_NEAR(minusResult.x, -2.76f, tolerance) << " - Vector2f - Test 1" << std::endl;
	COUT_NEAR(minusResult.y, 11.35f, tolerance) << " - Vector2f - Test 2" << std::endl;

	BH::Vector2f scaleResult = vec1 * 2.5123f;
	COUT_NEAR(scaleResult.x, 3.0f * 2.5123f, tolerance) << " - Vector2f * Test 1" << std::endl;
	COUT_NEAR(scaleResult.y, 12.5f * 2.5123f, tolerance) << " - Vector2f * Test 1" << std::endl;

	BH::Vector2f divResult = vec1 / 2.5123f;
	COUT_NEAR(divResult.x, 3.0f / 2.5123f, tolerance) << " - Vector2f / Test 1" << std::endl;
	COUT_NEAR(divResult.y, 12.5f / 2.5123f, tolerance) << " - Vector2f / Test 2" << std::endl;

	BH::Vector2f negativeResult = -vec1;
	COUT_NEAR(negativeResult.x, -3.0f, tolerance) << " - Vector2f -ve Test 1" << std::endl;
	COUT_NEAR(negativeResult.y, -12.5f, tolerance) << " - Vector2f -ve Test 2" << std::endl;

	vec1 += vec2;
	COUT_NEAR(vec1.x, 8.76f, tolerance) << " - Vector2f += Test 1" << std::endl;
	COUT_NEAR(vec1.y, 13.65f, tolerance) << " - Vector2f += Test 2" << std::endl;

	vec1 -= vec2;
	COUT_NEAR(vec1.x, 3.0f, tolerance) << " - Vector2f -= Test 1" << std::endl;
	COUT_NEAR(vec1.y, 12.5f, tolerance) << " - Vector2f -= Test 2" << std::endl;

	vec1 *= 2.5123f;
	COUT_NEAR(vec1.x, 3.0f * 2.5123f, tolerance) << " - Vector2f *= Test 1" << std::endl;
	COUT_NEAR(vec1.y, 12.5f * 2.5123f, tolerance) << " - Vector2f *= Test 2" << std::endl;

	vec1 /= 2.5123f;
	COUT_NEAR(vec1.x, 3.0f, tolerance) << " - Vector2f /= Test 1" << std::endl;
	COUT_NEAR(vec1.y, 12.5f, tolerance) << " - Vector2f /= Test 2" << std::endl;
}

void TestVector2f()
{
	std::cout << ConsoleYellow << "--------------------- Vector2f Test ------------------" << std::endl;

	Vector2fOperationsTest();

	std::cout << std::endl << std::endl;
}

#undef COUT_BOOL

#endif