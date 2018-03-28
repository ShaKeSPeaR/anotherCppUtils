#pragma once
#ifndef _APPU_TEST_TYPES_
#define _APPU_TEST_TYPES_

#incldue <string>

/*
    Data structures for tests and examples
*/

//simple struct with two fields of standart types
struct Foo
{
    std::string F1;
    int F2;
};

//some enum type
enum class Digit : uint8_t
{
    Zero = 0,
    One, 
    Two    
};

//struct with three fields, use user-defined enum, uint64_t and double
struct Bar
{
    Digit    F1;
    uint64_t F2;
    int      F3;
    double   F4;
};

#endif //_APPU_TEST_TYPES_