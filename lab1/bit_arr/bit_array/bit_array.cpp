#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <span>
#include <bitset>

//#include "bit_array.h"

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

	~BitArray()//+
	{
		if (bit_size != 0)
			delete[] bit_arr;
	}

	//Конструирует массив, хранящий заданное количество бит.
	//Первые sizeof(long) бит можно инициализровать с помощью параметра value.
	explicit BitArray(int num_bits, unsigned long long value = 0)//+
	{
		if (num_bits < 0)
		{
			throw std::exception("wrong size\n");
			num_bits = 0;
		}
		size_t n = num_bits / (sizeof(bitword) * 8) + 1;
		bit_arr = new bitword[n]{};
		bit_size = num_bits;
		if (num_bits != 0)
			bit_arr[0] = value;
	}

	BitArray(const BitArray& b)//+
	{
		bit_size = 0;
		*this = b;
	}

	//Обменивает значения двух битовых массивов.
	void swap(BitArray& b)//+
	{
		BitArray buf(b);
		b = (*this);
		(*this) = buf;
	}
	BitArray& operator=(const BitArray& b)//+
	{
		if (bit_size != 0)
			delete[] bit_arr;
		bit_arr = new bitword[b.bit_size / (sizeof(bitword) * 8) + 1]{};
		bit_size = b.bit_size;
		memcpy(bit_arr, b.bit_arr, (b.bit_size / (sizeof(bitword) * 8) + 1) * sizeof(bitword));
		bit_size = b.bit_size;
		return *this;
	}

	//Изменяет размер массива. В случае расширения, новые элементы
	//инициализируются значением value.
	void resize(int num_bits, bool value = false)//+
	{
		if (num_bits < 0)
			throw std::exception("wrong num_bits\n");
		
		if (num_bits == 0)
		{
			this->clear();
			return;
		}
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
			if (num_bits < bit_size)
			{
				size_t start = bit_size;
				for (int i = num_bits; i >= start; i--)
					this->set(i, false);
				bit_size = num_bits;
			}
			return;
		}
		if (n < new_n)
		{
			bitword* new_bit_arr = new bitword[new_n]{};
			memcpy(new_bit_arr, bit_arr, sizeof(bitword) * n);
			delete[] bit_arr;
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
			delete[] bit_arr;
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
		//delete[] bit_arr;
		this->reset();
		bit_size = 0;
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
			throw std::exception("different sizes");
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
			throw std::exception("different sizes");
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
			throw std::exception("different sizes");
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
	//ДЕЛАТЬ В ДРУГУЮ СТОРОНУ
	BitArray& operator<<= (int n)//+
	{
		if (n < 0)
		{
			throw std::exception("wrong n\n");
			return *this;
		}
		if (n == 0)
			return *this;

		size_t elem_size = bit_size / (sizeof(bitword) * 8) + 1;
		if (n >= sizeof(bitword) * 8)
		{
			if (n >= bit_size)
			{
				this->reset();
				return *this;
			}

			int to_zero = n / (sizeof(bitword) * 8);
			int how_much = elem_size - to_zero;

			memmove(bit_arr, &bit_arr[to_zero], how_much * sizeof(bitword));
			for (int i = how_much; i < elem_size; ++i)
				bit_arr[i] = 0;
			n = n % (sizeof(bitword) * 8);
			elem_size = how_much;
		}

		bitword sub = ULLONG_MAX >> (sizeof(bitword) * 8) - n;
		bitword buf1 = 0, buf2 = 0;

		buf1 = (bit_arr[elem_size - 1] & sub) << ((sizeof(bitword) * 8) - n);
		bit_arr[elem_size - 1] = bit_arr[elem_size - 1] >> n;
		for (int i = elem_size - 2; i >= 0; i--)
		{
			buf2 = (bit_arr[i] & sub) << ((sizeof(bitword) * 8) - n);
			bit_arr[i] = bit_arr[i] >> n;
			bit_arr[i] += buf1;
			buf1 = buf2;
		}
		return *this;
	}

	BitArray& operator>>= (int n)//+
	{
		if (n < 0)
		{
			throw std::exception("wrong n\n");
			return *this;
		}
		if (n == 0)
			return *this;

		size_t elem_size = bit_size / (sizeof(bitword) * 8) + 1;
		int start = 0;

		if (n >= sizeof(bitword) * 8)
		{
			if (n >= bit_size)
			{
				this->reset();
				return *this;
			}

			int to_zero = n / (sizeof(bitword) * 8);
			int how_much = elem_size - to_zero;
			memmove(&bit_arr[to_zero], bit_arr, how_much * sizeof(bitword));
			for (int i = 0; i < to_zero; ++i)
				bit_arr[i] = 0;
			n = n % (sizeof(bitword) * 8);
			start = to_zero;
		}

		bitword sub = ULLONG_MAX << (sizeof(bitword) * 8) - n;
		bitword buf1 = 0, buf2 = 0;

		buf1 = (bit_arr[start] & sub) >> ((sizeof(bitword) * 8) - n);
		bit_arr[start] = bit_arr[start] << n;
		for (int i = start + 1; i < elem_size; i++)
		{
			buf2 = (bit_arr[i] & sub) >> ((sizeof(bitword) * 8) - n);
			bit_arr[i] = bit_arr[i] << n;
			bit_arr[i] += buf1;
			buf1 = buf2;
		}

		int reset_bits = elem_size * (sizeof(bitword) * 8) - bit_size;
		sub = ULLONG_MAX >> reset_bits;
		bit_arr[elem_size - 1] = bit_arr[elem_size - 1] & sub;

		return *this;
	}

	BitArray operator<<(int n) const//+
	{
		BitArray new_arr(*this);
		new_arr <<= n;
		return new_arr;
	}

	BitArray operator>>(int n) const//+
	{
		BitArray new_arr(*this);
		new_arr >>= n;
		return new_arr;
	}

	//Устанавливает бит с индексом n в значение val.
	BitArray& set(int n, bool val = true)//+
	{
		if (n < 0)
		{
			throw std::exception("wrong n\n");
			return *this;
		}
		if (n >= bit_size)
		{
			throw std::exception("wrong n\n");
			return *this;
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
		memset(bit_arr, 255, sizeof(bitword) * (n - 1));
		bitword sub = ULLONG_MAX >> (n * 8 * sizeof(bitword) - bit_size);
		bit_arr[n - 1] = bit_arr[n - 1] | sub;
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
		if (i < 0 || i >= bit_size)
		{
			throw std::exception("wrong idx\n");
			return false;
		}

		size_t ind_in_arr = i / (sizeof(bitword) * 8);
		size_t ind_in_elem = i % (sizeof(bitword) * 8);
		bitword sub = (bitword)1 << (ind_in_elem);
		if ((bit_arr[ind_in_arr] & sub) != 0)
			return true;
		return false;
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
		if (bit_size <= 0)
		{
			str = "empty arr";
			return str;
		}
		
		bitword sub = 1;
		for (bitword i = 0; i < bit_size; ++i)
		{
			if (((bit_arr[i / (sizeof(bitword) * 8)] >> i) & sub) != 0)
				str += '1';
			else
				str += '0';
		}
		return str;
	}

	std::string to_string_full() const//+
	{
		std::string str = "";
		bitword sub = 1;
		size_t elems = bit_size / (sizeof(bitword) * 8) + 1;
		for (bitword i = 0; i < bit_size; ++i)
		{
			if (i % 64 == 0)
				str += '.';
			if (((bit_arr[i / (sizeof(bitword) * 8)] >> i) & sub) != 0)
				str += '1';
			else
				str += '0';
		}
		str += '|';
		for (bitword i = bit_size; i < elems * (sizeof(bitword) * 8); ++i)
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
	return (new_array &= b2);
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