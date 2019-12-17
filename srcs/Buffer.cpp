/**
* \file				Buffer.cpp
* \author			Sébastien Le Maire
* \date				21/02/2019
*/

#include <utility>

#include "AME/Buffer.hpp"

namespace   AME
{
    /* Public */

    /* Ctor & Dtor on canonics form */

    Buffer::Buffer(size_t const capacity)
        : _data()
    {
        this->_data.reserve(capacity);
    }

    Buffer::Buffer(char const *data)
        : _data(data, data + ::std::strlen(data))
    {

    }
    
    Buffer::Buffer(uint8_t const *data, size_t const size)
        : _data(data, data + size)
    {

    }
    
    Buffer::Buffer(::std::string const &str)
        : _data(str.begin(), str.end())
    {

    }

    Buffer::Buffer(Buffer const &copy)
        : _data(copy._data)
    {
    }

    Buffer::Buffer(Buffer &&buffer)
        : _data(::std::move(buffer._data))
    {
    }

    Buffer      &Buffer::operator=(Buffer const &other)
    {
        if (&other != this)
        {
            this->_data = other._data;
        }
        return *this;
    }

    Buffer      &Buffer::operator=(Buffer &&other)
    {
        if (&other != this)
        {
            this->_data = ::std::move(other._data);
        }
        return *this;
    }

    Buffer::~Buffer(void)
    {
    }

    /* Methods */

    void        Buffer::addMemory(size_t const size)
    {
        this->_data.reserve(this->capacity() + size);
    }

    void        Buffer::reset(size_t const size)
    {
        this->clear();
        this->shrinkToFit();
        this->addMemory(size);
    }
}