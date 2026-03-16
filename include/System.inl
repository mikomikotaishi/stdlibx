#pragma once

class System final {
public:
    static constexpr auto EXIT_SUCCESS = ::core::prelude::EXIT_SUCCESS;
    static constexpr auto EXIT_FAILURE = ::core::prelude::EXIT_FAILURE;
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

    template <typename T>
    static constexpr void swap(T& a, T& b) {
        ::core::util::swap(a, b);
    }

    template <typename T, usize N>
    static constexpr void swap(T (&a)[N], T (&b)[N]) {
        ::core::util::swap(a, b);
    }

    template <typename T>
    static constexpr auto size(const T& x) {
        return std::size(x);
    }

    template <typename T>
    static constexpr auto ssize(const T& x) {
        return std::ssize(x);
    }

    template <typename T, usize N>
    [[nodiscard]]
    static constexpr usize size(const T (&a)[N]) noexcept {
        return N;
    }

    template <typename T, isize N>
    [[nodiscard]]
    static constexpr isize ssize(const T (&a)[N]) noexcept {
        return N;
    }

    template <typename T>
    [[nodiscard]]
    static constexpr decltype(auto) forward(RemoveReferenceType<T>& t) noexcept {
        return ::core::util::forward<T>(t);
    }

    template <typename T>
    [[nodiscard]]
    static constexpr decltype(auto) forward(RemoveReferenceType<T>&& t) noexcept {
        return ::core::util::forward<T>(t);
    }

    template <typename T, typename U = T>
    static constexpr decltype(auto) exchange(T& obj, U&& new_value)
        noexcept(IsNothrowMoveConstructibleValue<T> && IsNothrowAssignableValue<T&, U>){
        return ::core::util::exchange(obj, forward<U>(new_value));
    }

    template <typename T>
    [[nodiscard]]
    static constexpr decltype(auto) in_range(T t) noexcept {
        return ::core::util::in_range(t);
    }

    #ifdef __cpp_lib_forward_like
    template <typename T, typename U>
    [[nodiscard]]
    static constexpr decltype(auto) forward_like(U&& u) noexcept {
        return ::core::util::forward_like<T, U>(u);
    }
    #endif

    template <typename E>
    [[nodiscard]]
    static constexpr decltype(auto) to_underlying(E e) noexcept {
        return ::core::util::to_underlying(e);
    }

    template <typename T>
    [[nodiscard]]
    static constexpr decltype(auto) move(T&& t) noexcept {
        return ::core::util::move(t);
    }

    template <typename T>
    [[nodiscard]]
    static constexpr decltype(auto) move_if_noexcept(T&& t) noexcept {
        return ::core::util::move_if_noexcept(t);
    }

    template <typename T>
    [[nodiscard]]
    static constexpr decltype(auto) as_const(T& t) noexcept {
        return ::core::util::as_const(t);
    }

    template <typename T>
    static void as_const(const T&&) = delete;

    template <typename T>
    [[nodiscard]]
    static AddRvalueReferenceType<T> declval() noexcept {
        return ::core::util::declval<T>();
    }

    [[noreturn]]
    static void unreachable() noexcept {
        ::core::sys::unreachable();
    }

    template <typename F, typename... Args>
    static constexpr decltype(auto) bind(F&& f, Args&&... args) {
        return ::core::prelude::bind(forward<F>(f), forward<Args>(args)...);
    }

    template <typename T>
    static decltype(auto) ref(T& t) noexcept {
        return ::core::prelude::ref(t);
    }

    template <typename T>
    static decltype(auto) ref(ReferenceWrapper<T> t) noexcept {
        return ::core::prelude::ref(t);
    }

    template <typename T>
    static void ref(const T&&) = delete;

    template <typename T>
    static decltype(auto) cref(const T& t) noexcept {
        return ::core::prelude::cref(t);
    }

    template <typename T>
    static decltype(auto) cref(ReferenceWrapper<const T> t) noexcept {
        return ::core::prelude::cref(t);
    }

    template <typename T>
    static void cref(const T&&) = delete;

    template <typename F, typename... Args>
    static constexpr decltype(auto) invoke(F&& f, Args&&... args)
        noexcept(IsNothrowInvocableValue<F, Args...>) {
        return ::core::prelude::invoke(forward<F>(f), forward<Args>(args)...);
    }

    template <typename R, typename F, typename... Args>
    static constexpr decltype(auto) invoke_r(F&& f, Args&&... args)
        noexcept(IsNothrowInvocableReturnValue<R, F, Args...>) {
        return ::core::prelude::invoke_r<R>(forward<F>(f), forward<Args>(args)...);
    }

    template <usize I, typename T>
    [[nodiscard]]
    static constexpr decltype(auto) get(T&& t) noexcept {
        return ::core::prelude::get<I>(forward<T>(t));
    }

    template <typename T, typename V>
    [[nodiscard]]
    static constexpr decltype(auto) get(V&& v) {
        return ::core::prelude::get<T>(forward<V>(v));
    }

    template <usize I, typename... Ts>
    [[nodiscard]]
    static constexpr decltype(auto) get_if(Variant<Ts...>* v) noexcept {
        return ::core::prelude::get_if<I>(v);
    }

    template <usize I, typename... Ts>
    [[nodiscard]]
    static constexpr decltype(auto) get_if(const Variant<Ts...>* v) noexcept {
        return ::core::prelude::get_if<I>(v);
    }

    template <typename T, typename... Ts>
    [[nodiscard]]
    static constexpr decltype(auto) get_if(Variant<Ts...>* v) noexcept {
        return ::core::prelude::get_if<T>(v);
    }

    template <typename T, typename... Ts>
    [[nodiscard]]
    static constexpr decltype(auto) get_if(const Variant<Ts...>* v) noexcept {
        return ::core::prelude::get_if<T>(v);
    }

    template <typename Vst, typename... Vars>
    static constexpr decltype(auto) visit(Vst&& v, Vars&&... vars) {
        return ::core::prelude::visit(forward<Vst>(v), forward<Vars>(vars)...);
    }

    template <typename R, typename Vst, typename... Vars>
    static constexpr R visit(Vst&& v, Vars&&... vars) {
        return ::core::prelude::visit(forward<Vst>(v), forward<Vars>(vars)...);
    }

    template <typename F, typename Tpl>
    static constexpr decltype(auto) apply(F&& f, Tpl&& tpl)
        noexcept(noexcept(::core::prelude::apply(forward<F>(f), forward<Tpl>(tpl)))) {
        return ::core::prelude::apply(forward<F>(f), forward<Tpl>(tpl));
    }

    [[nodiscard]]
    static const ErrorCategory& generic_category() noexcept {
        return ::core::prelude::generic_category();
    }

    [[nodiscard]]
    static const ErrorCategory& system_category() noexcept {
        return ::core::prelude::system_category();
    }

    #ifndef STDLIBX_NO_STD_CORE_FEATURES
    [[nodiscard]]
    static const ErrorCategory& future_category() noexcept {
        return ::stdx::future::future_category();
    }
    #endif

    [[nodiscard]]
    static ErrorCode error_code(Errc e) noexcept {
        return ::core::prelude::make_error_code(e);
    }

    [[nodiscard]]
    static ErrorCode error_code(IOErrc e) noexcept {
        return ::core::prelude::make_error_code(e);
    }

    #ifndef STDLIBX_NO_STD_CORE_FEATURES
    [[nodiscard]]
    static ErrorCode error_code(FutureErrc e) noexcept {
        return ::stdx::future::make_error_code(e);
    }
    #endif

    [[nodiscard]]
    static ErrorCondition error_condition(Errc e) noexcept {
        return ::core::prelude::make_error_condition(e);
    }

    [[nodiscard]]
    static ErrorCondition error_condition(IOErrc e) noexcept {
        return ::core::prelude::make_error_condition(e);
    }

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

    template <typename T>
    [[nodiscard]]
    static constexpr decltype(auto) optional(T&& value) noexcept {
        return ::core::prelude::make_optional(forward<T>(value));
    }

    template <typename T, typename... Args>
    [[nodiscard]]
    static constexpr decltype(auto) optional(Args&&... args) noexcept {
        return ::core::prelude::make_optional<T>(forward<Args>(args)...);
    }

    template <typename T, typename U, typename... Args>
    [[nodiscard]]
    static constexpr decltype(auto) optional(InitializerList<U> il, Args&&... args) noexcept {
        return ::core::prelude::make_optional<T>(il, forward<Args>(args)...);
    }

    template <typename... Ts>
    [[nodiscard]]
    static constexpr decltype(auto) tie(Ts&... args) noexcept {
        return ::core::prelude::tie(args...);
    }

    template <typename... Ts>
    [[nodiscard]]
    static constexpr decltype(auto) tuple(Ts&&... args) {
        return ::core::prelude::make_tuple(forward<Ts>(args)...);
    }

    template <typename... Ts>
    [[nodiscard]]
    static constexpr decltype(auto) forward_as_tuple(Ts&&... args) noexcept {
        return ::core::prelude::forward_as_tuple(forward<Ts>(args)...);
    }

    template <typename... Tpls>
    [[nodiscard]]
    static constexpr decltype(auto) tuple_cat(Tpls&&... tpls) {
        return ::core::prelude::tuple_cat(forward<Tpls>(tpls)...);
    }

    template <typename T, typename Tpl>
    [[nodiscard]]
    static constexpr decltype(auto) from_tuple(Tpl&& tpl) {
        return ::core::prelude::make_from_tuple<T>(forward<Tpl>(tpl));
    }

    template <typename T, typename U>
    [[nodiscard]]
    static constexpr decltype(auto) pair(T&& x, U&& y) noexcept {
        return ::core::prelude::make_pair(forward<T>(x), forward<U>(y));
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
            stdx::io::print(Cout, fmt, forward<Args>(args)...);
        }

        void printf(StringView fmt) const {
            stdx::io::printf(Cout, fmt);
        }

        template <typename... Args>
        void printf(TypeIdentityType<PrintfString<Args...>> fmt, Args&&... args) const {
            stdx::io::printf(Cout, fmt, forward<Args>(args)...);
        }

        template <typename... Args>
        void println(FormatString<Args...> fmt, Args&&... args) const {
            stdx::io::println(Cout, fmt, forward<Args>(args)...);
        }

        template <typename T>
        void print(T&& x) const {
            stdx::io::print(Cout, forward<T>(x));
        }

        template <typename T>
            requires (!IsConvertibleValue<T, StringView>)
        void printf(T&& x) const {
            stdx::io::printf(Cout, forward<T>(x));
        }

        void println() const {
            stdx::io::println(Cout);
        }

        template <typename T>
        void println(T&& x) const {
            stdx::io::println(Cout, forward<T>(x));
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
            stdx::io::printf(Cerr, fmt, forward<Args>(args)...);
        }

        template <typename... Args>
        void println(FormatString<Args...> fmt, Args&&... args) const {
            stdx::io::println(Cerr, fmt, forward<Args>(args)...);
        }

        template <typename T>
        void print(T&& x) const {
            stdx::io::print(Cerr, forward<T>(x));
        }

        template <typename T>
            requires (!IsConvertibleValue<T, StringView>)
        void printf(T&& x) const {
            stdx::io::printf(Cerr, forward<T>(x));
        }

        void println() const {
            stdx::io::println(Cerr);
        }

        template <typename T>
        void println(T&& x) const {
            stdx::io::println(Cerr, forward<T>(x));
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
            stdx::io::print(Clog, fmt, forward<Args>(args)...);
        }

        void printf(StringView fmt) const {
            stdx::io::printf(Clog, fmt);
        }

        template <typename... Args>
        void printf(TypeIdentityType<PrintfString<Args...>> fmt, Args&&... args) const {
            stdx::io::printf(Clog, fmt, forward<Args>(args)...);
        }

        template <typename... Args>
        void println(FormatString<Args...> fmt, Args&&... args) const {
            stdx::io::println(Clog, fmt, forward<Args>(args)...);
        }

        template <typename T>
        void print(T&& x) const {
            stdx::io::print(Clog, forward<T>(x));
        }

        template <typename T>
            requires (!IsConvertibleValue<T, StringView>)
        void printf(T&& x) const {
            stdx::io::printf(Clog, forward<T>(x));
        }

        void println() const {
            stdx::io::println(Clog);
        }

        template <typename T>
        void println(T&& x) const {
            stdx::io::println(Clog, forward<T>(x));
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

    template <typename F, typename... Args>
    static auto async(F&& f, Args&&... args) {
        return ::stdx::future::async(forward<F>(f), forward<Args>(args)...);
    }

    template <typename F, typename... Args>
    static auto async(LaunchPolicy policy, F&& f, Args&&... args) {
        return ::stdx::future::async(policy, forward<F>(f), forward<Args>(args)...);
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
