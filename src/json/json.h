#pragma once

#include <type_traits>

namespace json {

    namespace domain {

        static const char _NUMERIC[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

        static const char* _OPEN_BAR = "{ ";
        static const char* _CLOSE_BAR = " }";
        static const char* _QUOTE = "\"";
        static const char* _SEPARATOR = " : ";

        static const unsigned int _ZERO_BUF = 0;
        static const unsigned int _UINT_BUF = 16;

        template <typename T>
		struct IsNumeric {
			static const bool value = std::is_arithmetic<T>::value;
		};
        
    } // namespace domain
    
    namespace c_string {
        
        // Возвращает длину строки
        unsigned int StrLen(const char* str);

        // Возвращает длину строки
        unsigned int StrLen(char* str);

        // Делает копию строки из полученной в потоке
        unsigned int StrCpy(char* dest, const char* src, unsigned int size);

        // Делает копию строки из полученной в потоке
        char* StrCpy(const char* src, unsigned int size);

        // Делает копию строки из полученной в потоке
        char* StrCpy(const char* src);

        // Дополняет первую строку второй указанной длины, начиная с указанной позиции. 
        // Функция не проверяет наличие места и выделенную память строки, в которую добавлют!
        unsigned int StrCat(char* dest, unsigned int* pos, const char* src, unsigned int count);

        // Дополняет первую строку второй указанной длины, начиная с указанной позиции. 
        // Возвращает измененную позицию посде добавления {pos}
        unsigned int StrCat(char* dest, unsigned int pos, const char* src, unsigned int count);

        // Сравнение двух строк с указаными длинами
        unsigned int StrCmp(const char* lhs_str, unsigned int lhs_size, const char* rhs_str, unsigned int rhs_size);

        // Сравнение двух строк с указаными длинами
        unsigned int StrCmp(const char* lhs, const char* rhs);

        // Разворачивает строку задом наперед
        unsigned int StrRev(char* str);

        // Разворачивает строку задом наперед
        unsigned int StrRev(char* str, unsigned int size);

        // Переводит целое беззнаковое число в строку
        const char* ToString(unsigned int num);

        const char* ToString(const char* str);

    } // namespace c_string

#define OPEN_BAR domain::_OPEN_BAR
#define CLOSE_BAR domain::_CLOSE_BAR
#define QUOTE domain::_QUOTE
#define SEPARATOR domain::_SEPARATOR
#define ZERO_BUF domain::_ZERO_BUF
#define UINT_BUF domain::_UINT_BUF
#define NUMERIC domain::_NUMERIC

    enum class jsonType {
        NULL_t, INTEGER, STRING
    };

    struct jsonObject {
        jsonType _type = jsonType::NULL_t;

        ~jsonObject();

        char* _key = nullptr;
        unsigned int _kSize = 0;
        char* _value = nullptr;
        unsigned int _vSize = 0;

        // Возвращает строковое представление объекта
        const char* ToString();
    };

    // Создаёт JSON-объект ключ_значение cstring + cstring
    unsigned int MakeJsonObject(const char* key, const char* value, jsonType type, void** output);

    // Создаёт JSON-объект ключ_значение cstring + numeric
    unsigned int MakeJsonObject(const char* key, unsigned int* value, void** output);

    // Создаёт JSON-объект ключ_значение cstring + cstring
    unsigned int MakeJsonObject(const char* key, const char* value, void** output);
    
} // namespace json
