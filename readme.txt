 /*
    Используя только stdlib, API cURL и SQlite3 написать консольное приложение, 
    которое с периодичностью 1 секунда генерирует новое значение из ряда Фибоначчи 
    и отправляет его на сервер в виде JSON объекта в теле POST запроса по протоколу 
    HTTP2, а также записывает это число в локальную БД на диске.

    Подготовить Docker образ на базе Debian12 и bash-скрипт для запуска консольного приложения из Docker-контейнера.
*/

Консольные ключи запуска
№ | Short | Large command     | Description
1 | -h    | --help            | Show console commands descriptions
2 | -u    | --utest           | Launch Unit-tests
3 | -v    | --verbose         | Show debug information in std::cerr
4 | -c    | --fib-in-cerr     | Show fibonacci numbers in std::cerr
5 | -t    | --timer-period    | Set timer period, in sec
6 | -w    | --web-address     | URL-address to send info
7 | -p    | --web-port        | URL-address port to send info
8 | -d    | --db-name         | Database filename

Сборка под Linux

# mkdir -p build-release 
# cd build-release
# conan install .. --build=missing -s build_type=Release -s compiler.libcxx=libstdc++11
# cmake .. -DCMAKE_BUILD_TYPE=Release
# cd ..

# mkdir -p build-debug
# cd build-debug
# conan install .. --build=missing -s build_type=Debug -s compiler.libcxx=libstdc++11
# cmake .. -DCMAKE_BUILD_TYPE=Debug
# cd ..


Сборка под Windows
Нужно выполнить два шага:

В conanfile.txt нужно изменить cmake на cmake_multi. После этого можно конфигурировать таким способом:
В CMakeLists.txt заменить include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake) на include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake).
После этого можно запустить подобный снипет:

# mkdir build 
# cd build
# conan install .. --build=missing -s build_type=Debug
# conan install .. --build=missing -s build_type=Release
# conan install .. --build=missing -s build_type=RelWithDebInfo
# conan install .. --build=missing -s build_type=MinSizeRel
# cmake ..
