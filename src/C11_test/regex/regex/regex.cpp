// regex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <regex>
#include <string>
#include <iostream>
using namespace std;

// +：one or more
// *：zero or more
// The regex_search function stops looking as soon as it finds a matching substring in the input sequence.
// If regex_search finds a match, it returns true.
// the pattern [[:alpha:]] matches any alphabetic character
void test_regex_search()
{
	string pattern("[^c]ei");
	pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
	regex r(pattern);
	
	smatch result;
	string test = "receipt friend keio theif  receive";
	if (regex_search(test, result, r))
		cout << result.str();
}
// regex::icase  大小写不敏感
//alnum：letters or digits

//\\. :Just as there are special characters in C++ (§ 2.1.3, p. 39), regular - expression
//languages typically also have special characters.For example, the dot(.) character
//usually matches any character.As we do in C++, we can escape the special nature
//of a character by preceding it with a backslash.Because the backslash is also a
//special character in C++, we must use a second backslash inside a string literal to
//indicate to C++ that we want a backslash.Hence, we must write \\.to represent a
//regular expression that will match a period.

//It is important to realize that the syntactic correctness of a regular
//expression is evaluated at run time.
void test_regex_icase()
{
	regex r("[[:alnum:]]+\\.(cpp|cxx|cc)$", regex::icase);
	smatch results;
	string filename;
	while (cin >> filename)
		if (regex_search(filename, results, r))
			cout << results.str() << endl;  // print the current match
}

void test_regex_err()
{
	try {
		// error: missing close bracket after alnum; the constructor will throw
		regex r("[[:alnum:]+\\.(cpp|cxx|cc)$", regex::icase);
	}
	catch (regex_error e)
	{
		cout << e.what() << "\ncode: " << e.code() << endl;
	}
}

//regex wregex
//smatch wsmatch cmatch wcmatch
//======================================
//string:		regex,smatch,ssub_match,sregex_iterator
//const char *:	regex,cmatch,csub_match,cregex_iterator
//wstring:		wregex,wsmatch,wssub_match,wsregex_iterator
//const wchar *:wregex,wcmatch,wcsub_match,wcregex_iterator


//The for loop iterates through each match to r inside file.The initializer in the for
//defines it and end_it.When we define it, the sregex_iterator constructor calls
//regex_search to position it on the first match in file.The empty sregex_iterator,
//end_it, acts as the off - the - end iterator.The increment in the for “advances” the
//iterator by calling regex_search.When we dereference the iterator, we get an smatch
//object representing the current match.We call the str member of the match to
//print the matching word.
void test_sregex_iterator()
{
	string file("receipt friend keio ie theif  receive");
	// find the characters ei that follow a character other than c
	string pattern("[^c]ei");
	// we want the whole word in which our pattern appears
	pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
	regex r(pattern, regex::icase); // we'll ignore case in doing the match
	// it will repeatedly call regex_search to find all matches in file
	for (sregex_iterator it(file.begin(), file.end(), r), end_it;
		it != end_it; ++it)
		cout << it->str() << endl; // matched word
	cout << "============================" << endl;

	for (sregex_iterator it(file.begin(), file.end(), r), end_it;
		it != end_it; ++it) {
		auto pos = it->prefix().length();   // size of the prefix
		pos = pos > 40 ? pos - 40 : 0;      // we want up to 40 characters
		cout << it->prefix().str().substr(pos)   // last part of the prefix
			<< "\n\t\t>>> " << it->str() << " <<<\n" // matched word
			<< it->suffix().str().substr(0, 40) // first part of the suffix
			<< endl;
	}

}


bool is_email_valid(const std::string& email)
{
	const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	return std::regex_match(email, pattern);
}


void test_regex_match()
{
	std::string email1 = "marius.bancila@domain.com";

	std::string email2 = "marius_ban_cila@domain.com";

	std::string email3 = "marius_b@domain.co.uk";

	std::string email4 = "marius@domain";



	std::cout << email1 << " : " << (is_email_valid(email1) ?
		"valid" : "invalid") << std::endl;

	std::cout << email2 << " : " << (is_email_valid(email2) ?
		"valid" : "invalid") << std::endl;

	std::cout << email3 << " : " << (is_email_valid(email3) ?
		"valid" : "invalid") << std::endl;

	std::cout << email4 << " : " << (is_email_valid(email4) ?
		"valid" : "invalid") << std::endl;

}
void show_ip_parts(const std::string& ip)
{
   // regular expression with 4 capture groups defined with
   // parenthesis (...)
   const std::regex pattern("(\\d{1,3}):(\\d{1,3}):(\\d{1,3}):(\\d{1,3})");
   // object that will contain the sequence of sub-matches
   std:: match_results<std::string::const_iterator> result;
   // match the IP address with the regular expression
   bool valid = std:: regex_match(ip, result, pattern);
   std::cout << ip << " \t: " << (valid ? "valid" : "invalid")
		  << " size= " << result.size()
             << std::endl;
   // if the IP address matched the regex, then print the parts
   if(valid)
   {
	   std::cout << "b1: " << result[0] << std::endl;
      std::cout << "b1: " << result[1] << std::endl;
      std::cout << "b2: " << result[2] << std::endl;
      std::cout << "b3: " << result[3] << std::endl;
      std::cout << "b4: " << result[4] << std::endl;
   }
}

void test_match_results()
{
	show_ip_parts("1:22:33:444");
	show_ip_parts("1:22:33:4444");
	show_ip_parts("100:200");
}


void test_regex_search2()
{
	const std::tr1::regex pattern("(\\w+day)");
	// the source text
	std::string weekend = "Saturday and Sunday";
	std::smatch result;
	bool match = std::regex_search(weekend, result, pattern);

	if (match)
	{
		std::cout << result[0] << std::endl;
		std::cout << result[1] << std::endl;
		std::cout << result[2] << std::endl;
		std::cout << result[3] << std::endl;
		std::cout << result.size() << std::endl;
	}

	std::cout << std::endl;
}

void test_sregex_token_iterator()
{
	// regular expression
	const std::regex pattern("\\w+day");
	// the source text
	std::string weekend = "Saturday and Sunday, but some Fridays also.";
	const std::sregex_token_iterator end;  //需要注意一下这里
	for (std::sregex_token_iterator i(weekend.begin(), weekend.end(), pattern); i != end; ++i)
	{
		std::cout << *i << std::endl;
	}
	std::cout << std::endl;
}

void test_regex_replace()
{
	// text to transform
	std::string text = "This is a element and this a unique ID.";

	// regular expression with two capture groups
	const std::regex pattern("(\\ba (a|e|i|u|o))+");

	// the pattern for the transformation, using the second
	// capture group
	std::string replace = "an $2";
	std::string newtext = std::regex_replace(text, pattern, replace);
	std::cout << newtext << std::endl;
	std::cout << std::endl;
}

std::string format_date(const std::string& date)
{
	// regular expression
	const std::regex pattern("(\\d{1,2})(\\.|-|/)(\\d{1,2})(\\.|-|/)(\\d{4})");

	// transformation pattern, reverses the position of all capture groups
	std::string replacer = "$5$4$3$2$1";

	// apply the tranformation
	return std::regex_replace(date, pattern, replacer);
}
void test_regex_replace2()
{
	std::string date1 = "1/2/2008";
	std::string date2 = "12.08.2008";

	std::cout << date1 << " -> " << format_date(date1) << std::endl;
	std::cout << date2 << " -> " << format_date(date2) << std::endl;
	std::cout << std::endl;
}

void test_regex_search3() {
	// "new" and "delete" 出现的次数是否一样？
	std::regex reg("(new)|(delete)");
	std::smatch m;
	std::string s =
		"Calls to new must be followed by delete. \
		Calling simply new results in a leak!";
		int new_counter = 0;
	int delete_counter = 0;
	std::string::const_iterator it = s.begin();
	std::string::const_iterator end = s.end();

	while (std::regex_search(it, end, m, reg))
	{
		cout << "1     "<<m[1].matched << endl;
		cout << "2     " << m[2].matched << endl;
		// 是 new 还是 delete?
		m[1].matched ? ++new_counter : ++delete_counter;
		it = m[0].second;
	}

	if (new_counter != delete_counter)
		std::cout << "Leak detected!\n";
	else
		std::cout << "Seems ok...\n";
	std::cout << std::endl;
}

void test_sregex_token_iterator2()
{
	regex reg("/");
	vector<std::string> vec;
	string s = "Split/Vulue/Teather/Neusoft/Write/By/Lanwei";
	sregex_token_iterator it(s.begin(), s.end(), reg, -1);//// -1逆向匹配,就是匹配除了'/'之外的
	sregex_token_iterator end;
	while (it != end)
		vec.push_back(*it++);
	copy(vec.begin(), vec.end(), ostream_iterator<std::string>(cout, "\n"));
}
int _tmain(int argc, _TCHAR* argv[])
{
	test_sregex_token_iterator2();
	//test_regex_search3();
	//test_regex_replace2();
	//test_sregex_token_iterator();
	//test_regex_search2();
	//test_match_results();
	//test_regex_match();
	//test_sregex_iterator();
	//test_regex_err();
	//test_regex_icase();
	//test_regex_search();
	return 0;
}

