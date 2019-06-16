#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <iterator>
#include <string>
#include <tuple>


struct Account
{
	// public:
		using AcctKey = std::tuple<std::string, int, int, int>; //!< alias

		std::string name;
		int bank;
		int agency;
		int account_num;
		float m_balance;

	// public:

	//	Account( std::string name = "null", int bank = 0, int agency = 0, int account_num = 0, float m_balance = 0 )
	//	: name{name}, bank{bank}, agency{agency}, account_num{account_num}, m_balance{m_balance}
	//	{ /*empty*/
	//	//	std::cout << name << " " << bank << " " << agency << std::endl;
	//	}

	//	~Account() 
	//	{ /*empty*/ }

		AcctKey get_key()
		{
			return std::make_tuple( name, bank, agency, account_num );
		}

		bool operator==( const Account &rhs ) const
		{
			return name == rhs.name and bank == rhs.bank and agency == rhs.agency and account_num == rhs.account_num and m_balance == rhs.m_balance;
 		}
};

struct KeyHash
{
	int operator()( const Account::AcctKey& k_ ) const
	{
		return std::get<3>(k_);
	}
};

struct KeyEqual
{
	bool operator()( const Account::AcctKey& lhs, const Account::AcctKey& rhs )
	{
		return ( std::get<3>(lhs) == std::get<3>(rhs) );
	}
};

#endif
