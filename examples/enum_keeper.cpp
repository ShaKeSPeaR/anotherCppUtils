#include <cassert>
#include <flagkeepers/enum_keeper.h>
/*
    Examples for enum flag keeper
*/

//old-style enum
enum OldStypeEnum
{
    FlagA = 0, FlagB, FlagC
};

//new-style enum with correct underlying type
enum class NewStyleEnum : uint8_t
{
    A = 0, B, C
};

//new-style enum with incorrect underlying type
enum class BadEnumType : int
{
    A = -1, B, C
};

int main()
{
    //TODO: move this to test suite
    
    //1. Old style enum
    flags::EnumFlagKeeper<OldStypeEnum, FlagC> oldEnumKeeper;

    //1.1 set two flag
    oldEnumKeeper.set(FlagA);
    oldEnumKeeper.set(FlagB);
       
    //test flags
    assert(oldEnumKeeper.contain(FlagA));
    assert(oldEnumKeeper.contain(FlagB));
    assert(!oldEnumKeeper.contain(FlagC));
    
    //1.2. remove flag
    oldEnumKeeper.remove(FlagB);

    //test flags
    assert(oldEnumKeeper.contain(FlagA));
    assert(!oldEnumKeeper.contain(FlagB));
    assert(!oldEnumKeeper.contain(FlagC));
    
    //-----------------------------------------------------------------
    //2. New style enum 
    flags::EnumFlagKeeper<NewStyleEnum, NewStyleEnum::C> newEnumKeeper;
    
    //construct const keeper from enum by list
    //may be constexpr in c++14
    const flags::EnumFlagKeeper<NewStyleEnum, NewStyleEnum::C> newEnumKeeper_v2{NewStyleEnum::A, NewStyleEnum::B}; 
    
    //2.1 set two flag
    newEnumKeeper.set(NewStyleEnum::A);
    newEnumKeeper.set(NewStyleEnum::B);
    
       
    //test flags
    assert(newEnumKeeper.contain(NewStyleEnum::A));
    assert(newEnumKeeper.contain(NewStyleEnum::B));
    assert(!newEnumKeeper.contain(NewStyleEnum::C));
    
    //2.2. remove flag
    newEnumKeeper.remove(NewStyleEnum::B);

    //test flags
    assert(newEnumKeeper.contain(NewStyleEnum::A));
    assert(!newEnumKeeper.contain(NewStyleEnum::B));
    assert(!newEnumKeeper.contain(NewStyleEnum::C));    
    
    //-----------------------------------------------------------------
    //2. New style enum with incorrect type
    // won't compile
    //flags::EnumFlagKeeper<BadEnumType, BadEnumType::C> badEnumKeeper;
    

    return 0;
}

