#include <vector>
#include <functional>
#include <set> 
#include <algorithm> 
#include <iostream>

//INHERITANCE
//Base class that provides an interface the two subclasses will inherit
//This is an abstract (pure virtual) class that does nothing
//But it allows us to avoid copy/pasting code in main because the interface is the same for both hash tables
template <class T, size_t SIZE>
class Hash {
    public:
        virtual ~Hash() {}
        virtual void insert(T new_data) = 0;
        virtual void remove(T old_data) = 0;
        virtual bool search(T test_data) const = 0;
        virtual void change(T old_data, T new_data) = 0;
};

//YOU: Do the same as the above, with with hashing with chaining instead of linear probing
//You can do chaining either with LLs or with BSTs
//NG - Used BST Chaining
template <class T, size_t SIZE>
class ChainingHash : public Hash<T,SIZE> {
    std::vector<std::set<T>> buckets;   //YOU:
    public:
    //YOU:
    ChainingHash() {
        buckets.resize(SIZE);
    }
    //YOU:
    void insert(T new_data) override { //Override means it is overriding a virtual function
        size_t address = std::hash<T>{}(new_data) % SIZE;
        buckets.at(address).insert(new_data);
    }
    //YOU:
    void remove(T old_data) override {
        size_t address = std::hash<T>{}(old_data) % SIZE;
        buckets.at(address).erase(old_data);
    }
    //YOU:
    bool search(T test_data) const override {
        size_t address = std::hash<T>{}(test_data) % SIZE;
        const auto& chain = buckets.at(address);
        return chain.count(test_data) > 0;
    }
    //YOU:
    void change(T old_data, T new_data) override {
        if(search(old_data)){
            remove(old_data);
            insert(new_data);
        }
    }
}; 

// This Linear Chaining would be substituted within the original hash.h

