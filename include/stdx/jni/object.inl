#pragma once

export namespace java::lang {
    /**
     * @class Object
     * @brief Binding for java.lang.Object, the root of every other binding.
     *
     * A binding class carries no state of its own: it is a typed view over a
     * jobject whose lifetime belongs to a Local, Global or borrowed Ref. The
     * inheritance exists so Ref, Local and Global can widen along the Java
     * hierarchy, which is what lets a String reach a parameter typed Object -
     * the position every erased generic method ends up in.
     */
    class Object {
    private:
        ::jobject object = nullptr;
    public:
        [[nodiscard]]
        static consteval StringView className() noexcept {
            return "java/lang/Object"sv;
        }

        Object() = default;

        explicit Object(::jobject handle) noexcept:
            object{handle} {}

        /**
         * @brief The receiver for an instance call.
         *
         * Binding classes are only ever materialised behind Ref/Local/Global's
         * operator->, which builds one over the handle it already owns. This is
         * the one piece of state a binding carries, and it is borrowed: the
         * reference's lifetime is the owner's, never the binding's.
         */
        [[nodiscard]]
        ::jobject self() const noexcept {
            return object;
        }
    };

    /**
     * @class Class
     * @brief Binding for java.lang.Class, the result of Java's `X.class`.
     * @extends Object
     *
     * Not templated on the represented type. Java's Class<T> is generic, but
     * the parameter is erased and nothing at the JNI level can act on it, so a
     * template parameter here would be decoration that invites false
     * confidence. Use class_of<T>() to obtain one.
     */
    class Class: public virtual Object {
    public:
        explicit Class(::jobject handle) noexcept: Object(handle) {}

        [[nodiscard]]
        static consteval StringView className() noexcept {
            return "java/lang/Class"sv;
        }
    };
}

export namespace stdx::jni {
    /**
     * @brief The java.lang.Class object for @p T, i.e. Java's `T.class`.
     *
     * Handles the three kinds separately, because JNI does:
     *  - a class is found by binary name, "java/lang/String"
     *  - an array is found by descriptor, "[Ljava/lang/String;"
     *  - a primitive is not findable at all; int.class is the TYPE field on
     *    java.lang.Integer, and FindClass("I") simply fails
     *
     * The result is cached per type in a global reference, so the classloader
     * walk happens once. Generated bindings use this rather than each caching
     * their own jclass.
     *
     * Note the classloader caveat that comes with FindClass: on a thread this
     * library attached, lookup uses the system classloader, so application
     * classes may be invisible unless the first call happens on a thread the
     * VM already knew about.
     *
     * @tparam T A binding class, an JavaArray, or a Java primitive type.
     * @throws JniException if the class cannot be found.
     */
    template <typename T>
    [[nodiscard]]
    THROWS(JniException)
    Ref<java::lang::Class> class_of() {
        static ::jclass cached = [] {
            Env env = Env::current();
            ::jclass found = nullptr;

            if constexpr (details::IsPrimitive<T>) {
                ::jclass boxed = env->FindClass(details::Wire<T>::BOX);
                env.check_pending();
                if (boxed != nullptr) {
                    ::jfieldID type = env->GetStaticFieldID(
                        boxed, "TYPE", "Ljava/lang/Class;");
                    env.check_pending();
                    found = static_cast<::jclass>(env->GetStaticObjectField(boxed, type));
                    env->DeleteLocalRef(boxed);
                }
            } else {
                found = env->FindClass(ClassNameOf<T>);
                env.check_pending();
            }

            if (found == nullptr) {
                throw JniException("class not found");
            }

            auto global = static_cast<::jclass>(env->NewGlobalRef(found));
            env->DeleteLocalRef(found);
            if (global == nullptr) {
                throw JniException("failed to retain the class reference");
            }
            return global;
        }();

        return Ref<java::lang::Class>(cached);
    }
}

export namespace stdx::jni {
    /**
     * @brief Whether @p object is an instance of @p T, i.e. `x instanceof T`.
     * @tparam T A binding class or an JavaArray type.
     */
    template <typename T>
    [[nodiscard]]
    THROWS(JniException)
    bool instance_of(Ref<java::lang::Object> object) {
        if (!object) {
            return false;   // Java: null instanceof T is always false
        }
        Env env = Env::current();
        const bool result = env->IsInstanceOf(
            object.handle(), static_cast<::jclass>(class_of<T>().handle())) != JNI_FALSE;
        env.check_pending();
        return result;
    }

    /**
     * @brief Checked downcast, i.e. Java's `(T) x`.
     *
     * The static type parameter is what makes this safe: the runtime check and
     * the resulting C++ type come from the same T, so they cannot drift apart.
     * A null reference casts to null, matching Java.
     *
     * @throws JavaException if @p object is not an instance of T, mirroring the
     * ClassCastException a Java checkcast would raise.
     */
    template <typename T>
    [[nodiscard]]
    THROWS(JniException)
    Ref<T> cast(Ref<java::lang::Object> object) {
        if (!object) {
            return {};
        }
        if (!instance_of<T>(object)) {
            throw JavaException("class cast failed");
        }
        return Ref<T>(object.handle());
    }
}
