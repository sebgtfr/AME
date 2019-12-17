#include "AME/Timeout.hpp"

namespace   AME
{
    /********** Public **********/

    /* Ctor & dtor on canonic form */

    Timeout::Timeout(Clock::time_point const &deadline)
        : _deadline(deadline)
    {
    }

    Timeout::Timeout(uint64_t const timeout)
        : _deadline(Clock::now() + Ms(timeout))
    {
    }

    Timeout::Timeout(Ms const &timeout)
        : _deadline(Clock::now() + timeout)
    {
    }

    Timeout::Timeout(Timeout const &timeout)
        : _deadline(timeout._deadline)
    {
    }

    Timeout &Timeout::operator=(Timeout const &timeout)
    {
        if (this != &timeout)
        {
            this->_deadline = timeout._deadline;
        }
        return *this;
    }

    Timeout::~Timeout(void)
    {
    }

    /* Methods */

    void    Timeout::expiresAt(Clock::time_point const &deadline)
    {
        this->_deadline = deadline;
    }

    void    Timeout::expiresAfter(uint64_t const timeout)
    {
        this->expiresAt(Clock::now() + Ms(timeout));
    }

    void    Timeout::expiresAfter(Ms const &timeout)
    {
        this->expiresAt(Clock::now() + timeout);
    }

    Timeout::Ms Timeout::msLeftBeforeExpires(void) const
    {
        return ::std::chrono::duration_cast<Timeout::Ms>(this->_deadline - Clock::now());
    }

    bool    Timeout::hasExpired(void) const
    {
        return (this->_deadline <= Clock::now());
    }

    /* Operator */

    bool    Timeout::operator==(Timeout const &timeout) const
    {
        return (this->_deadline == timeout._deadline);
    }

    bool    Timeout::operator!=(Timeout const &timeout) const
    {
        return (this->_deadline != timeout._deadline);
    }

    bool    Timeout::operator<(Timeout const &timeout) const
    {
        return (this->_deadline < timeout._deadline);
    }

    bool    Timeout::operator<=(Timeout const &timeout) const
    {
        return (this->_deadline <= timeout._deadline);
    }

    bool    Timeout::operator>(Timeout const &timeout) const
    {
        return (this->_deadline > timeout._deadline);
    }

    bool    Timeout::operator>=(Timeout const &timeout) const
    {
        return (this->_deadline >= timeout._deadline);
    }

    /* Static methods */

    Timeout Timeout::now(void)
    {
        return Timeout(Clock::now());
    }
}