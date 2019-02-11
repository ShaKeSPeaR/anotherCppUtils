#pragma once
#ifndef _APPU_MIDDLEWARE_PARSER_
#define _APPU_MIDDLEWARE_PARSER_

#include <bitset>
#include <array>
#include <reflection/basic_reflection.h>
#include <parser/parser_item.h>

namespace parser
{   
    //------------------------------------------------------------------------------------
    //получить значение поля с именем name и дефолтным значением def
    //T - тип самого значения
    //SourceT - некий тип, откуда доставать значения по имени (QJsonObject, map и пр.)
    //StrT - тип строки для имени поля
    template<class T, class StrT, class SourceT>
    T getField(const SourceT& source, const StrT& name, const T& def);

    //получить значение поля с именем name. Тип поля должен иметь конструктор по умолчанию
    template<class T, class StrT, class SourceT>
    T getField(const SourceT& source, const StrT& name)
    {
        return getField<T, StrT, SourceT>(source, name, T{});
    }

    template<class T, class StrT, class SourceT>
    void setField(SourceT& source, const StrT& name, const T& val);

    //------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------


    template<class T, class StrT, size_t N = reflection::basic::reflect_size_v<T>>
    struct Parser
    {
        using StringType = StrT;

        //внутренинй тип айтема для конструктора
        template<class DT>
        using ItemType = ParserItem<StrT, T, DT>;

        //используемые типы tuple и tuple со ссылками
        using TupleType     = reflection::basic::reflect_info_vt<T>;
        using TupleTypeRef  = reflection::basic::reflect_info_rt<T>;
        using TupleTypeCRef = reflection::basic::reflect_info_ct<T>;
        
        //имена полей для парсера
        const std::array<StrT, N> m_fields;
        //дефолтные значения полей
        const TupleType m_defValues;


        template<class... Args>
        constexpr Parser(ItemType<Args>&&... args)    
            :m_defValues{std::make_tuple(args.DefaultVal...)}, m_fields{args.FieldName...}
        {
            static_assert(sizeof...(Args) == N, "Size mismatch");
        }

        template<class SourseT>
        T decode(const SourseT& source) const
        {
            //создаем выходную структуру и получаем tuple со сылками на все поля
            T result;
            auto resultTuple = reflection::basic::reflect_as_tie<T>(result);

            //запускаем разбор
            decode_impl(source, resultTuple, std::make_index_sequence<N>());
            return result;
        }

        template<class SourseT>
        SourseT encode(const T& data) const
        {
            SourseT result;
            auto dataTuple = reflection::basic::reflect_as_tie<T>(data);

            encode_impl(result, dataTuple, std::make_index_sequence<N>());
            return result;
        }

    private:

        template <class SourseT, std::size_t ... Indices>
        bool encode_impl(SourseT& source, TupleTypeCRef data, std::index_sequence<Indices...>) const
        {
            using std::get;
            (void)(std::initializer_list<int>{ (setField(source, get<Indices>(m_fields), get<Indices>(data)), 0)...});        
            return true;
        }

        template <class SourseT, std::size_t ... Indices>
        bool decode_impl(const SourseT& source, TupleTypeRef result, std::index_sequence<Indices...>) const
        {
            //раскрытие шаблонов с перевызовом персера каждого значения
            //(void)(std::initializer_list<int>{ (parse_val<SourseT, Indices>(source, result), 0)... });

            //готовм битсет для сборки результатов от каждого элемента
            std::bitset<sizeof...(Indices)> retVal;
            std::size_t ui { retVal.size() };        

            //собираем результаты parse_val в битсет
            (void)(std::initializer_list<int>{ (retVal.set(--ui, decode_val<SourseT, Indices>(source, result) ), 0)...});
            
            //true - только если всем удалось
            return retVal.all();
        }

        template <class SourseT, std::size_t I>
        bool decode_val(const SourseT& source, TupleTypeRef result) const
        {   
            using std::get;
            //получаем ссылку на значение одно элемента в tuple
            auto& val = get<I>(result);

            //вызываем собственно функцию парсинга
            val = getField<std::remove_reference_t<decltype(val)>, StringType, SourseT>(source, std::get<I>(m_fields), get<I>(m_defValues));
            return true;
        }    

    };
    
}//namespace parser

#endif //_APPU_MIDDLEWARE_PARSER_