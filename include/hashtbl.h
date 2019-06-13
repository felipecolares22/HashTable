#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <string>
#include <iterator>
#include <initializer_list>
#include <math.h>
#include <forward_list>
#include <functional>

template < typename KeyType, typename DataType >
class HashEntry
{
	public:
		HashEntry ( KeyType k_, DataType d_ ) : m_key( k_ ), m_data( d_ )
		{  }
		KeyType m_key;
		DataType m_data;
} // HashEntry Class

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

		HashTbl( const hashtbl& other )
		{
			m_data_table = new std::forward_list< Entry >[ other.m_size ];

			m_size = other.m_size;
			m_count = other.m_count;

			for( int i = 0 ; i < m_size ; i++ )
			{
				auto otherIt = other.m_data_table[i].begin();

				while( otherIt != other.m_data_table[i].end() )
				{
					m_data_table.push_front( *otherIt );
					otherIt++;
				}
			}
		}

		HashTbl( std::initializer_list < Entry > ilist )
		{
			for( Entry & e : ilist )
			{
				insert( e.m_key, e.m_data );
			}

			m_size = ilist.size();
		}

		HashTbl& operator=( const HashTbl & other )
		{
			clear();
			delete [] m_data_table;
			m_data_table = new std::forward_list< Entry >[ other.m_size ];

			m_size = other.m_size;
			m_count = other.m_count;

			for( int i = 0 ; i < m_size ; i++ )
			{
				auto otherIt = other.m_data_table[i].begin();

				while( otherIt != other.m_data_table[i].end() )
				{
					m_data_table.push_back( *otherIt );
					otherIt++;
				}
			}
		}

		HashTbl& operator=( std::initializer_list < Entry > ilist )
		{
			clear();
			delete [] m_data_table;

			m_size = ilist.size();
			m_data_table = new std::forward_list< Entry >[ m_size ];
			m_count = 0;

			for( Entry & e : ilist )
			{
				insert( e.m_key, e.m_data );
			}
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

		bool erase ( const KeyType & k_ );
		bool retrieve ( const KeyType & k_, DataType & d_ ) const;
		void clear ( void )
		{
			m_count = 0;
			for( int i = 0 ; i < m_size ; i++ )
				m_data_table[i].clear();
		}

		bool empty ( void ) const
		{ return m_count == 0; }

		size_t size( void ) const
		{ return m_size; }

		DataType& at ( const KeyType& k_ );
		DataType& operator[]( const KeyType& k_ );
		size_t count( const KeyType& k_ ) const;
		friend std::ostream& operator<< ( std::ostream & os, const HashTbl & htbl);

	private:
		void rehash(  );
		size_t m_size = 0u;
		size_t m_count = 0u;
		std::forward_list< Entry > * m_data_table;
		static const short DEFAULT_SIZE = 11;

		size_t next_prime( size_t num )
		{
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

} // HashTbl class


#endif
