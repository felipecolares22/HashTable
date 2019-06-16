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

/*! \namespace ac
	\brief namespace to differ from std.

*/
namespace ac
{
	/*! \class HashEntry
		\brief List element, containing a Key and a Data variable.

	*/
	template < typename KeyType, typename DataType >
	class HashEntry
	{
		public:
			HashEntry ( KeyType k_, DataType d_ ) : m_key( k_ ), m_data( d_ )
			{  }
			KeyType m_key; //!< Variable that stores the Key.
			DataType m_data; //!< Variable that stores any data.
	}; // HashEntry Class

	template < typename KeyType,
			   typename DataType,
			   typename KeyHash = std::hash<KeyType>,
			   typename KeyEqual = std::equal_to<KeyType> >
	class HashTbl
	{
		public:
			using Entry = HashEntry< KeyType, DataType >; //!< Alias
			
			//== Constructors
			/// Constructor with a defined size.
			HashTbl( size_t tbl_size_ = DEFAULT_SIZE )
			{
				tbl_size_ = next_prime( tbl_size_ );
				
				m_size = tbl_size_;
				m_count = 0;
	
				m_data_table = new std::forward_list< Entry >[tbl_size_];
			}
			
			/// Default destructor.
			virtual ~HashTbl()
			{
				delete [] m_data_table;
			}
	
			/// Copy constructor.
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
	
				if( ( m_count / m_size ) >= 1.0 )
					rehash();
			}
			
			/// std::initializer_list copy constructor.
			HashTbl( std::initializer_list < Entry > ilist )
			{
				m_size = DEFAULT_SIZE;
				m_data_table = new std::forward_list< Entry >[ m_size ];
	
				for( const Entry & e : ilist )
				{
					insert( e.m_key, e.m_data );
				}
	
				if( ( m_count / m_size ) >= 1.0 )
					rehash();
			}
			
			//=== Operators
			/// Operator = overload for HashTbl objects.
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
	
				if( ( m_count / m_size ) >= 1.0 )
					rehash();
	
				return *this;
			}
			
			/// Operator = overload for std::initializer_list
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
	
				if( ( m_count / m_size ) >= 1.0 )
					rehash();
	
				return *this;
			}
	
			//=== Methods
			/// Inserts on the table the information stored in d_ and associated to a k_ key. If the insertion process succeds the method returns true, if the key already exists, the method overwrites it's data with data stored in d_ and then returns false.
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
	
				if( ( m_count / m_size ) >= 1.0 )
					rehash();
	
				return true;
			}

			/// Removes from the table an item identified by its k_ key. If the key is found, this method returns true, otherwise it returns false.
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
	
			/// Retrieves in d_ the information associated with the key k_. If the key is found, the method returns true, otherwise it returns false.
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
	
			/// Clears all memory associated to the Hashtable's lists, removing all it's elements.
			void clear ( void )
			{
				m_count = 0;
				for( size_t i = 0 ; i < m_size ; i++ )
					m_data_table[i].clear();
			}
	
			/// Returns true if the Hashtable is empty, returns false otherwise.
			bool empty ( void ) const
			{ return m_count == 0; }
	
			/// Returns the number of elements stored in the Hashtable.
			size_t size( void ) const
			{ return m_count; }
	
			/// Returns a reference to the data associated to the k_ key, if the key is not on the table the method throws an std::out_of_range exception.
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
	
			/// Returns a reference to the data associated to the k_ key. If the key is not on the table the method inserts it and returns a reference to it.
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
	
			/// Returns the number of elemets from the hashtable that are on the list associated to the k_ key.
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

			/// A depuration method used to generate a textual representation of the hashtable and it's elements.
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
			/// Private method to be called when the hashtable's load factor is greater than 1. It creates a new hashtable whose size will be equal to the smallest prime number equal or greater than double the size of the table before rehash was called. Then all the elements will be inserted on the new table according to a new secondary hash funcion, based on the new table's size.
			void rehash()
			{
				KeyHash hashFunc;
	
				size_t m_count_backup = m_count;
				size_t new_size = next_prime(m_size*2);
				if( m_size == 0)
					new_size = next_prime(1);
	
				std::forward_list<Entry> * new_data_table = new std::forward_list< Entry >[ new_size ];
	
				for( size_t i = 0u; i < m_size ; i++ )
				{
					auto it = m_data_table[i].begin();
					while( it != m_data_table[i].end() )
					{
						Entry new_entry( it->m_key, it->m_data );
						auto end = hashFunc( it->m_key ) % new_size;
						new_data_table[end].push_front( new_entry );
						it++;
					}
				}
	
				clear();
				delete [] m_data_table;
				
				m_data_table = new_data_table;
	
				m_size = new_size;
				m_count = m_count_backup;
			}
			
			size_t m_size = 0u; //!< Table's size.
			size_t m_count = 0u; //!< Number of elements on the table.
			std::forward_list< Entry > * m_data_table; //!< Data structure used as basis to the table.
			static const short DEFAULT_SIZE = 11;

	
			/// Function used to find the first prime equal or greater than the size given to build the hashtable.
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
