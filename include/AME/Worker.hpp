/**
* \file				Worker.hpp
* \author			Sébastien Le Maire
* \date				28/04/2020
*/

#ifndef _AME_WORKER_HPP_
# define _AME_WORKER_HPP_

# include <utility>
# include <memory>
# include <functional>
# include <type_traits>

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

            void            push(Worker::Node *node);
            Worker::Node    *pop(void);
            void            clear(void);

            /* Template methods */

            template <typename Object, typename T, typename... Args, typename = ::std::enable_if_t<!::std::is_same_v<::std::remove_pointer_t<::std::decay_t<T>>, Object>>>
            inline void     push(bool (Object::* method)(), T&& obj, Args&&... args)
            {
                this->addNode(new Worker::Node(Worker::Create(method, ::std::forward<T>(obj), ::std::forward<Args>(args)...)));
            }

            template <typename Object>
            inline void     push(bool (Object::* method)())
            {
                this->addNode(new Worker::Node(Worker::Create(method)));
            }

            template <typename Object, typename... Args>
            inline void     push(bool (Object::* method)(Args...), Object& obj, Args&&... args)
            {
                this->addNode(new Worker::Node(Worker::Create(method, obj, ::std::forward<Args>(args)...)));
            }

            template <typename Object, typename... Args>
            inline void     push(bool (Object::* method)(Args...), Object* obj, Args&&... args)
            {
                this->addNode(new Worker::Node(Worker::Create(method, obj, ::std::forward<Args>(args)...)));
            }

            template <typename Callback, typename... Args>
            inline void     push(Callback &&callback, Args&&... args)
            {
                using RetType = ::std::invoke_result_t<Callback, Args...>;

                if constexpr (::std::is_same_v<RetType, void>)
                {
                    this->addNode(new Worker::Node(Worker::Create([callback](Args&&... args) -> bool
                    {
                        ::std::invoke(callback, ::std::forward<Args>(args)...);
                        return false;
                    }, ::std::forward<Args>(args)...)));
                }
                else
                {
                    this->addNode(new Worker::Node(Worker::Create(::std::forward<Callback>(callback),
                                                                    ::std::forward<Args>(args)...)));
                }
            }

            /* Static template methods */

            template <typename Object, typename T, typename... Args, typename = ::std::enable_if_t<!::std::is_same_v<::std::remove_pointer_t<::std::decay_t<T>>, Object>>>
            static Work     Create(bool (Object::* method)(), T &&obj, Args&&... args)
            {
                return ::std::bind(method, ::std::make_shared<Object>(::std::forward<T>(obj), ::std::forward<Args>(args)...));
            }

            template <typename Object>
            static Work     Create(bool (Object::* method)())
            {
                return ::std::bind(method, ::std::make_shared<Object>());
            }

            template <typename Object, typename... Args>
            static Work     Create(bool (Object::*method)(Args...), Object &obj, Args&&... args)
            {
                return ::std::bind(method, ::std::ref(obj), ::std::forward<Args>(args)...);
            }

            template <typename Object, typename... Args>
            static Work     Create(bool (Object::*method)(Args...), Object *obj, Args&&... args)
            {
                return ::std::bind(method, obj, ::std::forward<Args>(args)...);
            }

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
