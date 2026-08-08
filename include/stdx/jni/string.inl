#pragma once

export namespace java::lang {
    /**
     * @class String
     * @brief Binding for java.lang.String.
     * @extends Object
     */
    class String: public virtual Object {
    public:
        explicit String(::jobject handle) noexcept: Object(handle) {}

        [[nodiscard]]
        static consteval StringView className() noexcept {
            return "java/lang/String"sv;
        }

        /**
         * @brief The number of UTF-16 code units, as java.lang.String.length().
         * @throws JniException if the VM is unreachable.
         */
        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        i32 length() const;

        /**
         * @brief The UTF-16 code unit at @p index.
         * @throws JavaException if the index is out of bounds.
         */
        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        char16 charAt(i32 index) const;

        /**
         * @brief Builds a Java string from UTF-8 text.
         * @param text The UTF-8 source. Need not be NUL-terminated.
         * @return A new local reference to the Java string.
         * @throws JniException if the string cannot be allocated.
         */
        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static ::stdx::jni::Local<String> of(StringView text);

        /**
         * @brief Copies a Java string out into owned UTF-8 text.
         * @param text The Java string to read.
         * @return The UTF-8 encoding, empty if @p text is null.
         * @throws JniException if the characters cannot be pinned.
         *
         * Deliberately explicit rather than an implicit conversion: it copies
         * the whole string and gives up the reference, so it should be visible
         * at the point a caller decides to pay for it.
         */
        [[nodiscard]]
        THROWS(::stdx::jni::JniException)
        static ::stdx::core::String toUtf8(::stdx::jni::Ref<String> text);
    };
}

export namespace stdx::jni {
    /**
     * @class StringArg
     * @brief Parameter-position adapter that lets C++ text be passed wherever a
     * java.lang.String is expected.
     *
     * Generated methods taking a java.lang.String take this instead of
     * Ref<java::lang::String>, so `System::getProperty("os.name")` compiles.
     * The conversion cannot live on java::lang::String itself: parameters are
     * Ref<T>, and const char* to String to Ref<String> is two user-defined
     * conversions, which is one more than an implicit sequence allows.
     *
     * Constructing from an existing Java string reference allocates nothing, so
     * a value coming back out of Java can be handed straight back in.
     *
     * Copy and move are deleted because the owned reference lives only as long
     * as the full-expression containing the call. That is exactly the lifetime
     * of a temporary in an argument position, and nothing longer.
     *
     * Each text conversion allocates one local reference. In a loop, scope the
     * calls with a LocalFrame.
     */
    class StringArg {
    private:
        Local<java::lang::String> owned;
        Ref<java::lang::String> borrowed;
    public:
        THROWS(JniException)
        StringArg(const char* text):
            owned{java::lang::String::of(text)},
            borrowed{owned.get()} {}

        THROWS(JniException)
        StringArg(StringView text):
            owned{java::lang::String::of(text)},
            borrowed{owned.get()} {}

        THROWS(JniException)
        StringArg(const String& text):
            owned{java::lang::String::of(text)},
            borrowed{owned.get()} {}

        StringArg(Ref<java::lang::String> text) noexcept:
            borrowed(text) {}

        StringArg(const Local<java::lang::String>& text) noexcept:
            borrowed(text.get()) {}

        StringArg(const StringArg&) = delete;
        StringArg& operator=(const StringArg&) = delete;
        StringArg(StringArg&&) = delete;
        StringArg& operator=(StringArg&&) = delete;

        operator Ref<java::lang::String>() const noexcept {
            return borrowed;
        }
    };
}

export namespace stdx::jni::literals {
    /**
     * @brief Builds a Java string from a literal.
     *
     * For the common case the StringArg conversion already covers the call, so
     * this is for positions that take java.lang.Object rather than
     * java.lang.String - which, because generics erase to Object, is every
     * collection method. Runtime values cannot use a literal suffix; those go
     * through java::lang::String::of instead.
     */
    [[nodiscard]]
    THROWS(JniException)
    inline Local<java::lang::String> operator""_js(const char* text, usize size) {
        return java::lang::String::of(StringView(text, size));
    }
}

export namespace java::lang {
    [[nodiscard]]
    THROWS(::stdx::jni::JniException)
    inline ::stdx::jni::Local<String> String::of(StringView text) {
        using namespace ::stdx::jni;

        Env env = Env::current();
        const Vector<::jchar> units = details::utf8_to_utf16(text);
        ::jstring handle = env->NewString(units.data(), static_cast<::jsize>(units.size()));
        env.check_pending();
        return Local<String>(handle);
    }

    [[nodiscard]]
    THROWS(::stdx::jni::JniException)
    inline ::stdx::core::String String::toUtf8(::stdx::jni::Ref<String> text) {
        using namespace ::stdx::jni;

        if (!text) {
            return {};
        }

        Env env = Env::current();
        auto handle = static_cast<::jstring>(text.handle());
        const ::jchar* data = env->GetStringChars(handle, nullptr);
        env.check_pending();

        if (data == nullptr) {
            return {};
        }

        const auto size = static_cast<usize>(env->GetStringLength(handle));
        ::stdx::core::String out = details::utf16_to_utf8(data, size);
        env->ReleaseStringChars(handle, data);
        return out;
    }
}

export namespace java::lang {
    inline i32 String::length() const {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<String>().handle());
        static ::jmethodID id = env->GetMethodID(cls, "length", SignatureOf<i32>);

        const i32 result = env->CallIntMethod(self(), id);
        env.check_pending();
        return result;
    }

    inline char16 String::charAt(i32 index) const {
        using namespace ::stdx::jni;

        Env env = Env::current();
        auto cls = static_cast<::jclass>(class_of<String>().handle());
        static ::jmethodID id = env->GetMethodID(cls, "charAt", SignatureOf<char16, i32>);

        const auto result = static_cast<char16>(
            env->CallCharMethod(self(), id, details::to_wire<i32>(index)));
        env.check_pending();
        return result;
    }
}
