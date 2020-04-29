#include <iostream>
#include <string>
#include <functional>
#include <thread>

#include "AME/Timeout.hpp"
#include "AME/Core.hpp"

#include "test.hpp"

constexpr char const ns[] = " ns";
constexpr char const us[] = " us";
constexpr char const ms[] = " ms";

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
            return ;
        });
    }

    core->run();

    std::cout << "For " << NB_WORKS << " works: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(::AME::Timeout::Clock::now() - t1).count()
              << STR_TYPE_TIME << std::endl;
    return NB_WORKS;
}


static void         testWorks(::AME::Core *core)
{
    auto            t1 = ::AME::Timeout::Clock::now();
    ::std::size_t   totalWorks = 0;

    totalWorks += executeTestWorks<1, std::chrono::nanoseconds, ns>(core);
    totalWorks += executeTestWorks<256, std::chrono::nanoseconds, ns>(core);
    totalWorks += executeTestWorks<1024, std::chrono::microseconds, us>(core);
    totalWorks += executeTestWorks<10000, std::chrono::milliseconds, ms>(core);
    totalWorks += executeTestWorks<1000000, std::chrono::milliseconds, ms>(core);
    totalWorks += executeTestWorks<9000000, std::chrono::milliseconds, ms>(core);

    std::cout   << ::std::endl << "Total for "
                << totalWorks << " Works: "
                << ::std::chrono::duration_cast<std::chrono::milliseconds>(::AME::Timeout::Clock::now() - t1).count() << ms
                << ::std::endl;
}

class Service
{
    ::std::string const _name;

    public:
        inline Service(::std::string const &name = "World")
            : _name(name)
        {}

        Service(Service const &) = delete;
        Service(Service &&) = delete;

        bool update(void)
        {
            ::std::cout << "Hello " << _name << " !" << ::std::endl;
            return ::AME::Core::STOP;
        }

        bool speak(::std::string const &WhatIsSaying)
        {
            ::std::cout << _name << ": " << WhatIsSaying << ::std::endl;
            return ::AME::Core::STOP;
        }
};

static void             testService(::AME::Core *core)
{
    auto                t1 = ::AME::Timeout::Clock::now();
    Service             toto("Toto");
    ::std::string const hw("Hello World !");

    core->post(&Service::update);
    core->post(&Service::update, toto);
    core->post(&Service::update, &toto);
    core->post(&Service::update, "Tata");

    core->post(&Service::speak, toto, hw);
    core->post(&Service::speak, &toto, hw);

    core->run();

    std::cout   << ::std::endl << "Total: "
                << ::std::chrono::duration_cast<std::chrono::milliseconds>(::AME::Timeout::Clock::now() - t1).count() << ms
                << ::std::endl;
}

static void         testTimeout(::AME::Core *core)
{
    auto            t1 = ::AME::Timeout::Clock::now();
    ::AME::Timeout  timeout(3000);
    uint32_t        i = 0;

    core->setTimeout(timeout, [](::std::string const &name)
    {
        ::std::cout << "Hello " << name << " after 3000 ms" << ::std::endl;
    }, "Sébastien");
    core->setTimeout(timeout + 2000, [](void)
    {
        ::std::cout << "Hello after 5000 ms" << ::std::endl;
    });
    core->setInterval(1000, [&i]() -> bool
    {
        ::std::cout << ++i << ::std::endl;
        return (i == 5u) ? ::AME::Core::STOP : ::AME::Core::CONTINUE;
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

    core->post([](::AME::Core *core)
    {
        for (::std::size_t i = 0; i < NB_WORKS; ++i)
        {
            core->post([]()
            {
                ::std::this_thread::sleep_for(::AME::Timeout::Ms(8));
            });
        }
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
        launchTest(&core, "Test service on core with 1 thread", &testService);
        launchTest(&core, "Set timeout and Interval on core with 1 thread", &testTimeout);
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