#include <cassert>
#include <reflection/basic_reflection.h>
#include <examples/test_types.h>
/*
    Examples for tuple_utils 
*/

//specialization for enable reflection
namespace reflection {namespace basic {
    template<>
    inline constexpr auto reflect_as_tie(Foo& d) noexcept { return std::tie(d.F1, d.F2);};
    
    //F3 in AnotherData may be omitted!!
    template<>
    inline constexpr auto reflect_as_tie(Bar& d) noexcept { return std::tie(d.F1, d.F2, /*d.F3,*/ d.F4);};    
}}

using namespace std;
using namespace reflection::basic;

int main()
{
    //1. Data definition for Foo
    const Foo c_foo{"hello", 1};
    Foo foo;

    //refrection as references and const references
    auto foo_cref  = reflect_as_tie(c_foo);
    auto foo_ref = reflect_as_tie(foo);

    //1.1 check field size
    static_assert(reflect_size_v<Foo> == 2, "wrong reflection size");

    using ri_foo = reflect_info<Foo>;

    //1.2 check short aliases
    static_assert(is_same<ri_foo::ref_type, reflect_info_rt<Foo>>::value);
    static_assert(is_same<ri_foo::cref_type, reflect_info_ct<Foo>>::value);
    static_assert(is_same<ri_foo::value_type, reflect_info_vt<Foo>>::value);

    //1.3 check types from reflect_as_tie
    static_assert(is_same<ri_foo::ref_type, decltype(foo_ref)>::value);
    static_assert(is_same<ri_foo::cref_type, decltype(foo_cref)>::value);

    //TODO: make this as test suite

    //1.4 check const data    
    assert(get<0>(foo_cref) == "hello");
    assert(get<0>(foo_cref) == c_data.F1);
    
    assert(get<1>(foo_cref) == 1);
    assert(get<1>(foo_cref) == c_data.F2);

    //1.5 set value through reflection
    get<0>(foo_ref) = "world";
    get<1>(foo_ref) = 42;

    //check value in reflection and struct
    assert(get<0>(foo_ref) == "world");
    assert(get<0>(foo_ref) == data.F1);

    assert(get<1>(foo_ref) == 42);
    assert(get<1>(foo_ref) == data.F2);

    //2. AnotherData defenition
    const Bar c_adata{Digit::One, 1u, 2, 3.0};
    
    //2.1 check field size. Must be 3, because we don't use F3
    static_assert(reflect_size_v<Bar> == 3, "wrong reflection size");
    
    return 0

}