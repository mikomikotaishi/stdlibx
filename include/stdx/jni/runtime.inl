#pragma once

/**
 * @namespace stdx::jni
 * @brief Core JNI runtime: reference ownership, compile-time signature
 * construction, environment access and exception translation.
 *
 * Nothing in this file is specific to any Java class. Generated bindings under
 * stdx/jni/java/ build entirely on the primitives declared here.
 *
 * Naming follows the namespace it sits in: everything under stdx:: is
 * snake_case, matching the rest of the library, while everything under java::
 * is camelCase, carrying Java's own names verbatim. The two vocabularies meet
 * only at the call site, where the difference reads as a useful signal about
 * which side of the boundary a name comes from.
 */
export namespace stdx::jni {
    /**
     * @class JavaArray
     * @brief Marker for a Java array type. Never instantiated; it exists so a
     * C++ parameter list can spell `JavaArray<i32>` and have the signature
     * builder emit `[I`.
     *
     * Not named Array: stdx::core::Array is implicitly imported everywhere, so
     * an unqualified Array here would be ambiguous in any translation unit that
     * says `using namespace stdx::jni`.
     * @tparam T The element type, primitive or reference.
     */
    template <typename T>
    class JavaArray {
    public:
        using Element = T;
    };

}

namespace stdx::jni::details {
    template <typename T>
    constexpr bool IsArrayValue = false;

    template <typename T>
    constexpr bool IsArrayValue<::stdx::jni::JavaArray<T>> = true;

    /**
     * @struct Wire
     * @tparam T The stdx-facing type.
     * @brief Maps an stdx-facing type onto its JNI wire type and its
     * one-character field descriptor.
     *
     * Bindings are written entirely in stdx types: a method returns i32, never
     * jint. This trait is the only place the two vocabularies meet.
     *
     * Keying on the stdx types rather than the jni.h typedefs is deliberate.
     * They happen to be the same types on 64-bit Linux, so either spelling
     * would compile here, but that is a platform coincidence: jni_md.h picks
     * its integer typedefs per platform, and f32/f64 are already distinct from
     * jfloat/jdouble under GCC, which defines __STDCPP_FLOAT32_T__ and
     * __STDCPP_FLOAT64_T__. Anchoring on the stdx side makes the descriptor a
     * property of the declared API rather than of the host's typedefs.
     *
     * The primary template covers reference types, which all travel as jobject.
     * @tparam T The stdx-facing type.
     */
    template <typename T>
    struct Wire {
        using Type = ::jobject;
        static constexpr char TAG = '\0';
    };

    template <>
    struct Wire<void> {
        using Type = void;
        static constexpr char TAG = 'V';
        static constexpr const char* BOX = "java/lang/Void";
    };

    /**
     * @brief Java boolean. Modelled as bool rather than u8, which on this
     * platform is jbyte's neighbour and would read as a numeric type.
     */
    template <>
    struct Wire<bool> {
        using Type = ::jboolean;
        static constexpr char TAG = 'Z';
        static constexpr const char* BOX = "java/lang/Boolean";
    };

    template <>
    struct Wire<i8> {
        using Type = ::jbyte;
        static constexpr char TAG = 'B';
        static constexpr const char* BOX = "java/lang/Byte";
    };

    /**
     * @brief Java char is a UTF-16 code unit, so char16 carries it, not u16.
     */
    template <>
    struct Wire<char16> {
        using Type = ::jchar;
        static constexpr char TAG = 'C';
        static constexpr const char* BOX = "java/lang/Character";
    };

    template <>
    struct Wire<i16> {
        using Type = ::jshort;
        static constexpr char TAG = 'S';
        static constexpr const char* BOX = "java/lang/Short";
    };

    template <>
    struct Wire<i32> {
        using Type = ::jint;
        static constexpr char TAG = 'I';
        static constexpr const char* BOX = "java/lang/Integer";
    };

    template <>
    struct Wire<i64> {
        using Type = ::jlong;
        static constexpr char TAG = 'J';
        static constexpr const char* BOX = "java/lang/Long";
    };

    template <>
    struct Wire<f32> {
        using Type = ::jfloat;
        static constexpr char TAG = 'F';
        static constexpr const char* BOX = "java/lang/Float";
    };

    template <>
    struct Wire<f64> {
        using Type = ::jdouble;
        static constexpr char TAG = 'D';
        static constexpr const char* BOX = "java/lang/Double";
    };

    /**
     * @concept IsPrimitive
     * @brief Satisfied by the eight Java primitive types and void.
     * @tparam T The type to check.
     */
    template <typename T>
    concept IsPrimitive = Wire<T>::TAG != '\0';

    /**
     * @brief Converts an stdx value to its JNI wire type on the way into a call.
     * @tparam T The type to convert.
     * @param value The stdx value to convert.
     * @return The converted JNI wire value.
     *
     * Every such conversion is implicit and lossless in both directions, f64 to
     * jdouble included; the cast is spelled out only so the boundary stays
     * quiet under -Wconversion.
     */
    template <typename T>
    [[nodiscard]]
    constexpr typename Wire<T>::Type to_wire(T value) noexcept {
        return static_cast<typename Wire<T>::Type>(value);
    }

    /**
     * @brief Converts a JNI wire value back to its stdx type on the way out.
     * @tparam T The type to convert to.
     * @param value The JNI wire value to convert.
     * @return The converted stdx value.
     */
    template <typename T>
    [[nodiscard]]
    constexpr T from_wire(typename Wire<T>::Type value) noexcept {
        return static_cast<T>(value);
    }

    /**
     * @concept IsArray
     * @brief Satisfied by JavaArray specializations.
     * @tparam T The type to check.
     */
    template <typename T>
    concept IsArray = IsArrayValue<T>;

    /**
     * @concept IsReference
     * @brief Satisfied by generated binding classes, which each declare their
     * own binary name.
     * @tparam T The type to check.
     */
    template <typename T>
    concept IsReference = requires {
        T::className();
    };

    /**
     * @concept IsJavaType
     * @brief Satisfied by anything nameable in a Java signature.
     *
     * Types outside this set are the ones Java has no counterpart for: u16, u32,
     * f128, plain char, and so on. Checking it up front turns those into one
     * readable error instead of a failed lookup deep inside the descriptor
     * machinery.
     */
    template <typename T>
    concept IsJavaType = IsPrimitive<T> || IsArray<T> || IsReference<T>;

    /**
     * @brief Exact descriptor length of @p T in bytes, so the buffer below can
     * be sized precisely rather than padded to a guessed capacity.
     * @tparam T The type to measure.
     * @return The number of bytes the descriptor takes, not counting a NUL.
     */
    template <typename T>
    consteval usize descriptor_length() noexcept {
        static_assert(IsJavaType<T>,
            "type has no Java counterpart: use bool, i8, char16, i16, i32, i64, "
            "f32, f64, void, an JavaArray, or a generated binding class");

        if constexpr (IsPrimitive<T>) {
            return 1;
        } else if constexpr (IsArray<T>) {
            return 1 + descriptor_length<typename T::Element>();
        } else {
            // L + binary name + ;
            return T::className().size() + 2;
        }
    }

    /**
     * @struct SignatureBuffer
     * @brief Fixed compile-time byte buffer that descriptor fragments append
     * into. Sized exactly by descriptor_length, so no trimming pass is needed.
     * @tparam Capacity Total bytes including the terminating NUL.
     */
    template <usize Capacity>
    struct SignatureBuffer {
        char data[Capacity] = {};
        usize length = 0;

        consteval void append(char c) noexcept {
            data[length++] = c;
        }

        consteval void append(StringView text) noexcept {
            for (char c: text) {
                data[length++] = c;
            }
        }
    };

    /**
     * @brief Appends the field descriptor of @p T to @p buffer.
     * @tparam T The type to describe.
     * @param buffer The compile-time buffer to append into.
     *
     * Recursive on the element type for arrays, so `JavaArray<JavaArray<jint>>` yields
     * `[[I`. Reference types delegate to the binding's className(), which each
     * generated class declares as a static consteval member.
     */
    template <typename T>
    consteval void append_descriptor(auto& buffer) noexcept {
        if constexpr (IsPrimitive<T>) {
            buffer.append(Wire<T>::TAG);
        } else if constexpr (IsArray<T>) {
            buffer.append('[');
            append_descriptor<typename T::Element>(buffer);
        } else {
            buffer.append('L');
            buffer.append(T::className());
            buffer.append(';');
        }
    }
}

export namespace stdx::jni {
    /**
     * @struct Signature
     * @brief The JNI method signature for a call returning @p Return and taking
     * @p Params, materialised at compile time as a NUL-terminated string.
     * @tparam Return The return type.
     * @tparam Params The parameter types.
     *
     * `SignatureOf<jlong>` is "()J"; `SignatureOf<void, jint, String>` is
     * "(ILjava/lang/String;)V". Generated bindings pass this straight to
     * GetMethodID, so no descriptor string is ever built at runtime.
     */
    template <typename Return, typename... Params>
    struct Signature {
        static constexpr auto STORAGE = [] consteval {
            constexpr usize size = 2
                + (details::descriptor_length<Params>() + ... + 0)
                + details::descriptor_length<Return>();

            details::SignatureBuffer<size + 1> buffer{};
            buffer.append('(');
            (details::append_descriptor<Params>(buffer), ...);
            buffer.append(')');
            details::append_descriptor<Return>(buffer);
            return buffer;
        }();

        static constexpr const char* VALUE = STORAGE.data;
    };

    template <typename Return, typename... Params>
    constexpr const char* SignatureOf = Signature<Return, Params...>::VALUE;

    /**
     * @struct ClassName
     * @brief The binary name of @p T as a NUL-terminated string, for FindClass.
     * @tparam T The type to name.
     *
     * className() returns a StringView, whose data() carries no guarantee of a
     * terminator. In practice every generated binding builds it from a literal
     * and so happens to be terminated, but FindClass takes a C string and a
     * binding that ever computed its name would break silently. Copying into a
     * zero-initialised compile-time buffer makes the terminator a fact rather
     * than a coincidence, at no runtime cost.
     */
    template <typename T>
    struct ClassName {
        static constexpr auto STORAGE = [] consteval {
            if constexpr (details::IsArray<T>) {
                // FindClass takes the *descriptor* for an array type -
                // "[Ljava/lang/String;", not a binary name.
                details::SignatureBuffer<details::descriptor_length<T>() + 1> buffer{};
                details::append_descriptor<T>(buffer);
                return buffer;
            } else {
                // ...but the binary name for a class - "java/lang/String",
                // with no leading L and no trailing semicolon.
                details::SignatureBuffer<T::className().size() + 1> buffer{};
                buffer.append(T::className());
                return buffer;
            }
        }();

        static constexpr const char* VALUE = STORAGE.data;
    };

    template <typename T>
    constexpr const char* ClassNameOf = ClassName<T>::VALUE;

    /**
     * @class JniException
     * @brief Raised for failures of the JNI layer itself: no VM, thread attach
     * refused, class or member not found. A pending Java throwable is
     * translated separately, into the generated java::lang::Throwable tree.
     * @extends Exception
     */
    class JniException: public Exception {
    public:
        using Exception::Exception;
    };

    /**
     * @class JavaException
     * @brief A Java throwable that crossed back into C++.
     * @extends JniException
     *
     * Carries the result of the Java side's toString(), which is the closest
     * thing to a message that every Throwable is guaranteed to have. The
     * throwable itself is not retained: holding it would need a global
     * reference whose lifetime outlives the frame that raised it, and the
     * common case only ever reads the text.
     */
    class JavaException: public JniException {
    public:
        using JniException::JniException;
    };

    /**
     * @class Env
     * @brief Borrowed JNIEnv for the calling thread.
     *
     * @warning a JNIEnv is valid only for one thread, and a stdx::exec Task may
     * resume on a thread other than the one it suspended on. Never store an Env
     * in a coroutine frame, a class member, or anything that outlives a single
     * synchronous run of native code; call current() again after every suspend
     * point. The object is deliberately tiny and non-owning so that re-fetching
     * costs nothing worth optimising away.
     */
    class Env final {
    private:
        ::JNIEnv* env = nullptr;

        explicit Env(::JNIEnv* e) noexcept:
            env{e} {}
    public:
        /**
         * @brief The JNIEnv for the calling thread, attaching the thread to the
         * VM if it is not already attached.
         * @throws JniException if no VM has been created, or the attach fails.
         */
        [[nodiscard]]
        THROWS(JniException)
        static Env current();

        /**
         * @brief The JNIEnv for the calling thread, or nullptr if there is no
         * VM or the thread is not attached.
         *
         * For destructors and other noexcept paths, which must not throw just
         * because the VM has already gone away.
         */
        [[nodiscard]]
        static ::JNIEnv* raw_current() noexcept;

        [[nodiscard]]
        ::JNIEnv* handle() const noexcept {
            return env;
        }

        ::JNIEnv* operator->() const noexcept {
            return env;
        }

        /**
         * @brief Rethrows any pending Java exception as a C++ exception and
         * clears it. Generated bindings call this after every JNI call that can
         * raise; skipping it leaves the next JNI call undefined.
         */
        THROWS(JniException)
        void check_pending() const;
    };

    template <typename T>
    class Global;

    /**
     * @class Ref
     * @brief Non-owning typed view over a jobject.
     * @tparam T The binding class this handle refers to.
     *
     * This is what generated methods take as parameters and what `this` is
     * inside them. It never frees anything, so its validity is exactly the
     * validity of the Local, Global or incoming JNI argument it was taken from.
     */
    template <typename T>
    class Ref {
    private:
        ::jobject object = nullptr;
    public:
        Ref() = default;

        explicit Ref(::jobject h) noexcept:
            object{h} {}

        /**
         * @brief Widens to a base-class handle. Java single inheritance means
         * this is always a no-op at the JNI level; only the C++ type changes.
         */
        template <typename Base>
            requires __is_base_of(Base, T)
        operator Ref<Base>() const noexcept {
            return Ref<Base>(object);
        }

        [[nodiscard]]
        ::jobject handle() const noexcept {
            return object;
        }

        explicit operator bool() const noexcept {
            return object != nullptr;
        }

        /**
         * @class Arrow
         * @brief Materialises the binding class over the borrowed handle, so
         * `ref->method()` reads like Java.
         *
         * operator-> must yield something with its own operator->, so the
         * binding is built into this proxy and the chain resolves to it. The
         * proxy lives for the full-expression of the call, which is exactly as
         * long as the receiver is needed.
         */
        class Arrow {
        private:
            T value;
        public:
            explicit Arrow(::jobject handle) noexcept:
                value{handle} {}

            T* operator->() noexcept {
                return &value;
            }
        };

        /**
         * @brief Dispatches an instance method against the referenced object.
         */
        [[nodiscard]]
        Arrow operator->() const noexcept {
            return Arrow(object);
        }
    };

    /**
     * @class Local
     * @brief Owns a JNI local reference and deletes it on scope exit.
     * @tparam T The binding class this handle refers to.
     *
     * Every JNI call returning an object hands back a local reference, and the
     * guaranteed capacity is only around sixteen slots without an explicit
     * frame. Generated bindings therefore always return Local, never a raw
     * jobject. Local references belong to the frame that created them, so a
     * Local must not outlive its LocalFrame.
     */
    template <typename T>
    class Local {
    private:
        ::jobject object = nullptr;
    public:
        Local() = default;

        /**
         * @brief Adopts an existing local reference. Does not create a new one.
         */
        explicit Local(::jobject h) noexcept:
            object{h} {}

        Local(const Local&) = delete;
        Local& operator=(const Local&) = delete;

        Local(Local&& other) noexcept:
            object(Ops::exchange(other.object, nullptr)) {}

        Local& operator=(Local&& other) noexcept {
            if (this != &other) {
                reset();
                object = Ops::exchange(other.object, nullptr);
            }
            return *this;
        }

        ~Local() {
            reset();
        }

        /**
         * @brief Decays to a borrowed handle, widening to any base at the same
         * time. The base case matters: a parameter typed Ref<Object> has to
         * accept a Local<String> in one step, because Local to Ref to base Ref
         * would be two user-defined conversions.
         * @tparam Base The base class to widen to.
         */
        template <typename Base>
            requires __is_base_of(Base, T)
        operator Ref<Base>() const noexcept {
            return Ref<Base>(object);
        }

        [[nodiscard]]
        Ref<T> get() const noexcept {
            return Ref<T>(object);
        }

        [[nodiscard]]
        ::jobject handle() const noexcept {
            return object;
        }

        explicit operator bool() const noexcept {
            return object != nullptr;
        }

        /**
         * @brief Relinquishes ownership without deleting the reference.
         */
        [[nodiscard]]
        ::jobject release() noexcept {
            return Ops::exchange(object, nullptr);
        }

        void reset() noexcept;

        /**
         * @brief Promotes to a global reference, which survives the current
         * frame and may cross threads.
         */
        [[nodiscard]]
        THROWS(JniException)
        Global<T> global() const;

        /**
         * @class Arrow
         * @brief Materialises the binding class over the borrowed handle, so
         * `ref->method()` reads like Java.
         *
         * operator-> must yield something with its own operator->, so the
         * binding is built into this proxy and the chain resolves to it. The
         * proxy lives for the full-expression of the call, which is exactly as
         * long as the receiver is needed.
         */
        class Arrow {
        private:
            T value;
        public:
            explicit Arrow(::jobject handle) noexcept:
                value{handle} {}

            T* operator->() noexcept {
                return &value;
            }
        };

        /**
         * @brief Dispatches an instance method against the referenced object.
         */
        [[nodiscard]]
        Arrow operator->() const noexcept {
            return Arrow(object);
        }
    };

    /**
     * @class Global
     * @brief Owns a JNI global reference: frame-independent, thread-independent
     * and copyable. Use for anything cached across calls, such as class handles
     * or a long-lived object held by a C++ type.
     * @tparam T The binding class this handle refers to.
     */
    template <typename T>
    class Global {
    private:
        ::jobject object = nullptr;
    public:
        Global() = default;

        explicit Global(::jobject h) noexcept:
            object{h} {}

        Global(const Global& other);
        Global& operator=(const Global& other);

        Global(Global&& other) noexcept:
        object(Ops::exchange(other.object, nullptr)) {}

        Global& operator=(Global&& other) noexcept {
            if (this != &other) {
                reset();
                object = Ops::exchange(other.object, nullptr);
            }
            return *this;
        }

        ~Global() {
            reset();
        }

        /**
         * @brief Decays to a borrowed handle, widening to any base at the same
         * time, for the same reason as Local.
         * @tparam Base The base class to widen to.
         */
        template <typename Base>
            requires __is_base_of(Base, T)
        operator Ref<Base>() const noexcept {
            return Ref<Base>(object);
        }

        [[nodiscard]]
        Ref<T> get() const noexcept {
            return Ref<T>(object);
        }

        [[nodiscard]]
        ::jobject handle() const noexcept {
            return object;
        }

        explicit operator bool() const noexcept {
            return object != nullptr;
        }

        void reset() noexcept;

        /**
         * @class Arrow
         * @brief Materialises the binding class over the borrowed handle, so
         * `ref->method()` reads like Java.
         *
         * operator-> must yield something with its own operator->, so the
         * binding is built into this proxy and the chain resolves to it. The
         * proxy lives for the full-expression of the call, which is exactly as
         * long as the receiver is needed.
         */
        class Arrow {
        private:
            T value;
        public:
            explicit Arrow(::jobject handle) noexcept:
                value{handle} {}

            T* operator->() noexcept {
                return &value;
            }
        };

        /**
         * @brief Dispatches an instance method against the referenced object.
         */
        [[nodiscard]]
        Arrow operator->() const noexcept {
            return Arrow(object);
        }
    };

    /**
     * @class LocalFrame
     * @brief Scopes a batch of local references, so a loop that creates many
     * objects releases them per iteration rather than exhausting the frame.
     *
     * Popping the frame invalidates every local reference created inside it.
     * To carry one result outward, hand it to escape(), which pops the frame
     * and re-creates that single reference in the enclosing one.
     */
    class LocalFrame final {
    private:
        bool popped = false;
    public:
        [[nodiscard]]
        THROWS(JniException)
        explicit LocalFrame(i32 capacity = 16);

        LocalFrame(const LocalFrame&) = delete;
        LocalFrame& operator=(const LocalFrame&) = delete;
        LocalFrame(LocalFrame&&) = delete;
        LocalFrame& operator=(LocalFrame&&) = delete;

        ~LocalFrame();

        /**
         * @brief Pops the frame, preserving @p value in the enclosing frame.
         * Every other local reference made inside the frame becomes invalid.
         * @tparam T The binding class of the reference to preserve.
         * @param value The reference to preserve.
         * @return A new local reference to the same object, valid in the
         * enclosing frame.
         */
        template <typename T>
        [[nodiscard]]
        Local<T> escape(Local<T>&& value) noexcept;
    };
}

/**
 * @namespace stdx::jni
 * @brief Virtual machine lifetime and the out-of-line handle bodies.
 */
export namespace stdx::jni {
    /**
     * @class Vm
     * @brief Owns the embedded Java virtual machine.
     *
     * JNI permits exactly one VM per process and, once destroyed, it cannot be
     * recreated - so this is a process-scoped resource whether or not the
     * design wants it to be. It is still a constructible object rather than an
     * ambient singleton: nothing here exposes an instance() accessor, and
     * Env::current finds the VM through JNI's own JNI_GetCreatedJavaVMs
     * registry rather than through state this library keeps.
     *
     * Destroying the VM blocks until every non-daemon Java thread has finished.
     */
    class Vm final {
    private:
        ::JavaVM* vm = nullptr;
    public:
        /**
         * @brief Starts a virtual machine.
         * @param options JVM options, e.g. "-Djava.class.path=...", verbatim.
         * @throws JniException if a VM already exists or startup fails.
         */
        THROWS(JniException)
        explicit Vm(Span<const StringView> options = {});

        Vm(const Vm&) = delete;
        Vm& operator=(const Vm&) = delete;

        Vm(Vm&& other) noexcept:
            vm{Ops::exchange(other.vm, nullptr)} {}

        Vm& operator=(Vm&& other) noexcept {
            if (this != &other) {
                destroy();
                vm = Ops::exchange(other.vm, nullptr);
            }
            return *this;
        }

        ~Vm() {
            destroy();
        }

        [[nodiscard]]
        ::JavaVM* handle() const noexcept {
            return vm;
        }

        /**
         * @brief Whether a VM exists in this process, however it was started.
         * @return True if a VM exists, false if not.
         *
         * True for a VM this library created and also for one that already
         * existed, which is the case when C++ is being called *from* Java.
         */
        [[nodiscard]]
        static bool is_running() noexcept {
            ::JavaVM* found = nullptr;
            ::jsize count = 0;
            return ::JNI_GetCreatedJavaVMs(&found, 1, &count) == JNI_OK && count > 0;
        }

        void destroy() noexcept {
            if (vm != nullptr) {
                vm->DestroyJavaVM();
                vm = nullptr;
            }
        }
    };

    inline Vm::Vm(Span<const StringView> options) {
        if (is_running()) {
            throw JniException("a Java VM already exists in this process");
        }

        Vector<String> owned;
        owned.reserve(options.size());
        for (StringView option: options) {
            owned.emplace_back(option);
        }

        Vector<::JavaVMOption> raw;
        raw.reserve(owned.size());
        for (String& option: owned) {
            raw.push_back(::JavaVMOption{ option.data(), nullptr });
        }

        ::JavaVMInitArgs args{};
        args.version = JNI_VERSION_10;
        args.nOptions = static_cast<::jint>(raw.size());
        args.options = raw.empty() ? nullptr : raw.data();
        args.ignoreUnrecognized = JNI_FALSE;

        ::JNIEnv* env = nullptr;
        const ::jint status = ::JNI_CreateJavaVM(&vm, reinterpret_cast<void**>(&env), &args);
        if (status != JNI_OK || vm == nullptr) {
            vm = nullptr;
            throw JniException("failed to start the Java VM");
        }
    }

    inline ::JNIEnv* Env::raw_current() noexcept {
        ::JavaVM* vm = nullptr;
        ::jsize count = 0;
        if (::JNI_GetCreatedJavaVMs(&vm, 1, &count) != JNI_OK || count == 0 || vm == nullptr) {
            return nullptr;
        }

        ::JNIEnv* env = nullptr;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_10) == JNI_OK) {
            return env;
        }
        return nullptr;
    }

    inline Env Env::current() {
        ::JavaVM* vm = nullptr;
        ::jsize count = 0;
        if (::JNI_GetCreatedJavaVMs(&vm, 1, &count) != JNI_OK || count == 0 || vm == nullptr) {
            throw JniException("no Java VM in this process");
        }

        ::JNIEnv* env = nullptr;
        const ::jint status = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_10);
        if (status == JNI_OK) {
            return Env(env);
        }

        if (status == JNI_EDETACHED) {
            if (vm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr) != JNI_OK) {
                throw JniException("failed to attach the calling thread to the Java VM");
            }
            return Env(env);
        }

        throw JniException("the Java VM does not support this JNI version");
    }

    template <typename T>
    inline void Local<T>::reset() noexcept {
        if (object != nullptr) {
            if (::JNIEnv* env = Env::raw_current()) {
                env->DeleteLocalRef(object);
            }
            object = nullptr;
        }
    }

    template <typename T>
    inline Global<T> Local<T>::global() const {
        if (object == nullptr) {
            return {};
        }
        Env env = Env::current();
        ::jobject promoted = env->NewGlobalRef(object);
        if (promoted == nullptr) {
            throw JniException("failed to create a global reference");
        }
        return Global<T>(promoted);
    }

    template <typename T>
    inline void Global<T>::reset() noexcept {
        if (object != nullptr) {
            if (::JNIEnv* env = Env::raw_current()) {
                env->DeleteGlobalRef(object);
            }
            object = nullptr;
        }
    }

    template <typename T>
    inline Global<T>::Global(const Global& other) {
        if (other.object != nullptr) {
            if (::JNIEnv* env = Env::raw_current()) {
                object = env->NewGlobalRef(other.object);
            }
        }
    }

    template <typename T>
    inline Global<T>& Global<T>::operator=(const Global& other) {
        if (this != &other) {
            reset();
            if (other.object != nullptr) {
                if (::JNIEnv* env = Env::raw_current()) {
                    object = env->NewGlobalRef(other.object);
                }
            }
        }
        return *this;
    }

    inline LocalFrame::LocalFrame(i32 capacity) {
        Env env = Env::current();
        if (env->PushLocalFrame(static_cast<::jint>(capacity)) != JNI_OK) {
            throw JniException("failed to push a local reference frame");
        }
    }

    inline LocalFrame::~LocalFrame() {
        if (!popped) {
            if (::JNIEnv* env = Env::raw_current()) {
                env->PopLocalFrame(nullptr);
            }
            popped = true;
        }
    }

    template <typename T>
    inline Local<T> LocalFrame::escape(Local<T>&& value) noexcept {
        if (popped) {
            return Ops::move(value);
        }
        popped = true;

        ::JNIEnv* env = Env::raw_current();
        if (env == nullptr) {
            return {};
        }
        // PopLocalFrame re-creates its argument in the enclosing frame and
        // invalidates everything else made inside this one.
        return Local<T>(env->PopLocalFrame(value.release()));
    }
}
