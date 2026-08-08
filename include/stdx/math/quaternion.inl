#pragma once

using stdx::fmt::Formatter;

/**
 * @namespace stdx::math
 * @brief Standard library mathematical operations.
 */
export namespace stdx::math {
    /**
     * @struct Quaternion
     * @brief A quaternion class for representing rotations and orientations in 3D space.
     * @tparam F The scalar type.
     *
     * Quaternion is represented as q = w + xi + yj + zk, where w is the scalar part
     * and (x, y, z) is the vector part.
     */
    template <FloatingPoint F = f64>
    struct Quaternion {
        F w{1}; ///< The scalar part (real component).
        F x{0}; ///< The x component of the vector part (imaginary i).
        F y{0}; ///< The y component of the vector part (imaginary j).
        F z{0}; ///< The z component of the vector part (imaginary k).

        /**
         * @brief Default constructor: identity quaternion (1, 0, 0, 0).
         */
        constexpr Quaternion() noexcept = default;

        /**
         * @brief Construct from components.
         * @param w The scalar part.
         * @param x The x component of the vector part.
         * @param y The y component of the vector part.
         * @param z The z component of the vector part.
         */
        constexpr Quaternion(F w, F x, F y, F z) noexcept:
            w{w}, x{x}, y{y}, z{z} {}

        /**
         * @brief Construct a pure quaternion from a vector part (w = 0).
         * @param x The x component of the vector part.
         * @param y The y component of the vector part.
         * @param z The z component of the vector part.
         */
        constexpr Quaternion(F x, F y, F z) noexcept:
            w{F(0)}, x{x}, y{y}, z{z} {}

        /**
         * @brief Construct a scalar quaternion (vector part = 0).
         * @param w The scalar part.
         */
        constexpr explicit Quaternion(F w) noexcept:
            w{w}, x{F(0)}, y{F(0)}, z{F(0)} {}

        static const Quaternion ZERO; ///< The zero quaternion (0, 0, 0, 0).
        static const Quaternion IDENTITY; ///< The identity quaternion (1, 0, 0, 0).

        /**
         * @brief Create a quaternion from an axis-angle representation.
         * @param ax The x component of the rotation axis (must be normalized).
         * @param ay The y component of the rotation axis (must be normalized).
         * @param az The z component of the rotation axis (must be normalized).
         * @param angle The rotation angle in radians.
         * @return A quaternion representing the rotation.
         */
        [[nodiscard]]
        static Quaternion from_axis_angle(F ax, F ay, F az, F angle) {
            const F half = angle / F(2);
            const F s = sin(half);
            return Quaternion{cos(half), ax * s, ay * s, az * s};
        }

        /**
         * @brief Create a quaternion from Euler angles (roll, pitch, yaw) in radians.
         * @param roll The rotation around the x-axis.
         * @param pitch The rotation around the y-axis.
         * @param yaw The rotation around the z-axis.
         * @return A quaternion representing the combined rotation.
         */
        [[nodiscard]]
        static Quaternion from_euler(F roll, F pitch, F yaw) {
            const F cr = cos(roll / F(2));
            const F sr = sin(roll / F(2));
            const F cp = cos(pitch / F(2));
            const F sp = sin(pitch / F(2));
            const F cy = cos(yaw / F(2));
            const F sy = sin(yaw / F(2));

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
         * @param rhs The right-hand side quaternion.
         * @return The product of this quaternion and @p rhs.
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
         * @param scalar The scalar value to multiply by.
         * @return The product of this quaternion and @p scalar.
         */
        [[nodiscard]]
        constexpr Quaternion operator*(F scalar) const noexcept {
            return Quaternion{w * scalar, x * scalar, y * scalar, z * scalar};
        }

        /**
         * @brief Scalar division.
         * @param scalar The scalar value to divide by.
         * @return The quotient of this quaternion and @p scalar.
         * @note Division by zero is undefined and will result in NaN or Inf components.
         */
        [[nodiscard]]
        constexpr Quaternion operator/(F scalar) const noexcept {
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

        constexpr Quaternion& operator*=(F scalar) noexcept {
            w *= scalar;
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        constexpr Quaternion& operator/=(F scalar) noexcept {
            w /= scalar;
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }
        /**
         * @brief Check if this quaternion is equal to another.
         * @param rhs The right-hand side quaternion.
         * @return true if the quaternions are equal, false otherwise.
         */
        [[nodiscard]]
        constexpr bool operator==(const Quaternion&) const noexcept = default;

        /**
         * @brief Squared norm.
         * @return The squared Euclidean norm of the quaternion.
         */
        [[nodiscard]]
        constexpr F norm_squared() const noexcept {
            return w * w + x * x + y * y + z * z;
        }

        /**
         * @brief Euclidean norm.
         * @return The Euclidean norm of the quaternion.
         */
        [[nodiscard]]
        F norm() const noexcept {
            return sqrt(norm_squared());
        }

        /**
         * @brief Conjugate: q* = (w, -x, -y, -z).
         * @return The conjugate of this quaternion.
         */
        [[nodiscard]]
        constexpr Quaternion conjugate() const noexcept {
            return Quaternion{w, -x, -y, -z};
        }

        /**
         * @brief Multiplicative inverse: q^-1 = q* / |q|^2.
         * @return The multiplicative inverse of this quaternion.
         */
        [[nodiscard]]
        Quaternion inverse() const {
            return conjugate() / norm_squared();
        }

        /**
         * @brief Return a unit quaternion (normalized).
         * @return A unit quaternion in the same direction as this quaternion.
         */
        [[nodiscard]]
        Quaternion normalized() const {
            return *this / norm();
        }

        /**
         * @brief Dot product of two quaternions.
         * @param rhs The right-hand side quaternion.
         * @return The dot product of this quaternion and @p rhs.
         */
        [[nodiscard]]
        constexpr F dot(const Quaternion& rhs) const noexcept {
            return w * rhs.w + x * rhs.x + y * rhs.y + z * rhs.z;
        }

        /**
         * @brief Rotate a 3D vector (vx, vy, vz) by this unit quaternion.
         * @param vx The x-component of the vector to rotate.
         * @param vy The y-component of the vector to rotate.
         * @param vz The z-component of the vector to rotate.
         * @return The rotated vector.
         */
        [[nodiscard]]
        constexpr Tuple<F, F, F> rotate(F vx, F vy, F vz) const noexcept {
            // q * v * q^-1, optimized for unit quaternions (inverse == conjugate)
            const Quaternion v{F(0), vx, vy, vz};
            const Quaternion result = *this * v * conjugate();
            return {result.x, result.y, result.z};
        }

        /**
         * @brief Convert to Euler angles (roll, pitch, yaw) in radians.
         * @return A tuple of the Euler angles (roll, pitch, yaw).
         */
        [[nodiscard]]
        Tuple<F, F, F> to_euler() const {
            // Roll (x-axis)
            const F sinr_cosp = F(2) * (w * x + y * z);
            const F cosr_cosp = F(1) - F(2) * (x * x + y * y);
            const F roll = atan2(sinr_cosp, cosr_cosp);

            // Pitch (y-axis)
            const F sinp = F(2) * (w * y - z * x);
            F pitch = (abs(sinp) >= F(1))
                ? copysign(PI_VALUE<F> / F(2), sinp)
                : asin(sinp);

            // Yaw (z-axis)
            const F siny_cosp = F(2) * (w * z + x * y);
            const F cosy_cosp = F(1) - F(2) * (y * y + z * z);
            const F yaw = atan2(siny_cosp, cosy_cosp);

            return {roll, pitch, yaw};
        }

        /**
         * @brief Extract axis and angle from a unit quaternion.
         * @return A tuple of the axis (x, y, z) and the rotation angle in radians.
         * @note The axis is normalized. If the angle is small, the axis may be arbitrary.
         */
        [[nodiscard]]
        Tuple<F, F, F, F> to_axis_angle() const {
            const F angle = F(2) * acos(w);
            const F s = sqrt(F(1) - w * w);
            return s < F(1e-8)
                ? Tuple{F(1), F(0), F(0), angle} // Arbitrary axis if angle is small
                : Tuple{x / s, y / s, z / s, angle};
        }
    };

    template <FloatingPoint F>
    inline constexpr Quaternion<F> Quaternion<F>::ZERO{0, 0, 0, 0};

    template <FloatingPoint F>
    inline constexpr Quaternion<F> Quaternion<F>::IDENTITY{1, 0, 0, 0};

    /**
     * @brief Scalar * Quaternion.
     * @param scalar The scalar value.
     * @param q The quaternion.
     * @return The product of @p scalar and @p q.
     */
    template <FloatingPoint F>
    [[nodiscard]]
    constexpr Quaternion<F> operator*(F scalar, const Quaternion<F>& q) noexcept {
        return q * scalar;
    }

    /**
     * @brief Spherical linear interpolation between two unit quaternions.
     * @param a The starting quaternion.
     * @param b The ending quaternion.
     * @param t The interpolation parameter in [0, 1].
     * @return The interpolated quaternion.
     */
    template <FloatingPoint F>
    [[nodiscard]]
    constexpr Quaternion<F> slerp(const Quaternion<F>& a, const Quaternion<F>& b, F t) {
        F cos_theta = a.dot(b);

        // If the dot product is negative, negate one to take the shorter arc.
        Quaternion<F> b2 = b;
        if (cos_theta < F(0)) {
            b2 = -b;
            cos_theta = -cos_theta;
        }

        // If quaternions are very close, fall back to normalized lerp.
        if (cos_theta > F(1) - F(1e-6)) {
            return (a * (F(1) - t) + b2 * t).normalized();
        }

        const F theta = acos(cos_theta);
        const F sin_theta = sin(theta);
        const F wa = sin((F(1) - t) * theta) / sin_theta;
        const F wb = sin(t * theta) / sin_theta;

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
    template <typename F, typename Char>
    struct Formatter<Quaternion<F>, Char> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(const Quaternion<F>& q, FormatContext& ctx) const {
            return format_to(ctx.out(), "({} + {}i + {}j + {}k)", q.w(), q.x(), q.y(), q.z());
        }
    };
}

template <typename F, typename Char>
struct stdx::fmt::formatter<Quaternion<F>, Char>: public Formatter<Quaternion<F>, Char> {};
