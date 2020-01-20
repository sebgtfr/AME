#include <iostream>
#include <string>
#include <functional>
#include <exception>

#include "AME/Timeout.hpp"
#include "AME/Core.hpp"

#include "test.hpp"

static void launchTest(::AME::Core *core, ::std::string const &testName,
                        ::std::function<void(::AME::Core *)> const &testFunc)
{
    ::std::cout << "########## Start " << testName << " tests ##########" << ::std::endl << ::std::endl;
    testFunc(core);
    ::std::cout << ::std::endl << "########## End " << testName << " tests ##########" << ::std::endl;
}

template<::std::size_t NB_WORKS, typename TYPE_TIME, char const *STR_TYPE_TIME>
static ::std::size_t    executeTestWorks(::AME::Core *core)
{
    auto t1 = ::AME::Timeout::Clock::now();

    for (::std::size_t i = 0; i < NB_WORKS; ++i)
    {
        core->post([]()
        {
            return;
        });
    }

    core->run();

    std::cout << "For " << NB_WORKS << " works: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(::AME::Timeout::Clock::now() - t1).count()
              << STR_TYPE_TIME << std::endl;
    return NB_WORKS;
}

constexpr char const ns[] = " ns";
constexpr char const us[] = " us";
constexpr char const ms[] = " ms";

static void    testWorks(::AME::Core *core)
{
    auto t1 = ::AME::Timeout::Clock::now();
    ::std::size_t   totalWorks = 0;

    totalWorks += executeTestWorks<1, std::chrono::nanoseconds, ns>(core);
    totalWorks += executeTestWorks<256, std::chrono::nanoseconds, ns>(core);
    totalWorks += executeTestWorks<1024, std::chrono::microseconds, us>(core);
    totalWorks += executeTestWorks<10000, std::chrono::milliseconds, ms>(core);
    totalWorks += executeTestWorks<1000000, std::chrono::milliseconds, ms>(core);
    totalWorks += executeTestWorks<9000000, std::chrono::milliseconds, ms>(core);

    std::cout   << ::std::endl << "Total for "
                << totalWorks << " Works: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(::AME::Timeout::Clock::now() - t1).count() << ms
                << ::std::endl;
}

int main(void)
{
    ::AME::Core core;

    try
    {
        launchTest(&core, "Push/Pop work on core with 1 thread", &testWorks);
    }
    catch (::std::exception const &ex)
    {
        ::std::cerr << ex.what() << ::std::endl;
        return -1;
    }
    
    //::std::cout << ::std::endl;
    return 0;
}