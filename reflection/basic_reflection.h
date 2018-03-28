#pragma once
#ifndef _APPU_BASIC_REFLECTION_
#define _APPU_BASIC_REFLECTION_

#include <reflection/tuple_utils.h>

/*
    Types for "manual" reflection type as std::tuple
    Basic idea: for every type you must specialize function 
        template<class T>
        inline constexpr auto reflect_as_tie(T&) noexcept;       
    for represent type as std::tuple<Types&...>

    For example *Data* struct (namespaces omitted):

        struct Data
        {
            string V1;
            int V2;
        };
        template<>
        inline constexpr auto reflect_as_tie(Data& d) noexcept
        { return tie(d.V1, d.V2);};
        
    After that you can use buildin types as 

        reflect_size_v<Data> - number of fields
        reflect_info_vt<Data> - Data as std::tuple<std::string, int>
        
    and standart fuctions, like *std::get* for accessing to elements.
    
    Resuires at least c++14 compiler.
*/

namespace reflection { namespace basic
{
    //Enter to reflection. Specialize it for you type.
    //Expected return type - result of std::tie(...) for every field
    template<class T>
    inline constexpr auto reflect_as_tie(T&) noexcept;

    //Const variant for reflect_as_tie. 
    //Reuse reflect_as_tie(T&) with const_cast. In this case is safe, because tuple with const types is returned
    //Can be specialized manualy if necessary
    template<class T>
    inline constexpr auto reflect_as_tie(const T& v) noexcept
    {
        //get non const result
        auto ref_v = reflect_as_tie(const_cast<T&>(v));
        //create const version of data references
        tuple_utils::add_const_t<decltype(ref_v)> cref_v = ref_v;
        return cref_v;
    }

    //Main reflection info structure
    template<class T>
    struct reflect_info
    {
        using ref_type = decltype(reflect_as_tie(std::declval<T&>())); //reference type (tuple<Types&...>)
        using cref_type = tuple_utils::add_const_t<ref_type>;          //const reference type (tuple<const Types&...>)
        using value_type = tuple_utils::remove_ref_t<ref_type>;        //value type (tuple<Types...>)
    };

    //short aliases for reflection_info types
    template<class T>
    using reflect_info_rt = typename reflect_info<T>::ref_type; //const reference type (tuple<const Types&...>)
    template<class T>
    using reflect_info_ct = typename reflect_info<T>::cref_type; //reference type (tuple<Types&...>)
    template<class T>
    using reflect_info_vt = typename reflect_info<T>::value_type; //value type (tuple<Types...>)

    //Size of fields in reflection. std::tuple_size analogue.
    template<class>
    struct reflect_size;
    template<class T>
    struct reflect_size<reflect_info<T>>
    : public std::integral_constant<size_t, std::tuple_size<typename reflect_info<T>::value_type>::value>
    {};

    //short alias for reflect_size::value
    template<class T>
    constexpr size_t reflect_size_v = reflect_size<reflect_info<T>>::value;

}};//namespace basic | reflection

#endif //_APPU_BASIC_REFLECTION_