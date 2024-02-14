#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <span>

//В этой задаче для простоты не требуется делать контейнер шаблонным,
//но это вполне допускается по желанию студента.
class BitArray
{
	typedef unsigned long long bitword;
private:
	bitword* bit_arr;
	size_t bit_size;
public:
	//typedef unsigned short bitword;

	BitArray()//+
	{
		bit_arr = new bitword[0];
		bit_size = 0;
	}

	~BitArray() = default;

	//Конструирует массив, хранящий заданное количество бит.
	//Первые sizeof(long) бит можно инициализровать с помощью параметра value.
	explicit BitArray(int num_bits, unsigned long value = 0)//+
	{
		size_t n = num_bits / (sizeof(bitword) * 8) + 1;
		bit_arr = new bitword[n]{};
		bit_size = num_bits;
	}

	BitArray(const BitArray& b)//+
	{
		bit_arr = new bitword[0];
		bit_size = 0;
		*this = b;
	}

	//Обменивает значения двух битовых массивов.
	void swap(BitArray& b)//+
	{
		BitArray buf(b);
		b = *this;
		*this = buf;
	}

	BitArray& operator=(const BitArray& b)//+
	{
		memcpy(bit_arr, b.bit_arr, (b.bit_size / (sizeof(bitword) * 8) + 1) * sizeof(bitword));
		bit_size = b.bit_size;
		return *this;
	}

	//Изменяет размер массива. В случае расширения, новые элементы
	//инициализируются значением value.
	void resize(int num_bits, bool value = false)//+
	{
		if (bit_size == num_bits)
		{
			std::cout << "Same size\n";
			return;
		}
		size_t n = bit_size / (sizeof(bitword) * 8) + 1;//current number of elements in long array
		size_t new_n = num_bits / (sizeof(bitword) * 8) + 1;//new number of elements in long array
		if (n == new_n)
		{
			if (num_bits > bit_size)
			{
				size_t start = bit_size;
				bit_size = num_bits;
				for (int i = start; i < num_bits; ++i)
					this->set(i, value);
			}
			return;
		}
		if (n < new_n)
		{
			bitword* new_bit_arr = new bitword[new_n]{};
			memcpy(new_bit_arr, bit_arr, sizeof(bitword) * n);
			bit_arr = nullptr;
			bit_arr = new bitword[new_n];
			memcpy(bit_arr, new_bit_arr, sizeof(bitword) * n);
			delete[] new_bit_arr;
			size_t start = bit_size;
			bit_size = num_bits;
			for (int i = start; i < num_bits; ++i)
				this->set(i, value);
			return;
		}
		if (n > new_n)
		{
			bitword* new_bit_arr = new bitword[new_n]{};
			memcpy(new_bit_arr, bit_arr, sizeof(bitword) * new_n);
			bit_arr = nullptr;
			bit_arr = new bitword[new_n];
			memcpy(bit_arr, new_bit_arr, sizeof(bitword) * new_n);
			delete[] new_bit_arr;
			bit_size = num_bits;
			return;
		}
	}

	//Очищает массив.
	void clear()//+
	{
		size_t n = bit_size / (sizeof(bitword) * 8) + 1;
		memset(bit_arr, 0, sizeof(bitword) * n);
	}

	//Добавляет новый бит в конец массива. В случае необходимости
	//происходит перераспределение памяти.
	void push_back(bool bit)//+
	{
		this->resize(bit_size + 1, bit);
	}

	//Битовые операции над массивами.
	//Работают только на массивах одинакового размера.
	//Обоснование реакции на параметр неверного размера входит в задачу.
	BitArray& operator&=(const BitArray& b)//+
	{
		if (bit_size != b.bit_size)
		{
			std::cout << "different sizes\n";
			return *this;
		}
		size_t n = bit_size / (sizeof(bitword) * 8) + 1;
		for (int i = 0; i < n; ++i)
		{
			bit_arr[i] = bit_arr[i] & b.bit_arr[i];
		}
		return *this;
	}

	BitArray& operator|=(const BitArray& b)//+
	{
		if (bit_size != b.bit_size)
		{
			std::cout << "different sizes\n";
			return *this;
		}
		size_t n = bit_size / (sizeof(bitword) * 8) + 1;
		for (int i = 0; i < n; ++i)
		{
			bit_arr[i] = bit_arr[i] | b.bit_arr[i];
		}
		return *this;
	}

	BitArray& operator^=(const BitArray& b)//+
	{
		if (bit_size != b.bit_size)
		{
			std::cout << "different sizes\n";
			return *this;
		}
		size_t n = bit_size / (sizeof(bitword) * 8) + 1;
		for (int i = 0; i < n; ++i)
		{
			bit_arr[i] = bit_arr[i] ^ b.bit_arr[i];
		}
		return *this;
	}

	//Битовый сдвиг с заполнением нулями.
	BitArray& operator<<= (int n)
	{

	}

	BitArray& operator>>= (int n)
	{

	}

	BitArray operator<<(int n) const
	{

	}

	BitArray operator>>(int n) const
	{

	}

	//Устанавливает бит с индексом n в значение val.
	BitArray& set(int n, bool val = true)//+
	{
		if (n >= bit_size)
		{
			std::cout << "out of bounds\n";
		}
		else
		{
			long long ind_in_bit_arr = n / (sizeof(bitword) * 8);
			int ind = n % (sizeof(bitword) * 8);
			if (val)
			{
				bit_arr[ind_in_bit_arr] = bit_arr[ind_in_bit_arr] | ((bitword)1 << (ind));
			}
			else
			{
				bitword sub = ULLONG_MAX - ((bitword)1 << (ind));
				bit_arr[ind_in_bit_arr] = bit_arr[ind_in_bit_arr] & sub;
			}
		}
		return *this;
	}

	//Заполняет массив истиной.
	BitArray& set()//+
	{
		size_t n = bit_size / (8 * sizeof(bitword)) + 1;
		memset(bit_arr, 255, sizeof(bitword) * n);
		return *this;
	}

	//Устанавливает бит с индексом n в значение false.
	BitArray& reset(int n)//+
	{
		return this->set(n, false);
	}

	//Заполняет массив ложью.
	BitArray& reset()//+
	{
		size_t n = bit_size / (8 * sizeof(bitword)) + 1;
		memset(bit_arr, 0, sizeof(bitword) * n);
		return *this;
	}

	//true, если массив содержит истинный бит.
	bool any() const//+
	{
		size_t n = bit_size / (8 * sizeof(bitword)) + 1;
		for (size_t i = 0; i < n; ++i)
		{
			if (bit_arr[i] != 0)
				return true;
		}
		return false;
	}

	//true, если все биты массива ложны.
	bool none() const//+
	{
		return !(this->any());
	}

	//Битовая инверсия
	BitArray operator~() const//+
	{
		BitArray new_arr(*this);
		bitword sub = ULLONG_MAX;
		size_t n = bit_size / (sizeof(bitword) * 8) + 1;
		for (size_t i = 0; i < n; ++i)
		{
			new_arr.bit_arr[i] = bit_arr[i] ^ sub;
		}
		return new_arr;
	}

	//Подсчитывает количество единичных бит.
	int count() const//+
	{
		int cnt = 0;
		bitword sub = (bitword)1;
		for (size_t i = 0; i < bit_size; ++i)
		{
			if ((bit_arr[i / (sizeof(bitword) * 8)] & sub) != 0)
				++cnt;
			sub = sub << 1;
		}
		return cnt;
	}

	//Возвращает значение бита по индексу i.
	bool operator[](int i) const//+
	{
		if (i >= bit_size)
		{
			std::cout << "index out of bounds" << '\n';
			return false;
		}
		else
		{
			size_t ind_in_arr = i / (sizeof(bitword) * 8);
			size_t ind_in_elem = i % (sizeof(bitword) * 8);
			bitword sub = (bitword)1 << (ind_in_elem);
			if ((bit_arr[ind_in_arr] & sub) != 0)
				return true;
			return false;
		}
	}

	int size() const//+
	{
		return bit_size;
	}

	bool empty() const//+
	{
		return bit_size == 0;
	}

	//Возвращает строковое представление массива.
	std::string to_string() const//+
	{
		std::string str = "";
		bitword sub = (bitword)1;
		for (size_t i = 0; i < bit_size; ++i)
		{
			if (((bit_arr[i / (sizeof(bitword) * 8)] >> i) & sub) != 0)
				str += '1';
			else
				str += '0';
		}
		return str;
	}
};

bool operator==(const BitArray& a, const BitArray& b)//+
{
	return a.to_string() == b.to_string();
}

bool operator!=(const BitArray& a, const BitArray& b)//+
{
	return a.to_string() != b.to_string();
}

BitArray operator&(const BitArray& b1, const BitArray& b2)//+
{
	BitArray new_array(b1);
	return new_array &= b2;
}

BitArray operator|(const BitArray& b1, const BitArray& b2)//+
{
	BitArray new_array(b1);
	return new_array |= b2;
}

BitArray operator^(const BitArray& b1, const BitArray& b2)//+
{
	BitArray new_array(b1);
	return new_array ^= b2;
}


/*
ЗНАЧЕНИЯ В ЯЧЕЙКАХ ХРАНЯТЬСЯ ЗАДОМ НАПЕРЕД, Т.Е. 1 = 1
*/
int main()
{
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
}