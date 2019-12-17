#include <memory>

#include "AME/ThreadManager.hpp"
#include "AME/Core.hpp"

namespace   AME
{
    /********** Public **********/

    /* Ctor & dtor */

    ThreadManager::ThreadManager(void)
    {
    }

    ThreadManager::~ThreadManager(void)
    {
        ThreadManager::joinThreads(this, ::std::move(this->_threadPool));
        this->clearGarbage();
    }

    /* Methods */

    void    ThreadManager::add(::std::thread::id const &id)
    {
        ::std::lock_guard<::std::shared_mutex> lock(this->_locker);

        this->_threads.insert(id);
    }

    void    ThreadManager::add(Core *core, unsigned int const nbThread)
    {
        ::std::lock_guard<::std::shared_mutex> lock(this->_lockerPool);

        for (unsigned int i = 0; i < nbThread; ++i)
        {
            this->_threadPool.emplace([](Core *core)
            {
                core->run();
            }, core);
        }
    }

    void    ThreadManager::remove(::std::thread::id const &id)
    {
        ::std::lock_guard<::std::shared_mutex> lock(this->_locker);

        this->_threads.erase(id);
    }

    void    ThreadManager::remove(Core *core, unsigned int const nbThread)
    {
        ::std::lock_guard<::std::shared_mutex>          lock(this->_lockerPool);
        ::std::shared_ptr<::std::queue<::std::thread>>  threads = ::std::make_shared<::std::queue<::std::thread>>();

        for (unsigned int i = 0; i < nbThread && this->_threadPool.size(); ++i)
        {
            ::std::thread thread(::std::move(this->_threadPool.front()));

            this->_threadPool.pop();
            core->stop(thread.get_id());
            threads->push(::std::move(thread));
        }
        if (!threads->empty())
        {
            core->post([](ThreadManager *manager, ::std::shared_ptr<::std::queue<::std::thread>> const &threads)
            {
                ThreadManager::joinThreads(manager, ::std::move(*threads));
            }, this, ::std::shared_ptr<::std::queue<::std::thread>>(threads));
        }
    }

    void    ThreadManager::removeAll(void)
    {
        ::std::lock_guard<::std::shared_mutex>  lock(this->_locker);

        this->_threads.clear();
        ThreadManager::joinThreads(this, ::std::move(this->_threadPool));
    }

    bool    ThreadManager::exist(::std::thread::id const &id) const
    {
        ::std::shared_lock          lock(this->_locker);

        return (this->_threads.find(id) != this->_threads.end());
    }

    void    ThreadManager::clearGarbage(void)
    {
        ::std::lock_guard<::std::mutex> lock(this->_lockerGarbage);

        if (this->_garbage.joinable() && this->_garbage.get_id() != ::std::this_thread::get_id())
        {
            this->_garbage.join();
        }
    }

    /* Accessors */

    size_t		ThreadManager::getNbWorkingThread(void) const
    {
        ::std::shared_lock<::std::shared_mutex> lock(this->_locker);

        return this->_threads.size();
    }

    size_t		ThreadManager::getNbThreadPool(void) const
    {
        ::std::shared_lock<::std::shared_mutex> lock(this->_lockerPool);

        return this->_threadPool.size();
    }

    /********* Private **********/

    /* Static methods */

    void    ThreadManager::joinThreads(ThreadManager *manager, ::std::queue<::std::thread> &&threads)
    {
        while (!threads.empty())
        {
            ::std::thread thread(::std::move(threads.front()));

            threads.pop();
            if (thread.get_id() != ::std::this_thread::get_id())
            {
                thread.join();
            }
            else
            {
                manager->_lockerGarbage.lock();
                if (manager->_garbage.joinable() && manager->_garbage.get_id() != ::std::this_thread::get_id())
                {
                    manager->_garbage.join();
                }
                manager->_garbage = ::std::move(thread);
                manager->_lockerGarbage.unlock();
            }
        }
    }
}