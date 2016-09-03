
#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <cctype>
#include <cmath>
#include <fstream>

using namespace std;

enum Marker
{
	END, PRINT, CHARACTER, NUM
};

ifstream in;
ofstream out;
double numValue;
string strValue;
Marker marker = PRINT;

Marker GetCharac();
double Other(bool get);
double FirstOrder(bool get);
double SecondOrder(bool get);

Marker GetCharac()
{
		char ch;
		do {
			if (!in.get (ch))
				return marker = END;
		} while (ch != '\n' && isspace (ch));
		switch (ch) {
		case '\n':
			return marker = PRINT;
		case '^':
		case '*':
		case '/':
		case '+':
		case '-':
		case '(':
		case ')':
		case '=':
			return marker = Marker(ch);
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
			in.putback (ch);
			in >> numValue;
			return marker = NUM;
		default:
			if (isalpha(ch)) {
				strValue = ch;
				while (in.get (ch) && isalnum (ch))
					strValue.push_back(ch);
				in.putback(ch);
				return marker = CHARACTER;
			}
			cout<<"Error: bad tolken"<<endl;
			return marker = PRINT;
		}
}

double SecondOrder(bool get)
{
	double left = FirstOrder(get);
	for (;;)
		switch (marker) {
		case '+':
			{
				left += FirstOrder(true);
				break;
			}
		case '-':
			{
				left -= FirstOrder(true);
				break;
			}
		default:
			return left;
    }
}

double FirstOrder(bool get)
{
	double left = Other(get);
	for ( ; ; )
		switch (marker) {
		case '*': {
			left *= FirstOrder(true);
			break;
				  }
		case '/': {
			if (double d = Other(true)) {
				left /= d;
				break;
			}
			cout<<"Error: divide by 0"<<endl;
			return 1;
				  }
		case '^': {
			left = pow (left, Other(true));
			break;
				   }
		default:
			return left;
	}
}

double Other(bool get)
{
	if (get) 
		GetCharac();
	switch (marker) {
	case '-': 
		return (-SecondOrder(true));
	case NUM: {
		double num = numValue;
		GetCharac();
		return num;
			  }
	case '(': {
		double expression = SecondOrder(true);
		if (marker != ')')
		{
			cout<<"Error: ')' expected"<<endl;
			return 1;
		}
		GetCharac();
		return expression;
			  }
	default:
		{
			cout<<"Error: primary expected"<<endl;
			return 1;
		}
	}
}

void Replace(string& source, string const& find, string const& replace)
{
    for(string::size_type i = 0; (i = source.find(find, i)) != string::npos;)
    {
        source.replace(i, find.length(), replace);
        i += replace.length();
    }
}

void Rep()
{
	out.open("text.txt");
	string s;
	getline(cin, s);
	Replace(s, ",", ".");
	out<<s;
	out.close();
}

int main()
{
	Rep();
	in.open("text.txt");
	while (in) 
	{
		GetCharac();
		if (marker == END)
			break;
		if (marker == PRINT)
			continue;
		double answer = floor( SecondOrder(false) * 100 + .5 )/ 100; 
		cout << answer << '\n';
		system("pause");
	}
	in.close();
	
	return 0;
}
