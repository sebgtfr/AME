/**
* \file				Core.cpp
* \author			Sébastien Le Maire
* \date				28/04/2020
*/

#include "AME/Core.hpp"

namespace   AME
{
    /********** Public **********/

    /* Methods */

    void                        Core::run(void)
    {
        ThreadManager::Id const currentThreadId = ::std::this_thread::get_id();

        Worker::Node            *work = nullptr;

        this->_threadManager.add(currentThreadId);
        while (this->_threadManager.exist(currentThreadId) &&
                ((work = this->pop()) != nullptr || this->_threadBusy != 0))
        {
            if (work == nullptr)
            {
                ::std::this_thread::yield();
            }
            else
            {
                ++this->_threadBusy;
                if (work->run())
                {
                    this->_worker.push(work);
                }
                else
                {
                    delete work;
                }
                --this->_threadBusy;
            }
        }
        this->_threadManager.remove(currentThreadId);
    }

    void                    Core::clear(void)
    {
        {
            ::std::scoped_lock  lock(this->_lock);

            this->_worker.clear();
        }
        this->_threadManager.removeAll();
    }

    /********** Private **********/

    /* Methods */

    /* Works control */

    Worker::Node            *Core::pop(void)
    {
        ::std::scoped_lock  lock(this->_lock);
        
        return this->_worker.pop();
    }
}