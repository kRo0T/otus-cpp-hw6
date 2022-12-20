#include <iostream>
#include <map>
#include <cassert>

using namespace std;

template<typename T>
class IElement {
    using element_t = IElement<T>;
public:
    virtual element_t& operator=(T new_val) = 0;
    virtual operator T() = 0;
};

template<typename T>
class Element : public IElement<T> {
    using element_t = Element<T>;
    T value;
    map<pair<int, int>, T>& data;
    pair<int, int> index;
public:
    Element(map<int, T>& data, int xi, int yi) : data(data), index(xi, yi) {}
    element_t& operator=(T new_val) {
        data[index] = new_val;
        return *this;
    }
    operator T() {
        return data[index];
    }
};

template<typename T, T def_value>
class ProxyElement : public IElement<T> {
    using proxy_elem_t = ProxyElement<T, def_value>;
    map<pair<int, int>, T>& data;
    pair<int, int> index;
public:
    Element<T>* element;
    ProxyElement(map<pair<int, int>, T>& data, int xi, int yi) : data(data), index(xi, yi) {
        //cout << "Proxy ctr" << endl;
    }
    operator T() {
        auto res = data.find(index);
        if (res != data.end())
            return res->second;
            //return element->operator T();
        else
            return def_value;
    }
    proxy_elem_t& operator=(T new_val) {
        if (new_val == def_value) {
            //cout << "Free element" << endl;
            data.erase(index);
        } else {
            //*element = new_val;
            data[index] = new_val;
        }
        return *this;
    }
};

template <typename T, T def_value>
class Matrix {
    map<pair<int, int>, T> data;
    class IndexHelper {
        int i;
        Matrix& m;
    public:
        IndexHelper(Matrix& m, int i) : m(m), i(i) {}
        ProxyElement<T, def_value> operator[](int j) {
            return ProxyElement<T, def_value>(m.data, i, j);
        }
    };
public:
    IndexHelper operator[](int i) {
        return IndexHelper(*this, i);
    }
    size_t size() {
        return data.size();
    }
    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
};


int main() {
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0);
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);

    for(auto& c: matrix) {
        auto [x, v] = c;
        std::cout << x.first << x.second << v << std::endl;
    }
    return 0;
}
