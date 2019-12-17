/**
* \file				IWork.hpp
* \author			Sébastien Le Maire
* \date				25/02/2019
*/

#ifndef _AME_IWORK_HPP_
# define _AME_IWORK_HPP_

# include <memory>

# include "AME/Tools/import.hpp"

namespace   AME
{
    class AME_IMPORT	IWork
    {
        public:
            /* Definitions */

            using   Pointer = ::std::shared_ptr<IWork>;
            using   WPointer = ::std::weak_ptr<IWork>;

            /* Dtor */

            virtual ~IWork(void) {};

            /* Methods */

            virtual void    run(void) = 0;
            virtual uint8_t repeat(void) const = 0;
    };
}

#endif /* !_AME_IWORK_HPP_ */
