#pragma once

export namespace stdx::jni {
    /**
     * @brief Translates a pending Java exception into a C++ one.
     *
     * JNI never transfers control on failure: it sets a pending exception and
     * returns, and every subsequent JNI call is undefined until that exception
     * is cleared. Generated bindings therefore call this after every call that
     * can raise.
     *
     * The pending exception is cleared before anything else is asked of the
     * VM, because reading the message is itself a sequence of JNI calls.
     */
    inline void Env::check_pending() const {
        if (env->ExceptionCheck() == JNI_FALSE) {
            return;
        }

        ::jthrowable pending = env->ExceptionOccurred();
        env->ExceptionClear();

        if (pending == nullptr) {
            throw JavaException("a Java exception was raised");
        }

        String message;
        ::jclass throwable_class = env->FindClass("java/lang/Throwable");
        if (throwable_class != nullptr) {
            ::jmethodID to_string = env->GetMethodID(
                throwable_class, "toString", "()Ljava/lang/String;");
            if (to_string != nullptr) {
                auto text = static_cast<::jstring>(env->CallObjectMethod(pending, to_string));
                // A failure while reporting a failure is not worth recursing on.
                if (env->ExceptionCheck() != JNI_FALSE) {
                    env->ExceptionClear();
                } else if (text != nullptr) {
                    if (const ::jchar* data = env->GetStringChars(text, nullptr)) {
                        message = details::utf16_to_utf8(
                            data, static_cast<usize>(env->GetStringLength(text)));
                        env->ReleaseStringChars(text, data);
                    }
                    env->DeleteLocalRef(text);
                }
            }
            env->DeleteLocalRef(throwable_class);
        }

        env->DeleteLocalRef(pending);

        if (message.empty()) {
            message = "a Java exception was raised";
        }
        throw JavaException(message.c_str());
    }
}
