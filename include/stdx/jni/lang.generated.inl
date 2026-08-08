#pragma once

export namespace java::lang {
    // Forward declarations: a signature may name any class in the
    // manifest, whatever order the definitions end up in.
    class System;
    class Math;

    /**
     * @class System
     * @brief Binding for java.lang.System.
     */
    class System: public virtual java::lang::Object {
    private:
        /** @brief Not instantiable; java.lang.System is a utility class. */
        System() = default;

    public:
        [[nodiscard]]
        static consteval StringView className() noexcept {
            return "java/lang/System"sv;
        }

        THROWS(::stdx::jni::JniException)
        static void arraycopy(::stdx::jni::Ref<java::lang::Object> a0, i32 a1, ::stdx::jni::Ref<java::lang::Object> a2, i32 a3, i32 a4);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static ::stdx::jni::Local<java::lang::String> clearProperty(::stdx::jni::StringArg a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 currentTimeMillis();

        THROWS(::stdx::jni::JniException)
        static void exit(i32 a0);

        THROWS(::stdx::jni::JniException)
        static void gc();

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static ::stdx::jni::Local<java::lang::String> getProperty(::stdx::jni::StringArg a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static ::stdx::jni::Local<java::lang::String> getProperty(::stdx::jni::StringArg a0, ::stdx::jni::StringArg a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static ::stdx::jni::Local<java::lang::String> getenv(::stdx::jni::StringArg a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 identityHashCode(::stdx::jni::Ref<java::lang::Object> a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static ::stdx::jni::Local<java::lang::String> lineSeparator();

        THROWS(::stdx::jni::JniException)
        static void load(::stdx::jni::StringArg a0);

        THROWS(::stdx::jni::JniException)
        static void loadLibrary(::stdx::jni::StringArg a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static ::stdx::jni::Local<java::lang::String> mapLibraryName(::stdx::jni::StringArg a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 nanoTime();

        THROWS(::stdx::jni::JniException)
        static void runFinalization();

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static ::stdx::jni::Local<java::lang::String> setProperty(::stdx::jni::StringArg a0, ::stdx::jni::StringArg a1);
    };

    /**
     * @class Math
     * @brief Binding for java.lang.Math.
     */
    class Math: public virtual java::lang::Object {
    private:
        /** @brief Not instantiable; java.lang.Math is a utility class. */
        Math() = default;

    public:
        [[nodiscard]]
        static consteval StringView className() noexcept {
            return "java/lang/Math"sv;
        }

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 E();

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 PI();

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 TAU();

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 IEEEremainder(f64 a0, f64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 abs(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f32 abs(f32 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 abs(i64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 abs(i32 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 absExact(i64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 absExact(i32 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 acos(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 addExact(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 addExact(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 asin(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 atan(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 atan2(f64 a0, f64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 cbrt(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 ceil(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 ceilDiv(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 ceilDiv(i64 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 ceilDiv(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 ceilDivExact(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 ceilDivExact(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 ceilMod(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 ceilMod(i64 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 ceilMod(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f32 clamp(f32 a0, f32 a1, f32 a2);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 clamp(i64 a0, i64 a1, i64 a2);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 clamp(f64 a0, f64 a1, f64 a2);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 clamp(i64 a0, i32 a1, i32 a2);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 copySign(f64 a0, f64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f32 copySign(f32 a0, f32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 cos(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 cosh(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 decrementExact(i32 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 decrementExact(i64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 divideExact(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 divideExact(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 exp(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 expm1(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 floor(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 floorDiv(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 floorDiv(i64 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 floorDiv(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 floorDivExact(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 floorDivExact(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 floorMod(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 floorMod(i64 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 floorMod(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 fma(f64 a0, f64 a1, f64 a2);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f32 fma(f32 a0, f32 a1, f32 a2);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 getExponent(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 getExponent(f32 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 hypot(f64 a0, f64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 incrementExact(i64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 incrementExact(i32 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 log(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 log10(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 log1p(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 max(f64 a0, f64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f32 max(f32 a0, f32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 max(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 max(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 min(f64 a0, f64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f32 min(f32 a0, f32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 min(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 min(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 multiplyExact(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 multiplyExact(i64 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 multiplyExact(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 multiplyFull(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 multiplyHigh(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 negateExact(i32 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 negateExact(i64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f32 nextAfter(f32 a0, f64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 nextAfter(f64 a0, f64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 nextDown(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f32 nextDown(f32 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 nextUp(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f32 nextUp(f32 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 pow(f64 a0, f64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 random();

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 rint(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 round(f32 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 round(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f32 scalb(f32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 scalb(f64 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 signum(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f32 signum(f32 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 sin(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 sinh(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 sqrt(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 subtractExact(i64 a0, i64 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 subtractExact(i32 a0, i32 a1);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 tan(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 tanh(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 toDegrees(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i32 toIntExact(i64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 toRadians(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f64 ulp(f64 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static f32 ulp(f32 a0);

        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static i64 unsignedMultiplyHigh(i64 a0, i64 a1);
    };

    inline void System::arraycopy(::stdx::jni::Ref<java::lang::Object> a0, i32 a1, ::stdx::jni::Ref<java::lang::Object> a2, i32 a3, i32 a4) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "arraycopy", SignatureOf<void, java::lang::Object, i32, java::lang::Object, i32, i32>);

        env->CallStaticVoidMethod(cls, id, ::stdx::jni::Ref<java::lang::Object>(a0).handle(), details::to_wire<i32>(a1), ::stdx::jni::Ref<java::lang::Object>(a2).handle(), details::to_wire<i32>(a3), details::to_wire<i32>(a4));
        env.check_pending();
    }

    inline ::stdx::jni::Local<java::lang::String> System::clearProperty(::stdx::jni::StringArg a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "clearProperty", SignatureOf<java::lang::String, java::lang::String>);

        ::jobject result = env->CallStaticObjectMethod(cls, id, ::stdx::jni::Ref<java::lang::String>(a0).handle());
        env.check_pending();
        return ::stdx::jni::Local<java::lang::String>(result);
    }

    inline i64 System::currentTimeMillis() {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "currentTimeMillis", SignatureOf<i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id));
        env.check_pending();
        return result;
    }

    inline void System::exit(i32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "exit", SignatureOf<void, i32>);

        env->CallStaticVoidMethod(cls, id, details::to_wire<i32>(a0));
        env.check_pending();
    }

    inline void System::gc() {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "gc", SignatureOf<void>);

        env->CallStaticVoidMethod(cls, id);
        env.check_pending();
    }

    inline ::stdx::jni::Local<java::lang::String> System::getProperty(::stdx::jni::StringArg a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "getProperty", SignatureOf<java::lang::String, java::lang::String>);

        ::jobject result = env->CallStaticObjectMethod(cls, id, ::stdx::jni::Ref<java::lang::String>(a0).handle());
        env.check_pending();
        return ::stdx::jni::Local<java::lang::String>(result);
    }

    inline ::stdx::jni::Local<java::lang::String> System::getProperty(::stdx::jni::StringArg a0, ::stdx::jni::StringArg a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "getProperty", SignatureOf<java::lang::String, java::lang::String, java::lang::String>);

        ::jobject result = env->CallStaticObjectMethod(cls, id, ::stdx::jni::Ref<java::lang::String>(a0).handle(), ::stdx::jni::Ref<java::lang::String>(a1).handle());
        env.check_pending();
        return ::stdx::jni::Local<java::lang::String>(result);
    }

    inline ::stdx::jni::Local<java::lang::String> System::getenv(::stdx::jni::StringArg a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "getenv", SignatureOf<java::lang::String, java::lang::String>);

        ::jobject result = env->CallStaticObjectMethod(cls, id, ::stdx::jni::Ref<java::lang::String>(a0).handle());
        env.check_pending();
        return ::stdx::jni::Local<java::lang::String>(result);
    }

    inline i32 System::identityHashCode(::stdx::jni::Ref<java::lang::Object> a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "identityHashCode", SignatureOf<i32, java::lang::Object>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, ::stdx::jni::Ref<java::lang::Object>(a0).handle()));
        env.check_pending();
        return result;
    }

    inline ::stdx::jni::Local<java::lang::String> System::lineSeparator() {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "lineSeparator", SignatureOf<java::lang::String>);

        ::jobject result = env->CallStaticObjectMethod(cls, id);
        env.check_pending();
        return ::stdx::jni::Local<java::lang::String>(result);
    }

    inline void System::load(::stdx::jni::StringArg a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "load", SignatureOf<void, java::lang::String>);

        env->CallStaticVoidMethod(cls, id, ::stdx::jni::Ref<java::lang::String>(a0).handle());
        env.check_pending();
    }

    inline void System::loadLibrary(::stdx::jni::StringArg a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "loadLibrary", SignatureOf<void, java::lang::String>);

        env->CallStaticVoidMethod(cls, id, ::stdx::jni::Ref<java::lang::String>(a0).handle());
        env.check_pending();
    }

    inline ::stdx::jni::Local<java::lang::String> System::mapLibraryName(::stdx::jni::StringArg a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "mapLibraryName", SignatureOf<java::lang::String, java::lang::String>);

        ::jobject result = env->CallStaticObjectMethod(cls, id, ::stdx::jni::Ref<java::lang::String>(a0).handle());
        env.check_pending();
        return ::stdx::jni::Local<java::lang::String>(result);
    }

    inline i64 System::nanoTime() {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "nanoTime", SignatureOf<i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id));
        env.check_pending();
        return result;
    }

    inline void System::runFinalization() {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "runFinalization", SignatureOf<void>);

        env->CallStaticVoidMethod(cls, id);
        env.check_pending();
    }

    inline ::stdx::jni::Local<java::lang::String> System::setProperty(::stdx::jni::StringArg a0, ::stdx::jni::StringArg a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<System>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "setProperty", SignatureOf<java::lang::String, java::lang::String, java::lang::String>);

        ::jobject result = env->CallStaticObjectMethod(cls, id, ::stdx::jni::Ref<java::lang::String>(a0).handle(), ::stdx::jni::Ref<java::lang::String>(a1).handle());
        env.check_pending();
        return ::stdx::jni::Local<java::lang::String>(result);
    }

    inline f64 Math::E() {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jfieldID id = env->GetStaticFieldID(cls, "E", "D");

        const auto value = static_cast<f64>(env->GetStaticDoubleField(cls, id));
        env.check_pending();
        return value;
    }

    inline f64 Math::PI() {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jfieldID id = env->GetStaticFieldID(cls, "PI", "D");

        const auto value = static_cast<f64>(env->GetStaticDoubleField(cls, id));
        env.check_pending();
        return value;
    }

    inline f64 Math::TAU() {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jfieldID id = env->GetStaticFieldID(cls, "TAU", "D");

        const auto value = static_cast<f64>(env->GetStaticDoubleField(cls, id));
        env.check_pending();
        return value;
    }

    inline f64 Math::IEEEremainder(f64 a0, f64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "IEEEremainder", SignatureOf<f64, f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0), details::to_wire<f64>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::abs(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "abs", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f32 Math::abs(f32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "abs", SignatureOf<f32, f32>);

        const auto result = static_cast<f32>(env->CallStaticFloatMethod(cls, id, details::to_wire<f32>(a0)));
        env.check_pending();
        return result;
    }

    inline i64 Math::abs(i64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "abs", SignatureOf<i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0)));
        env.check_pending();
        return result;
    }

    inline i32 Math::abs(i32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "abs", SignatureOf<i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0)));
        env.check_pending();
        return result;
    }

    inline i64 Math::absExact(i64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "absExact", SignatureOf<i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0)));
        env.check_pending();
        return result;
    }

    inline i32 Math::absExact(i32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "absExact", SignatureOf<i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::acos(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "acos", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline i64 Math::addExact(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "addExact", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::addExact(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "addExact", SignatureOf<i32, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::asin(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "asin", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::atan(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "atan", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::atan2(f64 a0, f64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "atan2", SignatureOf<f64, f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0), details::to_wire<f64>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::cbrt(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "cbrt", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::ceil(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "ceil", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline i64 Math::ceilDiv(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "ceilDiv", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::ceilDiv(i64 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "ceilDiv", SignatureOf<i64, i64, i32>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::ceilDiv(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "ceilDiv", SignatureOf<i32, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::ceilDivExact(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "ceilDivExact", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::ceilDivExact(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "ceilDivExact", SignatureOf<i32, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::ceilMod(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "ceilMod", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::ceilMod(i64 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "ceilMod", SignatureOf<i32, i64, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::ceilMod(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "ceilMod", SignatureOf<i32, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline f32 Math::clamp(f32 a0, f32 a1, f32 a2) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "clamp", SignatureOf<f32, f32, f32, f32>);

        const auto result = static_cast<f32>(env->CallStaticFloatMethod(cls, id, details::to_wire<f32>(a0), details::to_wire<f32>(a1), details::to_wire<f32>(a2)));
        env.check_pending();
        return result;
    }

    inline i64 Math::clamp(i64 a0, i64 a1, i64 a2) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "clamp", SignatureOf<i64, i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1), details::to_wire<i64>(a2)));
        env.check_pending();
        return result;
    }

    inline f64 Math::clamp(f64 a0, f64 a1, f64 a2) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "clamp", SignatureOf<f64, f64, f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0), details::to_wire<f64>(a1), details::to_wire<f64>(a2)));
        env.check_pending();
        return result;
    }

    inline i32 Math::clamp(i64 a0, i32 a1, i32 a2) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "clamp", SignatureOf<i32, i64, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i32>(a1), details::to_wire<i32>(a2)));
        env.check_pending();
        return result;
    }

    inline f64 Math::copySign(f64 a0, f64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "copySign", SignatureOf<f64, f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0), details::to_wire<f64>(a1)));
        env.check_pending();
        return result;
    }

    inline f32 Math::copySign(f32 a0, f32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "copySign", SignatureOf<f32, f32, f32>);

        const auto result = static_cast<f32>(env->CallStaticFloatMethod(cls, id, details::to_wire<f32>(a0), details::to_wire<f32>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::cos(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "cos", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::cosh(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "cosh", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline i32 Math::decrementExact(i32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "decrementExact", SignatureOf<i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0)));
        env.check_pending();
        return result;
    }

    inline i64 Math::decrementExact(i64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "decrementExact", SignatureOf<i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0)));
        env.check_pending();
        return result;
    }

    inline i32 Math::divideExact(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "divideExact", SignatureOf<i32, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::divideExact(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "divideExact", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::exp(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "exp", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::expm1(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "expm1", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::floor(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "floor", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline i64 Math::floorDiv(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "floorDiv", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::floorDiv(i64 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "floorDiv", SignatureOf<i64, i64, i32>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::floorDiv(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "floorDiv", SignatureOf<i32, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::floorDivExact(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "floorDivExact", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::floorDivExact(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "floorDivExact", SignatureOf<i32, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::floorMod(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "floorMod", SignatureOf<i32, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::floorMod(i64 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "floorMod", SignatureOf<i32, i64, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::floorMod(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "floorMod", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::fma(f64 a0, f64 a1, f64 a2) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "fma", SignatureOf<f64, f64, f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0), details::to_wire<f64>(a1), details::to_wire<f64>(a2)));
        env.check_pending();
        return result;
    }

    inline f32 Math::fma(f32 a0, f32 a1, f32 a2) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "fma", SignatureOf<f32, f32, f32, f32>);

        const auto result = static_cast<f32>(env->CallStaticFloatMethod(cls, id, details::to_wire<f32>(a0), details::to_wire<f32>(a1), details::to_wire<f32>(a2)));
        env.check_pending();
        return result;
    }

    inline i32 Math::getExponent(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "getExponent", SignatureOf<i32, f64>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline i32 Math::getExponent(f32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "getExponent", SignatureOf<i32, f32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<f32>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::hypot(f64 a0, f64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "hypot", SignatureOf<f64, f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0), details::to_wire<f64>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::incrementExact(i64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "incrementExact", SignatureOf<i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0)));
        env.check_pending();
        return result;
    }

    inline i32 Math::incrementExact(i32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "incrementExact", SignatureOf<i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::log(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "log", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::log10(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "log10", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::log1p(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "log1p", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::max(f64 a0, f64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "max", SignatureOf<f64, f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0), details::to_wire<f64>(a1)));
        env.check_pending();
        return result;
    }

    inline f32 Math::max(f32 a0, f32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "max", SignatureOf<f32, f32, f32>);

        const auto result = static_cast<f32>(env->CallStaticFloatMethod(cls, id, details::to_wire<f32>(a0), details::to_wire<f32>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::max(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "max", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::max(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "max", SignatureOf<i32, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::min(f64 a0, f64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "min", SignatureOf<f64, f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0), details::to_wire<f64>(a1)));
        env.check_pending();
        return result;
    }

    inline f32 Math::min(f32 a0, f32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "min", SignatureOf<f32, f32, f32>);

        const auto result = static_cast<f32>(env->CallStaticFloatMethod(cls, id, details::to_wire<f32>(a0), details::to_wire<f32>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::min(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "min", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::min(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "min", SignatureOf<i32, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::multiplyExact(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "multiplyExact", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::multiplyExact(i64 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "multiplyExact", SignatureOf<i64, i64, i32>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::multiplyExact(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "multiplyExact", SignatureOf<i32, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::multiplyFull(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "multiplyFull", SignatureOf<i64, i32, i32>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline i64 Math::multiplyHigh(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "multiplyHigh", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::negateExact(i32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "negateExact", SignatureOf<i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0)));
        env.check_pending();
        return result;
    }

    inline i64 Math::negateExact(i64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "negateExact", SignatureOf<i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0)));
        env.check_pending();
        return result;
    }

    inline f32 Math::nextAfter(f32 a0, f64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "nextAfter", SignatureOf<f32, f32, f64>);

        const auto result = static_cast<f32>(env->CallStaticFloatMethod(cls, id, details::to_wire<f32>(a0), details::to_wire<f64>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::nextAfter(f64 a0, f64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "nextAfter", SignatureOf<f64, f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0), details::to_wire<f64>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::nextDown(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "nextDown", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f32 Math::nextDown(f32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "nextDown", SignatureOf<f32, f32>);

        const auto result = static_cast<f32>(env->CallStaticFloatMethod(cls, id, details::to_wire<f32>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::nextUp(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "nextUp", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f32 Math::nextUp(f32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "nextUp", SignatureOf<f32, f32>);

        const auto result = static_cast<f32>(env->CallStaticFloatMethod(cls, id, details::to_wire<f32>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::pow(f64 a0, f64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "pow", SignatureOf<f64, f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0), details::to_wire<f64>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::random() {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "random", SignatureOf<f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id));
        env.check_pending();
        return result;
    }

    inline f64 Math::rint(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "rint", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline i32 Math::round(f32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "round", SignatureOf<i32, f32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<f32>(a0)));
        env.check_pending();
        return result;
    }

    inline i64 Math::round(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "round", SignatureOf<i64, f64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f32 Math::scalb(f32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "scalb", SignatureOf<f32, f32, i32>);

        const auto result = static_cast<f32>(env->CallStaticFloatMethod(cls, id, details::to_wire<f32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::scalb(f64 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "scalb", SignatureOf<f64, f64, i32>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::signum(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "signum", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f32 Math::signum(f32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "signum", SignatureOf<f32, f32>);

        const auto result = static_cast<f32>(env->CallStaticFloatMethod(cls, id, details::to_wire<f32>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::sin(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "sin", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::sinh(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "sinh", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::sqrt(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "sqrt", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline i64 Math::subtractExact(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "subtractExact", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

    inline i32 Math::subtractExact(i32 a0, i32 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "subtractExact", SignatureOf<i32, i32, i32>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i32>(a0), details::to_wire<i32>(a1)));
        env.check_pending();
        return result;
    }

    inline f64 Math::tan(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "tan", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::tanh(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "tanh", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::toDegrees(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "toDegrees", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline i32 Math::toIntExact(i64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "toIntExact", SignatureOf<i32, i64>);

        const auto result = static_cast<i32>(env->CallStaticIntMethod(cls, id, details::to_wire<i64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::toRadians(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "toRadians", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f64 Math::ulp(f64 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "ulp", SignatureOf<f64, f64>);

        const auto result = static_cast<f64>(env->CallStaticDoubleMethod(cls, id, details::to_wire<f64>(a0)));
        env.check_pending();
        return result;
    }

    inline f32 Math::ulp(f32 a0) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "ulp", SignatureOf<f32, f32>);

        const auto result = static_cast<f32>(env->CallStaticFloatMethod(cls, id, details::to_wire<f32>(a0)));
        env.check_pending();
        return result;
    }

    inline i64 Math::unsignedMultiplyHigh(i64 a0, i64 a1) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<Math>().handle());
        static ::jmethodID id = env->GetStaticMethodID(cls, "unsignedMultiplyHigh", SignatureOf<i64, i64, i64>);

        const auto result = static_cast<i64>(env->CallStaticLongMethod(cls, id, details::to_wire<i64>(a0), details::to_wire<i64>(a1)));
        env.check_pending();
        return result;
    }

}
