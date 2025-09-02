#pragma once
#include <vector>
#include <functional>
#include <set>
#include <algorithm>
#include <iostream>

template <class T, size_t SIZE>
class Hash {
    public:
        virtual ~Hash() {}
        virtual void insert(T new_data) = 0;
        virtual void remove(T old_date) = 0;
        virtual bool search(T test_data) const = 0;
        virtual void change(T old_data, T new_data) = 0;
};

template <class T, size_t SIZE>
class QuadraticHash : public Hash<T,SIZE> {
    enum class STATUS : char {OPEN, FILLED, DELETED};
    std::vector<T> data;
    std::vector<STATUS> status;

    public:
        QuadraticHash() {
            data.resize(SIZE);
            status.resize(SIZE,STATUS::OPEN);
        }

        void insert(T new_data) override {
            size_t address = std::hash<T>{}(new_data) % SIZE;
            size_t i = 0;
            while (status.at(address) == STATUS::FILLED && i < SIZE) {
                i++;
                address = (address + (i*i)) % SIZE;
            }
            if (i < SIZE) {
                data.at(address) = new_data;
                status.at(address) = STATUS::FILLED;
            }
        }

        void remove(T old_data) override {
            size_t address = std::hash<T>{}(old_data) % SIZE;             size_t i = 0;
            while (i < SIZE) {
                if (status.at(address) == STATUS::OPEN) { break; }
                if (data.at(address) == old_data && status.at(address) == STATUS::FILLED) {
                        status.at(address) = STATUS::DELETED;
                        return;
                }
                i++;
                address = (address + (i*i)) % SIZE;
            }
        }

        bool search(T test_data) const override {
            size_t address = std::hash<T>{}(test_data) % SIZE;
            size_t i = 0;
            while (i < SIZE) {
                if (status.at(address) == STATUS::OPEN) { return false; }
                if (status.at(address) == STATUS::FILLED && data.at(address) == test_data) { return true; }
                i++;
                address = (address + (i*i)) % SIZE;
            }
            return false;
        }

        void change(T old_data, T new_data) override {
            remove(old_data);
            insert(new_data);
        }

};
