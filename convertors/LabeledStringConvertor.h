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
// Базовый конвертор для строк + caption на основе BaseValueConvertor
// Реализован в полностью константом виде, конструктора по умолчанию нет.
// Пердполагает заполнение в момент создания. Имет паблик методы только для получения данных
//------------------------------------------------------------------------------------------

template<class VT, class ST, class LT>
class BaseLabeledConvertor
{
public:
    using ConvertorType = BaseValueConvertor<VT, ST, LT>;
    using FullType = BaseLabeledConvertor<VT, ST, LT>;

    //конструктор по умолчанию недоступен
    BaseLabeledConvertor() = delete;

    constexpr BaseLabeledConvertor(std::initializer_list<typename ConvertorType::ValuesSetType> list)
        : m_convertor(list)
    {}

    //копирование = по умолчанию
    BaseLabeledConvertor(const FullType&) = default;
    FullType& operator=(const FullType&) = default;

    //перемещение - запрещено
    BaseLabeledConvertor(FullType&&) = delete;
    BaseLabeledConvertor(const FullType&&) = delete;

    FullType& operator=(FullType&&) = delete;
    FullType& operator=(const FullType&&) = delete;

    const ST& toString(const VT& v) const   { return m_convertor.template convertByPosition<0, 1>(v); }
    const VT& fromString(const ST& s) const { return m_convertor.template convertByPosition<1, 0>(s); }
    const LT& toCaption(const VT& v) const  { return m_convertor.template convertByPosition<0, 2>(v); }

protected:

    const ConvertorType m_convertor;
};

//------------------------------------------------------------------------------------------
// Полный конвертор строк на основе ValueConvertor
// Имееет как методы получения, так и методы добавления данных
// Позволяет настраивать дефолтные значения
//------------------------------------------------------------------------------------------

template<class VT, class ST, class LT>
class LabeledConvertor
{
public:
    using ConvertorType = ValueConvertor<VT, ST>;
    using ValuesSetType = typename ConvertorType::ValuesSetType;

    constexpr LabeledConvertor() = default;
    constexpr LabeledConvertor(std::initializer_list<ValuesSetType> list)
        : m_convertor(list)
    {}

    const ST& toString(const VT& v)   const { return m_convertor.template convertByPosition<0, 1>(v); }
    const VT& fromString(const ST& s) const { return m_convertor.template convertByPosition<1, 0>(s); }
    const LT& toCaption(const VT& v)  const { return m_convertor.template convertByPosition<0, 2>(v); }

    //задать пак значений
    void assingn(std::initializer_list<ValuesSetType> list) { m_convertor.assign(list); }

    //добавить значений через tuple
    void appendValues(ValuesSetType&& data) { m_convertor.appendValues(std::forward<ValuesSetType>(data));  }

    //добавить значений отдельными параметрами
    template<class VTT, class STT, class LTT>
    void appendValues(VTT&& value, STT&& str, LTT&& caption)
    {
        m_convertor.appendValues(std::forward<VTT>(value), std::forward<STT>(str), std::forward<LTT>(caption));
    }

    //задать дефотные значения через tuple
    void setupDefaults(ValuesSetType&& defs)
    {
        m_convertor.setupDefaults(std::forward<ValuesSetType>(defs));
    }

    //задать дефолттные значения через параметры
    template<class VTT, class STT, class LTT>
    void setupDefaults(VTT&& value, STT&& str, LTT&& caption)
    {
        m_convertor.setupDefaults(std::forward<VTT>(value), std::forward<STT>(str),
                                  std::forward<STT>(caption));
    }

private:

    ConvertorType m_convertor;
};



} //namespace Core
