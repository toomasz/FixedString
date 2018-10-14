#ifndef _FIXED_STRING_H
#define _FIXED_STRING_H

#include <Arduino.h>

class FixedStringBase
{
public:
	virtual const unsigned int length() const = 0;
	virtual const unsigned int capacity() const = 0;

	virtual const char *c_str() const = 0;
	virtual bool append(const char *str, unsigned int len) = 0;
	virtual bool append(char c) = 0;

	virtual ~FixedStringBase() = default;
	virtual void clear() = 0;
};

extern bool FixedString_OverflowDetected;

template <unsigned int N>
class FixedString : public FixedStringBase
{
private:
    char _string[N+1]{};
	unsigned int _length;
	bool AssertSpaceAvailable(int requestedLengthToAdd)
	{
	    if (_length + requestedLengthToAdd > N)
		{
			FixedString_OverflowDetected = true;
			return false;
		}
		return true;
	}
public:

    FixedString()
    {
        clear();
    }

    FixedString(const char* str)
    {
        clear();
        append(str);
    }

    const char* c_str() const
    {
        return _string;
    }

    const unsigned int length() const
    {
        return _length;
    }

    const unsigned int capacity() const
    {
        return N;
    }

    const unsigned int freeBytes() const
    {
        return N-length();
    }

	FixedString(FixedStringBase& str)
	{
        clear();
        append(str.c_str(), str.length());
	}
	FixedString(const FixedString& str)
	{
	    clear();
        append(str._string, str._length);
	}

	const FixedString<N> operator + (FixedStringBase& FixedString1)
	{
		FixedString<N> FixedString;

		FixedString += *this;
		FixedString +=FixedString1;

		return  FixedString;
	}

	FixedString<N> operator + (const char* str)
	{
		FixedString<N> FixedString;
		FixedString +=*this;
		FixedString += str;
		
		return FixedString;
	}

    FixedString<N>& operator=(const char *str)
    {
        clear();
        append(str);
        return *this;
    }

    FixedString<N>& operator+=(const char *str)
    {
        append(str);
        return *this;
    }
    FixedString<N>& operator =(const FixedStringBase& str)
    {
        clear();
        append(str.c_str(), str.length());
        return *this;
    }
    FixedString<N>& operator +=(FixedStringBase& str)
    {
        append(str.c_str(), str.length());
        return *this;
    }

    void clear()
    {
        _length = 0;
        memset(_string, 0, N+1);
    }

    bool append(const char* str, unsigned int len)
    {
        if (!AssertSpaceAvailable(len))
		{
			return false;
		}
        memcpy(_string + _length, str, len);
        _length += len;
        return true;
    }

    bool append(const char* str)
    {
        const unsigned int len = strlen(str);
        return append(str, len);
    }

	bool append(char c)
	{
		if (!AssertSpaceAvailable(1))
		{
			return false;
		}
		_string[_length] = c;
		_length++;
		return true;
	}

	void appendFormat(const char *format, ...)
	{
	    va_list argptr;
        va_start(argptr, format);
		auto remainingSpace = freeBytes();
	    auto addedCharacters = vsnprintf(_string + _length, remainingSpace, format, argptr);
		_length += addedCharacters;
		va_end(argptr);
	}

	void debug()
	{
        printf("content: '%s' length: %d, free = %d\n", c_str(), length(), freeBytes());
        if(FixedString_OverflowDetected)
        {
            printf("Fatal error, string overun detected!\n");
        }
	}
};

#endif

