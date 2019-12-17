#include "AME/Services/Time.hpp"

namespace   AME
{
    namespace   Service
    {
        /********** Public **********/

        /* Ctor & dtor */

        Time::Time(::AME::IWork::Pointer const &work,
                    unsigned int const time, bool const repeat)
            : _work(work), _time(time), _timer(time), _repeat(repeat)
        {
        }

        Time::~Time(void)
        {
        }

        /* Methods */
        ::AME::IService::State  Time::update(::AME::Core *)
        {
            int64_t const timeLeft = this->_timer.msLeftBeforeExpires().count();

            if (timeLeft <= 0)
            {
                this->_work->run();
                if (this->_repeat == false)
                {
                    return ::AME::IService::State::STOP;
                }
                this->_timer.expiresAfter((static_cast<uint64_t>(timeLeft * -1l) >= this->_time) ? 0 : this->_time + timeLeft);
            }
            return ::AME::IService::State::CONTINUE;
        }
    }
}