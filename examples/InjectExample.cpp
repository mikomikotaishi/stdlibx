import stdx;

#ifdef __cpp_impl_reflection
using stdx::inject::Binder;
using stdx::inject::Configuration;
using stdx::inject::Inject;
using stdx::inject::Injector;
using stdx::inject::Named;
using stdx::inject::Provider;
using stdx::inject::Singleton;
using stdx::mem::SharedPointer;
using stdx::util::logging::Logger;
using stdx::util::logging::LoggerFactory;
#endif

#ifdef __GNUC__
using namespace stdx::core;
#endif

#ifdef __cpp_impl_reflection
class [[=Singleton()]] GreetingService {
private:
    SharedPointer<Logger> logger;
    String greeting;
public:
    [[=Inject()]]
    GreetingService(SharedPointer<Logger> logger, [[=Named("greeting")]] String greeting):
        logger{Ops::move(logger)}, greeting{Ops::move(greeting)} {}

    void greet(StringView who) const {
        logger->info("{}, {}!", greeting, who);
    }
};

class [[=Singleton()]] Application {
private:
    GreetingService& service;
    SharedPointer<Logger> logger;
    Provider<String> greeting_provider;
public:
    [[=Inject()]]
    Application(
        GreetingService& service,
        SharedPointer<Logger> logger,
        [[=Named("greeting")]] Provider<String> greeting_provider
    ):
        service{service}, logger{Ops::move(logger)}, greeting_provider{Ops::move(greeting_provider)} {}

    void run() {
        logger->info("Application starting");
        service.greet("world");
        service.greet("dependency injection");
        logger->info("Lazily resolved greeting: {}", greeting_provider.get());
        logger->info("Application done");
    }
};

class AppConfiguration final: public Configuration {
public:
    void configure(Binder& binder) override {
        // The injector owns the logging factory; any component can inject the
        // factory itself or, as below, a logger it produces.
        binder.bind<SharedPointer<LoggerFactory>>()
            .to_instance(
                LoggerFactory::Builder()
                    .with_console(false)
                    .build_shared()
            );

        binder.bind<SharedPointer<Logger>>()
            .to_provider([](Injector& injector) -> SharedPointer<Logger> {
                return injector.get<SharedPointer<LoggerFactory>>()->of<Application>();
            });

        binder.bind<String>()
            .annotated_with(Named("greeting"))
            .to_instance("Hello");

        binder.add_binding<GreetingService>();
        binder.add_binding<Application>();
    }
};

#endif

int main(int argc, char* argv[]) {
    #ifdef __cpp_impl_reflection
    // The logging factory is owned by the injector (bound in AppConfiguration);
    // loggers reach components purely through injection.
    AppConfiguration config;
    Injector injector = Injector::create(config);

    injector.get<Application&>().run();
    #else
    System::out.println("Example disabled (compiler does not support reflection).");
    #endif
}
