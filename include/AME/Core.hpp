/**
* \file				Core.hpp
* \author			Sébastien Le Maire
* \date				25/02/2019
*/

#ifndef _AME_CORE_HPP_
# define _AME_CORE_HPP_

# include "AME/Tools/import.hpp"
# include "AME/Worker.hpp"
# include "AME/ThreadManager.hpp"
# include "AME/ServiceManager.hpp"

namespace   AME
{
    class AME_IMPORT    Core
    {
        public:
            /* Ctor & dtor */

            Core(void);
            ~Core(void);

            /* Methods */

            /* Works control */
            void                            post(IWork::Pointer work);
            void                            insert(IWork::Pointer &&work);

            /* Services control */
            IService::Id                    addService(IService::Pointer &service);
            IService::Id                    addService(IService::Pointer &&service);
            IService::Id                    setTimeout(IWork::Pointer const &work, unsigned int const timeout = 1000);
            IService::Id                    setInterval(IWork::Pointer const &work, unsigned int const timeout = 1000);
            void                            removeService(IService::Id const &id);
            
            /* threads control */
            void                            addThreads(unsigned int const nbThread = 1);
            size_t							getNbWorkingThread(void) const;
            size_t							getNbThreadPool(void) const;
            void                            stop(void);
            void                            stop(::std::thread::id const &id);
            void                            stop(unsigned int const nbThread);

            void                            run(void);

            /* Template methods */

            template<typename Callback, typename... Args>
            inline void                     post(Callback &&callback, Args&&... args)
            {
                this->_worker.push(::std::forward<Callback>(callback),
                                ::std::forward<Args>(args)...);
            }

            template<typename Callback, typename... Args>
            inline void                     postRepeat(uint8_t const repeat, Callback &&callback, Args&&... args)
            {
                this->_worker.pushRepeat(repeat, ::std::forward<Callback>(callback),
                                ::std::forward<Args>(args)...);
            }

            template<typename Callback, typename... Args>
            inline IService::Id             setTimeout(unsigned int const timeout, Callback &&callback, Args&&... args)
            {
                return this->setTimeout(::AME::Worker::Create(::std::forward<Callback>(callback),
                                        ::std::forward<Args>(args)...), timeout);
            }

            template<typename Callback, typename... Args>
            inline IService::Id             setInterval(unsigned int const timeout, Callback &&callback, Args&&... args)
            {
                return this->setInterval(::AME::Worker::Create(::std::forward<Callback>(callback),
                                        ::std::forward<Args>(args)...), timeout);
            }

            template<typename SERVICE, typename... Args>
            inline IService::Id             addService(Args&&... args)
            {
                return this->addService(::std::make_shared<SERVICE>(::std::forward<Args>(args)...));
            }

        private:
            /* Attributes */
            Worker                      _worker;    // Manager work's queue.
            ServiceManager              _services;  // Manager of actives services.
            ThreadManager               _threads;   // Manager of threads that work on Core.
    };
}

#endif /* !_AME_CORE_HPP_ */
