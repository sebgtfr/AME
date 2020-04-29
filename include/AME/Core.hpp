/**
* \file				Core.hpp
* \author			Sébastien Le Maire
* \date				28/04/2020
*/

#ifndef _AME_CORE_HPP_
# define _AME_CORE_HPP_

# include <atomic>

# include "AME/Tools/import.hpp"

# include "AME/Worker.hpp"
# include "AME/ThreadManager.hpp"
# include "AME/Timeout.hpp"
# include "AME/IntervalService.hpp"

namespace   AME
{
    class AME_IMPORT    Core
    {
        public:
            /* Constants */
            constexpr static bool   CONTINUE = true;
            constexpr static bool   STOP = false;

            /* Ctor & dtor */
            Core(void);
            ~Core(void);

            /* Methods */

            /* Threads control */

            void                    addThreads(unsigned int const nbThread = 1);
            size_t					getNbWorkingThread(void) const;
            size_t					getNbThreadPool(void) const;
            void                    stop(void);
            void                    stop(ThreadManager::Id const &id);
            void                    stop(unsigned int const nbThread);

            /* Inner services */

            void                    run(void);
            void                    clear(void);
            void                    setInterval(uint32_t const &time, Worker::Work &&work);

            /* Template methods */

            template<typename Callback, typename... Args>
            void                    post(Callback &&callback, Args&&... args)
            {
                ::std::scoped_lock<::std::mutex>    lock(this->_lock);

                this->_worker.push(::std::forward<Callback>(callback), ::std::forward<Args>(args)...);
            }

            template<typename Callback, typename... Args>
            inline void             setTimeout(::AME::Timeout const &timeout, Callback &&callback, Args&&... args)
            {
                this->post([](::AME::Timeout const &timeout, ::std::function<void()> const action) -> bool
                {
                    if (timeout.hasExpired())
                    {
                        action();
                        return STOP;
                    }
                    return CONTINUE;
                }, timeout, static_cast<::std::function<void()>>(::std::bind(::std::forward<Callback>(callback), ::std::forward<Args>(args)...)));
            }

            template<typename Callback, typename... Args>
            inline void             setInterval(uint32_t const &time, Callback &&callback, Args&&... args)
            {
                this->setInterval(time, Worker::Create(::std::forward<Callback>(callback),
                                                        ::std::forward<Args>(args)...));
            }

        private:
            /* Attributes */
            Worker                      _worker;        // Manager of work's queue.
            ThreadManager               _threadManager; // Manager of threads
            ::std::mutex                _lock;          // Lock for work's queue;
            ::std::atomic<uint32_t>     _threadBusy;    // Number of threads which has a work.

            /* Methods */

            /* Works control */

            Worker::Node                *pop(void);
    };

    /********** Inline Definitions **********/

    /* Public */

    /* Ctor & dtor */
    inline Core::Core(void)
        : _threadBusy(0)
    {
    }

    inline Core::~Core(void)
    {
    }

    /* Methods */

    /* Threads control */

    inline void     Core::addThreads(unsigned int const nbThread)
    {
        this->_threadManager.add(this, nbThread);
    }

    inline size_t   Core::getNbWorkingThread(void) const
    {
        return this->_threadManager.getNbWorkingThread();
    }

    inline size_t   Core::getNbThreadPool(void) const
    {
        return this->_threadManager.getNbThreadPool();
    }   

    inline void     Core::stop(void)
    {
        this->_threadManager.removeAll();
    }

    inline void     Core::stop(ThreadManager::Id const &id)
    {
        this->_threadManager.remove(id);
    }

    inline void     Core::stop(unsigned int const nbThread)
    {
        this->_threadManager.remove(this, nbThread);
    }

    /* Inner services */

    inline void     Core::setInterval(uint32_t const &time, Worker::Work &&work)
    {
        this->post(&IntervalService::update, time, work);
    }
}

#endif /* !_AME_CORE_HPP_ */
