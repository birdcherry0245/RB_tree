#pragma once

#include "rb_tree.h"
#include <set>
#include <exception>
#include <string>
#include <iostream>

template<typename T>
class TestSet {
public:
    TestSet() {}

    template<typename Iterator>
    TestSet(Iterator first, Iterator last) : data_(first, last), set_(first, last) {
        check("TestSet(first, last)");
    }

    TestSet(std::initializer_list<T> elems) : data_(elems), set_(elems) {
        check("TestSet(std::initializer_list<T> elems)");
    }

    TestSet(const TestSet& other): data_(other.data_), set_(other.set_) {
        check("TestSet(const TestSet& other)");
    }
    TestSet(TestSet&& other): data_(other.data_), set_(other.set_) {
        check("TestSet(TestSet&& other)");
    }

    TestSet& operator = (const TestSet& other) {
        data_ = other.data_;
        set_ = other.set_;
        check("operator = (const TestSet& other)");
        return *this;
    }
    TestSet& operator = (TestSet&& other) {
        data_ = other.data_;
        set_ = other.set_;
        check("operator = (TestSet&& other)");
        return *this;
    }

    size_t size() const {
        check("size()");
        return data_.size();
    }

    bool empty() const {
        check("empty()");
        return data_.empty();
    }

    void insert(const T& elem) {
        data_.insert(elem);
        set_.insert(elem);
        check("insert()");
    }

    void erase(const T& elem) {
        data_.erase(elem);
        set_.erase(elem);
        check("erase()");
    }

    typedef typename RB_tree<T>::Iterator iterator;

    iterator begin() const {
        return data_.begin();
    }

    iterator end() const {
        return data_.end();
    }

    iterator find(const T& elem) const {
        auto data_it = data_.find(elem);
        auto set_it = set_.find(elem);
        if (data_it == data_.end()) {
            if (set_it != set_.end()) {
                throw std::runtime_error("find()");
            }
        } else {
            if (set_it == set_.end() || (*data_it) != (*set_it)) {
                throw std::runtime_error("find()");
            }
        }
        return data_.find(elem);
    }

    iterator lower_bound(const T& elem) const {
        auto data_it = data_.lower_bound(elem);
        auto set_it = set_.lower_bound(elem);
        if (data_it == data_.end()) {
            if (set_it != set_.end()) {
                throw std::runtime_error("lower_bound()");
            }
        } else {
            if (set_it == set_.end() || (*data_it) != (*set_it)) {
                throw std::runtime_error("lower_bound()");
            }
        }
        return data_.lower_bound(elem);
    }

    void check(const std::string& message) const {
        data_.Check(message);
        std::vector<T> v_data;
        for (auto it = data_.begin(); it != data_.end(); ++it) {
            v_data.push_back(*it);
        }
        std::vector<T> v_set(set_.begin(), set_.end());
        if (v_data != v_set) {
            std::cout << "v_data = ";
            for (const T& x : v_data) {
                std::cout << x << " ";
            }
            std::cout << "\n";
            std::cout << "v_set = ";
            for (const T& x : v_set) {
                std::cout << x << " ";
            }
            std::cout << "\n";
            throw std::runtime_error(message);
        }
    }
private:
    RB_tree<T> data_;
    std::set<T> set_;
};
