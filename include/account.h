#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <iterator>
#include <string>
#include <tuple>


class Account
{
	public:
		using AcctKey = std::tuple<std::string, int, int, int>; //!< alias

		std::string name;
		int bank;
		size_t agency;
		size_t account_num;
		float m_balance;

	public:

		Account( std::string name, int bank, size_t agency, size_t account_num, float m_balance )
		: name{name}, bank{bank}, agency{agency}, account_num{account_num}, m_balance{m_balance}
		{ /*empty*/ }

		~Account() 
		{ /*empty*/ }

		AcctKey get_key()
		{
			return std::make_tuple( name, bank, agency, account_num );
		}
};

struct KeyHash
{
	std::size_t operator()( const Account::AcctKey& k_ ) const
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
