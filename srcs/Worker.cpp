/**
* \file				Core.cpp
* \author			Sébastien Le Maire
* \date				28/04/2020
*/

#include "AME/Worker.hpp"

namespace   AME
{
    /********** Public **********/

    /* Methods */

    Worker::Node        *Worker::pop(void)
    {
        Worker::Node    *ret = this->_begin;

        if (this->_begin != nullptr)
        {
            this->_begin = this->_begin->next;
        }
        return ret;
    }

    void        Worker::clear(void)
    {
        while (this->_begin != nullptr)
        {
            Worker::Node    *it = this->_begin;

            this->_begin = this->_begin->next;
            delete it;
        }
    }

    /********** Private **********/

    /* Methods */

    void        Worker::addNode(Worker::Node *node)
    {
        node->next = nullptr;
        if (this->_begin == nullptr)
        {
            this->_begin = this->_end = node;
        }
        else
        {
            this->_end = this->_end->next = node;
        }
    }
}
