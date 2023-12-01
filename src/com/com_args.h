#pragma once

#include <string>
#include <utility>
#include <numeric>

#include <iostream>
#include <iomanip>
#include <string_view>
#include <unordered_map>

using namespace std::string_view_literals;

namespace com {

    namespace args {

        namespace detail {

            // Not a Parameter
            static const std::string_view _NOAP = "NOAP";
            // Not a Timer
            static const int _NOAT = 0;
            // Минимальное число аргументов командной строки
            static const int _MIN_ARGS = 2;

            enum class PARAM_TYPE {
                NOAP, HELP, UTEST, VERBOSE, FIB_CERR, TIMER_PERIOD, WEB_ADDRESS, WEB_PORT, DB_NAME
                // NOAP - Not a Parameter
            };

            // Инкремент для enum PARAM_TYPE
            void operator++(PARAM_TYPE&);

            // Описание формальных параметров
            static const std::unordered_map<PARAM_TYPE, std::string_view> _PARAM_DESCR = {

                { PARAM_TYPE::UTEST, "Launch Unit-tests" },
                { PARAM_TYPE::HELP, "Show console commands descriptions" },
                { PARAM_TYPE::VERBOSE, "Show debug information in std::cerr" },
                { PARAM_TYPE::FIB_CERR, "Show fibonacci numbers in std::cerr" },
                { PARAM_TYPE::TIMER_PERIOD, "Set timer period, in sec" },

                { PARAM_TYPE::WEB_ADDRESS, "URL-address to send info" },
                { PARAM_TYPE::WEB_PORT, "URL-address port to send info" },

                { PARAM_TYPE::DB_NAME, "Database filename" }
            };

            // Варианты формальных параметров
            static const std::unordered_map<std::string_view, PARAM_TYPE> _COMMAND_VARIANTS = {

                { "--utest"sv, PARAM_TYPE::UTEST }, { "-u"sv, PARAM_TYPE::UTEST }, 
                { "--verbose"sv, PARAM_TYPE::VERBOSE }, { "-v"sv, PARAM_TYPE::VERBOSE }, 
                { "--fib-in-cerr"sv, PARAM_TYPE::FIB_CERR }, { "-c"sv, PARAM_TYPE::FIB_CERR },
                { "--help"sv, PARAM_TYPE::HELP }, { "-h"sv, PARAM_TYPE::HELP }, 

                { "--timer-period"sv, PARAM_TYPE::TIMER_PERIOD }, { "-t"sv, PARAM_TYPE::TIMER_PERIOD }, 
                { "--web-address"sv, PARAM_TYPE::WEB_ADDRESS }, { "-w"sv, PARAM_TYPE::WEB_ADDRESS }, 
                { "--web-port"sv, PARAM_TYPE::WEB_PORT }, { "-p"sv, PARAM_TYPE::WEB_PORT },
                { "--db-name"sv, PARAM_TYPE::DB_NAME }, { "-d"sv, PARAM_TYPE::DB_NAME }
            };

            struct ParamType {
                ParamType() = default;
                constexpr static std::string_view TEST_SHORT = "-u";
                constexpr static std::string_view TEST_LARGE = "--utest";

                constexpr static std::string_view FIB_CERR_SHORT = "-c";
                constexpr static std::string_view FIB_CERR_LARGE = "--fib-in-cerr";

                constexpr static std::string_view VERBOSE_SHORT = "-v";
                constexpr static std::string_view VERBOSE_LARGE = "--verbose";
                
                constexpr static std::string_view HELP_SHORT = "-h";
                constexpr static std::string_view HELP_LARGE = "--help";
                constexpr static std::string_view TIMER_PERIOD_SHORT = "-t";
                constexpr static std::string_view TIMER_PERIOD_LARGE = "--timer-period";

                constexpr static std::string_view WEB_ADDRESS_SHORT = "-w";
                constexpr static std::string_view WEB_ADDRESS_LARGE = "--web-address";
                constexpr static std::string_view WEB_PORT_SHORT = "-p";
                constexpr static std::string_view WEB_PORT_LARGE = "--web-port";

                constexpr static std::string_view DB_NAME_SHORT = "-d";
                constexpr static std::string_view DB_NAME_LARGE = "--db-name";
            };

            static const std::string_view _CW_SEPARATOR = " | ";

            // Ширина строки для вывода написания индекса | CW - Console Width
            static const int _CW_NUMBER = 2;
            // Ширина строки для вывода написания команды | CW - Console Width
            static const int _CW_LARGE_COMMAND = 20;
            // Ширина строки для вывода написания короткой команды | CW - Console Width
            static const int _CW_SHORT_COMMAND = 6;
            // Ширина строки для вывода текстового описания команды | CW - Console Width
            static const int _CW_COMMAND_DESCRIPTION = 50;

            // Количество формальных параметров
            static const size_t _PARAM_COUNT = _COMMAND_VARIANTS.size() / 2;

            // Возваращает пару короткого и длинного варианта команды
            std::pair<std::string_view, std::string_view> GetParamExpression(const PARAM_TYPE& param);

            // Выводит в поток лист помощи с описанием всех комманд
            void PrintTableHeader(std::ostream&);

            // Выводит в поток лист помощи с описанием всех комманд
            void PrintHelpList(std::ostream&);

            // Првоеряет количество переданных параметров
            bool ParamCountIsCorrect(int);

            // Возвращает тип параметра по полученной строке
            PARAM_TYPE GetParamType(const char*);

        } // namespace detail

        struct Arguments {
            bool _verbose = false;
            bool _fib_in_cerr = false;
            bool _run_unit_tests = false;
            bool _show_help = false;
            bool _timer_enable = false;
            unsigned int _timer_period = detail::_NOAT;
            bool _send_post_request = false;
            std::string _web_address = std::string(detail::_NOAP);
            std::string _web_port = std::string(detail::_NOAP);
            bool _save_in_data_base = false;
            std::string _db_name = std::string(detail::_NOAP);
        };

    } // namespace args

} // namespace com