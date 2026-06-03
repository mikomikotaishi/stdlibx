#pragma once

using stdx::fmt::Formatter;

/**
 * @namespace stdx::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace stdx::math {
    /**
     * @struct Quaternion
     * @brief A quaternion class for representing rotations and orientations in 3D space.
     * @tparam T The scalar type.
     *
     * Quaternion is represented as q = w + xi + yj + zk, where w is the scalar part
     * and (x, y, z) is the vector part.
     */
    template <FloatingPoint T = f64>
    class Quaternion {
    public:
        T w{1}; ///< The scalar part (real component).
        T x{0}; ///< The x component of the vector part (imaginary i).
        T y{0}; ///< The y component of the vector part (imaginary j).
        T z{0}; ///< The z component of the vector part (imaginary k).

        /**
         * @brief Default constructor: identity quaternion (1, 0, 0, 0).
         */
        constexpr Quaternion() noexcept = default;

        /**
         * @brief Construct from components.
         */
        constexpr Quaternion(T w, T x, T y, T z) noexcept:
            w{w}, x{x}, y{y}, z{z} {}

        /**
         * @brief Construct a pure quaternion from a vector part (w = 0).
         */
        constexpr Quaternion(T x, T y, T z) noexcept:
            w{T(0)}, x{x}, y{y}, z{z} {}

        /**
         * @brief Construct a scalar quaternion (vector part = 0).
         */
        constexpr explicit Quaternion(T w) noexcept:
            w{w}, x{T(0)}, y{T(0)}, z{T(0)} {}

        static const Quaternion ZERO; ///< The zero quaternion (0, 0, 0, 0).
        static const Quaternion IDENTITY; ///< The identity quaternion (1, 0, 0, 0).

        /**
         * @brief Create a quaternion from an axis-angle representation.
         */
        [[nodiscard]]
        static Quaternion from_axis_angle(T ax, T ay, T az, T angle) {
            const T half = angle / T(2);
            const T s = sin(half);
            return Quaternion{cos(half), ax * s, ay * s, az * s};
        }

        /**
         * @brief Create a quaternion from Euler angles (roll, pitch, yaw) in radians.
         */
        [[nodiscard]]
        static Quaternion from_euler(T roll, T pitch, T yaw) {
            const T cr = cos(roll / T(2));
            const T sr = sin(roll / T(2));
            const T cp = cos(pitch / T(2));
            const T sp = sin(pitch / T(2));
            const T cy = cos(yaw / T(2));
            const T sy = sin(yaw / T(2));

            return Quaternion{
                .w = cr * cp * cy + sr * sp * sy,
                .x = sr * cp * cy - cr * sp * sy,
                .y = cr * sp * cy + sr * cp * sy,
                .z = cr * cp * sy - sr * sp * cy
            };
        }

        [[nodiscard]]
        constexpr Quaternion operator+(const Quaternion& rhs) const noexcept {
            return Quaternion{w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z};
        }

        [[nodiscard]]
        constexpr Quaternion operator-(const Quaternion& rhs) const noexcept {
            return Quaternion{w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z};
        }

        [[nodiscard]]
        constexpr Quaternion operator-() const noexcept {
            return Quaternion{-w, -x, -y, -z};
        }

        /**
         * @brief Hamilton product.
         */
        [[nodiscard]]
        constexpr Quaternion operator*(const Quaternion& rhs) const noexcept {
            return Quaternion{
                .w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
                .x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
                .y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
                .z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w
            };
        }

        /**
         * @brief Scalar multiplication.
         */
        [[nodiscard]]
        constexpr Quaternion operator*(T scalar) const noexcept {
            return Quaternion{w * scalar, x * scalar, y * scalar, z * scalar};
        }

        /**
         * @brief Scalar division.
         */
        [[nodiscard]]
        constexpr Quaternion operator/(T scalar) const noexcept {
            return Quaternion{w / scalar, x / scalar, y / scalar, z / scalar};
        }

        constexpr Quaternion& operator+=(const Quaternion& rhs) noexcept {
            w += rhs.w;
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        constexpr Quaternion& operator-=(const Quaternion& rhs) noexcept {
            w -= rhs.w;
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        constexpr Quaternion& operator*=(const Quaternion& rhs) noexcept {
            *this = *this * rhs;
            return *this;
        }

        constexpr Quaternion& operator*=(T scalar) noexcept {
            w *= scalar;
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        constexpr Quaternion& operator/=(T scalar) noexcept {
            w /= scalar;
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        [[nodiscard]]
        constexpr bool operator==(const Quaternion&) const noexcept = default;

        /**
         * @brief Squared norm (avoids sqrt).
         */
        [[nodiscard]]
        constexpr T norm_squared() const noexcept {
            return w * w + x * x + y * y + z * z;
        }

        /**
         * @brief Euclidean norm.
         */
        [[nodiscard]]
        T norm() const noexcept {
            return sqrt(norm_squared());
        }

        /**
         * @brief Conjugate: q* = (w, -x, -y, -z).
         */
        [[nodiscard]]
        constexpr Quaternion conjugate() const noexcept {
            return Quaternion{w, -x, -y, -z};
        }

        /**
         * @brief Multiplicative inverse: q^-1 = q* / |q|^2.
         */
        [[nodiscard]]
        Quaternion inverse() const {
            return conjugate() / norm_squared();
        }

        /**
         * @brief Return a unit quaternion (normalized).
         */
        [[nodiscard]]
        Quaternion normalized() const {
            return *this / norm();
        }

        /**
         * @brief Dot product of two quaternions.
         */
        [[nodiscard]]
        constexpr T dot(const Quaternion& rhs) const noexcept {
            return w * rhs.w + x * rhs.x + y * rhs.y + z * rhs.z;
        }

        /**
         * @brief Rotate a 3D vector (vx, vy, vz) by this unit quaternion.
         */
        [[nodiscard]]
        constexpr Tuple<T, T, T> rotate(T vx, T vy, T vz) const noexcept {
            // q * v * q^-1, optimized for unit quaternions (inverse == conjugate)
            const Quaternion v{T(0), vx, vy, vz};
            const Quaternion result = *this * v * conjugate();
            return {result.x, result.y, result.z};
        }

        /**
         * @brief Convert to Euler angles (roll, pitch, yaw) in radians.
         */
        [[nodiscard]]
        Tuple<T, T, T> to_euler() const {
            // Roll (x-axis)
            const T sinr_cosp = T(2) * (w * x + y * z);
            const T cosr_cosp = T(1) - T(2) * (x * x + y * y);
            const T roll = atan2(sinr_cosp, cosr_cosp);

            // Pitch (y-axis)
            const T sinp = T(2) * (w * y - z * x);
            T pitch = (abs(sinp) >= T(1))
                ? copysign(PI_VALUE<T> / T(2), sinp)
                : asin(sinp);

            // Yaw (z-axis)
            const T siny_cosp = T(2) * (w * z + x * y);
            const T cosy_cosp = T(1) - T(2) * (y * y + z * z);
            const T yaw = atan2(siny_cosp, cosy_cosp);

            return {roll, pitch, yaw};
        }

        /**
         * @brief Extract axis and angle from a unit quaternion.
         */
        [[nodiscard]]
        Tuple<T, T, T, T> to_axis_angle() const {
            const T angle = T(2) * acos(w);
            const T s = sqrt(T(1) - w * w);
            return s < T(1e-8)
                ? Tuple{T(1), T(0), T(0), angle} // Arbitrary axis if angle is small
                : Tuple{x / s, y / s, z / s, angle};
        }
    };

    template <FloatingPoint T>
    inline constexpr Quaternion<T> Quaternion<T>::ZERO{0, 0, 0, 0};

    template <FloatingPoint T>
    inline constexpr Quaternion<T> Quaternion<T>::IDENTITY{1, 0, 0, 0};

    /**
     * @brief Scalar * Quaternion.
     */
    template <FloatingPoint T>
    [[nodiscard]]
    constexpr Quaternion<T> operator*(T scalar, const Quaternion<T>& q) noexcept {
        return q * scalar;
    }

    /**
     * @brief Spherical linear interpolation between two unit quaternions.
     */
    template <FloatingPoint T>
    [[nodiscard]]
    Quaternion<T> slerp(const Quaternion<T>& a, const Quaternion<T>& b, T t) {
        T cos_theta = a.dot(b);

        // If the dot product is negative, negate one to take the shorter arc.
        Quaternion<T> b2 = b;
        if (cos_theta < T(0)) {
            b2 = -b;
            cos_theta = -cos_theta;
        }

        // If quaternions are very close, fall back to normalized lerp.
        if (cos_theta > T(1) - T(1e-6)) {
            return (a * (T(1) - t) + b2 * t).normalized();
        }

        const T theta = acos(cos_theta);
        const T sin_theta = sin(theta);
        const T wa = sin((T(1) - t) * theta) / sin_theta;
        const T wb = sin(t * theta) / sin_theta;

        return a * wa + b2 * wb;
    }

    using std::iota;
    using std::accumulate;
    using std::reduce;
    using std::transform_reduce;
    using std::inner_product;
    using std::adjacent_difference;
    using std::partial_sum;
    using std::inclusive_scan;
    using std::exclusive_scan;
    using std::transform_inclusive_scan;
    using std::transform_exclusive_scan;
    using std::gcd;
    using std::lcm;
    using std::midpoint;
}

using stdx::math::Quaternion;

namespace stdx::fmt {
    template <typename T, typename Char>
    struct Formatter<Quaternion<T>, Char> {
        static constexpr const char* parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        static FormatContext::iterator format(const Quaternion<T>& q, FormatContext& ctx) {
            return format_to(ctx.out(), "({} + {}i + {}j + {}k)", q.w(), q.x(), q.y(), q.z());
        }
    };
}

template <typename T, typename Char>
struct stdx::fmt::formatter<Quaternion<T>, Char> : public Formatter<Quaternion<T>, Char> {};
