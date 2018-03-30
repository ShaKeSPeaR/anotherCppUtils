#include <cassert>
#include <flagkeepers/simple_keeper.h>
/*
    Examples for simple flag keeper
*/

//simple flag on int type
struct Flags
{
    static const int A = 0x1;
    static const int B = 0x2;
    static const int C = 0x4;
};

int main()
{
    
    flags::SimpleFlagKeeper<int> options;
    
    //construct const keeper from int list
    //may be constexpr in c++14
    const flags::SimpleFlagKeeper<int> options_const{Flags::A, Flags::C};
    
    //1.set two flag
    options.set(Flags::A);
    options.set(Flags::B);
    
    //TODO: move this to test suite
    
    //test flags
    assert(options.contain(Flags::A));
    assert(options.contain(Flags::B));
    assert(!options.contain(Flags::C));
    
    //2. remove flag
    options.remove(Flags::B);

    //test flags
    assert(options.contain(Flags::A));
    assert(!options.contain(Flags::B));
    assert(!options.contain(Flags::C));    
    
    //3.direct value 
    assert(options.contain(1));
    assert(!options.contain(1000));
    
    return 0;
}

