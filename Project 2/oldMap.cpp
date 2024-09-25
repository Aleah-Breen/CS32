#include "oldMap.h"
#include<iostream>
using namespace std;

//initialize a default map with starting size 0 and head at nullptr since linkedList has no nodes yet
Map::Map(){
    m_size=0;
    head=nullptr;
}
//deconstruct Map by deleting all nodes. Point at each node with a temp variable that way I can still use node->next and delete.
Map::~Map(){
    Node*ptr=head;
    while(ptr!=nullptr){
        Node*temp=ptr;
        ptr=ptr->next;
        delete temp;
    }
}
//copy constructor
//simply copy map size to the new map
//using a pointer, loop through the old Map and insert a corresponding key and value from old into our new map, until our ptr is null
Map::Map(const Map &old){   
    m_size=0;
    head = nullptr;
    Node*ptr=old.head;
    while(ptr!=nullptr){
        insert(ptr->key,ptr->val);
        ptr=ptr->next;
       
    }
}; 
//assignment operator
//first, check for same map. We dont want to swap with itself. If it is being set equal to itself, just return *this.
//if it is not the same map, create a map m using my copy constructor, and swap the maps. 
// since m is locally created, it will deconstruct when the function ends. 
// when we swap map m with old, all the old values are put in m. since it deconstructs we successfully destroy old nodes
// at the same time, we create a copy of the map this function was called for, creating a copy so the pointers are different.
Map& Map::operator=(const Map &old){ 
    if(this!=&old){
        Map m(old);
        swap(m);
    }   
    return *this;
}
// if head is nullptr we have no nodes to destroy, so return true. 
// if head is not null, we have a node and therefore are not empty. return false.
bool Map::empty() const{
    if(head==nullptr){
        return true;
    }
    else{
        return false;
    }
}
// simply return our member variable for this function
int Map::size() const{
    return m_size;
}
// since we want our get(i,key,val) to be easier, we are inserting our nodes alphabetically.
bool Map::insert(const KeyType& key, const ValueType& value){
    
    // Dynamically allocate a new node, n. Since it is being inserted it needs its own memory, thus the dynamic allocation.
    Node* n = new Node;

    // update n's values with the input parameters.
    n->key=key;
    n->val=value;
    
    // point with ptr to our maps head.
    Node*ptr = head;
    
    // if our head is nullptr, it is empty right now. simple set head = to n, and have n->next + n->prev point to nullptr. n is now our head.
    if(head==nullptr){
        head=n;
        n->next=nullptr;
        n->prev=nullptr;
        m_size++;
        return true;
    }  

    // if our heads key is greater than our parameter key, that means our input node is the smallest, so it needs to be the new head. 
    // accomplish this by linking node n->next to our old head, and prev to nullptr. Next set head = n
    if(head->key>key){
        n->next=head;
        n->prev=nullptr;
        head->prev=n;
        head=n;
        m_size++;
        return true;
    }

    //otherwise, loop through our map until the value of the next node is nullptr. We will handle inserting at the end after our loop
    while(ptr->next!=nullptr){
        // if the param key is already in the map, delete our allocated node and return false because we can not have duplicate keys.
        if(ptr->key==key){
            delete n;
            return false;
        }
        // if the next key is > our key, it means we are at the node before where our new node belongs. 
        else if(ptr->next->key>key){
            // link our new nodes next pointer to the next pointer of the node we are looking at
            n->next=ptr->next;
            // make our previous pointer point to the current node, since our new node is going right after it
            n->prev=ptr;
            // make the node we are looking at point to our new node
            ptr->next=n;
            //increment size.
            m_size++;
            return true;
        }
        else{
            // otherwise, we are not in the right spot yet. Move on to the next node.
            ptr=ptr->next;
        }
    }
    
    // if we have gotten this far, we are at the very last node in the Linked list.

    //first, once again check if the last node has the same key as our param. 
    if(ptr->key==key){
        //No duplicate keys so delete our allocated node and return false
        delete n;
        return false;
    }
    //at this point, our node is valid and belongs at the end of the linked list.
    // have our current last node point to our allocated node n.
    ptr->next=n;
    // have our new node previous point to the current last node.
    n->prev=ptr;
    // our new node must point to nullptr since it is the last Node
    n->next=nullptr;
    //increment size
    m_size++;
    return true;
}
// we only update if the key already exists, so loop through Map. 
//If any key is equal to our param key, update its value and return.
bool Map::update(const KeyType& key, const ValueType& value){
    Node*ptr=head;
    while(ptr!=nullptr){
        if(ptr->key==key){
            ptr->val=value;
            return true;
        }
        ptr=ptr->next;
    }
    return false;
}
// Sicne we have functions to insert and update, we can just reuse those.
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value){
    //first, call insert. It will return false if the key is already in the map. 
    //If it returns true, it means it successfully inserted, so return true again.
    if (insert(key,value)){
        return true;
    }
    // otherwise, update the key. Since insert failed, update will always return true
    // this aligns with the spec requirement, that insertOrUpdate always returns true
    update(key,value);
    return true;
}
// to erase we first need to loop through map to see if any of the keys match
bool Map::erase(const KeyType& key){
    Node* ptr = head;
    while(ptr!=nullptr){
        // if the pointer's key is the one we are looking for, it means we need to erase this node
        if(ptr->key==key){
            // first, check that this is not the first node.
            // this prevents undefined behavior
            if(ptr->prev!=nullptr){
                // if it is not the first node, have the previous node point to the next node, thus skipping this one
                ptr->prev->next=ptr->next;
            }
            // second, check that this is not the last node.
            // this also prevents undefined behavior
            if(ptr->next!=nullptr){
                // if this is not the last node, set the next nodes previous pointer to point at the node before our current
                ptr->next->prev=ptr->prev;
            }
            // if the node we are deleting is the head, we need to reassign head so our functions work.
            // make our head point to the next value instead.
            if(ptr==head){
                head=ptr->next;
            }
            // Since we point at the node we are releasing, delete ptr and decrement size
            delete ptr;
            m_size--;
            return true;
        }
        ptr=ptr->next;
    }
    // if none of the keys match, return false because that key is not in our map.
    return false;
};
// simply loop through the linked list and if the key matches, return true.
bool Map::contains(const KeyType& key) const{
    Node*ptr=head;
    while(ptr!=nullptr){
        if(ptr->key==key){
            return true;
        }
        ptr=ptr->next;
    }
    // if we find the nullptr, we checked every node and our list does not contain key
    return false;
}
// loop through our linked list until we find the right key, then set value = to that val.
bool Map::get(const KeyType& key, ValueType& value) const{
    Node*ptr=head;
    while(ptr!=nullptr){
        if(ptr->key==key){
            value=ptr->val;
            return true;
        }
        ptr=ptr->next;
    }
    // if we dont find the key, return false
    return false;
}
// since we alphabetically ordered our linked list, loop down until we are at the ith node
// set the key and value = to that nodes key and val
bool Map::get(int i, KeyType& key, ValueType& value) const{
    int count=0;
    Node* ptr= head;
    while(ptr!=nullptr){
        if(count==i){
            key = ptr->key;
            value = ptr->val;
            return true;
        }
        count++;
        ptr=ptr->next;
    }
    // if we find nullptr before hitting index i, we are getting a node at an index larger than our map
    // therefore return false
    return false;
}

void Map::swap(Map& other){
    //first, swap the member sizes of each map
    int temp = m_size;
    m_size=other.m_size;
    other.m_size=temp;
    // next, swap the head pointers of each map. 
    Node* n = head;
    head=other.head;
    other.head=n;
}
// ignore dump function
void Map::dump() const{
    Node*ptr=head;
    //Node*last;
    while(ptr!=nullptr){
        cerr << ptr->key<<" " << ptr->val<<endl; 
        //last=ptr;
        ptr=ptr->next;
    }
    /*ptr=last;
    cerr<<"going back now"<<endl;
    while(ptr!=nullptr){
        cerr<<ptr->key<<" " << ptr->val<<endl; 
        ptr=ptr->prev;
    }*/
}

// for merge, we can assume m1 and m2 are both alphabetically ordered because of our insert method
// we are going to track index for m1 and m2. 
// we can simply insert whichever key is smallest first.
// if ther keys match, we check if the values match.
// if the values do match, insert one node of that pair. If they dont, increment both our pointers
// by doing this we skip all edge cases. 
bool merge(const Map& m1, const Map& m2, Map& result){
    // m will be a temporary map that we will eventually swap.
    Map m;
    int m1track=0;
    int m2track=0;
    KeyType k;
    ValueType v;
    KeyType k2;
    ValueType v2; 
    // nonFaulted keeps track of whether we have skipped over two nodes with the same key and different values
    // since we need to continue the method after finding the duplicate, we just store a bool.
    bool nonFaulted=true;
    // while both our pointers are below size means we still have nodes to handle in either map. 
    while(m1track<m1.size()||m2track<m2.size()){
        // if we are at the max size for m1, we only need to insert m2 nodes.
        if(m1track>=m1.size()){
            m2.get(m2track,k,v);
            m.insert(k,v);
            m2track++;
        }
        // if we are at the max size for m2, we only need to insert m1 nodes.
        else if(m2track>=m2.size()){
            m1.get(m1track,k,v);
            m.insert(k,v);
            m1track++;
        }
        // otherwise, we will insert whichever is larger.
        else{
            m1.get(m1track,k,v);
            m2.get(m2track,k2,v2);
            
            if(k==k2){
                //if the keys are the same and the values are the same, inset one copy and increment both counters
                if(v==v2){
                    m.insert(k,v);
                    m1track++;
                    m2track++;
                }
                // if the keys are the same and the values are NOT, skip both of these keys and record so in nonFaulte.
                else{
                    nonFaulted=false;
                    m1track++;
                    m2track++;
                }
            }
            // if the keys are not the same
            else{
                // insert whichever key is larger, and increment the corresponding tracker
                if(k<k2){
                    m.insert(k,v);
                    m1track++;
                }
                else{
                    m.insert(k2,v2);
                    m2track++;
                }
            }
        }
    }
    // use assignment operator because it will work regardless of what was previously stored in record.
    // our assignment calls swap, which will deallocate all of the nodes that were previously in result
    result = m;
    // return nonFaulted. This will be true if no key duplicates with differing values. Otherwise false.
    return nonFaulted;
}
// for reassing I decided to pair each key with the value of the next node.
// I do this by storing the previous key, so we are inserting (previosKey, currentVal);
// this leaves a loose end because my first val gets ignored. 
// I am matching the last key witht the first val -> (lastKey, firstVal);
// this circularly assigns every value to the node after it, which meets this questions criteria
void reassign(const Map& m, Map& result){
    // temporary map and appropriate keytype and valuetype variables
    // we need a temp map because it says we have to cover aliasing. This way we can use assignment operator later
    Map temp;
    KeyType key;
    KeyType lastkey;
    ValueType val;
    ValueType firstval;
    if(m.size()==0){
        // empty map cant be reassigned, so return
        result = temp;
        return;
    }
    // if our map only has 1 node, there is no way to swap values, so just add the one node to temp, assign, then return

    if(m.size()==1){
        m.get(0,key,val);
        temp.insert(key,val);
        result = temp;
        return;
    }
    // otherwise, loop through the nodes, inserting Prevkey, current val/
    for(int i=0;i<=m.size();i++){
        // we need to loop past the last item one time, so that we can add the last Key and first Value.
        if(i<m.size()){
            m.get(i,key,val);
        }
        if(i==0){
            // if we are at our first pair, set firstVal = to that val.
            // we dont want to insert anything yet since we have no prevKey
            firstval=val;
        }
        else if(i==m.size()){
            // if we are at the very last node, insert the last key and the first val
            temp.insert(key,firstval);
        }
        else{
            // otherwise, simply add lastKey and current Val to the map/
            temp.insert(lastkey,val);
        }
        //record the lastKey.
        lastkey=key;
    }
    // use assignment operator to override whatever was previously in result.
    result=temp;
}
