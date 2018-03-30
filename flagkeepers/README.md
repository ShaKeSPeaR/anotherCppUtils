# Flag keepers

Set of classes for store flag values and it combination.
Represent interface with set,remove and test for flag type.

## Simple Keeper

Simple keeper is use for old - style flags, like const int values

    struct Flags
    {
        static const int A = 0x1;
        static const int B = 0x2;
        static const int C = 0x4;
    }

Specialize `SimpleFlagKeeper` with type of flags and use it

    SimpleFlagKeeper<int> options;
    
    options.set(Flags::A);    //set flag A
    options.remove(Flags::B); //remove flag B
    options.contain(Flags::C);  //test C flag is set
    
## Enum Keeper    
    
For more modern-style flags as

    enum OldStyleEnum
    {
        A = 0, B, C
    };
    
    enum class NewStypeEnum : uint8_t
    {
        A = 0, B, C
    };
    
Means the use of consecutive values of enums (like 0,1 etc). 
Max value must be set as `MaxVal` in template parameters.
Enums must use `usigned type` as underlying type.

EnumFlagKeeper required at least c++11 compiler. 

Specialize type and MaxVal and use it as SimpleKeeper:


    EnumFlagKeeper<OldStyleEnum, C> oldStyleKeeper;
    EnumFlagKeeper<NewStypeEnum, NewStypeEnum::C> newStyleKeeper;
    
    newStyleKeeper.set(NewStypeEnum::A);    //set flag A
    newStyleKeeper.remove(NewStypeEnum::B); //remove flag B
    newStyleKeeper.contain(NewStypeEnum::C);  //test C flag is set

    const EnumFlagKeeper<OldStyleEnum, C> constKeeper{A, B}; //const keeper with init by list