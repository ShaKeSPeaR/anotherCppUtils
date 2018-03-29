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
    
    //1.set two flag
    options.set(Flags::A);
    options.set(Flags::B);
    
    //TODO: move this to test suite
    
    //test flags
    assert(options.isSet(Flags::A));
    assert(options.isSet(Flags::B));
    assert(!options.isSet(Flags::C));
    
    //2. remove flag
    options.remove(Flags::B);

    //test flags
    assert(options.isSet(Flags::A));
    assert(!options.isSet(Flags::B));
    assert(!options.isSet(Flags::C));    
    
    //3.direct value 
    assert(options.isSet(1));
    assert(!options.isSet(1000));
    
    return 0;
}

