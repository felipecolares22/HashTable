#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <string>
#include <iterator>
#include <initializer_list>
#include <math.h>
#include <forward_list>
#include <functional>
#include <tuple>
#include <stdexcept>


namespace ac
{
template < typename KeyType, typename DataType >
class HashEntry
{
	public:
		HashEntry ( KeyType k_, DataType d_ ) : m_key( k_ ), m_data( d_ )
		{  }
		KeyType m_key;
		DataType m_data;
}; // HashEntry Class

template < typename KeyType,
		   typename DataType,
		   typename KeyHash = std::hash<KeyType>,
		   typename KeyEqual = std::equal_to<KeyType> >
class HashTbl
{
	public:
		using Entry = HashEntry< KeyType, DataType >; //!< Alias

		HashTbl( size_t tbl_size_ = DEFAULT_SIZE )
		{
			tbl_size_ = next_prime( tbl_size_ );
			
			m_size = tbl_size_;
			m_count = 0;

			m_data_table = new std::forward_list< Entry >[tbl_size_];
		}

		virtual ~HashTbl()
		{
			delete [] m_data_table;
		}

		HashTbl( const HashTbl& other )
		{
			m_data_table = new std::forward_list< Entry >[ other.m_size ];

			m_size = other.m_size;
			m_count = other.m_count;

			for( size_t i = 0 ; i < m_size ; i++ )
			{
				auto otherIt = other.m_data_table[i].begin();

				while( otherIt != other.m_data_table[i].end() )
				{
					m_data_table[i].push_front( *otherIt );
					otherIt++;
				}
			}
		}

		HashTbl( std::initializer_list < Entry > ilist )
		{
			m_size = DEFAULT_SIZE;
			m_data_table = new std::forward_list< Entry >[ m_size ];

			for( const Entry & e : ilist )
			{
				insert( e.m_key, e.m_data );
			}
		}

		HashTbl& operator=( const HashTbl & other )
		{
			clear();
			delete [] m_data_table;
			m_data_table = new std::forward_list< Entry >[ other.m_size ];

			m_size = other.m_size;
			m_count = other.m_count;

			for( size_t i = 0 ; i < m_size ; i++ )
			{
				auto otherIt = other.m_data_table[i].begin();

				while( otherIt != other.m_data_table[i].end() )
				{
					m_data_table[i].push_front( *otherIt );
					otherIt++;
				}
			}

			return *this;
		}

		HashTbl& operator=( std::initializer_list < Entry > ilist )
		{
			clear();
			delete [] m_data_table;

			m_size = ilist.size();
			m_data_table = new std::forward_list< Entry >[ m_size ];
			m_count = 0;

			for( const Entry & e : ilist )
			{
				insert( e.m_key, e.m_data );
			}

			return *this;
		}

		bool insert ( const KeyType & k_, const DataType & d_ )
		{
			KeyHash hashFunc;
			KeyEqual equalFunc;
			Entry new_entry( k_, d_ );

			auto end = hashFunc( k_ ) % m_size;

			auto it = m_data_table[end].begin();

			while( it != m_data_table[end].end() )
			{
				if( equalFunc( it->m_key, new_entry.m_key ) )
				{
					it->m_data = d_;
					return false;
				}

				it++;
			}

			m_data_table[end].push_front( new_entry );
			m_count++;

			return true;
		}

		bool erase ( const KeyType & k_ )
		{
			KeyHash hashFunc;
			KeyEqual equalFunc;

			auto end = hashFunc( k_ ) % m_size;

			auto it = m_data_table[end].begin();
			auto first = it;

			while( it != m_data_table[end].end() )
			{
				if( equalFunc( it->m_key, k_ ) )
				{	
					if(it == first)
						m_data_table[end].pop_front();
					else
						m_data_table[end].erase_after(first);
					m_count--;
					return true;
				}

				it++;
				if(not (first == it))
					first++;
			}

			return false;
		}

		bool retrieve ( const KeyType & k_, DataType & d_ ) const
		{
			KeyHash hashFunc;
			KeyEqual equalFunc;

			auto end = hashFunc( k_ ) % m_size;
			
			auto it = m_data_table[end].begin();

			while( it != m_data_table[end].end() )
			{
				if(equalFunc( it->m_key, k_) )
				{
					d_ = it->m_data;
					return true;
				}

				it++;
			}

			return false;
		}

		void clear ( void )
		{
			m_count = 0;
			for( size_t i = 0 ; i < m_size ; i++ )
				m_data_table[i].clear();
		}

		bool empty ( void ) const
		{ return m_count == 0; }

		size_t size( void ) const
		{ return m_count; }

		DataType& at ( const KeyType& k_ )
		{
			KeyHash hashFunc;
			KeyEqual equalFunc;

			auto end = hashFunc( k_ ) % m_size;
			
			auto it = m_data_table[end].begin();

			while( it != m_data_table[end].end() )
			{
				if(equalFunc( it->m_key, k_) )
				{
					return it->m_data;
				}

				it++;
			}
			
			throw std::out_of_range("out of range, bro");
		}

		DataType& operator[]( const KeyType& k_ )
		{
			KeyHash hashFunc;
			KeyEqual equalFunc;

			auto end = hashFunc( k_ ) % m_size;
			
			auto it = m_data_table[end].begin();

			while( it != m_data_table[end].end() )
			{
				if(equalFunc( it->m_key, k_) )
				{
					return it->m_data;
				}

				it++;
			}
			

			DataType new_data = {0};
			insert( k_, new_data);
			auto it2 = m_data_table[end].begin();
			return it2->m_data; 
		}

		size_t count( const KeyType& k_ ) const
		{
			KeyHash hashFunc;

			auto end = hashFunc( k_ ) % m_size;			
			auto it = m_data_table[end].begin();

			size_t counter = 0u;

			while( it != m_data_table[end].end() )
			{
				counter++;
				it++;
			}

			return counter;
		}

		friend std::ostream& operator<< ( std::ostream & os, const HashTbl & tbl )
		{
			for(int i=0; i<tbl.prime_size; i++)
			{
				os << "[" << i << "]";
				for(auto it = tbl.m_data_table[i].begin(); it != tbl.m_data_table[i].end(); it++)
				{
					os << " -> " << it->m_data;
				}
				os << std::endl;
			}
			return os;
		}

	private:
		void rehash(  );
		size_t m_size = 0u;
		size_t m_count = 0u;
		std::forward_list< Entry > * m_data_table;
		static const short DEFAULT_SIZE = 11;

		size_t next_prime( size_t num )
		{
			if( num == 0 or num == 1)
				return num;

			while( true )
			{
				int numdiv = 0;
				for( size_t i = 2 ; i <= (size_t)sqrt(num) ; i++ )
					if( num % i == 0 )
						numdiv++;

				if( numdiv == 0 )
					return num;
				else
					num++;
			}
		}

}; // HashTbl class
} // ac Namespace

#endif
