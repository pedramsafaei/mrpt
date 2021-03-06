/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2020, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */
#pragma once

#include <mrpt/core/bits_math.h>
#include <mrpt/core/format.h>
#include <mrpt/math/TPoseOrPoint.h>
#include <mrpt/math/math_frwds.h>  // CMatrixFixed
#include <cmath>
#include <vector>

namespace mrpt::math
{
/** \ingroup geometry_grp */
template <typename T>
struct TPoint2D_data
{
	/** X,Y coordinates */
	T x, y;
};

/** Base template for TPoint2D and TPoint2Df
 * \ingroup geometry_grp
 */
template <typename T>
struct TPoint2D_ : public TPoseOrPoint,
				   public TPoint2D_data<T>,
				   public internal::ProvideStaticResize<TPoint2D_<T>>
{
	enum
	{
		static_size = 2
	};
	/** Default constructor. Initializes to zeros  */
	constexpr TPoint2D_() : TPoint2D_data<T>{0, 0} {}
	/** Constructor from coordinates  */
	constexpr TPoint2D_(T xx, T yy) : TPoint2D_data<T>{xx, yy} {}

	/** Explicit constructor from coordinates.  */
	template <typename U>
	TPoint2D_(const TPoint2D_data<U>& p)
	{
		TPoint2D_data<T>::x = static_cast<T>(p.x);
		TPoint2D_data<T>::y = static_cast<T>(p.y);
	}

	/** Constructor from column vector. */
	template <typename U>
	TPoint2D_(const mrpt::math::CMatrixFixed<U, 2, 1>& m)
	{
		TPoint2D_data<T>::x = static_cast<T>(m[0]);
		TPoint2D_data<T>::y = static_cast<T>(m[1]);
	}

	/** Constructor from TPose2D, discarding phi.
	 * \sa TPose2D
	 */
	explicit TPoint2D_(const TPose2D& p);
	/**
	 * Constructor from TPoint3D, discarding z.
	 * \sa TPoint3D
	 */
	explicit TPoint2D_(const TPoint3D_<T>& p);
	/**
	 * Constructor from TPose3D, discarding z and the angular coordinates.
	 * \sa TPose3D
	 */
	explicit TPoint2D_(const TPose3D& p);

	/** Return a copy of this object using type U for coordinates */
	template <typename U>
	TPoint2D_<U> cast() const
	{
		return TPoint2D_<U>(static_cast<U>(this->x), static_cast<U>(this->y));
	}

	/** Coordinate access using operator[]. Order: x,y */
	T& operator[](size_t i)
	{
		switch (i)
		{
			case 0:
				return this->x;
			case 1:
				return this->y;
			default:
				throw std::out_of_range("index out of range");
		}
	}
	/** Coordinate access using operator[]. Order: x,y */
	constexpr T operator[](size_t i) const
	{
		switch (i)
		{
			case 0:
				return this->x;
			case 1:
				return this->y;
			default:
				throw std::out_of_range("index out of range");
		}
	}

	/**
	 * Transformation into vector.
	 */
	template <typename U>
	void asVector(std::vector<U>& v) const
	{
		v.resize(2);
		v[0] = static_cast<U>(this->x);
		v[1] = static_cast<U>(this->y);
	}

	bool operator<(const TPoint2D_& p) const;

	TPoint2D_& operator+=(const TPoint2D_& p)
	{
		this->x += p.x;
		this->y += p.y;
		return *this;
	}

	TPoint2D_& operator-=(const TPoint2D_& p)
	{
		this->x -= p.x;
		this->y -= p.y;
		return *this;
	}

	TPoint2D_& operator*=(T d)
	{
		this->x *= d;
		this->y *= d;
		return *this;
	}

	TPoint2D_& operator/=(T d)
	{
		ASSERT_(d != 0);
		this->x /= d;
		this->y /= d;
		return *this;
	}

	constexpr TPoint2D_ operator+(const TPoint2D_& p) const
	{
		return {this->x + p.x, this->y + p.y};
	}

	constexpr TPoint2D_ operator-(const TPoint2D_& p) const
	{
		return {this->x - p.x, this->y - p.y};
	}

	constexpr TPoint2D_ operator*(T d) const
	{
		return {d * this->x, d * this->y};
	}
	constexpr TPoint2D_ operator/(T d) const
	{
		ASSERT_(d != 0);
		return {this->x / d, this->y / d};
	}
	/** Returns a human-readable textual representation of the object (eg:
	 * "[0.02 1.04]" )
	 * \sa fromString
	 */
	void asString(std::string& s) const
	{
		s = mrpt::format("[%f %f]", this->x, this->y);
	}

	std::string asString() const
	{
		std::string s;
		asString(s);
		return s;
	}

	/** Set the current object value from a string generated by 'asString' (eg:
	 * "[0.02 1.04]" )
	 * \sa asString
	 * \exception std::exception On invalid format
	 */
	void fromString(const std::string& s);

	static TPoint2D_ FromString(const std::string& s)
	{
		TPoint2D_ o;
		o.fromString(s);
		return o;
	}

	/** Squared norm: |v|^2 = x^2+y^2 */
	T sqrNorm() const { return this->x * this->x + this->y * this->y; }

	/** Point norm: |v| = sqrt(x^2+y^2) */
	T norm() const { return std::sqrt(sqrNorm()); }
};

/**
 * Lightweight 2D point. Allows coordinate access using [] operator.
 * \sa mrpt::poses::CPoint2D
 * \ingroup geometry_grp
 */
using TPoint2D = TPoint2D_<double>;
using TPoint2Df = TPoint2D_<float>;

/** Useful type alias for double 2-vectors */
using TVector2D = TPoint2D;
/** Useful type alias for float 2-vectors */
using TVector2Df = TPoint2Df;

/** Exact comparison between 2D points */
template <typename T>
constexpr bool operator==(const TPoint2D_<T>& p1, const TPoint2D_<T>& p2)
{
	return (p1.x == p2.x) && (p1.y == p2.y);  //-V550
}

/**  Exact comparison between 2D points */
template <typename T>
constexpr bool operator!=(const TPoint2D_<T>& p1, const TPoint2D_<T>& p2)
{
	return (p1.x != p2.x) || (p1.y != p2.y);  //-V550
}

}  // namespace mrpt::math

namespace mrpt::typemeta
{
// Specialization must occur in the same namespace
MRPT_DECLARE_TTYPENAME_NO_NAMESPACE(TPoint2D, mrpt::math)
}  // namespace mrpt::typemeta
