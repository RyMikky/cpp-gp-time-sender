#include "app/application.h"

int main(int args, char** argv) {

    setlocale(LC_ALL, "Russian");
    setlocale(LC_NUMERIC, "English");

    app::Application app(args, argv);
    return 0;   
}