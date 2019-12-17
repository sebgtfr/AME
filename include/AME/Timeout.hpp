/**
* \file				IWork.hpp
* \author			Sébastien Le Maire
* \date				25/02/2019
*/

#ifndef _AME_TIMEOUT_HPP_
# define _AME_TIMEOUT_HPP_

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

            /* Ctor & dtor on canonic form */

            Timeout(Clock::time_point const &deadline = Clock::now());
            Timeout(uint64_t const timeout);
            Timeout(Ms const &timeout);
            Timeout(Timeout const &timeout);
            Timeout &operator=(Timeout const &timeout);
            ~Timeout(void);

            /* Methods */

            void        expiresAt(Clock::time_point const &deadline);
            void        expiresAfter(uint64_t const timeout);
            void        expiresAfter(Ms const &timeout);
            Timeout::Ms msLeftBeforeExpires(void) const;
            bool        hasExpired(void) const;

            /* Operator */

            bool        operator==(Timeout const &timeout) const;
            bool        operator!=(Timeout const &timeout) const;
            bool        operator<(Timeout const &timeout) const;
            bool        operator<=(Timeout const &timeout) const;
            bool        operator>(Timeout const &timeout) const;
            bool        operator>=(Timeout const &timeout) const;

            /* Static methods */

            static Timeout  now(void);

        private:
            Clock::time_point   _deadline;
    };
}

#endif /* !_AME_TIMEOUT_HPP_ */
