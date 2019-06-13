#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <iterator>
#include <string>

//using AccKey = ;

class account
{
	private:
		string name;
		int bank;
		size_t agency;
		size_t account_num;
		size_t balance;

	public:

	account();

	~account();
}

#endif
