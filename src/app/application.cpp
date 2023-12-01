#include "application.h"
#include "stdlib.h"

#include <iostream>
#include <functional>

#ifdef __linux__
    #include "pthread.h"
    #include <unistd.h>
#elif _WIN32
    #include <thread>
    #include <chrono>
#endif

namespace app {

    using namespace domain;

    Application::Application(com::args::Arguments&& args, bool run = true) 
        : _args(std::move(args)), _verbose(args._verbose), _fib_in_cerr(args._fib_in_cerr) {
        PrepareResources();
        if (run) Run();
    } 

    Application::Application(com::args::Arguments&& args) 
        : Application(std::move(args), true) {
    } 

    Application::Application(int args, char** argv, bool run = true) 
        : Application(std::move(com::ParseCommandLine(args, argv)), run) {

    }

    Application::Application(int args, char** argv)
        : Application(args, argv, true) {}

    Application::~Application() {
        FreeResources();
    }

    Application& Application::Run() {
        return (_args._timer_enable) ? InTimeExecution() : OneTimeExecution();
    }
    Application& Application::Abort() {
        return *this;
    }

    Application& Application::InTimeExecution() {

        auto lambda = [this](std::ostream& out) {

            unsigned int fib = (*_fib)();

            if (_send) {
                _send->SendRequest(DB_TABLE, &fib);
            }

            if (_sql) {
                _sql->Open().Begin().Create().Insert(DB_TABLE, fib).Commit().Close();
            }

        };

        while (true)
        {
            lambda(std::ref(std::cerr));

#ifdef __linux__
            sleep(_args._timer_period);
#elif _WIN32
            std::this_thread::sleep_for(std::chrono::seconds(_args._timer_period));
#endif
            
        }

        return *this;
    }

    
    Application& Application::OneTimeExecution() {
        unsigned int fib = (*_fib)();

        if (_send) {
            _send->SendRequest(DB_TABLE, &fib);
        }

        if (_sql) {
            _sql->Open().Begin().Create().Insert(DB_TABLE, fib).Commit().Close();
        }

        return *this;
    }

    Application& Application::VerboseInStdCerr(const char* report) {
        return (_verbose) ? ReportInStdCerr(report) : *this;
    }

    Application& Application::ReportInStdCerr(const char* report) {
        std::cerr << report << std::endl;
        return *this;
    }

    void Application::RunUnitTests() {
        test::UnitTests();
        std::exit(0);
    }

    void Application::PrepareResources() {

        if (_args._run_unit_tests) {
            VerboseInStdCerr(MSG_RUN_UTESTS).RunUnitTests();
        }

        /*
            Так как все можули написаны в стиле С++, то алокация памяти 
            вызывается стандартным образом для работы конструкторов
        */

        if (_args._send_post_request) {
            _send = new sender::POSTSender(
                std::cerr, _args._web_address.data(), _args._web_port.data(), _fib_in_cerr);
            VerboseInStdCerr(MSG_DEFINE_SENDER);
        }

        if (_args._save_in_data_base) {
            _sql = new sql::db_Handler(_args._db_name.data());
            VerboseInStdCerr(MSG_DEFINE_SQL);
        }
        
        _fib = new math::Fibonacci(domain::FIB_BUFFER);
        VerboseInStdCerr(MSG_DEFINE_FIB);
    }

    void Application::FreeResources() {

        if (_fib) {
            delete _fib;
        } 
        
        if (_send) {
            delete _send;
        }

        if (_sql) {
             delete _sql;
        }

    }

} // namespace app