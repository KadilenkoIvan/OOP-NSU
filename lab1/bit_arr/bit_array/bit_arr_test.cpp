#include <iostream>
#include "bit_array.h"

/*
ÇÍÀ×ÅÍÈß Â ß×ÅÉÊÀÕ ÕĞÀÍßÒÜÑß ÇÀÄÎÌ ÍÀÏÅĞÅÄ, Ò.Å. 1 = 1
*/
int main()
{
	//BitArray q;
	//std::cout << q.size();
	BitArray a(100, 0);
	std::cout << a.to_string() << '\n';
	a.set();
	std::cout << a.to_string() << '\n';
	a.reset();
	std::cout << a.to_string() << '\n';
	a.set(3, true);
	std::cout << a.to_string() << '\n';
	a.set();
	a.set(5, false);
	std::cout << a.to_string() << '\n';
	a.set(0, false);
	std::cout << a.to_string() << '\n';
	a.reset(1);
	std::cout << a.to_string() << '\n';
	a.set(0, true);
	a.set(99, false);
	a.set(98, false);
	std::cout << a.to_string() << '\n';
	std::cout << a.count() << '\n';
	std::cout << a[0] << '\n';
	std::cout << a[1] << '\n';
	std::cout << a[2] << '\n';
	std::cout << a[14] << '\n';
	std::cout << a[15] << '\n';
	std::cout << a[16] << '\n';
	std::cout << "wqeqwhkdhgasjkdhwqj\n";
	std::cout << a.to_string() << '\n';
	BitArray b(a);
	a.set(99, true);
	std::cout << a.to_string() << '\n';
	std::cout << b.to_string() << '\n';
	b = ~a;
	std::cout << b.to_string() << '\n';
	std::cout << "before swap\n" << a.to_string() << '\n';
	std::cout << b.to_string() << '\n';
	a.swap(b);
	std::cout << "after swap\n" << a.to_string() << '\n';
	std::cout << b.to_string() << '\n';
	std::cout << b.size() << '\n';
	std::cout << "\n\n" << b.to_string() << '\n';
	b.resize(130, false);
	std::cout << b.to_string() << '\n';
	b.set(130);
	b.set(127);
	std::cout << b.to_string() << '\n';
	std::cout << "\n-----------------------------------------------------------------------------\n";
	b.resize(100);
	a.set(10);
	a.set(13);
	a.set(15);
	std::cout << a.to_string() << '\n';
	std::cout << b.to_string() << '\n';
	b &= a;
	std::cout << b.to_string() << '\n';
	std::cout << "\n-----------------------------------------------------------------------------\n";
	b = a;
	std::cout << a.to_string() << '\n';
	std::cout << b.to_string() << '\n';
	std::cout << (a == b) << '\n';
	std::cout << (a != b) << '\n';
	std::cout << "\n-----------------------------------------------------------------------------\n";
	b = ~a;
	a.set(16);
	a.set(19);
	a.set(21);
	std::cout << a.to_string() << '\n';
	std::cout << b.to_string() << '\n';
	std::cout << (a & b).to_string() << '\n';
	std::cout << (a | b).to_string() << '\n';
	std::cout << (a ^ b).to_string() << '\n';
	std::cout << "\n-----------------------------------------------------------------------------\nÏğîâåğêà ñäâèãîâ\n";
	BitArray c(63);
	c.set();
	std::cout << c.to_string_full() << '\n';
	for (size_t i = 2; i < 65; i += 3)
		c.set(i, false);
	int t = 62;
	std::cout << c.to_string_full() << '\n';
	c >>= t;
	std::cout << c.to_string_full() << '\n';
}