#pragma once
#ifndef _APPU_SIMPLE_FLAG_KEEPER_
#define _APPU_SIMPLE_FLAG_KEEPER_

namespace flags
{   
    /*
     * Simple flag keepers for old - style flags (like 'static int' values)
     * Store one value of type that represent combine of flags
     * Use bit operation for set, chack and remove options
     *
     * Flag examples:
     *  - use_cache     = 0x1
     *  - clear_cache   = 0x2
     *  - keep_in_cache = 0x4
     *
     *  
     * User is responsible for the correctness of flags values
    */    
    template<class T>
    class SimpleFlagKeeper
    {
    public:

        typedef T FlagType; ///< internal flag for using

        constexpr SimpleFlagKeeper(): m_value(0) {}
        constexpr SimpleFlagKeeper(FlagType init): m_value(init) {}


        /**
         * \brief Set (enable) flag by value
         * \param[in] val value (position) for enabling
         */
        inline void set(FlagType val)    { m_value |= val; }

        /**
         * \brief Remove (disable) flag by value
         * \param[in] val value (position) for remove
         */
        inline void remove(FlagType val) { m_value &= ~val; }

        /**
         * \brief Check value is set 
         * \param[in] val value (position) for test
         * \return true if value is set in options
         */
        constexpr inline bool isSet(FlagType val) const { return m_value & val; }
            
    private:
        FlagType m_value; ///< flags storage 
    };

}//namespace flags

#endif //_APPU_SIMPLE_FLAG_KEEPER_