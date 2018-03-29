#pragma once
#ifndef _APPU_ENUM_FLAG_KEEPER_
#define _APPU_ENUM_FLAG_KEEPER_

#include <limits>
#include <bitset>


namespace flags
{
    
    /*
     * Flag keeper for enum flags
     * Store bitset for flag combination
     *
     * Flag examples:
     *   enum OldStyleEnum
     *   {
     *       A = 0, B, C
     *   };
     *
     *   enum class NewStypeEnum : uint8_t
     *   {
     *       A = 0, B, C
     *   };
     *
     * Means the use of consecutive values of enums (like 0,1 etc)
     * Max value must be set as MaxVal. 
     * Enums must use usigned type as underlying type
     *
     * Required at least c++11 compiler
    */    
     template<class T, T MaxVal,
             size_t N = static_cast<size_t>(MaxVal) + 1>
    class EnumFlagKeeper
    {
    public:

        //Type of flag in keeper
        typedef T FlagType;

        //static checks
        //T must be enum + underlying - unsigned type 

        static_assert(std::is_enum<T>::value,
                      "OptionsKeeper required enum");
        static_assert(!std::numeric_limits<typename std::underlying_type<T>::type>::is_signed,
                      "Underlying Enum type must be unsigned!");

        constexpr EnumFlagKeeper(): m_value(0) {}

        /**
         * \brief Set (enable) flag by value
         * \param[in] val value (position) for enabling
         */                  
        inline void set(FlagType val)    { m_value.set(static_cast<size_t>(val)); }

        /**
         * \brief Remove (disable) flag by value
         * \param[in] val value (position) for remove
         */
        inline void remove(FlagType val) { m_value.reset(static_cast<size_t>(val)); }

        /**
         * \brief Check value is set 
         * \param[in] val value (position) for test
         * \return true if value is set in options
         */
        inline bool isSet(FlagType val) const { return m_value.test(static_cast<size_t>(val)); }

    private:
        std::bitset<N> m_value;
    };

}//namespace flags

#endif //_APPU_ENUM_FLAG_KEEPER_