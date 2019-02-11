#pragma once
#ifndef _APPU_TUPLE_UTILS_
#define _APPU_TUPLE_UTILS_

#include <tuple>

/*
    Helpers for types tranformation in std::tuple
*/

namespace tuple_utils
{
    //append const to tuple of referefces
    template<typename>
    struct add_const;
    template<class... Types>
    struct add_const<std::tuple<Types&...>> 
    {
        using type = std::tuple<const Types&...>; //new const ref types
    };
    
    //alias for add_const type
    template<class T>
    using add_const_t = typename add_const<T>::type;

    //-------------------------------------------------------------------------
    //remove const from tuple of referefces
    template<typename>
    struct remove_const;
    template<class... Types>
    struct remove_const<std::tuple<const Types&...>>
    {
        using type = std::tuple<Types&...>; //new references type
    };

    //alias for remove_const type
    template<class T>
    using remove_const_t = typename remove_const<T>::type;

    //-------------------------------------------------------------------------
    //remove references from tuple types
    template<typename>
    struct remove_ref;
    template<class... Types>
    struct remove_ref<std::tuple<Types&...>>
    {
        using type = std::tuple<Types...>; //new tuple of values type
    };

    //alias for remove_ref type
    template<class T>
    using remove_ref_t = typename remove_ref<T>::type;


}//namespace tuple_utils

#endif //_APPU_TUPLE_UTILS_