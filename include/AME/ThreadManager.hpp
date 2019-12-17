/**
* \file				ThreadManager.hpp
* \author			Sébastien Le Maire
* \date				16/09/2019
*/

#ifndef _AME_THREADMANAGER_HPP_
# define _AME_THREADMANAGER_HPP_

# include <unordered_set>
# include <thread>
# include <queue>
# include <mutex>
# include <shared_mutex>

# include "AME/Tools/import.hpp"

namespace   AME
{
    class   Core;

    class AME_IMPORT    ThreadManager
    {
        public:
            /* Ctor & dtor */

            ThreadManager(void);
            ~ThreadManager(void);

            /* Methods */

            void    add(::std::thread::id const &id);
            void    add(Core *core, unsigned int const nbThread = 1);            
            void    remove(::std::thread::id const &id);
            void    remove(Core *core, unsigned int const nbThread = 1);
            void    removeAll(void);
            bool    exist(::std::thread::id const &id) const;
            void    clearGarbage(void);

            /* Accessors */

            size_t	getNbWorkingThread(void) const;
            size_t	getNbThreadPool(void) const;

        private:
            /* Attributes */
            ::std::unordered_set<::std::thread::id> _threads;       // Container of all working thread of the pool.
            ::std::queue<::std::thread>             _threadPool;    // Container of thread's instances.
            ::std::thread                           _garbage;       // Thread need to be clear if joinning failed.
            mutable ::std::shared_mutex             _locker;        // locker for concurrency using by threads.
            mutable ::std::shared_mutex             _lockerPool;    // locker for concurrency using by threadPool.
            mutable ::std::mutex                    _lockerGarbage; // locker for concurrency using by garbage.

            /* Static methods */
            static void joinThreads(ThreadManager *manager, ::std::queue<::std::thread> &&threads);
    };
}

#endif /* !_AME_THREADMANAGER_HPP_ */
