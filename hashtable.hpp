// default 1 param constructor
template <typename K, typename V>
HashTable<K, V>::HashTable(size_t size) 
{
	tracksize = 0;
	table.resize(prime_below(size));
}

//destructor
template <typename K, typename V>
HashTable<K, V>::~HashTable()
{
	makeEmpty();
}

//checks if key k is in the table
template <typename K, typename V>
bool HashTable<K, V>::contains(const K& k) const
{
	auto & index = table[myhash(k)];
	auto itr = index.begin();
	while(itr != index.end())
	{
	  if ( itr->first == k)
		return true;
	  ++itr; 
	}
	
	return false;
}
 
// checks if key-value pair is in the hashtable
template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V>& kv) const
{
	auto & index = table[myhash(kv.first)];
	return(find(begin(index), end(index), kv) != end(index));
}

// adds key value pair into the hash table
template <typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V>& kv)
{
	if (match(kv))
		return false;

	auto & whichList = table[myhash(kv.first)];
	auto itr = whichList.begin();

	if (!contains(kv.first))
	{
	  whichList.push_back(kv);
	  tracksize++;
	}
	else
	{
	  while(itr != whichList.end())
	  {
	    if (itr->first == kv.first)
	    {
		whichList.insert(itr, 1, kv);
		itr = whichList.erase(itr);
	    }
	    else
	    {
		itr++;
	    }
	  }
	}
	if(tracksize > table.size())
		rehash();

  return true;
}

// move version of insert
template <typename K, typename V>
bool HashTable<K, V>:: insert(std::pair<K, V>&& kv)
{
	if (match(kv))
		return false;

	auto & whichList = table[myhash(kv.first)];
	auto itr = whichList.begin();

	if (!contains(kv.first))
	{
	  whichList.push_back(std::move(kv));
	  tracksize++;
	}
	else
	{
	  while(itr != whichList.end())
	  {
	    if (itr->first == kv.first)
	    {
		whichList.insert(itr, 1, kv);
		itr = whichList.erase(itr);
	    }
	    else
	    {
		itr++;
	    }
	  }
	}
	if(tracksize > table.size())
		rehash();

  return true;
}

// deletes key k if it is in the hash table
template <typename K, typename V>
bool HashTable<K, V>::remove(const K& k)
{
	if(!contains(k))
		return false;

	auto & whichList = table[myhash(k)];
	auto itr = whichList.begin();

	while(itr != whichList.end())
	{
	  if ( itr->first == k)
	  {
		itr = whichList.erase(itr);
		tracksize--;
	  }
	  else
		itr++;
	}

	return true;
}

//clears all elements in the hash table
template <typename K, typename V>
void HashTable<K, V>::clear()
{
	makeEmpty();
}

//loads content of the file into the hash table 
template <typename K, typename V>
bool HashTable<K, V>::load(const char* filename)
{
	std::ifstream is;
	
	K key;
	V value;

	is.clear();
	is.open(filename);
	
	if (!is)
		return false;
	
	while(!is.eof())
	{
	  is >> key >> value;
	  std::pair<K, V> kv(key, value);
	  if(!is.fail())
		insert(kv);
	}	

	is.close();

	return true;
}

// displays all entries in the hash table 
template <typename K, typename V>
void HashTable<K, V>::dump()
{
	for(int i = 0; i < table.size(); i++)
	{
	  auto& whichList = table[i];
	  auto itr = whichList.begin();

	  std::cout << "v[" << i << "]: ";

	  if(!whichList.empty())
	  {
		std::cout << itr->first << " " << itr->second;
	  	
		while(++itr != whichList.end())
		{
		  std::cout << " : " << itr->first << " " << itr->second; 
		}
	  }
	  std::cout << '\n';
	}
}

// writes eleemnts of the hash table to a file
template <typename K, typename V>
bool HashTable<K, V>::write_to_file(const char* filename)
{
	std::ofstream os;
	K key;
	V value;
	
	os.open(filename);

	if(!os)
		return false;

	for (int i = 0; i < table.size(); i++)
	{
	  auto& whichList = table[i];
	  auto itr = whichList.begin();

	  if(!whichList.empty())
	  {
		os << itr->first << " " << itr->second << '\n';

		while(++itr != whichList.end())
		{
		  os << itr->first << " " << itr->second << '\n';
		}
	  }
	}

	os.close();

	return true;
}

// deletes all elements in the hash table
template <typename K, typename V>
void HashTable<K, V>::makeEmpty()
{
	for(auto& i : table)
		i.clear();
}

// called when the number of elements in the hash table is greater than the size of the vector
template <typename K, typename V>
void HashTable<K, V>::rehash()
{
	auto oldtable = table;
	
	table.resize(prime_below(2*table.size()));
	for(auto& i : table)
		i.clear();

	tracksize = 0;
	for (auto & i : oldtable)
		for (auto& x : i)
			insert(std::move(x));
}

template <typename K, typename V>
size_t HashTable<K, V>::myhash(const K& k) const
{
	static std::hash<K> hf;
	return hf(k) % table.size();
}




// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n)
{
  if (n > max_prime)
    {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
		std::cerr << "** input too small \n";
      return 0;
    }

  // now: 2 <= n < max_prime
  std::vector <unsigned long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
	return n;
      --n;
    }

  return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n) 
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}

template <typename K, typename V>
int HashTable<K, V>::size()
{
	return tracksize;
}
