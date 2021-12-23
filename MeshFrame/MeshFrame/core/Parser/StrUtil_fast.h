/*!
*      \file strutil.h
*      \brief std::string utilities
*      \date Documented on 10/08/2010
*
*/

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

// declaration
namespace strutilTetMesh {

	//std::ostringstream oss;
	//std::istringstream iss;

	inline std::string trimLeft(const std::string& str)
	{
		std::string t = str;
		t.erase(0, t.find_first_not_of(" \t\n\r"));
		return t;
	};

	inline std::string trimRight(const std::string& str)
	{
		std::string t = str;
		t.erase(t.find_last_not_of(" \t\n\r") + 1);
		return t;
	};

	inline std::string trim(const std::string& str)
	{
		std::string t = str;
		t.erase(0, t.find_first_not_of(" \t\n\r"));
		t.erase(t.find_last_not_of(" \t\n\r") + 1);
		return t;
	};

	inline std::string trim(const std::string& str, const std::string & delimitor)
	{
		std::string t = str;
		t.erase(0, t.find_first_not_of(delimitor));
		t.erase(t.find_last_not_of(delimitor) + 1);
		return t;
	};

	inline	std::string toLower(const std::string& str)
	{
		std::string t = str;
		transform(t.begin(), t.end(), t.begin(), tolower);
		return t;
	};

	inline  std::string toUpper(const std::string& str)
	{
		std::string t = str;
		transform(t.begin(), t.end(), t.begin(), toupper);
		return t;
	};

	inline  bool startsWith(const std::string& str, const std::string& substr)
	{
		return str.find(substr) == 0;
	};

	inline bool endsWith(const std::string& str, const std::string& substr)
	{
		return (str.rfind(substr) == (str.length() - substr.length()) && str.rfind(substr) >= 0);
	};

	inline bool equalsIgnoreCase(const std::string& str1, const std::string& str2)
	{
		return toLower(str1) == toLower(str2);
	};


	inline    std::string toString(const bool& value, std::ostringstream& oss)
	{
		oss.clear();
		oss.str("");
		oss << std::boolalpha << value;
		return oss.str();
	};

	template<bool> bool parseString(const std::string& str, std::istringstream& iss)
	{
		bool value;
		iss.clear();
		iss.str(str);
		iss >> std::boolalpha >> value;
		return value;
	};


	template<class T> T parseString(const std::string& str, std::istringstream& iss) {
		T value;
		iss.clear();
		iss.str(str);
		iss >> value;
		return value;
	};

	template<class T> T parseHexString(const std::string& str, std::istringstream& iss) {
		T value;
		iss.str(str);
		iss >>std::hex >> value;
		return value;
	};

	template<class T> std::string toString(const T& value, std::ostringstream& oss) {
		oss.clear();
		oss.str("");
		oss << value;
		return oss.str();
	};

	template<class T> std::string toHexString(const T& value, int width, std::ostringstream& oss) {
		oss.clear();
		oss.str("");
		oss << std::hex;
		if (width > 0) {
			oss << std::setw(width) << std::setfill('0');
		}
		oss << value;
		return oss.str();
	};

// Tokenizer class


	/*!
	*	\brief String Tokenizer
	*
	*	String tokenizer, which separate the whole string to tokens.
	*/
	class Tokenizer
	{
	public:
		Tokenizer(const std::string& str)
			: m_String(str), m_Offset(0), m_Delimiters("  ") {};

		Tokenizer(const std::string& str, const std::string& delimiters)
			: m_String(str), m_Offset(0), m_Delimiters(delimiters) {};

		bool nextToken() { return nextToken(m_Delimiters); };


		bool nextToken(const std::string& delimiters)
		{
			// find the start charater of the next token.
			size_t i = m_String.find_first_not_of(delimiters, m_Offset);
			if (i == std::string::npos) {
				m_Offset = m_String.length();
				return false;
			}

			// find the end of the token.
			size_t j = m_String.find_first_of(delimiters, i);
			if (j == std::string::npos) {
				m_Token = m_String.substr(i);
				m_Offset = m_String.length();
				return true;
			}

			// to intercept the token and save current position
			m_Token = m_String.substr(i, j - i);
			m_Offset = j;
			return true;
		};


		const std::string getToken() const { return m_Token; };

		/**
		* to reset the tokenizer. After reset it, the tokenizer can get
		* the tokens from the first token.
		*/
		void reset() { m_Offset = 0; };

	protected:
		size_t m_Offset;
		const std::string m_String;
		std::string m_Token;
		std::string m_Delimiters;
	};

	inline std::vector<std::string> split(const std::string& str, const std::string& delimiters)
	{
		std::vector<std::string> ss;

		Tokenizer tokenizer(str, delimiters);
		while (tokenizer.nextToken())
		{
			ss.push_back(tokenizer.getToken());
		}

		return ss;
	};

};

/*
struct string_token_iterator
: public std::iterator<std::input_iterator_tag, std::string>
{
public:
string_token_iterator() : str(0), start(0), end(0) {}
string_token_iterator(const std::string & str_, const char * separator_ = " ") :
separator(separator_),
str(&str_),
end(0)
{
find_next();
}
string_token_iterator(const string_token_iterator & rhs) :
separator(rhs.separator),
str(rhs.str),
start(rhs.start),
end(rhs.end)
{
}

string_token_iterator & operator++()
{
find_next();
return *this;
}

string_token_iterator operator++(int)
{
string_token_iterator temp(*this);
++(*this);
return temp;
}

std::string operator*() const
{
return std::string(*str, start, end - start);
}

bool operator==(const string_token_iterator & rhs) const
{
return (rhs.str == str && rhs.start == start && rhs.end == end);
}

bool operator!=(const string_token_iterator & rhs) const
{
return !(rhs == *this);
}

private:

void find_next(void)
{
start = str->find_first_not_of(separator, end);
if(start == std::string::npos)
{
start = end = 0;
str = 0;
return;
}

end = str->find_first_of(separator, start);
}

const char * separator;
const std::string * str;
std::string::size_type start;
std::string::size_type end;
};
*/

