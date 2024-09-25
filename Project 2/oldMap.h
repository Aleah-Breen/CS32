#include<iostream>
using KeyType = std::string;
using ValueType = double;

class Map
{
  public:
    Map();
    ~Map(); // destructor
    Map(const Map &old); // copy constructor
    Map& operator=(const Map &old);//assignment operator

    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    void dump() const;
  private:
    int m_size;
    struct Node{
      KeyType key;
      ValueType val;
      Node* prev;
      Node* next;
    };
    Node* head;
};
bool merge(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);
