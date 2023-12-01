#pragma once
#include "com_args.h"

// Обработка командной строки запуска
namespace com {

    class HelpViewer : std::invalid_argument
    {
    public:
        explicit HelpViewer(const char* line) : std::invalid_argument(line) {}
        ~HelpViewer();
    private:
        // Выводит в поток ошибок информацию по команде --help
        void PrintArgumentsTypes() const;
    };
    

    args::Arguments ParseCommandLine(int args, char** argv);

} // namespace com