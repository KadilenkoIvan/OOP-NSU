#include "pch.h"
#include "../bit_array/bit_array.cpp"

TEST(TestCaseName, ConstructorsTest) {
	BitArray empty_array;
	EXPECT_EQ(empty_array.size(), 0);
	BitArray arr1(50);
	EXPECT_EQ(arr1.size(), 50);
	BitArray arr2(arr1);
	EXPECT_EQ(arr2.size(), 50);
	EXPECT_TRUE(true);
}

TEST(TestCaseName, SwapSetReset) {
	BitArray arr1(5);
	arr1.set();
	arr1.reset(2);
	EXPECT_TRUE(arr1.to_string() == "11011");

	BitArray arr2(5);
	arr2.set();
	arr2.reset();
	arr2.set(2);
	EXPECT_TRUE(arr2.to_string() == "00100");

	std::string str1 = arr1.to_string();
	std::string str2 = arr2.to_string();
	arr1.swap(arr2);
	EXPECT_TRUE(arr1.to_string() == str2);
	EXPECT_TRUE(arr2.to_string() == str1);
	EXPECT_TRUE(true);
}

TEST(TestCaseName, ResizeClearPush) {
	BitArray arr(5);
	arr.resize(3);
	EXPECT_EQ(arr.size(), 3);
	arr.resize(7, true);
	EXPECT_EQ(arr.size(), 7);
	EXPECT_TRUE(arr.to_string() == "0001111");	

	arr.push_back(false);
	EXPECT_EQ(arr.size(), 8);
	EXPECT_TRUE(arr.to_string() == "00011110");

	arr.clear();
	EXPECT_EQ(arr.size(), 0);
	std::cout << arr.to_string() << '\n';
	EXPECT_TRUE(arr.to_string() == "empty arr");

	EXPECT_TRUE(true);
}

TEST(TestCaseName, Bool) {
	BitArray arr1(5);
	arr1.clear();
	EXPECT_TRUE(arr1.empty());

	arr1.resize(5);
	EXPECT_TRUE(arr1.none());
	EXPECT_FALSE(arr1.any());

	arr1.set(2, true);
	EXPECT_TRUE(arr1.any());
	EXPECT_FALSE(arr1.none());

	BitArray arr2(5);
	arr2.set(2, true);
	EXPECT_TRUE(arr1 == arr2);
	EXPECT_FALSE(arr1 != arr2);

	EXPECT_TRUE(true);
}

TEST(TestCaseName, ReverseCountIdx) {
	BitArray arr(5);
	arr.set(2);
	EXPECT_TRUE(arr.to_string() == "00100");
	EXPECT_TRUE(arr[2]);
	EXPECT_EQ(arr.count(), 1);

	arr = ~arr;
	EXPECT_TRUE(arr.to_string() == "11011");
	EXPECT_FALSE(arr[2]);
	EXPECT_EQ(arr.count(), 4);

	EXPECT_TRUE(true);
}

TEST(TestCaseName, BoolOperations) {
	BitArray arr1(5);
	arr1.set(1);
	arr1.set(3);
	arr1.set(4);//01011

	BitArray arr2(5);
	arr2.set(0);
	arr2.set(1);//11000

	EXPECT_TRUE((arr1 & arr2).to_string() == "01000");
	EXPECT_TRUE((arr1 | arr2).to_string() == "11011");
	EXPECT_TRUE((arr1 ^ arr2).to_string() == "10011");

	EXPECT_TRUE(true);
}

TEST(TestCaseName, BitShifts) {
	BitArray arr(10);
	arr.set();
	EXPECT_TRUE((arr >> 5).to_string() == "0000011111");
	EXPECT_TRUE((arr << 5).to_string() == "1111100000");

	EXPECT_TRUE(true);
}

TEST(TestCaseName, Exeptions)
{
	EXPECT_ANY_THROW(BitArray a(-1));

	BitArray arr(10);
	EXPECT_ANY_THROW(arr.resize(-1));

	BitArray arr2(9);
	EXPECT_ANY_THROW(arr & arr2);
	EXPECT_ANY_THROW(arr | arr2);
	EXPECT_ANY_THROW(arr ^ arr2);
	EXPECT_ANY_THROW(arr &= arr2);
	EXPECT_ANY_THROW(arr |= arr2);
	EXPECT_ANY_THROW(arr ^= arr2);

	EXPECT_ANY_THROW(arr >>= -1);
	EXPECT_ANY_THROW(arr >> -1);
	EXPECT_ANY_THROW(arr <<= -1);
	EXPECT_ANY_THROW(arr << -1);

	EXPECT_ANY_THROW(arr.set(15));
	arr2.clear();
	EXPECT_ANY_THROW(arr2.set(0));
	EXPECT_ANY_THROW(arr.set(-1));

	EXPECT_ANY_THROW(arr[-1]);
	EXPECT_ANY_THROW(arr[15]);
	EXPECT_TRUE(true);
}