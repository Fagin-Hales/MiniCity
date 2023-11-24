#include "../Game/catch_amalgamated.hpp"
#include "../Game/FVector.h"

TEST_CASE("Test that the container is empty after construction")
{
	FVector<int> myVector{};
	REQUIRE(myVector.size() == 0);
}

TEST_CASE("Test that the size and capasity increases as items are added")
{
	FVector<float> myVector{};
	myVector.push(5.2f);
	REQUIRE(myVector.size() == 1);
	REQUIRE(myVector.capacity() >= 1);
	myVector.push(2.1f);
	myVector.push(2.1f);
	myVector.push(2.1f);
	REQUIRE(myVector.size() == 4);
	REQUIRE(myVector.capacity() >= 4);
}

TEST_CASE("Test that the container inserts elements correctly")
{
	FVector<bool> myVector{};
	myVector.insert(true, 4);
	REQUIRE(myVector.size() == 5);
	REQUIRE(myVector.capacity() >= 5);
	myVector.insert(false, 0);
	REQUIRE(myVector.size() == 6);
	REQUIRE(myVector.capacity() >= 6);
	myVector.insert(true, 6);
	REQUIRE(myVector.size() == 7);
	REQUIRE(myVector.capacity() >= 7);
}

TEST_CASE("Test that the container stores and acces elements correctly")
{
	FVector<int> myVector{};
	myVector.push(1);
	myVector.push(10000);
	myVector.insert(5, 2);
	REQUIRE(myVector[0] == 1);
	REQUIRE(myVector[1] == 10000);
	REQUIRE(myVector[2] == 5);
	myVector.insert(3, 1);
	REQUIRE(myVector[1] == 3);
	REQUIRE(myVector[2] == 10000);
}


TEST_CASE("Test that the container removes elements correctly")
{
	FVector<int> myVector{};
	myVector.push(1);
	myVector.push(2);
	myVector.push(3);
	myVector.push(4);
	REQUIRE(myVector.size() == 4);
	myVector.remove(3);
	REQUIRE(myVector.size() == 3);
	myVector.remove(0);
	REQUIRE(myVector.size() == 2);
	REQUIRE(myVector[0] == 2);
	REQUIRE(myVector[1] == 3);
}

TEST_CASE("Test that the container clears correctly")
{
	FVector<int> myVector{};
	myVector.push(1);
	myVector.push(2);
	myVector.push(3);
	myVector.push(4);
	REQUIRE(myVector.size() == 4);
	myVector.clear();
	REQUIRE(myVector.size() == 0);
}

TEST_CASE("Test that the container itterators work correctly")
{
	FVector<int> myVector{};
	myVector.push(3);
	myVector.push(2);
	myVector.push(1);
	myVector.push(0);
	int value = 3;
	for (auto it = myVector.begin(); it != myVector.end(); it++)
	{
		REQUIRE(*it == value--);
	}
	std::sort(myVector.begin(), myVector.end());
	value = 0;
	for (auto it = myVector.begin(); it != myVector.end(); it++)
	{
		REQUIRE(*it == value++);
	}
}

TEST_CASE("Test that the container copies correctly")
{
	FVector<int> myVector{};
	myVector.push(0);
	myVector.push(1);
	myVector.push(2);
	myVector.push(3);
	FVector<int> myVector2{myVector};
	FVector<int> myVector3{};
	myVector3 = myVector;

	REQUIRE(myVector2.size() == 4);
	REQUIRE(myVector3.size() == 4);

	myVector[0] = 50;
	myVector[1] = 50;
	myVector[2] = 50;
	myVector[3] = 50;

	for (int i = 0; i < 4; i++)
	{
		REQUIRE(myVector2[i] == i);
		REQUIRE(myVector3[i] == i);
	}
}