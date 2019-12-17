#include "AME/ServiceManager.hpp"

namespace   AME
{
    /********** Public **********/

    /* Ctor & dtor */

    ServiceManager::ServiceManager(void)
    {
    }

    ServiceManager::~ServiceManager(void)
    {
    }

    /* Methods */

    void    ServiceManager::activate(IService::Id const &id)
    {
        ::std::lock_guard<::std::shared_mutex>  lock(this->_locker);

        this->_services.emplace(id);
    }

    void    ServiceManager::desactivate(IService::Id const &id)
    {
        ::std::lock_guard<::std::shared_mutex>  lock(this->_locker);

        this->_services.erase(id);
    }

    bool    ServiceManager::isActivated(IService::Id const &id) const
    {
        ::std::shared_lock  lock(this->_locker);
        
        return (this->_services.find(id) != this->_services.end());
    }

    bool    ServiceManager::empty(void) const
    {
        ::std::shared_lock  lock(this->_locker);

        return this->_services.empty();
    }

    /* Static methods */

    IService::Id ServiceManager::getId(IWork const *work)
    {
        return reinterpret_cast<IService::Id>(work);
    }
}