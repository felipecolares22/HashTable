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
		size_type agency;
		size_type account_num;
		size_type balance;

	public:

	account();

	~account();
}

#endif
