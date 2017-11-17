#ifndef DOUBLE_HASH_H
#define DOUBLE_HASH_H

#include <vector>
#include <algorithm>
#include <functional>


namespace {

// // Internal method to test if a positive number is prime.
// bool IsPrime(size_t n) {
//   if( n == 2 || n == 3 )
//     return true;
  
//   if( n == 1 || n % 2 == 0 )
//     return false;
  
//   for( int i = 3; i * i <= n; i += 2 )
//     if( n % i == 0 )
//       return false;
  
//   return true;
// }


// // Internal method to return a prime number at least as large as n.
// int NextPrime(size_t n) {
//   if (n % 2 == 0)
//     ++n;  
//   while (!IsPrime(n)) n += 2;  
//   return n;
// }

}  // namespace


// Double hashing implementation.
template <typename HashedObj>
class HashTableDouble {
 public:
  mutable int collisions;
  int num_elements;

  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableDouble(size_t size = 101) : array_(NextPrime(size))
    { MakeEmpty(); collisions = 0; num_elements = 0; }
  
  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }

  //used for finding number of probes
  bool Contains(const HashedObj & x, int &probes) const {
    return IsActive(FindPosProbes(x, probes));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    num_elements++;

    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; if table starts to overflow then create another table with twice the size
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  }
    
  bool Insert(HashedObj && x) {
    // Insert x as active

    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }

  int getTableSize(){
    return array_.size();
  }
 private:        
  struct HashEntry {
    HashedObj element_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
    

  std::vector<HashEntry> array_;
  size_t current_size_;

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }

  //used for finding number of probes
  size_t FindPosProbes(const HashedObj & x, int &probes) const {
    size_t offset = 1;
    size_t current_pos = (InternalHash(x) + offset * InternalHash2(x)) % array_.size();
      
    while (array_[current_pos].info_ != EMPTY &&
     array_[current_pos].element_ != x) {
      current_pos += offset;  // Compute ith probe.
      //offset += 2;
      collisions++;
      probes++;

      if (current_pos >= array_.size())
	    current_pos -= array_.size();
    }
    return current_pos;
  }


  size_t FindPos(const HashedObj & x) const {
    size_t offset = 1;
    size_t current_pos = (InternalHash(x) + offset * InternalHash2(x)) % array_.size();
      
    while (array_[current_pos].info_ != EMPTY &&
     array_[current_pos].element_ != x) {
      current_pos += offset;  // Compute ith probe.
      //offset += 2;
      collisions++;

      if (current_pos >= array_.size())
	    current_pos -= array_.size();
    }
    return current_pos;
  }
  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
  }
  
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }

  size_t InternalHash2(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return 2 - (hf(x) % 2);
  }

};

#endif  // DOUBLE_HASH_H
