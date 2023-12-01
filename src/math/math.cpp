#include "math.h"

#include <stdlib.h>

namespace math {

#define luck(x)   __builtin_expect(!!(x), 1)
#define unluck(x) __builtin_expect(!!(x), 0)

    Fibonacci::Fibonacci(unsigned int buf_size) : _buf_size(buf_size) {}
    
    unsigned int Fibonacci::operator()() {
        return this->GetNextValue();
    }

    unsigned int Fibonacci::GetConcretValue(unsigned int index) {
        if (index >= 0 && index + 1 < _size) {
            return _line[index];
        }
        else {
            while (index + 1 > _size)
            {
                GetNextValue();
            }
            return _line[index];
        }
    }

    Fibonacci::~Fibonacci() {

        if (_line != nullptr) 
            free((void*) _line);

        _line = nullptr;
        _n2 = nullptr;
        _n1 = nullptr;
        _size = 0;
    }


    // Возвращает следующее значение в ряде Фибоначчи
    unsigned int Fibonacci::GetNextValue() {
        if (unluck(_line == nullptr)) {
            _line = (unsigned int*) malloc(sizeof(unsigned int) * _buf_size);
        }

        if (unluck(_size + 1 == _buf_size)) {
            ReallocData();
        }

        if (_size == 0) {
            _line[_size] = constantes::_FIB_N0;
            _n2 = _line;
        }
        else if (_size == 1) {
            _line[_size] = constantes::_FIB_N1;
            _n1 = _n2 + _size;
        }
        else {
            _line[_size] = *_n2 + *_n1;
            ++_n1;
            ++_n2;
        }

        return _line[_size++];
        
    }

    void Fibonacci::ReallocData() {

        unsigned int n1_temp = *_n1;
        unsigned int n2_temp = *_n2;
        unsigned int size_temp = _size;
        _buf_size *= 2;

        unsigned int* new_line = (unsigned int*) malloc(sizeof(unsigned int) * _buf_size);

        for (unsigned int i = 0; i != _size; ++i) {
            new_line[i] = _line[i];
        }

        free((void*) _line);
        _line = new_line;

        _n2 = _line + _size - 2;
        _n1 = _line + _size - 1;
    }


} // namespace math