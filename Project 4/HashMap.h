#ifndef HASHMAP_H
#define HASHMAP_H
#include<vector>
#include<list>
#include<iostream>
#include <algorithm>

using namespace std;
template <typename T>
class HashMap
{
public:
    HashMap(double max_load = 0.75)
    :keys(10)
    {
        m_maxLoad= max_load;
        m_size=0;
        m_numBuckets=10;
    }; // constructor
    ~HashMap(){}; // destructor; deletes all of the items in the hashmap
    int size() const{
        return m_size;
    }; // return the number of associations in the hashmap

 // The insert method associates one item (key) with another (value).
 // If no association currently exists with that key, this method inserts
 // a new association into the hashmap with that key/value pair. If there is
 // already an association with that key in the hashmap, then the item
 // associated with that key is replaced by the second parameter (value).
 // Thus, the hashmap must contain no duplicate keys.
 void insert(const std::string& key, const T& value){
    size_t h = hash<string>()(key);
    list<association>* l= &keys[h%m_numBuckets];
    typename list<association>::iterator it;
    for (it = l->begin(); it != l->end(); ++it){
        if(it->key==key){
            it->value=value;
            return;
        }
    }
    // if adding one more item would surpass the load Limit, reshash:
    if((m_size+1)/m_numBuckets > m_maxLoad){
        vector<list<association>> newHashM(m_numBuckets*2);
        m_numBuckets*=2;
        typename list<association>::iterator it;
        for(int i=0;i<keys.size();i++){
            list<association>* l= &keys[i];
            for (it = l->begin(); it != l->end(); ++it){
                size_t h = hash<string>()(it->key);
                list<association>* newL= &newHashM[h%m_numBuckets];
                newL->push_back(*it);
            }
        }
        keys=newHashM;
        association n;
        n.key= key;
        n.value=value;
        size_t h = hash<string>()(key);
        list<association>* l= &keys[h%m_numBuckets];
        l->push_back(n);
        m_size++;
    }
    // ----
    else{
        association n;
        n.key= key;
        n.value=value;
        l->push_back(n);
        m_size++;
    }
    
 };
 // If no association exists with the given key, return nullptr; otherwise,
 // return a pointer to the value associated with that key. This pointer can be
 // used to examine that value or modify it directly within the map.

 const T* find(const std::string& key) const{
    size_t h = hash<string>()(key);
    auto l = &keys[h%m_numBuckets];
    auto it = l->begin();
    for(;it!=l->end();++it){
        if(it->key == key){
            return &it->value;
        }
    }
    return nullptr;
 };
 T* find(const std::string& key) {
 const auto& hm = *this;
 return const_cast<T*>(hm.find(key));
 }
 // Defines the bracket operator for HashMap, so you can use your map like this:
 // your_map["david"] = 2.99;
 // If the key does not exist in the hashmap, this will create a new entry in
 // the hashmap and map it to the default value of type T. Then it will return a
 // reference to the newly created value in the map.
 T& operator[](const std::string& key){
    T* foundVal = find(key);
    if(foundVal==nullptr){
        T t = {};
        insert(key,t);
        return *find(key);
    }
    else{
        return *foundVal;
    }
 };
 // preventing copy and assignment operators for HashMap objects
    HashMap(const HashMap&) = delete;
    HashMap& operator=(const HashMap&) = delete;
 private:
    struct association{
        string key;
        T value;
    };
    vector<list<association>> keys;
    int m_size;
    double m_maxLoad;
    int m_numBuckets;
}; 
#endif
