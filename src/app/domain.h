#pragma once

namespace domain {

    namespace constantes {
        
        // Адрес сервера для проверки POST-запросов
        static const char* _ECHO_TEST_ADDRESS = "https://echo.free.beeceptor.com/";
        // Порт сервера для проверки POST-запросов
        static const char* _ECHO_TEST_PORT = "80";
        // Название для тестовой базы данных
        static const char* _TEST_DB = "sqlite3_test_db";
        // Стартовый размер буфера для чисел Фибоначчи
        static const unsigned int _FIB_START_BUFFER = 256;
        // Название по умолчанию для таблицы в базе данных
        static const char* _DB_DEFAULT_TABLE = "Fibonacci";

    } // namespace constantes
    
#define ECHO_ADDRESS constantes::_ECHO_TEST_ADDRESS                              // Адрес сервера для проверки POST-запросов
#define ECHO_PORT constantes::_ECHO_TEST_PORT                                    // Порт сервера для проверки POST-запросов
#define TEST_DB_NAME constantes::_TEST_DB                                        // Название для тестовой базы данных
#define FIB_BUFFER constantes::_FIB_START_BUFFER                                 // Стартовый размер буфера для чисел Фибоначчи
#define DB_TABLE constantes::_DB_DEFAULT_TABLE                                   // Название по умолчанию для таблицы в базе данных

    namespace message {

        static const char* _RUN_UTESTS = "Application run UnitTests";
        static const char* _DEFINE_TIMER = "Define InTime executor";
        static const char* _DEFINE_SENDER = "Define CURL-request sender";
        static const char* _DEFINE_SQL = "Define SQLite3 handler";
        static const char* _DEFINE_FIB = "Define Fibonacci math. module";

    } // namespace message

#define MSG_RUN_UTESTS message::_RUN_UTESTS
#define MSG_DEFINE_TIMER message::_DEFINE_TIMER
#define MSG_DEFINE_SENDER message::_DEFINE_SENDER
#define MSG_DEFINE_SQL message::_DEFINE_SQL
#define MSG_DEFINE_FIB message::_DEFINE_FIB
    
} // namespace domain
