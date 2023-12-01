#include "sql.h"

#include <stdio.h>
#include <exception>
#include <stdexcept>

namespace sql {

    using namespace json::c_string;

    namespace call_back {


        int SelectCallBack(void* data, int argc, char** argv, char** azColName) {

            std::ostream* out = static_cast<std::ostream*>(data);
            for (int i = 0; i < argc; ++i) {
                *out<< azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
            }

            return 0;
        }
    }

    db_ExecutionContext& db_ExecutionContext::Begin() {
        return Execute(domain::_DB_BEGIN_TRANSACTION, op_Type::BEGIN, false);
    }

    db_ExecutionContext& db_ExecutionContext::Commit() {
        return Execute(domain::_DB_COMMIT, op_Type::COMMIT, false);
    }

    db_ExecutionContext& db_ExecutionContext::Delete(const char* command) {
        return Execute(command, op_Type::DELETE, false);
    }

    db_ExecutionContext& db_ExecutionContext::Drop(const char* command) {
        return Execute(command, op_Type::DROP, false);
    }

    db_ExecutionContext& db_ExecutionContext::Create(const char* command) {
        return Execute(command, op_Type::CREATE, false);
    }

    db_ExecutionContext& db_ExecutionContext::Select(const char* command){
        return Execute(command, op_Type::SELECT, false);
    }

    db_ExecutionContext& db_ExecutionContext::Select(const char* command, std::ostream* out) {
        _handler._report_code = sqlite3_exec(
                _handler._data_base, command, call_back::SelectCallBack, out, &_handler._error_msg);

        if (_handler._report_code != SQLITE_OK) {
            ThrowException(DB_ERROR, _handler._error_msg);
        }

        return *this;
    }


    db_Handler& db_ExecutionContext::Close() {
        sqlite3_close(_handler._data_base);
        free((void*)_handler._connection_line);
        _handler._connection_status = false;
        _handler._connection_line = nullptr;
        return _handler;
    }

    // Выполняет выбрасываение исключение возникшее из-за ошибки в работе
    db_ExecutionContext& db_ExecutionContext::ThrowException(const char* err_header, const char* err_msg) {
        unsigned int err_header_size = StrLen(err_header);
        unsigned int err_msg_size = StrLen(err_msg);

        unsigned buff_size = err_header_size + err_msg_size + 1;

        char* err_line = (char*) malloc(buff_size);
        unsigned int err_pos = 0;
        err_pos = StrCat(err_line, err_pos, err_header, err_header_size);
        err_pos = StrCat(err_line, err_pos, err_msg, err_msg_size);
        err_line[buff_size] = '\0';
        
        throw std::domain_error(err_line);
    }

    db_ExecutionContext& db_ExecutionContext::Execute(const char* command, op_Type operation, bool mem_free) {

        switch (operation)
        {
        case op_Type::SELECT:
            _handler._report_code = sqlite3_exec(
                _handler._data_base, command, call_back::SelectCallBack, 0, &_handler._error_msg);
            break;

        case op_Type::DROP:
        case op_Type::BEGIN:
        case op_Type::CREATE:    
        case op_Type::DELETE:
        case op_Type::COMMIT:
        case op_Type::INSERT:
            _handler._report_code = sqlite3_exec(
                _handler._data_base, command, 0, 0, &_handler._error_msg);
            break;

        default:
            break;
        }

        if (mem_free) {
            free((void*) command);
        }

        if (_handler._report_code != SQLITE_OK) {
            ThrowException(DB_ERROR, _handler._error_msg);
        }

        return *this;
    }

    db_Handler::~db_Handler() {
        if (_connection_status) sqlite3_close(_data_base);
        if (_connection_line != nullptr) free((void*) _connection_line);
        if (_error_msg != nullptr) sqlite3_free(_error_msg);
    }

    db_ExecutionContext db_Handler::Open()  {

        if (_connection_status) {
            ThrowException(DB_ERR_NOW_OPEN, "");
        }

        PrepareConnectLine();

        _report_code = sqlite3_open(_connection_line, &_data_base);

        if (_report_code) {
            ThrowException(DB_ERR_CANT_OPEN, sqlite3_errmsg(_data_base));
        }

        _connection_status = true;
        return db_ExecutionContext(*this);

    }
    
    db_Handler& db_Handler::SetName(const char* name) {
        _db_name = name;
        return *this;
    }

    db_Handler& db_Handler::PrepareConnectLine() {

        if (_connection_line != nullptr) {
            return ThrowException(domain::_HDLR_ERR_CON_LINE, "already exist");
        }
        unsigned int pos = 0;

        unsigned int db_name_size = 0;
        unsigned int db_end_size = 0;

        db_name_size = StrLen(_db_name);
        db_end_size = StrLen(DB_END_HEADER);

        unsigned buff_size = db_name_size + db_end_size + 1;

        _connection_line = (char*) malloc(buff_size);

        pos = StrCat(_connection_line, pos, _db_name, db_name_size);
        pos = StrCat(_connection_line, pos, DB_END_HEADER, db_end_size);
        _connection_line[pos] = '\0';
        
        return *this;

    }

    // Выполняет выбрасываение исключение возникшее из-за ошибки в работе
    db_Handler& db_Handler::ThrowException(const char* err_header, const char* err_msg) {
        unsigned int err_header_size = StrLen(err_header);
        unsigned int err_msg_size = StrLen(err_msg);

        unsigned buff_size = err_header_size + err_msg_size + 1;

        char* err_line = (char*) malloc(buff_size);
        unsigned int err_pos = 0;
        err_pos = StrCat(err_line, err_pos, err_header, err_header_size);
        err_pos = StrCat(err_line, err_pos, err_msg, err_msg_size);
        err_line[buff_size] = '\0';
        
        throw std::domain_error(err_line);
    }

} // namespace sql