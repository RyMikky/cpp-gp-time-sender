#include "json.h"


#include <stdio.h>
#include <stdlib.h>
#include <cstring>

// #include <iostream>

namespace json {

    namespace c_string {
        
        unsigned int StrLen(const char* str) {
            unsigned int result = 0;

            if (str == nullptr) return 0;
            
            while (*str)
            {
                ++result;
                ++str;
            }
            
            return result;
        }

        unsigned int StrLen(char* str) {
            return StrLen((const char*) str);
        }

        unsigned int StrCpy(char* dest, const char* src, unsigned int size) {

            dest = (char*) malloc(size);

            for (int i = 0; i != size; ++i) {
                dest[i] = src[i];
            }

            return 0;
        }

        char* StrCpy(const char* src, unsigned int size) {
            char* result = (char*) malloc(size);

            for (int i = 0; i != size; ++i) {
                result[i] = src[i];
            }

            return result;
        }

        char* StrCpy(const char* src) {
            return StrCpy(src, StrLen(src));
        }

        unsigned int StrCat(char* dest, unsigned int* pos, const char* src, unsigned int count) 
        {
            for (int i = 0; i != count; ++i, (*pos)++) {
                dest[*pos] = src[i];
            }

            return 0;
        }

        unsigned int StrCat(char* dest, unsigned int pos, const char* src, unsigned int count) 
        {
            for (int i = 0; i != count; ++i, ++pos) {
                dest[pos] = src[i];
            }

            return pos;
        }

        unsigned int StrCmp(const char* lhs_str, unsigned int lhs_size, const char* rhs_str, unsigned int rhs_size){
            if (lhs_size != rhs_size) return 2;

            for (unsigned int i = 0; i!= lhs_size; ++i) {
                if (lhs_str[i] != rhs_str[i]) return 1;
            }

            if (lhs_str[lhs_size] != rhs_str[rhs_size]) return 3;

            return 0;
        }

        unsigned int StrCmp(const char* lhs, const char* rhs) {
            unsigned int lhs_size = StrLen(lhs);
            unsigned int rhs_size = StrLen(rhs);

            return StrCmp(lhs, lhs_size, rhs, rhs_size);
        }

        unsigned int StrRev(char* str) {
            if (str == nullptr) return 0;
            return StrRev(str, StrLen(str));
        }

        unsigned int StrRev(char* str, unsigned int str_size){
            if (str == nullptr) return 0;

            for (unsigned int i = 0; i != str_size / 2; ++i) {
                char buf = str[i];
                str[i] = str[str_size - 1 - i];
                str[str_size - 1 - i] = buf;
            }

            return str_size;
        }

        const char* ToString(unsigned int num) {
            char* str = (char*) malloc(UINT_BUF + 1);

            if (str == NULL) {
                return nullptr;
            }

            unsigned int count = 0;
            while (num > 9)
            {
                unsigned int dec = num % 10;
                str[count++] = NUMERIC[dec];
                num /= 10;
            }

            str[count++] = NUMERIC[num];
            str[count] = '\0';

            StrRev(str, count);

            return str;
        }

        const char* ToString(const char* str) {
            return str;
        }

    } // namespace c_string
     
    jsonObject::~jsonObject() {
        if (_key != nullptr) free(_key);
        if (_value != nullptr) free(_value);

        _key = nullptr;
        _value = nullptr;
    }

    const char* jsonObject::ToString() {

        using namespace c_string;

        /* 1. Готовим размер буфера под выделение памяти*/

        unsigned int open_buff_size = StrLen(OPEN_BAR);              // открытие фигурной скобки
        unsigned int quote_buff_size = StrLen(QUOTE);                // кавычеки
        unsigned int separator_buff_size = StrLen(SEPARATOR);        // разделитель
        unsigned int close_buff_size = StrLen(CLOSE_BAR);            // закрытие фигурных скобок

        unsigned int str_buff = open_buff_size + _kSize + separator_buff_size + _vSize + close_buff_size;

        switch (_type)
        {
        case jsonType::INTEGER:
            str_buff += (quote_buff_size * 2);
            break;

        case jsonType::STRING:
            str_buff += (quote_buff_size * 4);
            break;
        
        default:
            break;
        }

        char* str = (char*) malloc(str_buff + 1);

        if (str == NULL) {
            return nullptr;
        }

        /* 2. Заполняем строку */

        unsigned int pos = 0;
        c_string::StrCat(str, &pos, OPEN_BAR, open_buff_size);
        c_string::StrCat(str, &pos, QUOTE, quote_buff_size);

        if (_kSize != 0 && _key != nullptr) {
            c_string::StrCat(str, &pos, _key, _kSize);
        }

        c_string::StrCat(str, &pos, QUOTE, quote_buff_size);
        c_string::StrCat(str, &pos, SEPARATOR, separator_buff_size);

        if (_vSize != 0 && _value != nullptr) {
            if (_type == jsonType::STRING) c_string::StrCat(str, &pos, QUOTE, quote_buff_size);
            c_string::StrCat(str, &pos, _value, _vSize);
            if (_type == jsonType::STRING) c_string::StrCat(str, &pos, QUOTE, quote_buff_size);
        }

        c_string::StrCat(str, &pos, CLOSE_BAR, close_buff_size);

        str[str_buff] = '\0';
        return str;
    }

    unsigned int MakeJsonObject(const char* key, const char* value, jsonType type, void** output) {
        using namespace c_string;

        unsigned int key_buff_size = (key != nullptr) ? StrLen(key) : ZERO_BUF;            // заголовок
        unsigned int value_buff_size = (value != nullptr) ? StrLen(value) : ZERO_BUF;      // значение

        jsonObject* object = nullptr;
        if (*output != nullptr) {
            object = (jsonObject*) *output;
        }
        else {
            *output = malloc(sizeof(jsonObject));
            object = (jsonObject*) *output;
        }

        if (key_buff_size != ZERO_BUF) {
            object->_key = (char*)malloc(key_buff_size + 1);
            std::memcpy(&(object->_key[ZERO_BUF]), key, key_buff_size);
            object->_key[key_buff_size] = '\0';
            object->_kSize = key_buff_size;
        }

        if (value_buff_size != ZERO_BUF) {
            object->_value = (char*)malloc(value_buff_size + 1);
            std::memcpy(&(object->_value[ZERO_BUF]), value, value_buff_size);
            object->_value[value_buff_size] = '\0';
            object->_vSize = value_buff_size;
        }

        object->_type = type;

        return 0;
    }

    unsigned int MakeJsonObject(const char* key, unsigned int* value, void** output) {
        const char* num_value = c_string::ToString(*((unsigned int*) value));
        unsigned int errcode = MakeJsonObject(key, num_value, jsonType::INTEGER, output);

        free((void*) num_value);
        return errcode;
    }

    unsigned int MakeJsonObject(const char* key, const char* value, void** output) {
        return MakeJsonObject(key, value, jsonType::STRING, output);
    }

} // namespace json