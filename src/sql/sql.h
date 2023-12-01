#pragma once

#include <utility>
#include <stdlib.h>
#include "sqlite3.h"
#include <sstream>
#include "../json/json.h"

namespace sql {

    class db_Handler;

    namespace domain {
        
        static const char* _DB_TCP_HEADER = "tcp://";
        static const char* _DB_SEPARATOR = ":";
        static const char* _DB_COM_AT = "@";
        static const char* _DB_SLASH = "/";
        static const char* _DB_BSLASH = "\\";
        static const char* _DB_END_HEADER = ".db";

        static const char* _DB_ERROR = "SQL error: ";
        static const char* _DB_ERR_CANT_OPEN = "Can't open database: ";
        static const char* _DB_ERR_NOW_OPEN = "Database already is open\n";

        static const char* _HDLR_ERR_CON_LINE = "Connection line error: ";

        static const char* _DB_CREAT_TABLE_FIBONACCI = "CREATE TABLE IF NOT EXISTS Fibonacci (id INTEGER PRIMARY KEY AUTOINCREMENT, value INTEGER NOT NULL);";

        static const char* _DB_INSERT_TO_FIBONACCI = "INSERT INTO Fibonacci (value) VALUES (?);";
        static const char* _DB_SELECT_FROM_FIBONACCI = "SELECT * FROM Fibonacci;";
        static const char* _DB_DELETE_FROM_FIBONACCI = "DELETE FROM Fibonacci;";
        static const char* _DB_DROP_FIBONACCI = "DROP TABLE IF EXISTS Fibonacci;";
        static const char* _DB_COMMIT = "COMMIT;";
        static const char* _DB_BEGIN_TRANSACTION = "BEGIN TRANSACTION;";
        

        static const char* _DB_INSERT_HEAD = "INSERT INTO ";
        static const char* _DB_INSERT_MID = " (value) VALUES (";
        static const char* _DB_INSERT_END = ");";

        static unsigned int _INSERT_BUFF_SIZE = 40;
        static unsigned int _ERROR_BUFF_SIZE = 256;
                                                        

    } // namespace domain

    namespace call_back {

        // обратный вызов при селекте
        int SelectCallBack(void*, int, char**, char**);
    }
    
#define DB_TCP_HEADER domain::_DB_TCP_HEADER
#define DB_SEPARATOR domain::_DB_SEPARATOR
#define DB_COM_AT domain::_DB_COM_AT
#define DB_SLASH domain::_DB_SLASH
#define DB_BSLASH domain::_DB_BSLASH
#define DB_END_HEADER domain::_DB_END_HEADER

#define DB_ERR_CANT_OPEN domain::_DB_ERR_CANT_OPEN
#define DB_ERR_NOW_OPEN domain::_DB_ERR_NOW_OPEN
#define DB_ERROR domain::_DB_ERROR

    class db_ExecutionContext {
    private:
        db_Handler& _handler;
    public:
        db_ExecutionContext() = delete;
        db_ExecutionContext(const db_ExecutionContext&) = delete;
        db_ExecutionContext& operator=(const db_ExecutionContext&) = delete;
        db_ExecutionContext(db_ExecutionContext&&) = delete;
        db_ExecutionContext& operator=(db_ExecutionContext&&) = delete;

        explicit db_ExecutionContext(db_Handler& handler) : _handler(handler) {};

        // Выполняет команду начала транзакции
        db_ExecutionContext& Begin();

        // Выполняет команду закрытия транзакции
        db_ExecutionContext& Commit();

        // Выполняет команду удаления таблицы, по умолчанию установлено удаление таблицы Фибоначчи
        db_ExecutionContext& Drop(const char* command = domain::_DB_DROP_FIBONACCI);

        // Выполняет команду создания таблицы, по умолчанию установлено создание таблицы для числа Фибоначчи
        db_ExecutionContext& Create(const char* command = domain::_DB_CREAT_TABLE_FIBONACCI);

        // Очищает базу данных, по умолчанию установлена очистка таблицы Fibonacci
        db_ExecutionContext& Delete(const char* command = domain::_DB_DELETE_FROM_FIBONACCI);

        // Выполняет получение информации из базы данных, по умолчанию команда установлена на получение всех данных
        db_ExecutionContext& Select(const char* command = domain::_DB_SELECT_FROM_FIBONACCI);

        // Выполняет получение информации из базы данных, по умолчанию команда установлена на получение всех данных
        db_ExecutionContext& Select(const char* command, std::ostream*);

        // Выполняет операцию над базой данных
        template <typename Value>
        db_ExecutionContext& Insert(const char*, Value);

        // Закрывает соединение и возвращает упраление обработчику
        db_Handler& Close();
    private:

        enum op_Type {
            INSERT, SELECT, CREATE, DELETE, COMMIT, BEGIN, DROP
        };

        // Выполняет выбрасываение исключение возникшее из-за ошибки в работе
        db_ExecutionContext& ThrowException(const char*, const char*);

        // Выполняет операцию над базой данных
        db_ExecutionContext& Execute(const char*, op_Type, bool);

        // Подготавливает строку запроса на внесение данных
        template <typename Value>
        char* PrepareInsertCommand(const char*, Value);
    };

    template <typename Value>
    db_ExecutionContext& db_ExecutionContext::Insert(const char* table, Value value) {
        return Execute(PrepareInsertCommand(table, value), op_Type::INSERT, true);
    }

    template <typename Value>
    char* db_ExecutionContext::PrepareInsertCommand(const char* table, Value value) {

        using namespace json::c_string;
        using namespace json::domain;

        char* command = nullptr;

        unsigned int ins_head_size = StrLen(domain::_DB_INSERT_HEAD);
        unsigned int ins_tab_name_size = StrLen(table);
        unsigned int ins_mid_size = StrLen(domain::_DB_INSERT_MID);

        const char* c_value = ToString(value);
        unsigned int ins_value_size = StrLen(c_value);
        unsigned int ins_end_size = StrLen(domain::_DB_INSERT_END);

        unsigned int buff_size = ins_head_size + ins_tab_name_size + ins_mid_size + ins_value_size + ins_end_size + 1;

        command = (char*) malloc(buff_size);
        unsigned int pos = 0;

        pos = StrCat(command, pos, domain::_DB_INSERT_HEAD, ins_head_size);
        pos = StrCat(command, pos, table, ins_tab_name_size);
        pos = StrCat(command, pos, domain::_DB_INSERT_MID, ins_mid_size);

        pos = StrCat(command, pos, c_value, ins_value_size);
        pos = StrCat(command, pos, domain::_DB_INSERT_END, ins_end_size);
        command[pos] = '\0';

        if (IsNumeric<Value>::value) {
            free((void*) c_value);
        }
        
        return command;
    }

    class db_Handler {
        friend class db_ExecutionContext;
    public:
        db_Handler() = delete;
        db_Handler(const db_Handler&) = delete;
        db_Handler& operator=(const db_Handler&) = delete;

        ~db_Handler();

        explicit db_Handler(const char* name) : _db_name(name) {};

        // Открывает базу данных по полученному конфигу и возвращает контекст выполнения
        db_ExecutionContext Open();

        db_Handler& SetName(const char*);
        const char* GetName() const {return _db_name; }

    private:
        const char* _db_name = {};

        sqlite3* _data_base = nullptr;
        char* _error_msg = nullptr;
        int _report_code = 0;

        char* _connection_line = nullptr;
        bool _connection_status = false;

        // Подготавливает строку с названием файла библиотеки
        db_Handler& PrepareConnectLine();

        // Выполняет выбрасываение исключение возникшее из-за ошибки в работе
        db_Handler& ThrowException(const char*, const char*);
    };
    
} // namespace sql
