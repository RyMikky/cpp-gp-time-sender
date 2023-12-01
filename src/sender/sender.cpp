#include "sender.h"

namespace sender {

    Response::~Response() {
        if (_response != nullptr)
            free(_response);
    }

    Body::~Body() {
        if (_body != nullptr)
             free(_body);
    }

    // Обратный вызов для печати ответа от сервера
    unsigned int RequestCallback(void* contents, unsigned int size, unsigned int nmemb, void* output){
        unsigned int buf_size = size * nmemb;
        struct Response *mem = static_cast<struct Response*>(output);

        char* ptr = (char*) realloc(mem->_response, mem->_size + buf_size + 1);
        if (ptr == NULL) {
            return 0;  /* out of memory! */
        }

        mem->_response = ptr;
        std::memcpy(&(mem->_response[mem->_size]), contents, buf_size);
        mem->_size += buf_size;
        mem->_response[mem->_size] = 0;
            
        return buf_size;
    }

    POSTSender::~POSTSender() {
        _url = nullptr;
        _port = nullptr;
        _print = false;
    }

    POSTSender& POSTSender::SetUrl(const char* url) { 
        _url = url; 
        return *this;
    }

    POSTSender& POSTSender::SetPort(const char* port){
        _port = port; 
        return *this;
    }

    // Устанавливает флаг печати ответа
    POSTSender& POSTSender::SetPrint(bool print) {
        _print = print; 
        return *this;
    }

    // Подготавливает и отправляет запрос с телом jsonObject cstring + cstring
    POSTSender& POSTSender::SendRequest(const char* key, const char* value) {
        json::jsonObject* object = nullptr;
        json::MakeJsonObject(key, value, (void**)& object);
        return SendRequest(object);
    }
    
    // Подготавливает и отправляет запрос с телом jsonObject cstring + unsigned int
    POSTSender& POSTSender::SendRequest(const char* key, unsigned int* value) {
        json::jsonObject* object = nullptr;
        json::MakeJsonObject(key, value, (void**)& object);
        return SendRequest(object);
    }

    POSTSender& POSTSender::SendRequest(json::jsonObject*object) {

        CURL* curl = nullptr;
        CURLcode err_code;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
            curl_easy_setopt(curl, CURLOPT_URL, _url);
            curl_easy_setopt(curl, CURLOPT_PORT, _port);
            curl_easy_setopt(curl, CURLOPT_POST, 1);

            const char* body = object->ToString();
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            
            struct Response response = {0};
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RequestCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);

            err_code = curl_easy_perform(curl);

            if (err_code != CURLE_OK) {
                std::cerr << "POSTSender::send_error:\n" << curl_easy_strerror(err_code) << std::endl; 
            }

            if (_print) {
                _out << "Response income:\n" << response._response << std::endl;
            }
            
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);

            free((void*) body);
        }

        curl_global_cleanup();
        object->~jsonObject();
        free(object);

        return *this;
    }

} // namespace sender