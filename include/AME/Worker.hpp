/**
* \file				Worker.hpp
* \author			Sébastien Le Maire
* \date				28/04/2020
*/

#ifndef _AME_WORKER_HPP_
# define _AME_WORKER_HPP_

# include <utility>
# include <functional>

# include "AME/Tools/import.hpp"

namespace   AME
{
    class AME_IMPORT    Worker
    {
        public:
            /* Aliases */
            using Work = ::std::function<bool()>;

            /* Sub classes */
            class AME_IMPORT    Node
            {
                public:
                    /* Attributes */
                    Node        *next;

                    /* Ctor & Dtor */
                    Node(Work const &work);
                    Node(Work &&work);

                    /* Operators */
                    bool    operator()(void) const;

                    /* Methods */
                    bool    run(void) const;

                private:
                    /* Attributes */
                    Work        _work;
            };

            /* Ctor & Dtor */
            Worker(void);
            ~Worker(void);

            void        push(Work const &work);
            void        push(Work &&work);
            void        push(Worker::Node *node);
            Worker::Node *pop(void);
            void        clear(void);

            /* Template methods */

            template <typename Callback, typename... Args>
            inline void push(Callback &&callback, Args&&... args)
            {
                this->push(Worker::Create(::std::forward<Callback>(callback),
                            ::std::forward<Args>(args)...));
            }

            /* Static template methods */
            
            template <typename Callback, typename... Args>
            static Work     Create(Callback &&callback, Args&&... args)
            {
                return ::std::bind(::std::forward<Callback>(callback), ::std::forward<Args>(args)...);
            }

        private:
            /* Attributes */
            Node        *_begin;
            Node        *_end;

            /* Methods */
            void        addNode(Node *node);
    };

    /********** Inline definitions **********/

    /* Public */

    /* Ctor & Dtor */

    inline Worker::Worker(void) : _begin(nullptr), _end(nullptr)
    {
    }

    inline Worker::~Worker(void)
    {
        this->clear();
    }

    /* Methods */

    inline void     Worker::push(Work const &work)
    {
        this->addNode(new Worker::Node(work));
    }

    inline void     Worker::push(Work &&work)
    {
        this->addNode(new Worker::Node(::std::move(work)));
    }

    inline void     Worker::push(Worker::Node *node)
    {
        this->addNode(node);
    }

    /* Sub classes */

    /** Node **/

    /* Public */

    /* Ctor & Dtor */
    inline Worker::Node::Node(Work const &work) : next(nullptr), _work(work)
    {
    }

    inline Worker::Node::Node(Work &&work) : next(nullptr), _work(::std::move(work))
    {
    }

    /* Operators */

    inline bool     Worker::Node::operator()(void) const
    {
        return this->run();
    }

    /* Methods */

    inline bool     Worker::Node::run(void) const
    {
        return this->_work();
    }
}

#endif /* !_AME_WORKER_HPP_ */
