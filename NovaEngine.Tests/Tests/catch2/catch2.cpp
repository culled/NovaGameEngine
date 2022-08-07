#include <catch.hpp>

TEST_CASE("test/1=2", "Prove that 1 equals 2")
{
	int one = 2;
	REQUIRE(one == 2);
}

unsigned int Factorial(unsigned int number) {
    return number > 1 ? Factorial(number - 1) * number : 1;
}

TEST_CASE("Factorials are computed", "[factorial]") {
    REQUIRE(Factorial(0) == 1);
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}