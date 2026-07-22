#pragma once

using stdx::collections::HashMap;
using stdx::io::Cerr;
using stdx::io::Cin;
using stdx::io::Clog;
using stdx::io::Cout;
using stdx::io::IOErrc;
using stdx::io::InputStream;
using stdx::io::File;
using stdx::io::OutputStream;
using stdx::meta::AddRvalueReferenceType;
using stdx::meta::IsConvertibleValue;
using stdx::meta::IsNothrowAssignableValue;
using stdx::meta::IsNothrowInvocableReturnValue;
using stdx::meta::IsNothrowInvocableValue;
using stdx::meta::IsNothrowMoveConstructibleValue;
using stdx::meta::RemoveReferenceType;
using stdx::meta::TypeIdentityType;
using stdx::time::LocalTime;
using stdx::time::Milliseconds;
using stdx::time::Nanoseconds;
using stdx::time::Seconds;
using stdx::time::SteadyClock;
using stdx::time::SystemClock;

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    /**
     * @class System
     * @brief Utility class for system-level operations, constants, and global streams.
     */
    class System final {
    public:
        static constexpr auto EXIT_SUCCESS = ::stdx::core::EXIT_SUCCESS;
        static constexpr auto EXIT_FAILURE = ::stdx::core::EXIT_FAILURE;

        static constexpr auto HUGE_VAL = ::stdx::core::HUGE_VAL;
        static constexpr auto HUGE_VALF = ::stdx::core::HUGE_VALF;
        static constexpr auto HUGE_VALL = ::stdx::core::HUGE_VALL;
        static constexpr auto INFINITY = ::stdx::core::INFINITY;
        static constexpr auto NAN = ::stdx::core::NAN;
        static constexpr auto MATH_ERRHANDLING = ::stdx::core::MATH_ERRHANDLING;
        static constexpr auto MATH_ERRNO = ::stdx::core::MATH_ERRNO;
        static constexpr auto MATH_ERREXCEPT = ::stdx::core::MATH_ERREXCEPT;
        static constexpr auto FP_NORMAL = ::stdx::core::FP_NORMAL;
        static constexpr auto FP_SUBNORMAL = ::stdx::core::FP_SUBNORMAL;
        static constexpr auto FP_ZERO = ::stdx::core::FP_ZERO;
        static constexpr auto FP_INFINITE = ::stdx::core::FP_INFINITE;
        static constexpr auto FP_NAN = ::stdx::core::FP_NAN;

        static constexpr auto RAND_MAX = ::stdx::core::RAND_MAX;

        static constexpr auto FE_ALL_EXCEPT = ::stdx::core::FE_ALL_EXCEPT;
        static constexpr auto FE_DIVBYZERO = ::stdx::core::FE_DIVBYZERO;
        static constexpr auto FE_INEXACT = ::stdx::core::FE_INEXACT;
        static constexpr auto FE_INVALID = ::stdx::core::FE_INVALID;
        static constexpr auto FE_OVERFLOW = ::stdx::core::FE_OVERFLOW;
        static constexpr auto FE_UNDERFLOW = ::stdx::core::FE_UNDERFLOW;

        static constexpr auto FE_TONEAREST = ::stdx::core::FE_TONEAREST;
        static constexpr auto FE_DOWNWARD = ::stdx::core::FE_DOWNWARD;
        static constexpr auto FE_UPWARD = ::stdx::core::FE_UPWARD;
        static constexpr auto FE_TOWARDZERO = ::stdx::core::FE_TOWARDZERO;

        static inline const auto FE_DFL_ENV = ::stdx::core::FE_DFL_ENV;
    private:
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
    public:
        System() = delete("System is a static utility class and cannot be instantiated.");

        class In final: public GlobalInputStream {
        private:
            inline static const InputStream& in = Cin;

            friend class System;
            In() noexcept = default;
            In(const In&) = delete("System::In is not copyable.");
            In& operator=(const In&) = delete("System::In is not copyable.");
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
            Out(const Out&) = delete("System::Out is not copyable.");
            Out& operator=(const Out&) = delete("System::Out is not copyable.");
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
            Err(const Err&) = delete("System::Err is not copyable.");
            Err& operator=(const Err&) = delete("System::Err is not copyable.");
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
            Log(const Log&) = delete("System::Log is not copyable.");
            Log& operator=(const Log&) = delete("System::Log is not copyable.");        public:
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
        static String current_time_formatted() {
            Instant<SystemClock> now = SystemClock::now();
            LocalTime<Seconds> currentTime = stdx::time::current_zone()
                ->to_local(stdx::time::floor<Seconds>(now));
            return Ops::format("{:%Y-%m-%d %H:%M:%S}", currentTime);
        }

        [[nodiscard]]
        static u64 nano_time() noexcept {
            return static_cast<u64>(
                stdx::time::duration_cast<Nanoseconds>(
                    SteadyClock::now().time_since_epoch()
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
    };
}
