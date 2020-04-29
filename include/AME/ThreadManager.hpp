/**
* \file				ThreadManager.hpp
* \author			Sébastien Le Maire
* \date				28/04/2020
*/

#ifndef _AME_THREADMANAGER_HPP_
# define _AME_THREADMANAGER_HPP_

# include <list>
# include <unordered_set>

# include <thread>
# include <mutex>
# include <shared_mutex>

# include "AME/Tools/import.hpp"

namespace   AME
{
    class   Core;

    class AME_IMPORT                ThreadManager
    {
        public:
            /* Aliases */
            using Id = ::std::thread::id;

            /* Ctor & Dtor */
            ThreadManager(void);
            ~ThreadManager(void);

            /* Methods */

            void    add(ThreadManager::Id const &id);
            void    add(Core *core, unsigned int const nbThread = 1);            
            void    remove(ThreadManager::Id const &id);
            void    remove(Core *core, unsigned int const nbThread = 1);
            void    removeAll(void);
            bool    exist(ThreadManager::Id const &id) const;
            void    clearGarbage(void);

            /* Accessors */

            size_t	getNbWorkingThread(void) const;
            size_t	getNbThreadPool(void) const;

        private:
            /* Attributes */
            ::std::unordered_set<Id>        _threads;       // Container of all working thread of the pool. (inner or outer)
            ::std::list<::std::thread>      _innerQueue;    // Inner Container of thread's instances.
            ::std::thread                   _garbage;       // Thread need to be clear if joinning failed.
            mutable ::std::shared_mutex     _locker;        // locker for concurrency using by threads.
            mutable ::std::shared_mutex     _lockerQueue;   // locker for concurrency using by innerQueue.
            mutable ::std::mutex            _lockerGarbage; // locker for concurrency using by garbage.

            /* Methods */
            void        remove(void);
            void        joinThread(::std::thread &&thread);
            void        joinThreads(::std::list<::std::thread> &&threads);
    };

    /********** Inline definitions **********/

    /* Public */

    /* Ctor & Dtor */

    inline ThreadManager::ThreadManager(void) 
    {
    }

}

#endif /* !_AME_THREADMANAGER_HPP_ */