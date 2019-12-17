
#include <algorithm>

#include "AME/Worker.hpp"

namespace   AME
{
    /********** Public **********/

    /* Ctor & dtor */

    Worker::Worker(void)
    {
    }

    Worker::~Worker(void)
    {
    }

    /* Methods */

    void    Worker::push(IWork::Pointer &work)
    {
        ::std::lock_guard<::std::mutex> lock(this->_locker);

        this->_works.push(work);
    }

    void    Worker::push(IWork::Pointer &&work)
    {
        ::std::lock_guard<::std::mutex> lock(this->_locker);

        this->_works.push(::std::move(work));
    }

    void    Worker::push(Worker::Queue::Node *node)
    {
        ::std::lock_guard<::std::mutex> lock(this->_locker);

        this->_works.push(node);
    }

    Worker::Queue::Node *Worker::pop(void)
    {
        ::std::lock_guard<::std::mutex> lock(this->_locker);

        return this->_works.pop();
    }

    /* Subclasses */

        /********** Queue **********/

        /********** Public **********/

        /* Ctor & dtor */

        Worker::Queue::Queue(void) : _begin(nullptr), _end(nullptr)
        {
        }

        Worker::Queue::~Queue(void)
        {
            this->clear();
        }

        /* Methods */

        void    Worker::Queue::push(IWork::Pointer &work)
        {
            this->addNode(new Worker::Queue::Node(work));
        }

        void    Worker::Queue::push(IWork::Pointer &&work)
        {
            this->addNode(new Worker::Queue::Node(::std::move(work)));
        }

        void    Worker::Queue::push(Worker::Queue::Node *node)
        {
            this->addNode(node);
        }

        Worker::Queue::Node *Worker::Queue::pop(void)
        {
            Worker::Queue::Node *ret = this->_begin;

            if (this->_begin != nullptr)
            {
                this->_begin = this->_begin->next();
            }
            return ret;
        }

        void    Worker::Queue::clear(void)
        {
            while (this->_begin != nullptr)
            {
                Worker::Queue::Node *it = this->_begin;

                this->_begin = this->_begin->next();
                delete it;
            }
        }

        /********** Private **********/

        /* Methods */

        void    Worker::Queue::addNode(Worker::Queue::Node *node)
        {
            node->next(nullptr);
            if (this->_begin == nullptr)
            {
                this->_begin = this->_end = node;
            }
            else
            {
                this->_end->next(node);
                this->_end = node;
            }
        }

        /* Subclasses */

            /********** Public **********/

            /* Ctor & dtor */

            Worker::Queue::Node::Node(IWork::Pointer &work) : _work(::std::move(work)), _next(nullptr)
            {
            }

            Worker::Queue::Node::Node(IWork::Pointer &&work) : _work(::std::move(work)), _next(nullptr)
            {
            }

            Worker::Queue::Node::~Node(void)
            {
            }

            /* Accessors */
            void    Worker::Queue::Node::next(Node *node)
            {
                this->_next = node;
            }

            Worker::Queue::Node *Worker::Queue::Node::next(void) const
            {
                return this->_next;
            }

            /* Operators */

            IWork           &Worker::Queue::Node::operator*(void)
            {
                return *(this->_work.get());
            }

            IWork           *Worker::Queue::Node::operator->(void)
            {
                return this->_work.get();
            }
}
