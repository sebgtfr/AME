/**
* \file				Worker.hpp
* \author			Sébastien Le Maire
* \date				25/02/2019
*/

#ifndef _AME_WORKER_HPP_
# define _AME_WORKER_HPP_

# include <memory>
# include <utility>
# include <shared_mutex>

# include "AME/Tools/import.hpp"
# include "AME/Work.hpp"

namespace   AME
{
    class AME_IMPORT    Worker
    {
        public:
            /* Subclasses */
            class AME_IMPORT	Queue
            {
                public:
                    /* Subclasses */
                    class AME_IMPORT	Node
                    {
                        public:
                            /* Ctor & dtor */
                            Node(IWork::Pointer &work);
                            Node(IWork::Pointer &&work);
                            ~Node(void);

                            /* Accessors */
                            void    next(Node *node);
                            Node    *next(void) const;

                            /* Operators */
                            IWork           &operator*(void);
                            IWork           *operator->(void);

                        private:
                            IWork::Pointer  _work;
                            Node            *_next;
                    };

                    /* Ctor & dtor */
                    Queue(void);
                    ~Queue(void);

                    /* Methods */
                    void        push(IWork::Pointer &work);
                    void        push(IWork::Pointer &&work);
                    void        push(Queue::Node *node);
                    Queue::Node *pop(void);
                    void        clear(void);
                
                private:
                    /* Attributes */
                    Queue::Node *_begin;
                    Queue::Node *_end;

                    /* Methods */
                    void        addNode(Queue::Node *node);
            };

            /* Ctor & dtor */

            Worker(void);
            ~Worker(void);

            /* Methods */

            void            push(IWork::Pointer &work);
            void            push(IWork::Pointer &&work);
            void            push(Worker::Queue::Node *node);
            Queue::Node     *pop(void);

            /* Template methods */

            template <typename Callback, typename... Args>
            inline void     push(Callback &&callback, Args&&... args)
            {
                ::std::lock_guard<::std::mutex> lock(this->_locker);

                this->_works.push(Worker::Create(::std::forward<Callback>(callback),
                                                ::std::forward<Args>(args)...));
            }

            template <typename Callback, typename... Args>
            inline void     pushRepeat(uint8_t const repeat, Callback &&callback, Args&&... args)
            {
                ::std::lock_guard<::std::mutex> lock(this->_locker);

                this->_works.push(Worker::CreateRepeat(repeat, ::std::forward<Callback>(callback),
                                                ::std::forward<Args>(args)...));
            }

            /* Static template methods */
            template <typename Callback, typename... Args>
            static IWork::Pointer   Create(Callback &&callback, Args&&... args)
            {
                return ::std::make_shared<Work<Args...>>(::std::forward<Callback>(callback),
                                                        ::std::forward<Args>(args)...);
            }

            /* Static template methods */
            template <typename Callback, typename... Args>
            static IWork::Pointer   CreateRepeat(uint8_t const repeat, Callback &&callback, Args&&... args)
            {
                return ::std::make_shared<Repeatable::Work<Args...>>(repeat, ::std::forward<Callback>(callback),
                                                                    ::std::forward<Args>(args)...);
            }

        private:
            /* Attributes */
            Queue                           _works;
            mutable ::std::mutex            _locker;
    };
}

#endif /* !_AME_WORKER_HPP_ */
