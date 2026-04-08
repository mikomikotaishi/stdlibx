#pragma once

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
    using std::operator""s;
    using std::operator""sv;
    using std::operator""i;
    using std::operator""if;
    using std::operator""il;
    using std::operator""y;
    using std::operator""d;
    using std::operator""h;
    using std::operator""min;
    using std::operator""ms;
    using std::operator""ns;
    using std::operator""us;

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

    using std::hash;
    using std::formatter;
    using std::type_info;
    using std::swap;

    namespace filesystem {
        using std::filesystem::begin;
        using std::filesystem::end;
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
