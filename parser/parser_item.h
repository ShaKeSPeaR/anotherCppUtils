#pragma once
#ifndef _APPU_PARSER_ITEM_
#define _APPU_PARSER_ITEM_

#include <utility>

namespace parser
{   
    //Айтем для конструирования парсера 
    template<class StrT, class PT, class DataT>
    struct ParserItem
    {
        //все типы на всякий случай 
        using StringType = StrT;
        using ValueType  = DataT;
        using StructType = PT;
        
        //имя поля и дефолтное значение
        const StringType     FieldName;
        const ValueType      DefaultVal;
        
        //Полный конструктор. 
        //member - указатель на поле структуры. Используется для определения типа значения и строгости записи по полям парсера
        //fname - имя поля 
        //defVal - значение по умолчанию
        template<class ST, class VT>
        constexpr ParserItem(ValueType StructType::* member, ST&& fname, VT&& defVal)
            :FieldName{std::forward<ST>(fname)}, DefaultVal{std::forward<VT>(defVal)}
        {
            static_assert(std::is_convertible<ST, StringType>::value, "Wrong string type for field name");
            static_assert(std::is_convertible<VT, ValueType>::value, "Wrong default value type");
        }
        
        //Упрощенный конструктор без дефолтного значения (ValueType должен иметь конструктор по умолчанию)
        //member - указатель на поле структуры. Используется для определения типа значения и строгости записи по полям парсера
        //fname - имя поля 
        template<class ST>
        constexpr ParserItem(ValueType StructType::*, ST&& fname)
            :FieldName{std::forward<ST>(fname)}
        {
            static_assert(std::is_convertible<ST, StringType>::value, "Wrong string type for field name");
        }
    };

    //-------------------------------------------------------------------------------------------------------------
    //Вспомогательные функции - фабрики для автодетекта типов. Не нужны при переходе на С++17
    template<class StrT, class PT, class DataT, class ST, class VT>
    inline constexpr auto make_pitem(DataT PT::* member, ST&& fname, VT&& def)
    {
        return ParserItem<StrT, PT, DataT>{member, std::forward<ST>(fname), std::forward<VT>(def)};
    }

    template<class StrT, class PT, class DataT, class ST>
    inline constexpr auto make_pitem(DataT PT::* member, ST&& fname)
    {
        return ParserItem<StrT, PT, DataT>{member, std::forward<ST>(fname)};
    }

    //-------------------------------------------------------------------------------------------------------------
    template<class StrT>
    struct Creator
    {
        using StringType = StrT;
        
        template<class PT, class DataT, class ST, class VT>
        inline constexpr static auto pitem(DataT PT::* member, ST&& fname, VT&& def)
        {
            return make_pitem<StringType>(member, std::forward<ST>(fname), std::forward<VT>(def));
            //return ParserItem<StrT, PT, DataT>{member, fname, def};
        }

        template<class PT, class DataT, class ST>
        inline constexpr static auto pitem(DataT PT::* member, ST&& fname)
        {
            return make_pitem<StringType>(member, std::forward<ST>(fname));
        }    
    };
    
}//namespace parser

#endif //_APPU_PARSER_ITEM_