#include <reflection/tuple_utils.h>
/*
    Examples for tuple_utils 
*/

using namespace std;

int main()
{
    //some values
    int v1 = 1;
    double v2 = 2.0;
    uint64_t v3 = 3;

    //value type aliases
    using v1_t = decltype(v1);
    using v2_t = decltype(v2);
    using v3_t = decltype(v3);

    //base data and data type
    auto data = tie(v1, v2, v3);
    using data_type = decltype(data);

    //types for check
    using data_vt = tuple<v1_t, v2_t, v3_t>;
    using data_rt = tuple<v1_t&, v2_t&, v3_t&>;
    using data_crt = tuple<const v1_t&, const v2_t&, const v3_t&>;

    //types convert checks
    static_assert(is_same<data_type, data_rt>::value, "Wrong base type");
    static_assert(is_same<tuple_utils::remove_ref_t<data_type>, data_vt>::value, "Wrong value types");
    static_assert(is_same<tuple_utils::add_const_t<data_type>, data_crt>::value, "Wrong const ref types");
    static_assert(is_same<tuple_utils::remove_const_t<data_crt>, data_rt>::value, "Wrong const to ref types");
    
    return 0;
}
