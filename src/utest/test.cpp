#include "test.h"

#include <cassert>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <type_traits>

#include "../com/com_line.h"
#include "../sql/sql.h"
#include "../math/math.h"
#include "../sender/sender.h"
#include "../app/domain.h"

namespace test {
    
    void CStringTest() {

        unsigned int num1_i = 3;
        const char* num1_s = "3";

        unsigned int num2_i = 19;
        const char* num2_s = "19";

        unsigned int num3_i = 675;
        const char* num3_s = "675";

        unsigned int num9_i = 615798541;
        const char* num9_s = "615798541";

        {
            const char* str1 = nullptr; 
            str1 = json::c_string::ToString(num1_i);

            assert(str1 != nullptr);

            assert(
                json::c_string::StrCmp(
                    str1, json::c_string::StrLen(str1), 
                    num1_s, json::c_string::StrLen(num1_s)) == 0);
            free((void*) str1);

            printf("CStringTest 1. Test. Pass\n");
        }

        {
            const char* str2 = json::c_string::ToString(num2_i);
            assert(
                json::c_string::StrCmp(
                    str2, json::c_string::StrLen(str2), 
                    num2_s, json::c_string::StrLen(num2_s)) == 0);
            free((void*) str2);

            printf("CStringTest 2. Test. Pass\n");
        }

        {
            const char* str3 = json::c_string::ToString(num3_i);
            assert(
                json::c_string::StrCmp(
                    str3, json::c_string::StrLen(str3), 
                    num3_s, json::c_string::StrLen(num3_s)) == 0);
            free((void*) str3);

            printf("CStringTest 3. Test. Pass\n");
        }

        {
            const char* str9 = json::c_string::ToString(num9_i);
            assert(
                json::c_string::StrCmp(
                    str9, json::c_string::StrLen(str9), 
                    num9_s, json::c_string::StrLen(num9_s)) == 0);
            free((void*) str9);

            printf("CStringTest 4. Test. Pass\n");
        }

    }

    void JsonTest() {

        const char* key = "Name";
        const char* value = "Vasya";

        unsigned int num_value = 15;

        const char* json = "{ \"Name\" : \"Vasya\" }";
        const char* json2 = "{ \"Name\" : 15 }";

        {
            json::jsonObject* object1 = (json::jsonObject*)malloc(sizeof(json::jsonObject));
            free(object1);
            printf("JsonObject 1. Test. Pass\n");
        }

        {
            json::jsonObject* object2 = nullptr;
            json::MakeJsonObject(key, value, (void**)& object2);

            assert(object2 != nullptr);

            assert(object2->_kSize == json::c_string::StrLen(key));
            assert(
                json::c_string::StrCmp(object2->_key, object2->_kSize, key, json::c_string::StrLen(key)) == 0);

            assert(object2->_vSize == json::c_string::StrLen(value));
            assert(
                json::c_string::StrCmp(object2->_value, object2->_vSize, value, json::c_string::StrLen(value)) == 0);

            object2->~jsonObject();
            free(object2);
            printf("JsonObject 2. Test. Pass\n");
        }

        {
            json::jsonObject* object3 = (json::jsonObject*)malloc(sizeof(json::jsonObject));
            json::MakeJsonObject(key, value, (void**)& object3);

            assert(object3 != nullptr);

            assert(object3->_kSize == json::c_string::StrLen(key));
            assert(
                json::c_string::StrCmp(object3->_key, object3->_kSize, key, json::c_string::StrLen(key)) == 0);

            assert(object3->_vSize == json::c_string::StrLen(value));
            assert(
                json::c_string::StrCmp(object3->_value, object3->_vSize, value, json::c_string::StrLen(value)) == 0);

            object3->~jsonObject();
            free(object3);
            printf("JsonObject 3. Test. Pass\n");
        }

        {
            json::jsonObject* object4 = nullptr;
            json::MakeJsonObject(key, value, (void**)& object4);


            const char* string = object4->ToString();

            assert(object4 != nullptr);
            assert(string != nullptr);

            assert(
                json::c_string::StrCmp(
                    string, json::c_string::StrLen(string), 
                    json, json::c_string::StrLen(json)) == 0);

            object4->~jsonObject();
            free((void*)string);
            free(object4);
            printf("JsonObject 4. Test. Pass\n");
        }

        {
            json::jsonObject* object5 = nullptr;
            json::MakeJsonObject(key, &num_value, (void**)& object5);


            const char* string = object5->ToString();

            assert(object5 != nullptr);
            assert(string != nullptr);

            assert(
                json::c_string::StrCmp(
                    string, json::c_string::StrLen(string), 
                    json2, json::c_string::StrLen(json2)) == 0);

            object5->~jsonObject();
            free((void*)string);
            free(object5);
            printf("JsonObject 5. Test. Pass\n");
        }

    }

    void CommandTest() {

        {
            static const char* command_line[] = {"myapp.out", "-p=10", "--db-name=my_test_db", "-t=10", "-w=www.site.net", "--web-port=61"};
            com::args::Arguments arg = com::ParseCommandLine(6, const_cast<char**>(command_line));

            assert(!arg._run_unit_tests);
            assert(!arg._show_help);

            assert(arg._timer_enable);
            assert(arg._timer_period == 10);
            assert(arg._save_in_data_base);
            assert(arg._db_name == "my_test_db");
            assert(arg._send_post_request);
            assert(arg._web_address == "www.site.net");
            assert(arg._web_port == "61");

            printf("CommandTest 1. Test. Pass\n");
        }

        {
            static const char* command_line[] = {"myapp.out", "-u"};
            com::args::Arguments arg = com::ParseCommandLine(2, const_cast<char**>(command_line));

            assert(arg._run_unit_tests);
            assert(!arg._show_help);

            assert(!arg._timer_enable);
            assert(arg._timer_period == 0);
            assert(!arg._save_in_data_base);
            assert(arg._db_name == "NOAP");
            assert(!arg._send_post_request);
            assert(arg._web_address == "NOAP");
            assert(arg._web_port == "NOAP");

            printf("CommandTest 2. Test. Pass\n");
        }
    }

    void SenderTest() {

        unsigned int num1 = 42;

        sender::POSTSender sender(std::cout, domain::ECHO_ADDRESS, domain::ECHO_PORT, false);

        sender.SendRequest("Sign", "42");
        sender.SendRequest("Sign", &num1);

        printf("Sender 1. Test. Pass\n");
    }

    void FibonacciTest() {

        {
            math::Fibonacci fib(32);
            assert(fib() == 0);
            assert(fib() == 1);

            assert(fib._size == 2);
            assert(*fib._n1 == 1);
            assert(*fib._n2 == 0);

            assert(fib() == 1);
            assert(fib() == 2);

            assert(fib._size == 4);
            assert(*fib._n1 == 2);
            assert(*fib._n2 == 1);

            printf("Fibonacci 1. Test. Pass\n");
        }

        {
            math::Fibonacci fib(32);
            assert(fib.GetConcretValue(4) == 3);
            assert(fib._size == 5);

            assert(fib.GetConcretValue(10) == 55);
            assert(fib._size == 11);
            assert(*fib._n1 == 55);
            assert(*fib._n2 == 34);

            printf("Fibonacci 2. Test. Pass\n");
        }

        {
            math::Fibonacci fib(5);
            assert(fib.GetConcretValue(3) == 2);
            assert(fib._size == 4);

            assert(fib.GetConcretValue(10) == 55);
            assert(fib._size == 11);
            assert(*fib._n1 == 55);
            assert(*fib._n2 == 34);

            printf("Fibonacci 3. Test. Pass\n");
        }
        
    }

    void Sqlite3WriteTest() {

        sql::db_Handler handler(domain::TEST_DB_NAME);

        auto context = handler.Open();
        context.Begin();
        context.Create();
        context.Insert("Fibonacci", 42);
        context.Insert("Fibonacci", 9);
        context.Insert("Fibonacci", 714);
        context.Commit();
        auto& handler_ref = context.Close();
        assert(handler_ref.GetName() == domain::TEST_DB_NAME);

        printf("Sqlite3WriteTest. Test. Pass\n");
    }

    void Sqlite3ReadTest() {

        std::stringstream stream;
        const char* correct_return = "id: 1\nvalue: 42\nid: 2\nvalue: 9\nid: 3\nvalue: 714\n";

        sql::db_Handler handler(domain::TEST_DB_NAME);
        auto context = handler.Open();
        context.Begin();
        context.Select(sql::domain::_DB_SELECT_FROM_FIBONACCI, &stream);
        context.Commit();
        auto& handler_ref = context.Close();

        assert(json::c_string::StrCmp(stream.str().data(), correct_return) == 0);
        handler.Open().Begin().Drop().Commit().Close();

        printf("Sqlite3ReadTest. Test. Pass\n");
    }

    void UnitTests() {

        CStringTest();
        JsonTest();
        CommandTest();
        SenderTest();
        FibonacciTest();
        Sqlite3WriteTest();
        Sqlite3ReadTest();
    }

} // namespace test
