#pragma once

export namespace stdx::jni {
    /**
     * @brief Creates a Java array of @p length elements.
     *
     * Object arrays start null-filled and primitive arrays zero-filled, as in
     * Java. The element class comes from class_of<T>(), so the array's runtime
     * component type always matches the static one.
     *
     * @tparam T The element type: a binding class or a Java primitive.
     * @throws JniException if the array cannot be allocated.
     */
    template <typename T>
    [[nodiscard]]
    THROWS(JniException)
    Local<JavaArray<T>> new_array(i32 length) {
        Env env = Env::current();
        ::jarray created = nullptr;

        if constexpr (details::IsPrimitive<T>) {
            const auto size = static_cast<::jsize>(length);
            if constexpr (__is_same(T, bool)) {
                created = env->NewBooleanArray(size);
            } else if constexpr (__is_same(T, i8)) {
                created = env->NewByteArray(size);
            } else if constexpr (__is_same(T, char16)) {
                created = env->NewCharArray(size);
            } else if constexpr (__is_same(T, i16)) {
                created = env->NewShortArray(size);
            } else if constexpr (__is_same(T, i32)) {
                created = env->NewIntArray(size);
            } else if constexpr (__is_same(T, i64)) {
                created = env->NewLongArray(size);
            } else if constexpr (__is_same(T, f32)) {
                created = env->NewFloatArray(size);
            } else if constexpr (__is_same(T, f64)) {
                created = env->NewDoubleArray(size);
            } else {
                static_assert(details::IsPrimitive<T> && !__is_same(T, void),
                    "void has no array type");
            }
        } else {
            created = env->NewObjectArray(
                static_cast<::jsize>(length),
                static_cast<::jclass>(class_of<T>().handle()),
                nullptr);
        }

        env.check_pending();
        if (created == nullptr) {
            throw JniException("failed to allocate a Java array");
        }
        return Local<JavaArray<T>>(created);
    }

    /**
     * @brief The length of a Java array.
     */
    template <typename T>
    [[nodiscard]]
    THROWS(JniException)
    i32 length_of(Ref<JavaArray<T>> array) {
        if (!array) {
            return 0;
        }
        Env env = Env::current();
        return static_cast<i32>(
            env->GetArrayLength(static_cast<::jarray>(array.handle())));
    }

    /**
     * @brief Reads element @p index of an object array.
     * @throws JavaException if the index is out of bounds.
     */
    template <typename T>
    [[nodiscard]]
    THROWS(JniException)
    Local<T> element_at(Ref<JavaArray<T>> array, i32 index) {
        Env env = Env::current();
        ::jobject value = env->GetObjectArrayElement(
            static_cast<::jobjectArray>(array.handle()), static_cast<::jsize>(index));
        env.check_pending();
        return Local<T>(value);
    }

    /**
     * @brief Writes element @p index of an object array.
     * @throws JavaException if the index is out of bounds or the type is wrong.
     */
    template <typename T>
    THROWS(JniException)
    void set_element_at(Ref<JavaArray<T>> array, i32 index, Ref<T> value) {
        Env env = Env::current();
        env->SetObjectArrayElement(
            static_cast<::jobjectArray>(array.handle()),
            static_cast<::jsize>(index), value.handle());
        env.check_pending();
    }
}
