/**
* \file				Time.hpp
* \author			Sébastien Le Maire
* \date				31/08/2019
*/

#ifndef _AME_SERVICES_TIME_HPP_
# define _AME_SERVICES_TIME_HPP_

# include "AME/Core.hpp"
# include "AME/Interfaces/IService.hpp"

# include "AME/Interfaces/IWork.hpp"
# include "AME/Tools/import.hpp"
# include "AME/Timeout.hpp"

namespace   AME
{
    namespace   Service
    {
        class AME_IMPORT    Time final : public ::AME::IService
        {
            public:
                /* Ctor & dtor */
                Time(::AME::IWork::Pointer const &work,
                    unsigned int const time, bool const repeat = false);
                ~Time(void) final;

                /* Methods */
                ::AME::IService::State  update(::AME::Core *core) final;

            private:
                /* Attributes */
                ::AME::IWork::Pointer const _work;
                unsigned int const          _time;
                ::AME::Timeout              _timer;
                bool const                  _repeat;
        };
    }
}

#endif /* !_AME_SERVICES_TIME_HPP_ */
