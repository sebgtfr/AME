#include <memory>

#include "AME/Core.hpp"
#include "AME/Services/Time.hpp"

namespace   AME
{
    /********** Public **********/

    /* Ctor & dtor */

    Core::Core(void)
    {
    }

    Core::~Core(void)
    {
    }

    /* Methods */

    void    Core::post(IWork::Pointer work)
    {
        this->_worker.push(work);
    }

    void    Core::insert(IWork::Pointer &&work)
    {
        this->_worker.push(::std::move(work));
    }

    IService::Id    Core::addService(IService::Pointer &service)
    {
        using ServiceWork = Repeatable::Work<Core *, IService::Pointer, IWork *>;

        auto workingService = [](Core *core, IService::Pointer service, IWork *work)
        {
            ServiceWork         *serviceWork = static_cast<ServiceWork *>(work);
            IService::Id const  id = ServiceManager::getId(work);
			uint8_t const       repeat = (core->_services.isActivated(id)) ? static_cast<uint8_t>(service->update(core)) : 0;

            if (serviceWork->repeat(repeat) == 0)
            {
                core->_services.desactivate(id);
            }
        };
        auto work(::std::make_shared<ServiceWork>(static_cast<uint8_t>(1), workingService, this, ::std::move(service), nullptr));
        work->rewriteParameters<2>(work.get());
        IService::Id id = reinterpret_cast<IService::Id>(work.get());
        this->_services.activate(id);
        this->insert(::std::move(work));
        return id;
    }

    IService::Id        Core::addService(IService::Pointer &&service)
    {
        return this->addService(service);
    }

    IService::Id        Core::setTimeout(IWork::Pointer const &work,
                                            unsigned int const timeout)
    {
        return this->addService<::AME::Service::Time>(work, timeout);
    }

    IService::Id        Core::setInterval(IWork::Pointer const &work,
                                            unsigned int const timeout)
    {
        return this->addService<::AME::Service::Time>(work, timeout, true);
    }

    void        Core::removeService(IService::Id const &id)
    {
        this->_services.desactivate(id);
    }

    void        Core::run(void)
    {
        ::std::thread::id const     currentThreadId = ::std::this_thread::get_id();
        Worker::Queue::Node         *work;

        this->_threads.add(currentThreadId);
        while (this->_threads.exist(currentThreadId) &&
            ((work = this->_worker.pop()) != nullptr || this->_services.empty() == false))
        {
            if (work != nullptr)
            {
                (*work)->run();
                if ((*work)->repeat() == 0)
                {
                    delete work;
                }
                else
                {
                    this->_worker.push(work);
                }
            }
            else
            {
                ::std::this_thread::yield();
            }
        }
        this->_threads.remove(currentThreadId);
    }

    void    Core::addThreads(unsigned int const nbThread)
    {
        this->_threads.add(this, nbThread);
    }

    size_t	Core::getNbWorkingThread(void) const
    {
        return this->_threads.getNbWorkingThread();
    }

    size_t	Core::getNbThreadPool(void) const
    {
        return this->_threads.getNbThreadPool();
    }   

    void    Core::stop(void)
    {
        this->_threads.removeAll();
    }

    void    Core::stop(::std::thread::id const &id)
    {
        this->_threads.remove(id);
    }

    void    Core::stop(unsigned int const nbThread)
    {
        this->_threads.remove(this, nbThread);
    }
}