#pragma once

#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <type_traits>

#include "curl/curl.h"
#include "../json/json.h"
#include "../app/domain.h"

namespace sender {

    struct Response {
        char* _response = nullptr;
        unsigned int _size = 0;

        ~Response();
    };

    struct Body {
        char* _body = nullptr;
        unsigned int _size = 0;

        ~Body();
    };

    // Обратный вызов для печати ответа от сервера
    unsigned int RequestCallback(void* data, unsigned int size, unsigned int nmemb, void* output);

    class POSTSender {
    private:
        std::ostream& _out;
    public:
        POSTSender() = delete;

        POSTSender(std::ostream& out, const char* url, const char* port) 
            : POSTSender(out, url, port, false) {}

        POSTSender(std::ostream& out, const char* url, const char* port, bool print) 
            : _out(out), _url(url), _port(port), _print(print) {}

        // Деструктор не освобождает память _url и _port, так как они получаются извне, путь там же и удаляются
        ~POSTSender();

        // Устанавливает новый адрес отправки запроса
        POSTSender& SetUrl(const char*);

        // Возвращает текущий установленный адрес запроса
        const char* GetUrl() {
            return _url;
        }

        // Устанавливает новый порт отправки запроса
        POSTSender& SetPort(const char*);

        // Возвращает текущий установленный порт запроса
        const char* GetPort() {
            return _port;
        }

        // Устанавливает флаг печати ответа
        POSTSender& SetPrint(bool);
        
        // Подготавливает и отправляет запрос с телом jsonObject cstring + cstring
        POSTSender& SendRequest(const char*, const char*);

        // Подготавливает и отправляет запрос с телом jsonObject cstring + unsigned int
        POSTSender& SendRequest(const char*, unsigned int*);

    private:
        const char* _url = nullptr;
        const char* _port = nullptr;
        bool _print = false;

        POSTSender& SendRequest(json::jsonObject*);

    };

} // namespace sender