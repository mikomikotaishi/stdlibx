#pragma once

class System final {
public:
    static constexpr auto EXIT_SUCCESS = ::core::prelude::EXIT_SUCCESS;
    static constexpr auto EXIT_FAILURE = ::core::prelude::EXIT_FAILURE;

    static constexpr auto HUGE_VAL = ::core::math::HUGE_VAL;
    static constexpr auto HUGE_VALF = ::core::math::HUGE_VALF;
    static constexpr auto HUGE_VALL = ::core::math::HUGE_VALL;
    static constexpr auto INFINITY = ::core::math::INFINITY;
    static constexpr auto NAN = ::core::math::NAN;
    static constexpr auto MATH_ERRHANDLING = ::core::math::MATH_ERRHANDLING;
    static constexpr auto MATH_ERRNO = ::core::math::MATH_ERRNO;
    static constexpr auto MATH_ERREXCEPT = ::core::math::MATH_ERREXCEPT;
    static constexpr auto FP_NORMAL = ::core::math::FP_NORMAL;
    static constexpr auto FP_SUBNORMAL = ::core::math::FP_SUBNORMAL;
    static constexpr auto FP_ZERO = ::core::math::FP_ZERO;
    static constexpr auto FP_INFINITE = ::core::math::FP_INFINITE;
    static constexpr auto FP_NAN = ::core::math::FP_NAN;

    static constexpr auto RAND_MAX = ::core::random::RAND_MAX;

    static constexpr auto FE_ALL_EXCEPT = ::core::math::FE_ALL_EXCEPT;
    static constexpr auto FE_DIVBYZERO = ::core::math::FE_DIVBYZERO;
    static constexpr auto FE_INEXACT = ::core::math::FE_INEXACT;
    static constexpr auto FE_INVALID = ::core::math::FE_INVALID;
    static constexpr auto FE_OVERFLOW = ::core::math::FE_OVERFLOW;
    static constexpr auto FE_UNDERFLOW = ::core::math::FE_UNDERFLOW;

    static constexpr auto FE_TONEAREST = ::core::math::FE_TONEAREST;
    static constexpr auto FE_DOWNWARD = ::core::math::FE_DOWNWARD;
    static constexpr auto FE_UPWARD = ::core::math::FE_UPWARD;
    static constexpr auto FE_TOWARDZERO = ::core::math::FE_TOWARDZERO;

    static inline const auto FE_DFL_ENV = ::core::math::FE_DFL_ENV;

    #ifndef STDLIBX_NO_STD_ALLOC_FEATURES
    static constexpr usize HARDWARE_CONSTRUCTIVE_INTERFERENCE_SIZE = ::alloc::HARDWARE_CONSTRUCTIVE_INTERFERENCE_SIZE;
    static constexpr usize HARDWARE_DESTRUCTIVE_INTERFERENCE_SIZE = ::alloc::HARDWARE_DESTRUCTIVE_INTERFERENCE_SIZE;
    #endif
private:
    #ifndef STDLIBX_NO_STD_CORE_FEATURES
    class GlobalInputStream {
    public:
        [[nodiscard]]
        virtual operator const InputStream&() const noexcept = 0;

        [[nodiscard]]
        virtual operator File::Handle*() const noexcept = 0;

        [[nodiscard]]
        virtual String readln() const = 0;

        [[nodiscard]]
        virtual String readln(StringView prompt) const = 0;

        virtual int sync() const noexcept = 0;
    };

    class GlobalOutputStream {
    public:
        [[nodiscard]]
        virtual operator const OutputStream&() const noexcept = 0;

        [[nodiscard]]
        virtual operator File::Handle*() const noexcept = 0;

        virtual void flush() const = 0;
    };
    #endif
public:
    System() = delete;

    [[noreturn]]
    static void abort() noexcept {
        ::core::sys::abort();
    }

    [[noreturn]]
    static void exit(i32 status) noexcept {
        ::core::sys::exit(status);
    }

    [[noreturn]]
    static void quick_exit(i32 status) noexcept {
        ::core::sys::quick_exit(status);
    }

    [[noreturn]]
    static void immediate_exit(i32 status) noexcept {
        ::core::sys::_Exit(status);
    }

    static i32 system_command(StringView command) noexcept {
        return ::core::sys::system(command.data());
    }

    #ifndef STDLIBX_NO_STD_CORE_FEATURES
    class In final: public GlobalInputStream {
    private:
        inline static const InputStream& in = Cin;

        friend class System;
        In() noexcept = default;
        In(const In&) = delete;
        In& operator=(const In&) = delete;
    public:
        [[nodiscard]]
        operator const InputStream&() const noexcept override {
            return in;
        }

        [[nodiscard]]
        operator InputStream&() const noexcept {
            return const_cast<InputStream&>(in);
        }

        [[nodiscard]]
        operator File::Handle*() const noexcept override {
            return File::stdin();
        }

        [[nodiscard]]
        String readln() const override {
            String line;
            stdx::io::getline(const_cast<InputStream&>(in), line);
            return line;
        }

        [[nodiscard]]
        String readln(StringView prompt) const override {
            stdx::io::print(Cout, "{}", prompt);
            return readln();
        }

        int sync() const noexcept override {
            return Cin.sync();
        }
    };

    class Out final: public GlobalOutputStream {
    private:
        inline static const OutputStream& out = Cout;

        friend class System;
        Out() noexcept = default;
        Out(const Out&) = delete;
        Out& operator=(const Out&) = delete;
    public:
        [[nodiscard]]
        operator const OutputStream&() const noexcept override {
            return out;
        }

        [[nodiscard]]
        operator File::Handle*() const noexcept override {
            return File::stdout();
        }

        template <typename... Args>
        void print(FormatString<Args...> fmt, Args&&... args) const {
            stdx::io::print(Cout, fmt, Ops::forward<Args>(args)...);
        }

        void printf(StringView fmt) const {
            stdx::io::printf(Cout, fmt);
        }

        template <typename... Args>
        void printf(TypeIdentityType<PrintfString<Args...>> fmt, Args&&... args) const {
            stdx::io::printf(Cout, fmt, Ops::forward<Args>(args)...);
        }

        template <typename... Args>
        void println(FormatString<Args...> fmt, Args&&... args) const {
            stdx::io::println(Cout, fmt, Ops::forward<Args>(args)...);
        }

        template <typename T>
        void print(T&& x) const {
            stdx::io::print(Cout, Ops::forward<T>(x));
        }

        template <typename T>
            requires (!IsConvertibleValue<T, StringView>)
        void printf(T&& x) const {
            stdx::io::printf(Cout, Ops::forward<T>(x));
        }

        void println() const {
            stdx::io::println(Cout);
        }

        template <typename T>
        void println(T&& x) const {
            stdx::io::println(Cout, Ops::forward<T>(x));
        }

        void flush() const override {
            Cout.flush();
        }
    };

    class Err final: public GlobalOutputStream {
    private:
        inline static const OutputStream& err = Cerr;

        friend class System;
        Err() noexcept = default;
        Err(const Err&) = delete;
        Err& operator=(const Err&) = delete;
    public:
        [[nodiscard]]
        operator const OutputStream&() const noexcept override {
            return err;
        }

        [[nodiscard]]
        operator File::Handle*() const noexcept override {
            return File::stderr();
        }

        template <typename... Args>
        void print(FormatString<Args...> fmt, Args&&... args) const {
            stdx::io::print(Cerr, fmt, forward<Args>(args)...);
        }

        void printf(StringView fmt) const {
            stdx::io::printf(Cerr, fmt);
        }

        template <typename... Args>
        void printf(TypeIdentityType<PrintfString<Args...>> fmt, Args&&... args) const {
            stdx::io::printf(Cerr, fmt, Ops::forward<Args>(args)...);
        }

        template <typename... Args>
        void println(FormatString<Args...> fmt, Args&&... args) const {
            stdx::io::println(Cerr, fmt, Ops::forward<Args>(args)...);
        }

        template <typename T>
        void print(T&& x) const {
            stdx::io::print(Cerr, Ops::forward<T>(x));
        }

        template <typename T>
            requires (!IsConvertibleValue<T, StringView>)
        void printf(T&& x) const {
            stdx::io::printf(Cerr, Ops::forward<T>(x));
        }

        void println() const {
            stdx::io::println(Cerr);
        }

        template <typename T>
        void println(T&& x) const {
            stdx::io::println(Cerr, Ops::forward<T>(x));
        }

        void flush() const override {
            Cerr.flush();
        }
    };

    class Log final: public GlobalOutputStream {
    private:
        inline static const OutputStream& log = Clog;

        friend class System;
        Log() noexcept = default;
        Log(const Log&) = delete;
        Log& operator=(const Log&) = delete;
    public:
        [[nodiscard]]
        operator const OutputStream&() const noexcept override {
            return log;
        }

        [[nodiscard]]
        operator File::Handle*() const noexcept override {
            return File::stderr();
        }

        template <typename... Args>
        void print(FormatString<Args...> fmt, Args&&... args) const {
            stdx::io::print(Clog, fmt, Ops::forward<Args>(args)...);
        }

        void printf(StringView fmt) const {
            stdx::io::printf(Clog, fmt);
        }

        template <typename... Args>
        void printf(TypeIdentityType<PrintfString<Args...>> fmt, Args&&... args) const {
            stdx::io::printf(Clog, fmt, Ops::forward<Args>(args)...);
        }

        template <typename... Args>
        void println(FormatString<Args...> fmt, Args&&... args) const {
            stdx::io::println(Clog, fmt, Ops::forward<Args>(args)...);
        }

        template <typename T>
        void print(T&& x) const {
            stdx::io::print(Clog, Ops::forward<T>(x));
        }

        template <typename T>
            requires (!IsConvertibleValue<T, StringView>)
        void printf(T&& x) const {
            stdx::io::printf(Clog, Ops::forward<T>(x));
        }

        void println() const {
            stdx::io::println(Clog);
        }

        template <typename T>
        void println(T&& x) const {
            stdx::io::println(Clog, Ops::forward<T>(x));
        }

        void flush() const override {
            Clog.flush();
        }
    };

    inline static const In in = {};
    inline static const Out out = {};
    inline static const Err err = {};
    inline static const Log log = {};

    [[nodiscard]]
    static u64 current_time_millis() noexcept {
        return static_cast<u64>(
            stdx::time::duration_cast<Milliseconds>(
                SystemClock::now().time_since_epoch()
            ).count()
        );
    }

    [[nodiscard]]
    static String current_time_as_string() {
        return stdx::time::current_time_as_string();
    }

    [[nodiscard]]
    static u64 nano_time() noexcept {
        return static_cast<u64>(
            stdx::time::duration_cast<Nanoseconds>(
                SystemClock::now().time_since_epoch()
            ).count()
        );
    }

    [[nodiscard]]
    static StringView line_separator() noexcept {
        #ifdef _WIN32
        return "\r\n"sv;
        #else
        return "\n"sv;
        #endif
    }

    [[nodiscard]]
    static HashMap<String, StringView> getenv() {
        HashMap<String, StringView> map;
        #ifdef __unix__
        char** env_list = ::environ;
        #elifdef _WIN32
        char** env_list = ::_environ;
        #endif
        for (char** env = env_list; *env != nullptr; ++env) {
            StringView entry(*env);
            if (auto pos = entry.find('='); pos != StringView::npos) {
                map.emplace(String(entry.substr(0, pos)), entry.substr(pos + 1));
            }
        }
        return map;
    }

    [[nodiscard]]
    static StringView getenv(StringView var) noexcept {
        return ::core::sys::getenv(var.data());
    }
    #endif
};
