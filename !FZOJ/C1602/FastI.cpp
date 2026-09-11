#include <bits/stdc++.h>
namespace fasti {
    using namespace std;
	const int bufl = 1 << 20;
	const double base1[16] = {1, 1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9, 1e-10, 1e-11, 1e-12, 1e-13, 1e-14, 1e-15};
	const double base2[16] = {1, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15};
	struct IN {
		FILE *IT;
		char ibuf[bufl], *is = ibuf, *it = ibuf;
		IN() {
			IT = stdin;
		}
		IN(char *a) {
			IT = fopen(a, "r");
		}
		inline char getChar() {
			if (is == it) {
				it = (is = ibuf) + fread(ibuf, 1, bufl, IT);
				if (is == it)
					return EOF;
			}
			return *is++;
		}
		template <typename Tp>
		inline void getInt(Tp &a) {
			a = 0;
			int b = 0, c = getChar();
			while (c < 48 || c > 57)
				b ^= (c == 45), c = getChar();
			while (c >= 48 && c <= 57)
				a = (a << 1) + (a << 3) + c - 48, c = getChar();
			if (b)
				a = -a;
		}
		template <typename Tp>
		inline void getDouble(Tp &a) {
			a = 0;
			int b = 0, c = getChar(), d = 0;
			__int128 e = 0, f = 0;
			while (c < 48 || c > 57)
				b ^= (c == 45), c = getChar();
			while (c >= 48 && c <= 57)
				e = (e << 1) + (e << 3) + c - 48, c = getChar();
			if (c == 46) {
				c = getChar();
				while (c >= 48 && c <= 57)
					d++, f = (f << 1) + (f << 3) + c - 48, c = getChar();
			}
			a = e + base1[d] * f;
			if (b)
				a = -a;
		}
		IN &operator>>(char &a) {
			a = getChar();
			return *this;
		}
		IN &operator>>(char *a) {
			do {
				*a = getChar();
			} while (*a <= 32);
			while (*a > 32)
				*++a = getChar();
			*a = 0;
			return *this;
		}
		IN &operator>>(string &a) {
			a = " ";
			char b = getChar();
			while (b <= 32)
				b = getChar();
			while (b > 32)
				a += b, b = getChar();
			return *this;
		}
		IN &operator>>(int &a) {
			getInt(a);
			return *this;
		}
		IN &operator>>(long long &a) {
			getInt(a);
			return *this;
		}
		IN &operator>>(unsigned long long &a) {
			getInt(a);
			return *this;
		}
		IN &operator>>(__int128 &a) {
			getInt(a);
			return *this;
		}
		IN &operator>>(float &a) {
			getDouble(a);
			return *this;
		}
		IN &operator>>(double &a) {
			getDouble(a);
			return *this;
		}
		IN &operator>>(long double &a) {
			getDouble(a);
			return *this;
		}
	};
}
fasti::IN fin;