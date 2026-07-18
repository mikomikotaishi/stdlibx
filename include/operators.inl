#pragma once

#ifndef STDLIBX_EXPORT_IMPORT_STD
export namespace std {
    using std::operator<<;
    using std::operator>>;
    using std::operator+;
    using std::operator-;
    using std::operator*;
    using std::operator/;
    using std::operator%;
    using std::operator&;
    using std::operator|;
    using std::operator^;
    using std::operator~;
    using std::operator<;
    using std::operator>;
    using std::operator<=;
    using std::operator>=;
    using std::operator==;
    using std::operator!=;
    using std::operator<=>;
    using std::operator&&;
    using std::operator||;
    using std::operator<<=;
    using std::operator>>=;

    using std::begin;
    using std::cbegin;
    using std::end;
    using std::cend;
    using std::rbegin;
    using std::crbegin;
    using std::rend;
    using std::crend;
    using std::size;
    using std::ssize;
    using std::empty;
    using std::data;

    using std::coroutine_traits;
    using std::coroutine_handle;
    using std::noop_coroutine_promise;
    using std::noop_coroutine_handle;
    using std::noop_coroutine;
    using std::suspend_always;
    using std::suspend_never;
    using std::hash;
    using std::initializer_list;
    using std::formatter;
    using std::tuple;
    using std::tuple_size;
    using std::tuple_element;
    using std::get;
    using std::type_info;
    using std::swap;

    namespace chrono {
        using std::chrono::operator+;
        using std::chrono::operator-;
        using std::chrono::operator<;
        using std::chrono::operator>;
        using std::chrono::operator<=;
        using std::chrono::operator>=;
        using std::chrono::operator==;
        using std::chrono::operator<=>;
    }

    namespace filesystem {
        using std::filesystem::begin;
        using std::filesystem::end;
        using std::filesystem::hash_value;
        using std::filesystem::swap;
    }

    inline namespace literals {
        inline namespace string_literals {
            using std::operator""s;
        }

        inline namespace string_view_literals {
            using std::operator""sv;
        }

        inline namespace complex_literals {
            using std::operator""i;
            using std::operator""if;
            using std::operator""il;
        }

        inline namespace chrono_literals {
            using std::operator""y;
            using std::operator""d;
            using std::operator""h;
            using std::operator""min;
            using std::operator""s;
            using std::operator""ms;
            using std::operator""ns;
            using std::operator""us;
        }
    }

    namespace ranges {
        using std::ranges::begin;
        using std::ranges::cbegin;
        using std::ranges::end;
        using std::ranges::cend;
        using std::ranges::rbegin;
        using std::ranges::crbegin;
        using std::ranges::rend;
        using std::ranges::crend;
        using std::ranges::size;
        using std::ranges::ssize;
        using std::ranges::empty;
        using std::ranges::data;
        using std::ranges::cdata;
    }
}
#endif
