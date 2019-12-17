/**
* \file				Buffer.hpp
* \author			Sébastien Le Maire
* \date				21/02/2019
*/

#ifndef _AME_BUFFER_HPP_
# define _AME_BUFFER_HPP_

# include <cstdint>
# include <cstring>

# include <string>
# include <vector>
# include <iterator>
# include <functional>
# include <algorithm>
# include <iostream>

# include "AME/Tools/import.hpp"

namespace   AME
{
    class AME_IMPORT    Buffer
    {
    public:
        /* Definitions */
        using Container = ::std::vector<uint8_t>;
        using Iterator = Container::iterator;
        using ConstIterator = Container::const_iterator;
        using TransformCallback = ::std::function<uint8_t(uint8_t const)>;

    private:
        /* Attributs */
        Container                   _data;

    public:
        /* Ctor & Dtor on canonics form */
        Buffer(size_t const capacity = 0);
        Buffer(char const *data);
        Buffer(uint8_t const *data, size_t const size);
        Buffer(::std::string const &str);
        Buffer(Buffer const &copy);
        Buffer(Buffer &&buffer);
        Buffer                      &operator=(Buffer const &other);
        Buffer                      &operator=(Buffer &&other);
        ~Buffer(void);

        /* Accessors */
        size_t                      size(void) const;
        size_t                      capacity(void) const;
        uint8_t                     *data(void);
        uint8_t const               *data(void) const;
        Buffer::Iterator            begin(void);
        Buffer::Iterator            end(void);
        Buffer::ConstIterator       begin(void) const;
        Buffer::ConstIterator       end(void) const;

        /* Methods */
        void                        addMemory(size_t const size);
        void                        reset(size_t const size = 0);
        void                        resize(size_t const size);
        void                        push_back(uint8_t const c);
        void                        append(uint8_t const c);
        void                        append(char const *cstr);
        void                        append(uint8_t const *data, size_t const size);
        void                        append(Buffer const &buffer);
        void                        append(::std::string const &str);
        void                        append(char const *cstr, TransformCallback const &callback);
        void                        append(uint8_t const *data, size_t const size, TransformCallback const &callback);
        void                        append(Buffer const &buffer, TransformCallback const &callback);
        void                        append(::std::string const &str, TransformCallback const &callback);
        void                        transform(TransformCallback const &callback);
        void                        transform(TransformCallback const &callback, size_t const shift);
        void                        transform(TransformCallback const &callback, size_t const shift, size_t const nb);
        void                        clear(void);
        void                        erase(size_t const pos);
        void                        shrinkToFit(void);
        void                        shift(size_t const size);
        ::std::string               toString(void) const;

        /* Operators */
        uint8_t                     *operator*(void);
        uint8_t const               *operator*(void) const;
        uint8_t                     &operator[](size_t const i);
        uint8_t                     operator[](size_t const i) const;
        Buffer                      &operator+=(uint8_t const c);
        Buffer                      &operator+=(char const *cstr);
        Buffer                      &operator+=(::std::string const &str);
        Buffer                      &operator+=(Buffer const &buffer);
        bool                        operator==(char const *cstr);
        bool                        operator==(::std::string const &str);
        bool                        operator==(Buffer const &buffer);
        bool                        operator!=(char const *cstr);
        bool                        operator!=(::std::string const &str);
        bool                        operator!=(Buffer const &buffer);
                                    operator ::std::string(void) const;
    };

    /********** Inline definitions **********/

    /* Public */

    /* Accessors */
    inline size_t                   Buffer::size(void) const
    {
        return this->_data.size();
    }

    inline size_t                   Buffer::capacity(void) const
    {
        return this->_data.capacity();
    }

    inline uint8_t                  *Buffer::data(void)
    {
        return this->_data.data();
    }

    inline uint8_t const            *Buffer::data(void) const
    {
        return this->_data.data();
    }

    inline Buffer::Iterator         Buffer::begin(void)
    {
        return this->_data.begin();
    }

    inline Buffer::Iterator         Buffer::end(void)
    {
        return this->_data.end();
    }

    inline Buffer::ConstIterator    Buffer::begin(void) const
    {
        return this->_data.begin();
    }

    inline Buffer::ConstIterator    Buffer::end(void) const
    {
        return this->_data.end();
    }

    /* Methods */
    inline void                    Buffer::resize(size_t const size)
    {
        this->_data.resize(size);
    }

    inline void                     Buffer::push_back(uint8_t const c)
    {
        this->append(c);
    }

    inline void                     Buffer::append(uint8_t const c)
    {
        this->_data.push_back(c);
    }

    inline void                     Buffer::append(char const *cstr)
    {
        ::std::copy(cstr, cstr + ::std::strlen(cstr), ::std::back_inserter(this->_data));
    }

    inline void                     Buffer::append(uint8_t const *data,
                                                    size_t const size)
    {
        ::std::copy(data, data + size, ::std::back_inserter(this->_data));
    }

    inline void                     Buffer::append(Buffer const &buffer)
    {
        ::std::copy(buffer._data.begin(), buffer._data.end(), ::std::back_inserter(this->_data));
    }

    inline void                     Buffer::append(::std::string const &str)
    {
        ::std::copy(str.begin(), str.end(), ::std::back_inserter(this->_data));
    }

    inline void                     Buffer::append(char const *cstr, TransformCallback const &callback)
    {
        ::std::transform(cstr, cstr + ::std::strlen(cstr), ::std::back_inserter(this->_data), callback);
    }

    inline void                     Buffer::append(uint8_t const *data,
                                                    size_t const size, TransformCallback const &callback)
    {
        ::std::transform(data, data + size, ::std::back_inserter(this->_data), callback);
    }

    inline void                     Buffer::append(Buffer const &buffer, TransformCallback const &callback)
    {
        ::std::transform(buffer._data.begin(), buffer._data.end(), ::std::back_inserter(this->_data), callback);
    }

    inline void                     Buffer::append(::std::string const &str, TransformCallback const &callback)
    {
        ::std::transform(str.begin(), str.end(), ::std::back_inserter(this->_data), callback);
    }

    inline void                     Buffer::transform(TransformCallback const &callback)
    {
        ::std::transform(this->_data.begin(), this->_data.end(), this->_data.begin(), callback);   
    }

    inline void                     Buffer::transform(TransformCallback const &callback, size_t const shift)
    {
        if (shift < this->size())
        {
            ::std::transform(this->_data.begin() + shift, this->_data.end(), this->_data.begin() + shift, callback);
        }
    }

    inline void                     Buffer::transform(TransformCallback const &callback, size_t const shift, size_t const nb)
    {
        if (shift < this->size())
        {
            auto start = this->_data.begin() + shift;

            ::std::transform(start, (shift + nb >= this->size()) ? this->end() : start + nb, start, callback);
        }
    }

    inline void                     Buffer::clear(void)
    {
        this->_data.clear();
    }

    inline void                     Buffer::erase(size_t const pos)
    {
        if (pos < this->size())
            this->_data.erase(this->_data.begin() + pos);
    }

    inline void                     Buffer::shrinkToFit(void)
    {
        this->_data.shrink_to_fit();
    }

    inline void                     Buffer::shift(size_t const size)
    {
        if (this->size() <= size)
            this->clear();
        else
            this->_data.erase(this->_data.begin(), this->_data.begin() + size);
    }

    inline ::std::string            Buffer::toString(void) const
    {
        return ::std::string(this->begin(), this->end());
    }

    /* Operators */

    inline uint8_t                  *Buffer::operator*(void)
    {
        return this->_data.data();
    }

    inline uint8_t const            *Buffer::operator*(void) const
    {
        return this->_data.data();
    }

    inline uint8_t                  &Buffer::operator[](size_t const i)
    {
        return this->_data.at(i);
    }

    inline uint8_t                  Buffer::operator[](size_t const i) const
    {
        return this->_data.at(i);
    }

    inline Buffer                   &Buffer::operator+=(uint8_t const c)
    {
        this->append(c);
        return *this;
    }

    inline Buffer                   &Buffer::operator+=(char const *cstr)
    {
        this->append(cstr);
        return *this;
    }

    inline Buffer                   &Buffer::operator+=(::std::string const &str)
    {
        this->append(str);
        return *this;
    }

    inline Buffer                   &Buffer::operator+=(Buffer const &buffer)
    {
        this->append(buffer);
        return *this;
    }

    inline bool                     Buffer::operator==(char const *cstr)
    {
        return ((this->size() == ::std::strlen(cstr)) && ::std::equal(this->begin(), this->end(), cstr));
    }

    inline bool                     Buffer::operator==(::std::string const &str)
    {
        return ((this->size() == str.size()) && ::std::equal(this->begin(), this->end(), str.begin()));
    }

    inline bool                     Buffer::operator==(Buffer const &buffer)
    {
        return ((this->size() == buffer.size()) && ::std::equal(this->begin(), this->end(), buffer.begin()));
    }

    inline bool                     Buffer::operator!=(char const *cstr)
    {
        return !(*this == cstr);
    }

    inline bool                     Buffer::operator!=(::std::string const &str)
    {
        return !(*this == str);
    }

    inline bool                     Buffer::operator!=(Buffer const &buffer)
    {
        return !(*this == buffer);
    }

    inline                          Buffer::operator ::std::string(void) const
    {
        return this->toString();
    }
}

inline ::std::ostream             &operator<<(::std::ostream &os, ::AME::Buffer const &buffer)
{
    os << buffer.toString();
    return os;
}

#endif /* !_AME_BUFFER_HPP_ */
