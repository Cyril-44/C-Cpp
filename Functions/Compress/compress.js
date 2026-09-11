
// Mivik 2020.5.15

// Inspired by wxh's code from (https://floj.tech/submission/24731)

var endl = '\n';

String.prototype.isSpace = function() {
	return this == ' ' || this == '\t' || this == '\n' || this == '\r' || this == '\v' || this == '\f';
}

String.prototype.isIdentifierPart = function() {
	return ('a' <= this && this <= 'z') || ('A' <= this && this <= 'Z') || ('0' <= this && this <= '9') || this == '$' || this == '_';
}

String.prototype.cut = function(lef, rig, ind) {
	return this.substring(lef[ind], rig[ind] + 1);
}

String.prototype.safeIndexOf = function(ch, ind) {
	while (ind != this.length && !(this[ind].isSpace())) {
		if (this[ind] == ch) return ind;
		++ind;
	}
	return -1;
}

function check(a, b) {
	return (a.isIdentifierPart() && b.isIdentifierPart()) ||
		   ((a == ' + ' || a == '-') && a == b) ||
		   (a == '/' && b == '*');
}

function processTrigraph(str) {
	var trigraphs = {
		'=': '#',
		'/': '\\',
		'\'': '^',
		'(': '[',
		')': ']',
		'<': '{',
		'>': '}',
		'!': '|',
		'-': '~'
	};
	for (ch in trigraphs) {
		var ret = '';
		var lst = 0;
		var pattern = "??" + ch;
		while (lst !== str.length) {
			var ind = str.indexOf(pattern, lst);
			if (ind === -1) break;
			ret += str.substring(lst, ind) + trigraphs[ch];
			lst = ind + pattern.length;
		}
		ret += str.substring(lst);
		str = ret;
	}
	return str;
}

function processLineBreak(str) {
	var lines = str.split(endl);
	var ret = '';
	for (var i = 0; i < lines.length; ++i) {
		var line = lines[i];
		var j = line.length;
		while ((--j) >= 0 && line[j].isSpace());
		if (line[j] == '\\') ret += line.substring(0, j);
		else ret += line + endl;
	}
	if (ret.length !== 0 && ret[ret.length - 1] == endl)
		ret = ret.substring(0, ret.length - 1);
	return ret;
}

function processMultilineCommentAndReplaceMark(str) {
	var ret = '';
	var stringBegin, esc = false;
	var inString = false;
	var lst = 0;
	for (var i = 0; i < str.length; ++i) {
		if (inString) {
			if ((!esc) && str[i] == stringBegin) inString = false;
		} else if (str[i] == '\'' || str[i] == '"') {
			inString = true;
			stringBegin = str[i];
		} else if (i + 1 < str.length) {
			if (str[i] == '/' && str[i + 1] == '*') {
				ret += str.substring(lst, i) + ' ';
				i = str.indexOf('*/', i + 2) + 2;
				lst = i;
				if (i == -1) break;
			} else if (str[i] == '%' && str[i + 1] == ':') {
				ret += str.substring(lst, i) + "#";
				lst = i += 2;
			}
		}
		esc = str[i] == '\\';
	}
	ret += str.substring(lst);
	return ret;
}

function compress(str) { return $compress($compress(str)); }

function $compress(str) {
	var ret = '';
	var lst = 0;
	var patternS = "R\"(";
	var patternT = ")\"";
	while (lst !== str.length) {
		var ind = str.indexOf(patternS, lst);
		if (ind === -1) break;
		ret += compressSingle(str.substring(lst, ind));
		var rig = str.indexOf(patternT, ind + patternS.length);
		if (rig === -1) {
			ret += str.substring(ind);
			lst = str.length;
			break;
		}
		lst = rig + patternT.length;
		ret += str.substring(ind, lst);
	}
	ret += compressSingle(str.substring(lst));
	return ret;
}

function compressSingle(str) {
	str = processTrigraph(str);
	str = processLineBreak(str);
	str = processMultilineCommentAndReplaceMark(str);

	var arr = str.split(endl);
	var ret = "";
	var forceNewline = true;
	var last = '\0';
	forLine: for (var w = 0; w < arr.length; ++w) {
		var line = arr[w];
		if (line.length === 0) continue;
		var i = 0;
		var lef = [];
		var rig = [];
		lex: while (true) {
			while (i != line.length && line[i].isSpace()) ++i;
			if (i === line.length) break;
			lef.push(i);
			var stringBegin, esc = false;
			var inString = false;
			while (i != line.length && (inString || (!(line[i].isSpace())))) {
				if (inString) {
					if ((!esc) && line[i] == stringBegin) inString = false;
				} else if (line[i] == '"' || line[i] == '\'') {
					inString = true;
					stringBegin = line[i];
				} else if (line[i] == '/' && i != line.length - 1) {
					if (line[i + 1] == '/') {
						rig.push(i - 1);
						lef.push(i);
						rig.push(line.length - 1);
						break lex;
					}
				}
				++i;
				if (i == line.length) break;
				esc = line[i] == '\\';
			}
			rig.push(i - 1);
		}
		var n = lef.length;
		if (!n) continue;
		var originForceNewline = forceNewline;
		var originLast = last;
		if (last != '\0') {
			if (forceNewline) ret += endl;
			else if (check(last, line[lef[0]])) ret += ' ';
		}
		last = line[rig[n - 1]];
		var j = 0;
		if (line[lef[0]] == '#') {
			if (!forceNewline) ret += endl;
			forceNewline = true;
			if (!line[lef[0] + 1]) line[--lef[j = 1]] = '#';
			ret += line.cut(lef, rig, j);
			if (line[lef[j] + 1] == 'd' && line.safeIndexOf('(', lef[j + 1]) === -1) {
				ret += ' ' + line.cut(lef, rig, ++j);
				if (j + 1 < n) ret += ' ' + line.cut(lef, rig, ++j);
			}
			++j;
		} else forceNewline = false;
		for (; j < n; ret += line.cut(lef, rig, j++)) {
			if (lef[j] + 2 <= line.length) {
				if (line.substring(lef[j], lef[j] + 2) == '//') {
					if (j == 0) forceNewline = originForceNewline;
					break;
				}
			}
			if (j && check(line[rig[j - 1]], line[lef[j]])) ret += ' ';
		}
		last = j? line[rig[j - 1]]: originLast;
	}
	return ret;
}

console.log(compress(`namespace fasti {
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
			a = "";
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
fasti::IN fin;`))