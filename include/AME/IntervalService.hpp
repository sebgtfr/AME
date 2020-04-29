/**
* \file				IntervalService.hpp
* \author			Sébastien Le Maire
* \date				01/05/2020
*/

#ifndef _AME_INTERVALSERVICE_HPP_
# define _AME_INTERVALSERVICE_HPP_

# include <utility>
# include <type_traits>

# include "AME/Timeout.hpp"
# include "AME/Worker.hpp"
# include "AME/Tools/import.hpp"

namespace   AME
{
    class AME_IMPORT    IntervalService
    {
        public:
            /* Ctor & dtor */

            IntervalService(uint64_t const time, Worker::Work const &work);
            ~IntervalService(void);

            /* Methods */

            bool            update(void);

        private:
            uint64_t        _time;
            Timeout         _timer;
            Worker::Work    _work;
            
    };

    /********** Public **********/

    /* Ctor & Dtor */
    inline IntervalService::IntervalService(uint64_t const time, Worker::Work const &work)
        : _time(time), _timer(time), _work(work)
    {
    }

    inline IntervalService::~IntervalService(void)
    {
    }
}

#endif /* !_AME_INTERVALSERVICE_HPP_ */