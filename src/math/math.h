#pragma once

namespace math {

    namespace constantes {

        const static unsigned int _FIB_N0 = 0;
        const static unsigned int _FIB_N1 = 1;

    } // namespace constantes
    
    struct Fibonacci {

        Fibonacci() = delete;
        explicit Fibonacci(unsigned int);

        // Возвращает следующее значение в ряду Фибоначчи
        unsigned int operator()();

        // Возвращает указанное значение в ряде Фибоначчи
        unsigned int GetConcretValue(unsigned int);

        unsigned int _buf_size = 0;

        unsigned int* _line = nullptr;
        unsigned int _size = 0;

        unsigned int* _n1 = nullptr;
        unsigned int* _n2 = nullptr;

        ~Fibonacci();

    private:

        // Возвращает следующее значение в ряде Фибоначчи
        unsigned int GetNextValue();

        // Выделяет новую память в два раза большего размера и переносит в неё данные
        void ReallocData();
    };

} // namespace math
