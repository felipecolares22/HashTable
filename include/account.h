#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <iterator>
#include <string>
#include <tuple>


/*! \struct Account
	\brief Structure to save the data of an account.

*/
struct Account
{
		using AcctKey = std::tuple<std::string, int, int, int>; //!< alias

		std::string name; //!< Bank account owner's name.
		int bank; //!< Bank number.
		int agency; //!< Agency number.
		int account_num; //!< Account number.
		float m_balance; //!< Account's balance.

		//=== Methods
		/// Returns the key associated to an account.
		AcctKey get_key()
		{
			return std::make_tuple( name, bank, agency, account_num );
		}
		
		//== Operators
		/// Operator == overload to compare accounts.
		bool operator==( const Account &rhs ) const
		{
			return name == rhs.name and bank == rhs.bank and agency == rhs.agency and account_num == rhs.account_num and m_balance == rhs.m_balance;
 		}
};

/*! \struct KeyHash
	\brief Account's key generator.

*/
struct KeyHash
{
	int operator()( const Account::AcctKey& k_ ) const
	{
		return std::get<3>(k_);
	}
};

/*! \struct KeyEqual
	\brief Account's equality comparator.

*/
struct KeyEqual
{
	bool operator()( const Account::AcctKey& lhs, const Account::AcctKey& rhs )
	{
		return ( std::get<3>(lhs) == std::get<3>(rhs) );
	}
};

#endif
