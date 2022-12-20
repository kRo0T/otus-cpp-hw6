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

template<typename T, T def_value>
class Element : public IElement<T> {
    using element_t = Element<T, def_value>;
    T value = def_value;
    pair<int, int> index;
public:
    Element() = default;
    element_t& operator=(T new_val) {
        value = new_val;
        return *this;
    }
    operator T() {
        return value;
    }
};

template<typename T, T def_value>
class ProxyElement : public IElement<T> {
    using proxy_elem_t = ProxyElement<T, def_value>;
    map<pair<int, int>, Element<T, def_value>>& data;
    pair<int, int> index;
public:
    ProxyElement(map<pair<int, int>, Element<T, def_value>>& data, int xi, int yi) : data(data), index(xi, yi) { }
    operator T() {
        auto res = data.find(index);
        if (res != data.end())
            //return res->second;
            return res->second.operator T();
        else
            return def_value;
    }
    proxy_elem_t& operator=(T new_val) {
        if (new_val == def_value) {
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
    map<pair<int, int>, Element<T, def_value>> data;
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
    /*Matrix<int, -1> matrix;
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
    }*/
    
    Matrix<int, 0> matrix_1;
    for (int i=0; i<10; i++) {
        matrix_1[i][i] = i;
        matrix_1[i][9-i] = 9-i;
    }
    for (int i=1; i<9; i++) {
        for (int j=1; j<9; j++) {
            cout << matrix_1[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Size: " << matrix_1.size() << endl;
    for(auto& c: matrix_1) {
        auto [x, v] = c;
        std::cout << x.first << ", " << x.second << ": " << v << std::endl;
    }
    return 0;
}
