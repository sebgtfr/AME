/**
* \file				Timeout.hpp
* \author			Sébastien Le Maire
* \date				28/04/2020
*/

#ifndef _AME_TIMEOUT_HPP_
# define _AME_TIMEOUT_HPP_

# include <utility>
# include <type_traits>
# include <chrono>

# include "AME/Tools/import.hpp"

namespace   AME
{
    class AME_IMPORT    Timeout
    {
        public:
            /* Definitions */

            using Clock = ::std::chrono::high_resolution_clock;
            using Ms = ::std::chrono::milliseconds;

            /* Ctor & dtor */

            Timeout(Clock::time_point const &deadline = Clock::now());
            Timeout(Ms const &timeout);
            Timeout(Timeout const &timeout);
            Timeout(Timeout &&timeout);
            ~Timeout(void);

            /* Template Ctor */
            
            template<typename N, typename = ::std::enable_if_t<::std::is_integral_v<N>>>
            inline Timeout(N const timeout)
                : _deadline(Clock::now() + Ms(timeout))
            {
            }

            /* Methods */

            void        expiresAt(Clock::time_point const &deadline);
            void        expiresAfter(uint64_t const timeout);
            void        expiresAfter(Ms const &timeout);
            Timeout::Ms msLeftBeforeExpires(void) const;
            bool        hasExpired(void) const;

            /* Operator */

            Timeout     &operator=(Timeout const &timeout);
            bool        operator==(Timeout const &timeout) const;
            bool        operator!=(Timeout const &timeout) const;
            bool        operator<(Timeout const &timeout) const;
            bool        operator<=(Timeout const &timeout) const;
            bool        operator>(Timeout const &timeout) const;
            bool        operator>=(Timeout const &timeout) const;
            Timeout     &operator+=(Ms const &timeout);            
            Timeout     &operator-=(Ms const &timeout);
            Timeout     operator+(Ms const &timeout);            
            Timeout     operator-(Ms const &timeout);

            /* Template operators */

            template<typename N, typename = ::std::enable_if_t<::std::is_integral_v<N>>>
            inline Timeout  &operator+=(N const timeout)
            {
                this->_deadline += Ms(timeout);
                return *this;
            }

            template<typename N, typename = ::std::enable_if_t<::std::is_integral_v<N>>>
            inline Timeout  &operator-=(N const timeout)
            {
                this->_deadline -= Ms(timeout);
                return *this;
            }

            template<typename N, typename = ::std::enable_if_t<::std::is_integral_v<N>>>
            inline Timeout  operator+(N const timeout)
            {
                return Timeout(this->_deadline + Ms(timeout));
            }

            template<typename N, typename = ::std::enable_if_t<::std::is_integral_v<N>>>
            inline Timeout  operator-(N const timeout)
            {
                return Timeout(this->_deadline - Ms(timeout));
            }

            /* Static methods */

            static Timeout  now(void);

        private:
            Clock::time_point   _deadline;
    };

    /********** Public **********/

    /* Ctor & Dtor */
    inline Timeout::Timeout(Clock::time_point const &deadline)
        : _deadline(deadline)
    {
    }

    inline Timeout::Timeout(Ms const &timeout)
        : _deadline(Clock::now() + timeout)
    {
    }

    inline Timeout::Timeout(Timeout const &timeout)
        : _deadline(timeout._deadline)
    {
    }

    inline Timeout::Timeout(Timeout &&timeout)
        : _deadline(::std::move(timeout._deadline))
    {
    }

    inline Timeout::~Timeout(void)
    {
    }

    /* Methods */

    inline void    Timeout::expiresAt(Clock::time_point const &deadline)
    {
        this->_deadline = deadline;
    }

    inline void    Timeout::expiresAfter(uint64_t const timeout)
    {
        this->expiresAt(Clock::now() + Ms(timeout));
    }

    inline void    Timeout::expiresAfter(Ms const &timeout)
    {
        this->expiresAt(Clock::now() + timeout);
    }

    inline Timeout::Ms  Timeout::msLeftBeforeExpires(void) const
    {
        return ::std::chrono::duration_cast<Timeout::Ms>(this->_deadline - Clock::now());
    }

    inline bool   Timeout::hasExpired(void) const
    {
        return (this->_deadline <= Clock::now());
    }

    /* Operator */

    inline Timeout  &Timeout::operator=(Timeout const &timeout)
    {
        if (this != &timeout)
        {
            this->_deadline = timeout._deadline;
        }
        return *this;
    }

    inline bool     Timeout::operator==(Timeout const &timeout) const
    {
        return (this->_deadline == timeout._deadline);
    }

    inline bool   Timeout::operator!=(Timeout const &timeout) const
    {
        return (this->_deadline != timeout._deadline);
    }

    inline bool   Timeout::operator<(Timeout const &timeout) const
    {
        return (this->_deadline < timeout._deadline);
    }

    inline bool   Timeout::operator<=(Timeout const &timeout) const
    {
        return (this->_deadline <= timeout._deadline);
    }

    inline bool   Timeout::operator>(Timeout const &timeout) const
    {
        return (this->_deadline > timeout._deadline);
    }

    inline bool   Timeout::operator>=(Timeout const &timeout) const
    {
        return (this->_deadline >= timeout._deadline);
    }

    inline Timeout  &Timeout::operator+=(Ms const &timeout)
    {
        this->_deadline += timeout;
        return *this;
    }

    inline Timeout  &Timeout::operator-=(Ms const &timeout)
    {
        this->_deadline -= timeout;
        return *this;
    }

    inline Timeout  Timeout::operator+(Ms const &timeout)
    {
        return Timeout(this->_deadline + timeout);
    }

    inline Timeout  Timeout::operator-(Ms const &timeout)
    {
        return Timeout(this->_deadline - timeout);
    }

    /* Static methods */

    inline Timeout Timeout::now(void)
    {
        return Timeout(Clock::now());
    }
}

#endif /* !_AME_TIMEOUT_HPP_ */
