/**
* \file				ServiceManager.hpp
* \author			Sébastien Le Maire
* \date				16/09/2019
*/

#ifndef _AME_SERVICEMANAGER_HPP_
# define _AME_SERVICEMANAGER_HPP_

# include <unordered_set>
# include <shared_mutex>

# include "AME/Interfaces/IWork.hpp"
# include "AME/Interfaces/IService.hpp"
# include "AME/Tools/import.hpp"

namespace   AME
{
    class AME_IMPORT    ServiceManager
    {
        public:
            /* Ctor & dtor */

            ServiceManager(void);
            ~ServiceManager(void);

            /* Methods */
            void    activate(IService::Id const &id);
            void    desactivate(IService::Id const &id);
            bool    isActivated(IService::Id const &id) const;
            bool    empty(void) const;

            /* Static methods */
            static IService::Id getId(IWork const *work);

        private:
            /* Attributes */
            ::std::unordered_set<IService::Id>  _services; // List of services actives
            mutable ::std::shared_mutex         _locker; // locker for concurrency.
    };
}

#endif /* !_AME_SERVICEMANAGER_HPP_ */
