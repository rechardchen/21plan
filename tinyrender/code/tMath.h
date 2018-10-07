#pragma once

namespace TR
{
	template<size_t DIM, typename T>
	struct Vec
	{
		Vec() = default;
		T& operator[](size_t i) { return mData[i]; }
		const T& operator[] (size_t i) const { return mData[i]; }

		T m[DIM];
	};

	// Vector operators
	template<size_t DIM, typename T>
	Vec<DIM, T> operator*(const Vec<DIM, T>& lhs, const Vec<DIM, T>& rhs)
	{
		Vec<DIM, T> ret;
		for (size_t i = 0; i < DIM; ++i)
		{
			ret[i] = lhs[i] * rhs[i];
		}
		return ret;
	}
	template<size_t DIM, typename T, typename U>
	Vec<DIM, T> operator*(U lhs, const Vec<DIM, T>& rhs)
	{
		Vec<DIM, T> ret;
		for (size_t i = 0; i < DIM; ++i)
		{
			ret[i] = lhs * rhs[i];
		}
		return ret;
	}
	template<size_t DIM, typename T, typename U>
	Vec<DIM, T> operator*(const Vec<DIM, T>& lhs, U rhs)
	{
		return operator*(rhs, lhs);
	}
	template<size_t DIM, typename T>
	Vec<DIM, T>& operator*= (Vec<DIM, T>& lhs, const Vec<DIM, T>& rhs)
	{
		for (size_t i = 0; i < DIM; ++i) lhs[i] *= rhs[i];
		return lhs;
	}
	template<size_t DIM, typename T, typename U>
	Vec<DIM, T>& operator*= (Vec<DIM, T>& lhs, U rhs)
	{
		for (size_t i = 0; i < DIM; ++i) lhs[i] *= rhs;
		return lhs;
	}

	template<size_t DIM, typename T>
	Vec<DIM, T> operator+(const Vec<DIM, T>& lhs, const Vec<DIM, T>& rhs)
	{
		Vec<DIM, T> ret;
		for (size_t i = 0; i < DIM; ++i)
		{
			ret[i] = lhs[i] + rhs[i];
		}
		return ret;
	}
	template<size_t DIM, typename T, typename U>
	Vec<DIM, T> operator+(const Vec<DIM, T>& lhs, U rhs)
	{
		Vec<DIM, T> ret;
		for (size_t i = 0; i < DIM; ++i) ret[i] = lhs[i] + rhs;
		return ret;
	}
	template<size_t DIM, typename T, typename U>
	Vec<DIM, T> operator+(U lhs, const Vec<DIM, T>& rhs)
	{
		return operator+(rhs, lhs);
	}
	template <size_t DIM, typename T>
	Vec<DIM, T>& operator+=(Vec<DIM, T>& lhs, const Vec<DIM, T>& rhs)
	{
		for (size_t i = 0; i < DIM; ++i)
		{
			lhs[i] += rhs[i];
		}
		return lhs;
	}
	template <size_t DIM, typename T, typename U>
	Vec<DIM, T>& operator += (Vec<DIM, T>& lhs, U rhs)
	{
		for (size_t i = 0; i < DIM; ++i) lhs[i] += rhs;
		return lhs;
	}

	template<size_t DIM, typename T>
	Vec<DIM, T> operator-(const Vec<DIM, T>& lhs, const Vec<DIM, T>& rhs)
	{
		Vec<DIM, T> ret;
		for (size_t i = 0; i < DIM; ++i)
		{
			ret[i] = lhs[i] - rhs[i];
		}
		return ret;
	}
	template<size_t DIM, typename T, typename U>
	Vec<DIM, T> operator-(const Vec<DIM, T>& lhs, U rhs)
	{
		Vec<DIM, T> ret;
		for (size_t i = 0; i < DIM; ++i) ret[i] = lhs[i] - rhs;
		return ret;
	}
	template<size_t DIM, typename T, typename U>
	Vec<DIM, T> operator-(U lhs, const Vec<DIM, T>& rhs)
	{
		Vec<DIM, T> ret;
		for (size_t i = 0; i < DIM; ++i) ret[i] = lhs - rhs[i];
		return ret;
	}
	template<size_t DIM, typename T>
	Vec<DIM, T> operator-(const Vec<DIM, T>& lhs)
	{
		Vec<DIM, T> ret;
		for (size_t i = 0; i < DIM; ++i) ret[i] = -lhs[i];
		return ret;
	}
	template <size_t DIM, typename T>
	Vec<DIM, T>& operator-=(Vec<DIM, T>& lhs, const Vec<DIM, T>& rhs)
	{
		for (size_t i = 0; i < DIM; ++i)
		{
			lhs[i] -= rhs[i];
		}
		return lhs;
	}
	template <size_t DIM, typename T, typename U>
	Vec<DIM, T>& operator -= (Vec<DIM, T>& lhs, U rhs)
	{
		for (size_t i = 0; i < DIM; ++i) lhs[i] -= rhs;
		return lhs;
	}

	template<size_t DIM, typename T>
	Vec<DIM, T> operator/(const Vec<DIM, T>& lhs, const Vec<DIM, T>& rhs)
	{
		Vec<DIM, T> ret;
		for (size_t i = 0; i < DIM; ++i)
		{
			ret[i] = lhs[i] / rhs[i];
		}
		return ret;
	}
	template<size_t DIM, typename T, typename U>
	Vec<DIM, T> operator/(const Vec<DIM, T>& lhs, U rhs)
	{
		Vec<DIM, T> ret;
		for (size_t i = 0; i < DIM; ++i) ret[i] = lhs[i] / rhs;
		return ret;
	}
	template<size_t DIM, typename T, typename U>
	Vec<DIM, T> operator/(U lhs, const Vec<DIM, T>& rhs)
	{
		Vec<DIM, T> ret;
		for (size_t i = 0; i < DIM; ++i) ret[i] = lhs / rhs[i];
		return ret;
	}
	template <size_t DIM, typename T>
	Vec<DIM, T>& operator/=(Vec<DIM, T>& lhs, const Vec<DIM, T>& rhs)
	{
		for (size_t i = 0; i < DIM; ++i)
		{
			lhs[i] /= rhs[i];
		}
		return lhs;
	}
	template <size_t DIM, typename T, typename U>
	Vec<DIM, T>& operator /= (Vec<DIM, T>& lhs, U rhs)
	{
		for (size_t i = 0; i < DIM; ++i) lhs[i] /= rhs;
		return lhs;
	}

	template<size_t LEN, size_t DIM, typename T>
	Vec<LEN, T> embed(const Vec<DIM, T>& v, T fill = 0)
	{
		Vec<LEN, T> ret;
		for (size_t i = 0; i < LEN; ++i)
		{
			ret[i] = i < DIM ? v[i] : fill;
		}
		return ret;
	}
	template<size_t LEN,size_t DIM, typename T>
	Vec<LEN, T> proj(const Vec<DIM, T>& v)
	{
		Vec<LEN, T> ret;
		for (size_t i = 0; i < LEN && i < DIM; ++i)
		{
			ret[i] = v[i];
		}
		return ret;
	}

	//Vec2,3,4
	template<typename T>
	struct Vec<2, T>
	{
		Vec() = default;
		Vec(T x, T y) : x(x), y(y) {}

		T& operator[](size_t i) { return i == 0 ? x : y; }
		const T& operator[](size_t i) const { return i == 0 ? x : y; }
		T x, y;
	};

	template<typename T>
	struct Vec<3, T>
	{
		Vec() = default;
		Vec(T x, T y, T z) :x(x), y(y), z(z) {}

		T& operator[](size_t i) { return i == 0 ? x : i == 1 ? y : z; }
		const T& operator[](size_t i) const {
			return i == 0 ? x : i == 1 ? y : z;
		}

		T x, y, z;
	};

	template<typename T>
	Vec<3, T> cross(const Vec<3, T>& v1, const Vec<3, T>& v2)
	{
		return Vec<3, T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
	}
	
	//Matrix
	template<size_t NROW, size_t NCOL, typename T> struct Mat;

	template<size_t DIM, typename T>
	struct dt
	{
		static T det(const Mat<DIM, DIM, T>& m)
		{
			T ret = 0;
			for (size_t i = 0; i < DIM; ++i)
			{
				ret += m[0][i] * m.cofactor(0, i);
			}
			return ret;
		}
	};
	template<typename T>
	struct dt<1, T>
	{
		static T det(const Mat<1, 1, T>& m)
		{
			return m[0][0];
		}
	};
	template<size_t NROW, size_t NCOL, typename T>
	struct Mat
	{
		Mat() = default;
		Vec<NCOL, T>& operator[](size_t i) { return rows[i]; }
		const Vec<NCOL, T>& operator[](size_t i) const { return rows[i]; }
		Vec<NROW, T> col(size_t i) const
		{
			Vec<NROW, T> ret;
			for (size_t k = 0; k < NROW; ++k)ret[k] = rows[k][i];
			return ret;
		}
		void setCol(size_t i, const Vec<NROW, T>& v)
		{
			for (size_t k = 0; k < NROW; ++k) rows[k][i] = v[k];
		}
		
		static Mat<NROW, NCOL, T> identity()
		{
			Mat<NROW, NCOL, T> ret;
			for (size_t r = 0; r < NROW; ++r)
				for (size_t c = 0; c < NCOL; ++c)
					ret[r][c] = (r == c) ? 1 : 0;
			return ret;
		}

		T det() const
		{
			static_assert(NROW == NCOL, "only square matrix has determinant");
			return dt<NROW, T>::det(*this);
		}

		Mat<NROW - 1, NCOL - 1, T> getMinor(size_t i, size_t j) const
		{
			Mat<NROW - 1, NCOL - 1, T> ret;
			for (size_t r = 0; r < NROW-1; ++r)
			{
				for (size_t c = 0; c < NCOL - 1; ++c)
				{
					ret[r][c] = rows[r < i ? r : r + 1][c < j ? c : c + 1];
				}
			}
			return ret;
		}

		T cofactor(size_t r, size_t c) const
		{
			if ((r + c) % 2)
			{
				return -dt<NROW - 1, T>::det(getMinor(r, c));
			}
			else
			{
				return dt<NROW - 1£¬T>::det(getMinor(r, c));
			}
		}
		
		Mat<NROW, NCOL, T> adjugate() const
		{
			Mat<NROW, NCOL, T> ret;
			for (size_t r = 0; r < NROW; ++r)
			{
				for (size_t c = 0; c < NCOL; ++c)
				{
					ret[r][c] = cofactor(r, c);
				}
			}
			return ret;
		}
		Mat<NCOL, NROW, T> transpose() const
		{
			Mat<NCOL, NROW> ret;
			for (size_t r = 0; r < NCOL; ++r)
			{
				for (size_t c = 0; c < NROW; ++c)
				{
					ret[r][c] = rows[c][r];
				}
			}
			return ret;
		}

		Mat<NROW, NCOL, T> inv() const
		{
			static_assert(NROW == NCOL, "only square matrix has invert");
			auto adjMat = adjugate();
			T tmp = rows[0] * adjMat[0];
			return 1 / tmp * adjMat.transpose();
		}
		Vec<NCOL, T> rows[NROW];
	};
	
	template<size_t M, size_t N, typename T> Vec<M, T> operator*(const Mat<M, N, T>& m, const Vec<N, T>& v)
	{
		Vec<M, T> ret;
		for (size_t i = 0; i < M; ++i)
		{
			T tmp = 0;
			for (size_t j = 0; j < N; ++j)
			{
				tmp += m[i][j] * v[j];
			}
			ret[i] = tmp;
		}
		return ret;
	}

	template<size_t M, size_t N, size_t K, typename T> Mat<M, K, T> operator*(const Mat<M, N, T>& m1, const Mat<N, K, T>& m2)
	{
		Mat<M, K, T> ret;
		for (size_t i = 0; i < M; ++i)
		{
			for (size_t j = 0; j < K; ++j)
			{
				T tmp = 0;
				for (size_t n = 0; n < N; ++n)
				{
					tmp += m1[i][n] * m2[n][j];
				}
				ret[i][j] = tmp;
			}
		}
		return ret;
	}

	typedef Vec<2, int> Vec2i;
	typedef Vec<2, float> Vec2f;
	typedef Vec<3, int> Vec3i;
	typedef Vec<3, float> Vec3f;
	typedef Vec<4, float> Vec4f;
	typedef Mat<3, 3, float> Mat3;
	typedef Mat<4, 4, float> Mat4;

	////////////////////////////// utility functions /////////////////////////
	template <typename T>
	T clamp(T val, T min, T max)
	{
		return val < min ? min : val > max ? max : val;
	}
}