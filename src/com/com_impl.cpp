#include "com_line.h"

#include <exception>
#include <stdexcept>
#include <iostream>
#include <csignal>
#include <cstdlib>

namespace com {

    namespace args { 

        namespace detail {

            void operator++(PARAM_TYPE& param) {

                switch (param)
                {
                case PARAM_TYPE::NOAP:
                    param = PARAM_TYPE::HELP;
                    break;

                case PARAM_TYPE::HELP:
                    param = PARAM_TYPE::UTEST;
                    break;

                case PARAM_TYPE::UTEST:
                    param = PARAM_TYPE::VERBOSE;
                    break;

                case PARAM_TYPE::VERBOSE:
                    param = PARAM_TYPE::FIB_CERR;
                    break;

                case PARAM_TYPE::FIB_CERR:
                    param = PARAM_TYPE::TIMER_PERIOD;
                    break;
                
                case PARAM_TYPE::TIMER_PERIOD:
                    param = PARAM_TYPE::WEB_ADDRESS;
                    break;

                case PARAM_TYPE::WEB_ADDRESS:
                    param = PARAM_TYPE::WEB_PORT;
                    break;

                case PARAM_TYPE::WEB_PORT:
                    param = PARAM_TYPE::DB_NAME;
                    break;

                case PARAM_TYPE::DB_NAME:
                    param = PARAM_TYPE::NOAP;
                    break;
            
                default:
                    break;
                }
            }

            // Возваращает пару короткого и длинного варианта команды
            std::pair<std::string_view, std::string_view> GetParamExpression(const PARAM_TYPE& param) {
                switch (param)
                {
                case PARAM_TYPE::NOAP:
                    return std::make_pair(_NOAP, _NOAP);

                case PARAM_TYPE::VERBOSE:
                    return std::make_pair(ParamType::VERBOSE_SHORT, ParamType::VERBOSE_LARGE);

                case PARAM_TYPE::FIB_CERR:
                    return std::make_pair(ParamType::FIB_CERR_SHORT, ParamType::FIB_CERR_LARGE);

                case PARAM_TYPE::UTEST:
                    return std::make_pair(ParamType::TEST_SHORT, ParamType::TEST_LARGE);

                case PARAM_TYPE::HELP:
                    return std::make_pair(ParamType::HELP_SHORT, ParamType::HELP_LARGE);
                
                case PARAM_TYPE::TIMER_PERIOD:
                    return std::make_pair(ParamType::TIMER_PERIOD_SHORT, ParamType::TIMER_PERIOD_LARGE);

                case PARAM_TYPE::WEB_ADDRESS:
                    return std::make_pair(ParamType::WEB_ADDRESS_SHORT, ParamType::WEB_ADDRESS_LARGE);

                case PARAM_TYPE::WEB_PORT:
                    return std::make_pair(ParamType::WEB_PORT_SHORT, ParamType::WEB_PORT_LARGE);

                case PARAM_TYPE::DB_NAME:
                    return std::make_pair(ParamType::DB_NAME_SHORT, ParamType::DB_NAME_LARGE);

                default:
                    return std::make_pair(_NOAP, _NOAP);
                }
            }

            // Выводит в поток лист помощи с описанием всех комманд
            void PrintTableHeader(std::ostream& out) {

                out << "№"  << std::setw(_CW_NUMBER) << _CW_SEPARATOR;
                out << "Short" << _CW_SEPARATOR;
                out << "Large command" << std::setw(_CW_LARGE_COMMAND - 13) << _CW_SEPARATOR;
                out << "Description" << std::setw(0);
                out << std::endl;

            }

            // Выводит в поток лист помощи с описанием всех комманд
            void PrintHelpList(std::ostream& out) {
                using ParamExpression = std::pair<std::string_view, std::string_view>;
                out << "Use next console arguments" << std::endl;
                
                PrintTableHeader(out);
                PARAM_TYPE param = PARAM_TYPE::HELP;
                while (param != PARAM_TYPE::NOAP)
                {
                    ParamExpression expression = GetParamExpression(param);
                    if (expression.first != _NOAP && expression.second != _NOAP ) {

                        out << static_cast<int>(param) << std::setw(_CW_NUMBER) << _CW_SEPARATOR;
                        out << expression.first << std::setw(_CW_SHORT_COMMAND) << _CW_SEPARATOR;
                        out << expression.second << std::setw(_CW_LARGE_COMMAND - static_cast<int>(expression.second.size())) << _CW_SEPARATOR;
                        out << _PARAM_DESCR.at(param) << std::setw(0);
                        out << std::endl;
                    }

                    ++param;
                }

                out << "\nFor example:\n";
                out << "\t" << ParamType::TIMER_PERIOD_LARGE << "=15 "; 
                out << ParamType::WEB_ADDRESS_LARGE << "=https://www.web_site_com ";
                out << ParamType::WEB_PORT_SHORT << "=8183 ";
                out << ParamType::DB_NAME_LARGE << "=my_test_data_base\n";
                out << "\nTry run application again whit correct arguments" << std::endl;
            }

            // Првоеряет количество переданных параметров
            bool ParamCountIsCorrect(int count) {
                return count >= _MIN_ARGS && count < args::detail::_PARAM_COUNT + 1;
            }

            // Возвращает тип параметра по полученной строке
            PARAM_TYPE GetParamType(const char* argv) {
                return _COMMAND_VARIANTS.count(argv) ? _COMMAND_VARIANTS.at(argv) : PARAM_TYPE::NOAP;
            }            

        } // namespace detail
        
    } // namespace args

    using namespace std::string_literals;

    HelpViewer::~HelpViewer() {
        std::cerr << this->what() << std::endl;
        this->PrintArgumentsTypes();
        std::exit(0);
    }

    void HelpViewer::PrintArgumentsTypes() const {
        args::detail::PrintHelpList(std::cerr);
    }

    void UpdateArgumentsFlags(args::Arguments* args) {

        // вывод листа с помощью и выключение приложения
        if (args->_show_help)
            HelpViewer("").~HelpViewer();

        // включение таймера
        if (args->_timer_period != args::detail::_NOAT)
            args->_timer_enable = true;

        // включение отправки пост запроса
        if (args->_web_address != args::detail::_NOAP) 
            if (args->_web_port != args::detail::_NOAP) {
                args->_send_post_request = true;
            }
            else {
                HelpViewer("command line has incorrect web_port");
            }
            
        // включение сохранения файла базы данных
        if (args->_db_name != args::detail::_NOAP) 
            args->_save_in_data_base = true;
            

    }

    template <typename Arg>
    void SetDataToArguments(args::detail::PARAM_TYPE type, args::Arguments* data, Arg arg) {
        switch (type)
        {

        case args::detail::PARAM_TYPE::VERBOSE:
            data->_verbose = true;
            break;

        case args::detail::PARAM_TYPE::FIB_CERR:
            data->_fib_in_cerr = true;
            break;
        
        case args::detail::PARAM_TYPE::UTEST:
            data->_run_unit_tests = true;
            break;

        case args::detail::PARAM_TYPE::HELP:
            data->_show_help = true;
            break;
        
        case args::detail::PARAM_TYPE::TIMER_PERIOD:
            data->_timer_period = std::stoi(static_cast<std::string>(arg));
            break;

        case args::detail::PARAM_TYPE::WEB_ADDRESS:
            data->_web_address = arg;
            break;

        case args::detail::PARAM_TYPE::WEB_PORT:
            data->_web_port = arg;
            break;

        case args::detail::PARAM_TYPE::DB_NAME:
            data->_db_name = arg;
            break;
        
        default:
            break;
        }
    }

    void ParseSingleCommand(args::Arguments* data, const char* line) {
        std::string command = "";
        std::string argument = "";

        bool arg = false;


        while (*line != '\00')
        {
            if (*line != '=')
            {
                if (!arg)
                {
                    command.push_back(*line);
                }
                else 
                {
                    argument.push_back(*line);
                }
            }
            else 
            {
                arg = true;
            }

            ++line;
        }

        args::detail::PARAM_TYPE type = args::detail::GetParamType(command.data());
        SetDataToArguments(type, data, argument);
     
    }
    
    args::Arguments ParseCommandLine(int args, char** argv) {

        if (!args::detail::ParamCountIsCorrect(args)) {
            HelpViewer("command line has incorrect arguments count");
        }

        args::Arguments result;
        for (int i = 1; i != args; ++i) {
            ParseSingleCommand(&result, argv[i]);
        }

        UpdateArgumentsFlags(&result);
        return std::move(result);
    }

} // namespace com 