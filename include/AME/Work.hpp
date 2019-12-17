/**
* \file				Work.hpp
* \author			Sébastien Le Maire
* \date				25/02/2019
*/

#ifndef _AME_WORK_HPP_
# define _AME_WORK_HPP_

# include <functional>
# include <utility>
# include <tuple>

# include "AME/Interfaces/IWork.hpp"
# include "AME/Tools/import.hpp"

namespace   AME
{
    template<typename ...Ts>
    class AME_IMPORT    Work : public IWork
    {
    public:
        /* Ctor & dtor */

        Work(::std::function<void()> const &callback)
            : _callback(callback)
        {}

        template<typename Callback, typename ...Args>
        Work(Callback &&callback, Args&&... args)
            :   _callback(::std::forward<Callback>(callback)),
                _args(::std::forward<Args>(args)...)
        {}

        virtual ~Work(void) override { };

        /* Methods */

        void    run(void) override
        {
            this->callback(this->_args);
        }

        uint8_t repeat(void) const override
        {
            return 0;
        }

        template<unsigned int index, typename T>
        void    rewriteParameters(T const &parameter)
        {
            ::std::get<index>(this->_args) = parameter;
        }

    private:
        /* Attributes */
        ::std::function<void(Ts...)> const  _callback;
        ::std::tuple<Ts...>                 _args;

        /* Methods */

        template <typename... Args, ::std::size_t... Is>
        void    callback(::std::tuple<Args...> const &args, ::std::index_sequence<Is...>) const
        {
            this->_callback(::std::get<Is>(args)...);
        }

        template <typename... Args>
        void    callback(::std::tuple<Args...> const &args) const
        {
            this->callback(args, ::std::index_sequence_for<Args...>{});
        }
    };

    namespace Repeatable
    {
        template<typename ...Ts>
        class   Work final : public ::AME::Work<Ts...>
        {
            public:
                /* Ctor & dtor */
                Work(uint8_t const repeat, ::std::function<void()> const &callback)
                    :   ::AME::Work<Ts...>(callback), _repeat(repeat)
                {}

                template<typename Callback, typename ...Args>
                Work(uint8_t const repeat, Callback &&callback, Args&&... args)
                    :   ::AME::Work<Ts...>(::std::forward<Callback>(callback), ::std::forward<Args>(args)...),
                        _repeat(repeat)
                {}

                ~Work(void) final
                {};

                /* Methods */
                void    run(void) final
                {
                    --this->_repeat;
                    ::AME::Work<Ts...>::run();
                }

                uint8_t repeat(void) const final
                {
                    return this->_repeat;
                }

                uint8_t repeat(uint8_t const repeat)
                {
                    return this->_repeat = repeat;
                }
            
            private:
                uint8_t _repeat;
        };
    }    
}

#endif /* !_AME_WORK_HPP_ */
