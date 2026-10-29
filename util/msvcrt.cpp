#include "msvcrt.h"

#include <ctype.h>
#include <string.h>
#include <string>

#undef sprintf
#undef vsprintf
#undef snprintf
#undef vsnprintf
#undef _snprintf
#undef _vsnprintf
#undef _scprintf
#undef _vscprintf
#undef fprintf
#undef vfprintf
#undef printf
#undef rand
#undef srand
#undef strtod
#undef atof

namespace {

struct Decimal
{
	bool negative;
	int point;
	std::string digits;
};

Decimal decompose(double value)
{
	unsigned long long bits;
	memcpy(&bits, &value, sizeof(bits));

	Decimal result;
	result.negative = (bits >> 63) != 0;
	result.point = 1;

	const unsigned long long quiet = 0x8000000000000ULL;
	unsigned long long mantissa = bits & 0xfffffffffffffULL;
	if (((bits >> 52) & 0x7ff) == 0x7ff) {
		if (mantissa == 0) {
			result.digits = "1#INF";
		}
		else if (result.negative && (mantissa | quiet) == quiet) {
			result.digits = "1#IND";
		}
		else {
			result.digits = "1#QNAN";
		}
		return result;
	}

	if (value == 0) {
		result.point = 0;
		result.digits = "0";
		return result;
	}

	char text[40];
	::snprintf(text, sizeof(text), "%.16e", value < 0 ? -value : value);
	char* exponent = strchr(text, 'e');
	result.point = atoi(exponent + 1) + 1;
	result.digits.assign(1, text[0]);
	result.digits.append(text + 2, exponent);

	size_t last = result.digits.find_last_not_of('0');
	result.digits.erase(last == std::string::npos ? 1 : last + 1);
	return result;
}

std::string roundDigits(Decimal& value, int count)
{
	std::string result("0");
	size_t next = 0;
	for (int i = 0; i < count; ++i) {
		result += next < value.digits.size() ? value.digits[next++] : '0';
	}

	if (count >= 0 && next < value.digits.size() && value.digits[next] >= '5') {
		size_t i = result.size() - 1;
		while (result[i] == '9') {
			result[i--] = '0';
		}
		++result[i];
	}

	if (result[0] == '1') {
		++value.point;
		return result;
	}
	return result.substr(1);
}

std::string fixed(Decimal value, int precision, bool group)
{
	std::string digits = roundDigits(value, precision + value.point);
	std::string result;

	if (value.point <= 0) {
		result += '0';
	}
	else {
		result.append(digits, 0, value.point);
	}

	if (precision > 0 || group) {
		result += '.';
		if (value.point < 0) {
			int zeros = precision < -value.point ? precision : -value.point;
			result.append(zeros, '0');
			result.append(digits, 0, precision - zeros);
		}
		else {
			result.append(digits, value.point, std::string::npos);
		}
	}
	return result;
}

std::string exponent(int power, bool caps)
{
	char text[8];
	::snprintf(text, sizeof(text), "%c%c%03d", caps ? 'E' : 'e', power < 0 ? '-' : '+', power < 0 ? -power : power);
	return text;
}

std::string scientific(Decimal value, int precision, bool caps, bool group)
{
	bool nonzero = value.digits[0] != '0';
	std::string digits = roundDigits(value, precision + 1);

	std::string result(1, digits[0]);
	if (precision > 0 || group) {
		result += '.';
		result.append(digits, 1, precision);
	}
	return result + exponent(nonzero ? value.point - 1 : 0, caps);
}

std::string cropZeros(const std::string& text)
{
	size_t point = text.find('.');
	if (point == std::string::npos) {
		return text;
	}

	size_t stop = text.find_first_of("eE", point);
	size_t end = stop == std::string::npos ? text.size() : stop;
	size_t last = end;
	while (last > point + 1 && text[last - 1] == '0') {
		--last;
	}
	if (last == point + 1) {
		last = point;
	}
	return text.substr(0, last) + text.substr(end);
}

std::string general(Decimal value, int precision, bool caps, bool group)
{
	if (precision == 0) {
		precision = 1;
	}

	Decimal rounded = value;
	std::string digits = roundDigits(rounded, precision);
	digits.resize(precision);
	int power = rounded.point - 1;

	std::string result;
	if (power < -4 || power >= precision) {
		result.assign(1, digits[0]);
		if (precision > 1 || group) {
			result += '.';
			result.append(digits, 1, std::string::npos);
		}
		result += exponent(value.digits[0] != '0' ? power : 0, caps);
	}
	else if (rounded.point <= 0) {
		result = "0.";
		result.append(-rounded.point, '0');
		result += digits;
	}
	else {
		result.assign(digits, 0, rounded.point);
		if (precision > rounded.point || group) {
			result += '.';
			result.append(digits, rounded.point, std::string::npos);
		}
	}

	return group ? result : cropZeros(result);
}

struct Spec
{
	bool left;
	bool sign;
	bool space;
	bool zero;
	bool group;
	int width;
	int precision;
};

std::string pad(const Spec& spec, std::string body, bool numeric)
{
	std::string prefix;
	if (numeric) {
		if (!body.empty() && body[0] == '-') {
			prefix = "-";
			body.erase(0, 1);
		}
		else if (spec.sign) {
			prefix = "+";
		}
		else if (spec.space) {
			prefix = " ";
		}
	}

	int length = (int) (prefix.size() + body.size());
	if (spec.width <= length) {
		return prefix + body;
	}

	std::string fill(spec.width - length, ' ');
	if (spec.left) {
		return prefix + body + fill;
	}
	if (spec.zero && numeric) {
		return prefix + std::string(fill.size(), '0') + body;
	}
	return fill + prefix + body;
}

std::string formatDouble(const Spec& spec, char conversion, double value)
{
	Decimal decimal = decompose(value);
	int precision = spec.precision < 0 ? 6 : spec.precision;
	bool caps = conversion == 'E' || conversion == 'G';

	std::string body;
	if (conversion == 'f') {
		body = fixed(decimal, precision, spec.group);
	}
	else if (conversion == 'e' || conversion == 'E') {
		body = scientific(decimal, precision, caps, spec.group);
	}
	else {
		body = general(decimal, precision, caps, spec.group);
	}

	if (decimal.negative) {
		body.insert(body.begin(), '-');
	}
	return pad(spec, body, true);
}

std::string format(const char* format, va_list args)
{
	std::string out;

	for (const char* p = format; *p;) {
		if (*p != '%') {
			out += *p++;
			continue;
		}

		const char* start = p++;
		Spec spec = {false, false, false, false, false, 0, -1};

		for (;; ++p) {
			if (*p == '-') {
				spec.left = true;
			}
			else if (*p == '+') {
				spec.sign = true;
			}
			else if (*p == ' ') {
				spec.space = true;
			}
			else if (*p == '0') {
				spec.zero = true;
			}
			else if (*p == '#') {
				spec.group = true;
			}
			else {
				break;
			}
		}

		if (*p == '*') {
			spec.width = va_arg(args, int);
			if (spec.width < 0) {
				spec.left = true;
				spec.width = -spec.width;
			}
			++p;
		}
		else {
			while (isdigit((unsigned char) *p)) {
				spec.width = spec.width * 10 + (*p++ - '0');
			}
		}

		if (*p == '.') {
			++p;
			spec.precision = 0;
			if (*p == '*') {
				spec.precision = va_arg(args, int);
				++p;
			}
			else {
				while (isdigit((unsigned char) *p)) {
					spec.precision = spec.precision * 10 + (*p++ - '0');
				}
			}
		}

		enum Size
		{
			SIZE_INT,
			SIZE_SHORT,
			SIZE_LONG,
			SIZE_LONGLONG,
			SIZE_POINTER
		};
		Size size = SIZE_INT;
		if (*p == 'h') {
			size = SIZE_SHORT;
			++p;
		}
		else if (*p == 'l') {
			size = SIZE_LONG;
			if (*++p == 'l') {
				size = SIZE_LONGLONG;
				++p;
			}
		}
		else if (*p == 'L') {
			++p;
		}
		else if (*p == 'I') {
			if (p[1] == '6' && p[2] == '4') {
				size = SIZE_LONGLONG;
				p += 3;
			}
			else if (p[1] == '3' && p[2] == '2') {
				p += 3;
			}
			else {
				size = SIZE_POINTER;
				++p;
			}
		}

		char conversion = *p;
		if (!conversion) {
			out.append(start, p);
			break;
		}
		++p;

		switch (conversion) {
		case 'd':
		case 'i':
		case 'o':
		case 'u':
		case 'x':
		case 'X': {
			bool isSigned = conversion == 'd' || conversion == 'i';
			unsigned long long magnitude;
			bool negative = false;
			if (size == SIZE_LONGLONG || size == SIZE_POINTER) {
				long long v = size == SIZE_POINTER ? (long long) va_arg(args, ptrdiff_t) : va_arg(args, long long);
				negative = isSigned && v < 0;
				magnitude = negative ? 0ULL - (unsigned long long) v : (unsigned long long) v;
			}
			else if (size == SIZE_LONG) {
				long v = va_arg(args, long);
				negative = isSigned && v < 0;
				magnitude = negative   ? 0ULL - (unsigned long long) v
							: isSigned ? (unsigned long long) v
									   : (unsigned long) v;
			}
			else {
				int v = va_arg(args, int);
				if (size == SIZE_SHORT) {
					v = isSigned ? (int) (short) v : (int) (unsigned short) v;
				}
				negative = isSigned && v < 0;
				magnitude = negative ? 0ULL - (unsigned long long) (long long) v : (unsigned int) v;
			}

			char text[72];
			const char* layout = conversion == 'o'   ? "%llo"
								 : conversion == 'x' ? "%llx"
								 : conversion == 'X' ? "%llX"
													 : "%llu";
			::snprintf(text, sizeof(text), layout, magnitude);
			std::string digits(text);
			if (spec.precision >= 0) {
				if ((int) digits.size() < spec.precision) {
					digits.insert(0, spec.precision - digits.size(), '0');
				}
				if (spec.precision == 0 && magnitude == 0) {
					digits.clear();
				}
			}
			if (spec.group && magnitude != 0) {
				if (conversion == 'o' && digits[0] != '0') {
					digits.insert(0, "0");
				}
				else if (conversion == 'x') {
					digits.insert(0, "0x");
				}
				else if (conversion == 'X') {
					digits.insert(0, "0X");
				}
			}

			Spec numeric = spec;
			if (spec.precision >= 0) {
				numeric.zero = false;
			}
			if (!isSigned) {
				numeric.sign = false;
				numeric.space = false;
			}
			if (spec.group && (conversion == 'x' || conversion == 'X') && numeric.zero && !numeric.left &&
				magnitude != 0) {
				std::string body = digits.substr(2);
				int width = spec.width - 2;
				if ((int) body.size() < width) {
					body.insert(0, width - body.size(), '0');
				}
				out += digits.substr(0, 2) + body;
				break;
			}
			out += pad(numeric, negative ? "-" + digits : digits, true);
			break;
		}

		case 'c': {
			char c = (char) va_arg(args, int);
			Spec text = spec;
			text.zero = false;
			out += pad(text, std::string(1, c), false);
			break;
		}

		case 's': {
			const char* s = va_arg(args, const char*);
			if (!s) {
				s = "(null)";
			}
			size_t length = strlen(s);
			if (spec.precision >= 0 && (size_t) spec.precision < length) {
				length = spec.precision;
			}
			Spec text = spec;
			text.zero = false;
			out += pad(text, std::string(s, length), false);
			break;
		}

		case 'p': {
			char text[32];
			::snprintf(
				text,
				sizeof(text),
				"%0*llX",
				(int) (2 * sizeof(void*)),
				(unsigned long long) (size_t) va_arg(args, void*)
			);
			std::string digits(text);
			if (sizeof(void*) > 4) {
				size_t first = digits.find_first_not_of('0');
				size_t keep = first == std::string::npos ? 8 : digits.size() - first;
				if (keep < 8) {
					keep = 8;
				}
				digits = digits.substr(digits.size() - keep);
			}
			Spec text2 = spec;
			text2.zero = false;
			out += pad(text2, digits, false);
			break;
		}

		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
			out += formatDouble(spec, conversion, va_arg(args, double));
			break;

		case 'n':
			*va_arg(args, int*) = (int) out.size();
			break;

		case '%':
			out += '%';
			break;

		default:
			out.append(start, p);
			break;
		}
	}

	return out;
}

#ifdef _MSC_VER
__declspec(thread) unsigned int holdrand = 1;
#else
__thread unsigned int holdrand = 1;
#endif

bool exponentMarker(char c)
{
	return c == 'e' || c == 'E' || c == 'd' || c == 'D';
}

} // namespace

extern "C"
{

	int msvcrt_vsnprintf(char* buffer, size_t count, const char* format_, va_list args)
	{
		std::string text = format(format_, args);
		if (count > 0) {
			size_t length = text.size() < count - 1 ? text.size() : count - 1;
			memcpy(buffer, text.data(), length);
			buffer[length] = '\0';
		}
		return (int) text.size();
	}

	int msvcrt_snprintf(char* buffer, size_t count, const char* format_, ...)
	{
		va_list args;
		va_start(args, format_);
		int result = msvcrt_vsnprintf(buffer, count, format_, args);
		va_end(args);
		return result;
	}

	int msvcrt_vsprintf(char* buffer, const char* format_, va_list args)
	{
		std::string text = format(format_, args);
		memcpy(buffer, text.c_str(), text.size() + 1);
		return (int) text.size();
	}

	int msvcrt_sprintf(char* buffer, const char* format_, ...)
	{
		va_list args;
		va_start(args, format_);
		int result = msvcrt_vsprintf(buffer, format_, args);
		va_end(args);
		return result;
	}

	int msvcrt__vsnprintf(char* buffer, size_t count, const char* format_, va_list args)
	{
		std::string text = format(format_, args);
		if (text.size() > count) {
			memcpy(buffer, text.data(), count);
			return -1;
		}
		memcpy(buffer, text.data(), text.size());
		if (text.size() < count) {
			buffer[text.size()] = '\0';
		}
		return (int) text.size();
	}

	int msvcrt__snprintf(char* buffer, size_t count, const char* format_, ...)
	{
		va_list args;
		va_start(args, format_);
		int result = msvcrt__vsnprintf(buffer, count, format_, args);
		va_end(args);
		return result;
	}

	int msvcrt__vscprintf(const char* format_, va_list args)
	{
		return (int) format(format_, args).size();
	}

	int msvcrt__scprintf(const char* format_, ...)
	{
		va_list args;
		va_start(args, format_);
		int result = msvcrt__vscprintf(format_, args);
		va_end(args);
		return result;
	}

	int msvcrt_vfprintf(FILE* stream, const char* format_, va_list args)
	{
		std::string text = format(format_, args);
		return fwrite(text.data(), 1, text.size(), stream) == text.size() ? (int) text.size() : -1;
	}

	int msvcrt_fprintf(FILE* stream, const char* format_, ...)
	{
		va_list args;
		va_start(args, format_);
		int result = msvcrt_vfprintf(stream, format_, args);
		va_end(args);
		return result;
	}

	int msvcrt_printf(const char* format_, ...)
	{
		va_list args;
		va_start(args, format_);
		int result = msvcrt_vfprintf(stdout, format_, args);
		va_end(args);
		return result;
	}

	int msvcrt_rand(void)
	{
		holdrand = holdrand * 214013u + 2531011u;
		return (holdrand >> 16) & 0x7fff;
	}

	void msvcrt_srand(unsigned int seed)
	{
		holdrand = seed;
	}

	double msvcrt_strtod(const char* text, char** end)
	{
		const char* p = text;
		while (isspace((unsigned char) *p)) {
			++p;
		}

		std::string number;
		if (*p == '+' || *p == '-') {
			number += *p++;
		}

		bool digits = false;
		while (isdigit((unsigned char) *p)) {
			number += *p++;
			digits = true;
		}
		if (*p == '.') {
			number += *p++;
			while (isdigit((unsigned char) *p)) {
				number += *p++;
				digits = true;
			}
		}

		if (!digits) {
			if (end) {
				*end = const_cast<char*>(text);
			}
			return 0;
		}

		if (exponentMarker(*p)) {
			const char* q = p + 1;
			std::string power("e");
			if (*q == '+' || *q == '-') {
				power += *q++;
			}
			if (isdigit((unsigned char) *q)) {
				while (isdigit((unsigned char) *q)) {
					power += *q++;
				}
				number += power;
				p = q;
			}
		}

		if (end) {
			*end = const_cast<char*>(p);
		}
		return ::strtod(number.c_str(), NULL);
	}

	double msvcrt_atof(const char* text)
	{
		return msvcrt_strtod(text, NULL);
	}

} // extern "C"
