/**
* \file				ThreadManager.cpp
* \author			Sébastien Le Maire
* \date				28/04/2020
*/

#include <memory>
#include <algorithm>

#include "AME/ThreadManager.hpp"
#include "AME/Core.hpp"

namespace   AME
{
    /********** Public **********/

    /* Ctor & dtor */

    ThreadManager::~ThreadManager(void)
    {
        this->joinThreads(::std::move(this->_innerQueue));
        this->clearGarbage();
    }

    /* Methods */

    void    ThreadManager::add(ThreadManager::Id const &id)
    {
        ::std::scoped_lock<::std::shared_mutex> lock(this->_locker);

        this->_threads.insert(id);
    }

    void    ThreadManager::add(Core *core, unsigned int const nbThread)
    {
        ::std::scoped_lock<::std::shared_mutex> lock(this->_lockerQueue);

        for (unsigned int i = 0; i < nbThread; ++i)
        {
            this->_innerQueue.emplace_back([this](Core *core)
            {
                core->run();
                this->remove();
            }, core);
        }
    }

    void    ThreadManager::remove(ThreadManager::Id const &id)
    {
        ::std::scoped_lock<::std::shared_mutex> lock(this->_locker);

        this->_threads.erase(id);
    }

    void    ThreadManager::remove(Core *core, unsigned int const nbThread)
    {
        ::std::scoped_lock<::std::shared_mutex>         lock(this->_lockerQueue);
        ::std::shared_ptr<::std::list<::std::thread>>   threads = ::std::make_shared<::std::list<::std::thread>>();

        for (unsigned int i = 0; i < nbThread && this->_innerQueue.size(); ++i)
        {
            ::std::thread thread(::std::move(this->_innerQueue.front()));

            this->_innerQueue.pop_front();
            core->stop(thread.get_id());
            threads->push_back(::std::move(thread));
        }
        if (threads->empty())
        {
            core->post([](ThreadManager *manager, ::std::shared_ptr<::std::list<::std::thread>> const &threads) -> bool
            {
                manager->joinThreads(::std::move(*threads));
                return Core::STOP;
            }, this, threads);
        }
    }

    void    ThreadManager::removeAll(void)
    {
        ::std::scoped_lock<::std::shared_mutex>  lock(this->_locker);

        this->_threads.clear();
        this->joinThreads(::std::move(this->_innerQueue));
    }

    bool    ThreadManager::exist(ThreadManager::Id const &id) const
    {
        ::std::shared_lock  lock(this->_locker);

        return (this->_threads.find(id) != this->_threads.end());
    }

    void    ThreadManager::clearGarbage(void)
    {
        ::std::scoped_lock<::std::mutex> lock(this->_lockerGarbage);

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
        ::std::shared_lock<::std::shared_mutex> lock(this->_lockerQueue);

        return this->_innerQueue.size();
    }

    /********* Private **********/

    /* Methods */

    void    ThreadManager::remove(void)
    {
        ::std::scoped_lock  lock(this->_lockerQueue);
        auto                it = ::std::find_if(this->_innerQueue.begin(), this->_innerQueue.end(),
        [](::std::thread const &thread) -> bool
        {
            return thread.get_id() == ::std::this_thread::get_id();
        });

        this->joinThread(::std::move(*it));
        this->_innerQueue.erase(it);
    }

    /* Static methods */

    void    ThreadManager::joinThread(::std::thread &&thread)
    {
        if (thread.joinable() && thread.get_id() != ::std::this_thread::get_id())
        {
            thread.join();
        }
        else
        {
            ::std::scoped_lock      lock(this->_lockerGarbage);

            if (this->_garbage.joinable() && this->_garbage.get_id() != ::std::this_thread::get_id())
            {
                this->_garbage.join();
            }
            this->_garbage = ::std::move(thread);
        }
    }

    void    ThreadManager::joinThreads(::std::list<::std::thread> &&threads)
    {
        while (!threads.empty())
        {
            this->joinThread(::std::move(threads.front()));
            threads.pop_front();
        }
    }
}