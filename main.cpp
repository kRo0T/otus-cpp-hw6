#include <iostream>
#include <map>

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
    map<int, T>& data;
    int index;
public:
    Element(map<int, T>& data, int index) : data(data), index(index) {}
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
    map<int, T>& data;
    int index;
public:
    Element<T>* element;
    ProxyElement(map<int, T>& data, int index) : data(data), index(index) {
        cout << "Proxy ctr" << endl;
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
            cout << "Free element" << endl;
            data.erase(index);
        } else {
            //*element = new_val;
            data[index] = new_val;
        }
        return *this;
    }
};

template <typename T, T def_value>
class SparseVector {
    map<int, int> data;
public:
    ProxyElement<T, def_value> operator[](int i) {
        cout << "SV operator []" << endl;
        return ProxyElement<T, def_value>(data, i);
    }
};

int main() {
    SparseVector<int, -1> vec;
    cout << vec[0] << endl;
    vec[0] = 2;
    cout << vec[0] << endl;
    vec[0] = -1;
    cout << vec[0] << endl;
    //pe.element = &e;
    //int val = pe;
    //cout << val << endl;
    return 0;
}
