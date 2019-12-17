/**
* \file				IService.hpp
* \author			Sébastien Le Maire
* \date				25/02/2019
*/

#ifndef _AME_ISERVICE_HPP_
# define _AME_ISERVICE_HPP_

# include <memory>

# include "AME/Tools/import.hpp"

namespace   AME
{
    class   Core;

    class AME_IMPORT	IService
    {
        public:
            /* Definitions */

            using   Pointer = ::std::shared_ptr<IService>;
            using   Id = ::std::size_t;

            /* Enum */
            enum class  State
            {
                STOP = 0,
                CONTINUE = 1
            };

            /* Dtor */
            virtual ~IService(void)
            {};

            /* Methods */
            virtual IService::State update(Core *core) = 0;
    };
}

#endif /* !_AME_ISERVICE_HPP_ */
