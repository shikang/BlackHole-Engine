#ifndef TEST_HASH_H
#define TEST_HASH_H

#include "Core/Hash.h"
#include "Platform/ConsoleColor.h"

using namespace BH;

#define COUT_BOOL(exp)\
	{\
		bool condition = exp; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

// Test Hash
void GenerateHashTest()
{
	String testHashName = "TestHashName";
	Hash testHash = Hash("TestHashName");
	Hash newHash(testHashName.c_str());
	Hash wrongHash("Wrong hash!");

	COUT_BOOL(testHash.GetHash() == newHash.GetHash()) << " - Generate Hash Test 1" << std::endl;
	COUT_BOOL(testHash.GetHash() == Hash::Generate(testHashName.c_str())) << " - Generate Hash Test 2" << std::endl;
	COUT_BOOL(testHash.GetHash() != wrongHash.GetHash()) << " - Generate Hash Test 3" << std::endl;
	COUT_BOOL(testHash.GetHash() != 0) << " - Generate Hash Test 4" << std::endl;
}

void HashNameTest()
{
	String testHashName = "TestHashName";
	Hash testHash = Hash("TestHashName");

	for (size_t i = 0; i < 5; ++i)
	{
		Ostringstream o;
		o << "TestString" << i;

		Hash hash = Hash(o.str().c_str());
		COUT_BOOL(hash.GetText() == o.str()) << " - Hash Name Test " << i + 1 << std::endl;
	}
}

void TestHash()
{
	std::cout << ConsoleYellow << "--------------------- Hash Test ------------------" << std::endl;

	GenerateHashTest();
	HashNameTest();

	std::cout << std::endl << std::endl;
}

#undef COUT_BOOL

#endif