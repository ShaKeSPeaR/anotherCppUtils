/** ***************************************************************************
 * \file
 * \brief Шаблонный конвертор для значений
 * \author Спирин А.Д.
 *****************************************************************************/
#pragma once


#include <initializer_list>
#include <unordered_map>
#include <vector>
#include <tuple>

namespace Core
{

namespace _details_
{

    //набор структур для поиска индекса в tuple по типу
    template<typename T, typename C, std::size_t I>
    struct tuple_index_r;

    template<typename H, typename ...R, typename C, std::size_t I>
    struct tuple_index_r<std::tuple<H, R...>, C, I>
        : public std::conditional<std::is_same<C, H>::value,
              std::integral_constant<std::size_t, I>,
              tuple_index_r<std::tuple<R...>, C, I+1>>::type
    {};

    template<typename C, std::size_t I>
    struct tuple_index_r<std::tuple<>, C, I>
    {};

    template<typename T, typename C>
    struct tuple_index_of
    : public std::integral_constant<std::size_t, tuple_index_r<T, C, 0>::value> {};
} //namespace _details_

//------------------------------------------------------------------------------------------
// Базовый конвертор значений.
// Реализован в полностью константом виде, конструктора по умолчанию нет.
// Пердполагает заполнение в момент создания. Имет паблик методы только для получения данных
//------------------------------------------------------------------------------------------

template<class... VTypes>
class BaseValueConvertor
{
public:

    //внутренний тип для одного набора значений
    using ValuesSetType = std::tuple<VTypes...>;

    //BaseValueConvertor() = default;
    BaseValueConvertor(std::initializer_list<ValuesSetType> list)
    {
        for (const auto& data : list)  {
            pushImpl(data, std::index_sequence_for<VTypes...>());
        }
    }

    //Конвертирование по номеру откуда-куда
    template<std::size_t FromI, std::size_t ToI, class FromT>
    const auto& convertByPosition(const FromT& v) const
    {
        //получаем индексер для исходного типа
        const auto& index = std::get<FromI>(m_index);

        //получаем id в инексере и забираем данные из выходного массива
        //сразу возвращаем что - то из дефолтных значений
        return index.count(v) > 0 ? std::get<ToI>(m_values).at(index.at(v))
                                  : std::get<ToI>(m_defValues);
    }

    //Красивое конвертирвоание по типу вместо номеров.
    //Будет нормально работать только, если все используемые в конверторе типы разные
    template<class ToT, class FromT>
    const ToT& convertTo(const FromT& v) const
    {
        //получаем индексы через типы
        constexpr std::size_t from = _details_::tuple_index_of<ValuesSetType, FromT>::value;
        constexpr std::size_t to   = _details_::tuple_index_of<ValuesSetType, ToT>::value;

        //дергаем стандартую функцию
        return convertByPosition<from, to, FromT>(v);
    }

    //Получение набора значений для v с использвонием индекса типа (общий случай)
    template<std::size_t I, class ForT>
    ValuesSetType getValueSetByIndex(const ForT& v) const
    {
        //получаем индексер для исходного типа
        const auto& index = std::get<I>(m_index);

        //получаем id в инексере и собираем tuple
        //или просто возвращаем дефолтный пак
        return index.count(v) > 0 ? getValuesImpl(index.at(v), std::index_sequence_for<VTypes...>())
                                  : m_defValues;
    }

    //Получение набора значений для v по типу
    //(будет работать, только если все типы разные)
    template<class ForT>
    ValuesSetType getValueSet(const ForT& v) const
    {
        constexpr std::size_t typeIdx = _details_::tuple_index_of<decltype(m_defValues), ForT>::value;
        return getValueSetByIndex<typeIdx, ForT>(v);
    }

protected:

    //добавить элемент в конвертор
    //вспомогательная реализация для работы с каждым элементов tuple отдельно
    template <std::size_t ... Indices>
    void pushImpl(const ValuesSetType& pack, std::index_sequence<Indices...>)
    {
        //нужно распаковать индексы и для каждого вызвать push_val со значением.
        //Самый простой способ - сунуть в функцию с переменным кол-вом параметрв вызов push_val с каждым индексом
        //Можно использовать make_tuple или tie, для простоты вызываем initializer_list

        //Также обманываем компилятор доп оператором вызова (push_val(), 1), т.к. мы возвращаем void
        //Если там быдет что - то другое - можно их убрать

        //std::make_tuple((push_val<Indices>(pack), 1)...);
        (void)(std::initializer_list<int>{ (pushVal<Indices>(std::get<Indices>(pack)), 0)... });
    }

    //Значения по умолчанию
    ValuesSetType              m_defValues;
    //Хранилища для преобразвоания
    std::tuple<std::vector<VTypes>...> m_values;
    //хранилища индексов - по значению возвращают индекс
    std::tuple<std::unordered_map<VTypes, std::size_t>...> m_index;


private:

    //Реализация сборки пака значений в tuple
    template <std::size_t ... Indices>
    ValuesSetType getValuesImpl(size_t idx, std::index_sequence<Indices...>) const
    {
        return std::make_tuple(std::get<Indices>(m_values).at(idx)...);
    }

    //собственно добавление одного значения в хранилище и индексы
    template <std::size_t I, class VT>
    void pushVal(VT&& val)
    {
        auto& valStorage = std::get<I>(m_values);
        valStorage.push_back(val);

        auto& index = std::get<I>(m_index);
        index.insert({val, valStorage.size() - 1});
    }
};


//------------------------------------------------------------------------------------------
// Полный конвертор значений.
// Расширяет базовый конвертор, имеет конструктор по умолчанию
// Имееет как методы получения, так и методы добавления данных
// Позволяет настраивать дефолтные значения
//------------------------------------------------------------------------------------------

template<class... VTypes>
class ValueConvertor : public BaseValueConvertor<VTypes...>
{
public:
    using ValuesSetType = typename BaseValueConvertor<VTypes...>::ValuesSetType;

    constexpr ValueConvertor()
        :BaseValueConvertor<VTypes...>(std::initializer_list<ValuesSetType>{})
    {}
    constexpr ValueConvertor(std::initializer_list<ValuesSetType> list)
        :BaseValueConvertor<VTypes...>(list)
    {}

    //добавить значений через tuple
    void appendValues(ValuesSetType&& data)
    {
        pushImpl(data, std::index_sequence_for<VTypes...>());
    }

    //доабвить значений отдельными параметрами
    void appendValues(VTypes... data)
    {
        pushImpl(std::make_tuple(data...), std::index_sequence_for<VTypes...>());
    }

    //задать дефотные значения через tuple
    void setupDefaults(ValuesSetType&& defs)
    {
        this->m_defValues = std::forward<ValuesSetType>(defs);
    }

    //задачть дефолттные значения через параметры
    void setupDefaults(VTypes... defs)
    {
        this->m_defValues = std::make_tuple(defs...);
    }
};

} //namespace Core
