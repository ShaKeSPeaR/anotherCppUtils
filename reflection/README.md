# Reflection

C++ in current state have no refrection at all.

Here is some tools for "manual" reflection from types to std::tuple.

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

See full code in *examples* folder.

In this part code use "std like" codestyle. 