#pragma once
#ifndef _APPU_SIMPLE_FLAG_KEEPER_
#define _APPU_SIMPLE_FLAG_KEEPER_

namespace flags
{   
    namespace _detail
    {
        //Function - constructor for make internal value for keeper from initializer_list
        //constexpr in c++14
        template<class T>
        #if __cplusplus >= 201103L && __cplusplus < 201402L
        T from_list(std::initializer_list<T> init)
        #else
        constexpr T from_list(std::initializer_list<T> init)
        #endif
        {
            T d{0};        
            for (auto v : init)  {
                d |= v;
            }
            
            return d;
        }
    } 


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
     * Rquires at least c++11, more functional in c++14
    */    
    template<class T>
    class SimpleFlagKeeper
    {
    public:

        typedef T FlagType; ///< internal flag for using

        constexpr SimpleFlagKeeper(): m_value(0) {}
        constexpr SimpleFlagKeeper(FlagType init): m_value(init) {}
        
        #if __cplusplus >= 201103L && __cplusplus < 201402L
        SimpleFlagKeeper(std::initializer_list<T> init)
        #else
        constexpr SimpleFlagKeeper(std::initializer_list<T> init)
        #endif            
        : m_value(_detail::from_list(init)) 
        {}

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
        constexpr inline bool contain(FlagType val) const { return m_value & val; }
            
    private:
        FlagType m_value; ///< flags storage 
    };

}//namespace flags

#endif //_APPU_SIMPLE_FLAG_KEEPER_