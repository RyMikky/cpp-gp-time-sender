#pragma once

#include "domain.h"

#include "../sender/sender.h"
#include "../com/com_line.h"
#include "../math/math.h"
#include "../sql/sql.h"

#include "../utest/test.h"

namespace app {

#define ARGUMENTS com::args::Arguments
#define SENDER sender::POSTSender* _send
#define FIBONACCI math::Fibonacci* _fib
#define SQL_HANDLER sql::db_Handler* _sql

    class Application {
    private:
        ARGUMENTS _args = {};
    public: 
        Application() = delete;
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(Application&&) = delete;

        Application(com::args::Arguments&& /*args*/, bool /*run*/); 
        explicit Application(com::args::Arguments&& /*args*/); 

        Application(int /*args*/, char** /*argv*/, bool /*run*/);
        Application(int /*args*/, char** /*argv*/);
        
        ~Application();

        // Запускает выполнение приложения
        Application& Run();

        // Останавливает выполнение приложения
        Application& Abort();

    private:
        SQL_HANDLER = nullptr; 
        FIBONACCI = nullptr;
        SENDER = nullptr;

        bool _verbose = false;
        bool _fib_in_cerr = false;

        // Выполняет команды по таймеру с помощью EXECUTOR
        Application& InTimeExecution();

        // Выполняет команды единожды, при заданном нулевом периоде
        Application& OneTimeExecution();

        // Выполняет вывод строки в окно потока ошибок если установлен флаг _verbose
        Application& VerboseInStdCerr(const char*);

        // Выполняет вывод строки в окно потока ошибок если установлен флаг _verbose
        Application& ReportInStdCerr(const char*);

        // Запускает выполнение тестов и выключает приложение
        void RunUnitTests(); 

        // Подготавливает ресурсы для выполнения требуемых операций
        void PrepareResources();

        // Освобождает память использованных указателей на экземпляры компонентов системы
        void FreeResources();
    };


} // namespace app