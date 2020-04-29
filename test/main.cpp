#include <iostream>
#include <string>
#include <functional>
#include <thread>

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
        core->post([]() -> bool
        {
            return ::AME::Core::STOP;
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

static void         testWorks(::AME::Core *core)
{
    auto            t1 = ::AME::Timeout::Clock::now();
    ::std::size_t   totalWorks = 0;

    totalWorks += executeTestWorks<1, std::chrono::nanoseconds, ns>(core);
    totalWorks += executeTestWorks<256, std::chrono::nanoseconds, ns>(core);
    totalWorks += executeTestWorks<1024, std::chrono::microseconds, us>(core);
    totalWorks += executeTestWorks<10000, std::chrono::milliseconds, ms>(core);
    totalWorks += executeTestWorks<1000000, std::chrono::milliseconds, ms>(core);
    totalWorks += executeTestWorks<5000000, std::chrono::milliseconds, ms>(core);

    std::cout   << ::std::endl << "Total for "
                << totalWorks << " Works: "
                << ::std::chrono::duration_cast<std::chrono::milliseconds>(::AME::Timeout::Clock::now() - t1).count() << ms
                << ::std::endl;
}

static void         testTimeout(::AME::Core *core)
{
    auto            t1 = ::AME::Timeout::Clock::now();
    ::AME::Timeout  timeout(3000);

    core->setTimeout(timeout, [](::std::string const &name)
    {
        ::std::cout << "Hello " << name << " after 3000 ms" << ::std::endl;
    }, "Sébastien");
    core->setTimeout(timeout + 2000, [](void)
    {
        ::std::cout << "Hello after 5000 ms" << ::std::endl;
    });

    core->run();

    std::cout   << ::std::endl << "Total: "
                << ::std::chrono::duration_cast<std::chrono::milliseconds>(::AME::Timeout::Clock::now() - t1).count() << ms
                << ::std::endl;
}

template<::std::size_t NB_WORKS, typename TYPE_TIME, char const *STR_TYPE_TIME>
static ::std::size_t    executeTestWorksThreads(::AME::Core *core)
{
    auto t1 = ::AME::Timeout::Clock::now();

    core->post([](::AME::Core *core) -> bool
    {
        for (::std::size_t i = 0; i < NB_WORKS; ++i)
        {
            core->post([]() -> bool
            {
                ::std::this_thread::sleep_for(::AME::Timeout::Ms(8));
                return ::AME::Core::STOP;
            });
        }
        return ::AME::Core::STOP;
    }, core);

    core->addThreads(3);
    core->run();

    std::cout << "For " << NB_WORKS << " works: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(::AME::Timeout::Clock::now() - t1).count()
              << STR_TYPE_TIME << std::endl;
    ::std::this_thread::sleep_for(::AME::Timeout::Ms(8));
    return NB_WORKS;
}

static void         testWorksThreads(::AME::Core *core)
{
    auto            t1 = ::AME::Timeout::Clock::now();
    ::std::size_t   totalWorks = 0;

    totalWorks += executeTestWorksThreads<256, std::chrono::nanoseconds, ns>(core);
    totalWorks += executeTestWorksThreads<1024, std::chrono::microseconds, us>(core);
    totalWorks += executeTestWorksThreads<4096, std::chrono::milliseconds, ms>(core);
    totalWorks += executeTestWorksThreads<8192, std::chrono::milliseconds, ms>(core);

    std::cout   << ::std::endl << "Total for "
                << totalWorks << " Works: "
                << ::std::chrono::duration_cast<std::chrono::milliseconds>(::AME::Timeout::Clock::now() - t1).count() << ms
                << ::std::endl;
}

int main(void)
{
    ::AME::Core core;

    try
    {
        launchTest(&core, "Push/Pop work on core with 1 thread (loading test)", &testWorks);
        launchTest(&core, "Set timeout on core with 1 thread", &testTimeout);
        launchTest(&core, "Push/Pop work that sleep 8ms on core with 4 thread (perf test)", &testWorksThreads);
    }
    catch (::std::exception const &ex)
    {
        ::std::cerr << ex.what() << ::std::endl;
        return -1;
    }
    
    //::std::cout << ::std::endl;
    return 0;
}