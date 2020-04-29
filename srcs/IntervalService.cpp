/**
* \file				IntervalService.cpp
* \author			Sébastien Le Maire
* \date				01/05/2020
*/

#include "AME/IntervalService.hpp"
#include "AME/Core.hpp"

namespace   AME
{
    /********** Public **********/

    /* Methods */

    bool    IntervalService::update(void)
    {
        int64_t const timeLeft = this->_timer.msLeftBeforeExpires().count();

        if (timeLeft <= 0)
        {
            if (!this->_work())
            {
                return Core::STOP;
            }
            this->_timer.expiresAfter((static_cast<uint64_t>(timeLeft * -1l) >= this->_time) ? 0 : this->_time + timeLeft);
        }
        return Core::CONTINUE;
    }
}