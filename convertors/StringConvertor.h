/** ***************************************************************************
 * \file
 * \brief Шаблонный конвертор Для чего - то и строк
 * \author Спирин А.Д.
 *****************************************************************************/
#pragma once

#include "ValueConvertor.h"

namespace Core
{

//------------------------------------------------------------------------------------------
// Базовый конвертор для строк на основе BaseValueConvertor
// Реализован в полностью константом виде, конструктора по умолчанию нет.
// Пердполагает заполнение в момент создания. Имет паблик методы только для получения данных
//------------------------------------------------------------------------------------------

template<class VT, class ST>
class BaseStringConvertor
{
public:
    using ConvertorType = BaseValueConvertor<VT, ST>;
    using FullType = BaseStringConvertor<VT, ST>;

    //конструктор по умолчанию недоступен
    BaseStringConvertor() = delete;

    constexpr BaseStringConvertor(std::initializer_list<typename ConvertorType::ValuesSetType> list)
        : m_convertor(list)
    {}

    //копирование = по умолчанию
    BaseStringConvertor(const FullType&) = default;
    FullType& operator=(const FullType&) = default;

    //перемещение - запрещено
    BaseStringConvertor(FullType&&) = delete;
    BaseStringConvertor(const FullType&&) = delete;

    FullType& operator=(FullType&&) = delete;
    FullType& operator=(const FullType&&) = delete;


    const ST& toString(const VT& v) const   { return m_convertor.template convertTo<ST>(v); }
    const VT& fromString(const ST& s) const { return m_convertor.template convertTo<VT>(s); }

protected:

    const ConvertorType m_convertor;
};

//------------------------------------------------------------------------------------------
// Полный конвертор строк на основе ValueConvertor
// Имееет как методы получения, так и методы добавления данных
// Позволяет настраивать дефолтные значения
//------------------------------------------------------------------------------------------

template<class VT, class ST>
class StringConvertor
{
public:
    using ConvertorType = ValueConvertor<VT, ST>;
    using ValuesSetType = typename ConvertorType::ValuesSetType;

    constexpr StringConvertor() = default;
    constexpr StringConvertor(std::initializer_list<ValuesSetType> list)
        : m_convertor(list)
    {}

    const ST& toString(const VT& v) const   { return m_convertor.template convertTo<ST>(v); }
    const VT& fromString(const ST& s) const { return m_convertor.template convertTo<VT>(s); }

    //задать пак значений
    void assingn(std::initializer_list<ValuesSetType> list)
    {
        m_convertor.assign(list);
    }

    //добавить значений через tuple
    void appendValues(ValuesSetType&& data)
    {
        m_convertor.appendValues(std::forward<ValuesSetType>(data));
    }

    //добавить значений отдельными параметрами
    template<class VTT, class STT>
    void appendValues(VTT&& value, STT&& str)
    {
        m_convertor.appendValues(std::forward<VTT>(value), std::forward<STT>(str));
    }

    //задать дефотные значения через tuple
    void setupDefaults(ValuesSetType&& defs)
    {
        m_convertor.setupDefaults(std::forward<ValuesSetType>(defs));
    }

    //задать дефолттные значения через параметры
    template<class VTT, class STT>
    void setupDefaults(VTT&& value, STT&& str)
    {
        m_convertor.setupDefaults(std::forward<VTT>(value), std::forward<STT>(str));
    }

private:

    ConvertorType m_convertor;
};



} //namespace Core
