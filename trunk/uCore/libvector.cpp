#include "libvector.hpp"
/*
* ToDo:
* Rewrite functions from unsafe c style to C++ safe style
* Rewrite all legacy, make it more readable
* Replace this single file into many files
* Re-check and re-write (if needed) all hard mathematical & SSE-using functions
*/

template <typename T>
bool _valid(const _vec3<T>* v)
{
	T x; // edx
	T y; // ecx
	T z; // ecx
	bool result; // al

	x = v->x;
	result = false;
	if ((LODWORD(v->x) & 0x7F800000) != 0x7F800000 && (((LODWORD(x) + 0x800000) & 0x7FBFFFFF) != 0 || x == NAN))
	{
		y = v->y;
		if ((LODWORD(y) & 0x7F800000) != 0x7F800000 && (((LODWORD(y) + 0x800000) & 0x7FBFFFFF) != 0 || y == NAN))
		{
			z = v->z;
			if ((LODWORD(z) & 0x7F800000) != 0x7F800000 && (((LODWORD(z) + 0x800000) & 0x7FBFFFFF) != 0 || z == NAN))
				return true;
		}
	}
	return result;
}

class implement {
	float deg2rad(float val) {
		return val * std::numbers::pi_v<float> / static_cast<float>(180.0);
	}
	float rad2deg(float val) {
		return val * (180.0 / std::numbers::pi);
	}
	char* p_memmove(char* dst0, char* src0, unsigned int length)
	{
		unsigned int v3; // r10d
		char* v5; // r9
		int v6; // ecx
		char v7; // al
		unsigned __int64 j; // r8
		__int64 v9; // rax
		int v10; // ecx
		char* v11; // rdx
		char v12; // al
		char* v14; // rcx
		char* v15; // r8
		int v16; // edx
		char v17; // al
		unsigned __int64 i; // r9
		__int64 v19; // rax
		int v20; // edx
		char v21; // al

		v3 = length;
		v5 = dst0;
		if (dst0 >= src0)
		{
			v14 = &src0[length];
			v15 = &dst0[length];
			if ((((unsigned __int8)src0 | (unsigned __int8)dst0) & 7) != 0)
			{
				if ((((unsigned __int8)src0 ^ (unsigned __int8)dst0) & 7) != 0 || v3 <= 8)
					v16 = v3;
				else
					v16 = (unsigned __int8)src0 & 7;
				v3 -= v16;
				do
				{
					v17 = *--v14;
					*--v15 = v17;
					--v16;
				} while (v16);
			}
			for (i = (unsigned __int64)v3 >> 3; (unsigned long)i; LODWORD(i) = i - 1)
			{
				v19 = *((unsigned long long*)v14 - 1);
				v14 -= 8;
				v15 -= 8;
				*(unsigned long long*)v15 = v19;
			}
			v20 = v3 & 7;
			if ((v3 & 7) != 0)
			{
				do
				{
					v21 = *--v14;
					*--v15 = v21;
					--v20;
				} while (v20);
			}
			return dst0;
		}
		if ((((unsigned __int8)src0 | (unsigned __int8)dst0) & 7) != 0)
		{
			if ((((unsigned __int8)src0 ^ (unsigned __int8)dst0) & 7) != 0 || length < 8)
				v6 = length;
			else
				v6 = 8 - ((unsigned __int8)src0 & 7);
			v3 = length - v6;
			do
			{
				v7 = *src0;
				++v5;
				++src0;
				*(v5 - 1) = v7;
				--v6;
			} while (v6);
		}
		for (j = (unsigned __int64)v3 >> 3; (unsigned long)j; LODWORD(j) = j - 1)
		{
			v9 = *(unsigned long long*)src0;
			v5 += 8;
			src0 += 8;
			*((unsigned long long*)v5 - 1) = v9;
		}
		v10 = v3 & 7;
		if ((v3 & 7) == 0)
			return dst0;
		v11 = (char*)(src0 - v5);
		do
		{
			v12 = (v5++)[(unsigned long long)v11];
			*(v5 - 1) = v12;
			--v10;
		} while (v10);
		return dst0;
	}

};

class _vector {     //////////// recheck all this!!!
	float x, y, w, z;
	__m128 m;
	_vector();
	_vector(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		w = _w;
		z = _z;
	}
	_vector(const _vector* v)
	{
		*this = *v;
	}
	_vector(const _vec3<float>* v)
	{
		float v3; // xmm0_4

		*this = *(_vector*)&v->x;
		//*this = *static_cast<_vector*> ( &v->x );
		v3 = logf(-1.0);
		this->w = v3;
	}

	float dp3(const _vector* v, const _vector* d)
	{
		__m128 result; // xmm0

		result = _mm_mul_ps(v->m, d->m);
		return result.m128_f32[0] + (float)(_mm_shuffle_ps(result, result, 85).m128_f32[0] + _mm_shuffle_ps(result, result, 170).m128_f32[0]);
	}
	float dp3(const _vector* v)
	{
		__m128 result; // xmm0

		result = _mm_mul_ps(v->m, this->m);
		return result.m128_f32[0] + (float)(_mm_shuffle_ps(result, result, 85).m128_f32[0] + _mm_shuffle_ps(result, result, 170).m128_f32[0]);
	}
	_vector* set(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		w = _w;
		z = _z;
		return this;
	}
	_vector* sub(const _vector* v)
	{
		this->m = _mm_sub_ps(this->m, v->m);
		return this;
	}
	_vector* sub(const _vector* a, const _vector* v)
	{
		this->m = _mm_sub_ps(a->m, v->m);
		return this;
	}
	float distance3sq(const _vector* v)
	{
		__m128 v2; // xmm0
		__m128 v3; // xmm0

		v2 = _mm_sub_ps(this->m, v->m);
		v3 = _mm_mul_ps(v2, v2);
		return v3.m128_f32[0] + (float)(_mm_shuffle_ps(v3, v3, 85).m128_f32[0] + _mm_shuffle_ps(v3, v3, 170).m128_f32[0]);
	}
	_vector* operator= (const _vector* v)
	{
		*this = *v;
		return this;
	}
	_vector* max(const _vector* v)
	{
		this->m = _mm_max_ps(this->m, v->m);
		return this;
	}
	_vector* min(const _vector* v)
	{
		this->m = _mm_min_ps(this->m, v->m);
		return this;
	}
	_vector* mul(const _vector* a, const _vector* v)
	{
		this->m = _mm_mul_ps(a->m, v->m);
		return this;
	}
	_vector* set(const _vector* v)
	{
		*this = *v;
		return this;
	}
	_vector* splat_x(const _vector* v)
	{
		this->m = _mm_shuffle_ps(v->m, v->m, 0);
		return this;
	}
	_vector* splat_y(const _vector* v)
	{
		this->m = _mm_shuffle_ps(v->m, v->m, 85);
		return this;
	}
	_vector* splat_z(const _vector* v)
	{
		this->m = _mm_shuffle_ps(v->m, v->m, 170);
		return this;
	}
	float distance3(const _vector* v)
	{
		__m128 v2; // xmm0
		__m128 v3; // xmm0

		v2 = _mm_sub_ps(this->m, v->m);
		v3 = _mm_mul_ps(v2, v2);
		return sqrtf(v3.m128_f32[0] + (float)(_mm_shuffle_ps(v3, v3, 85).m128_f32[0] + _mm_shuffle_ps(v3, v3, 170).m128_f32[0]));
	}
	float get_angle_y()
	{
		float result;
		int v3;

		if (!_valid<float>((const _vec3<float> *)this))
			debug::fail(
				"_valid(vec3())",
				"d:\\trunk\\src\\ucore\\libvector.h",
				"_vector::get_angle_y",
				(const char*)0x3F,
				v3);
		result = 0.0;
		if (this->x != 0.0 || this->z != 0.0)
			return atan2f(this->x, this->z);
		return result;
	}
	float len3()
	{
		__m128 result; // xmm0

		result = _mm_mul_ps(this->m, this->m);
		return sqrtf(result.m128_f32[0] + (float)(_mm_shuffle_ps(result, result, 85).m128_f32[0] + _mm_shuffle_ps(result, result, 170).m128_f32[0]));
	}
	float len3sq()
	{
		__m128 result; // xmm0

		result = _mm_mul_ps(this->m, this->m);
		return result.m128_f32[0] + (float)(_mm_shuffle_ps(result, result, 85).m128_f32[0] + _mm_shuffle_ps(result, result, 170).m128_f32[0]);
	}
	_vector* negate3(const _vector* a)
	{
		this->m = _mm_xor_ps(a->m, MM_FPSIGN_PS_3_);
		return this;
	}
	float nrm3m()
	{
		__m128 result; // xmm3

		result = _mm_mul_ps(this->m, this->m);
		result.m128_f32[0] = sqrtf(
			result.m128_f32[0]
			+ (float)(_mm_shuffle_ps(result, result, 85).m128_f32[0] + _mm_shuffle_ps(result, result, 170).m128_f32[0]));
		this->m = _mm_div_ps(this->m, _mm_shuffle_ps(result, result, 0));
		return result.m128_f32[0];
	}
	_vector* set(const _vec3<float>* vec)
	{
		*this = *(_vector*)&vec->x;
		float v3 = logf(-1.0); // NAN
		this->w = v3;
		return this;
	}
	_vector* vec3() {
		return this;
	}
	bool equal(const _vector* vector)
	{
		return _mm_movemask_ps(_mm_cmpeq_ps(vector->m, this->m)) == 15;
	}
	__m128* _nr1_ss(__m128* result, const __m128* f, const __m128* a)
	{
		__m128 f2 = _mm_mul_ss(*f, *f);           // f²
		__m128 af2 = _mm_mul_ss(*a, f2);           // a * f²
		__m128 t = _mm_sub_ss(_mm_set_ss(3.0f), af2);  // 3.0 - a*f²
		__m128 m = _mm_mul_ss(*f, t);            // f * (3.0 - a*f²)
		*result = _mm_mul_ss(m, _mm_set_ss(0.5f));    // * 0.5

		return result;
	}
	_vector* cross3(const _vector* v0, const _vector* v1)
	{

		if (v0 == this || v1 == this)
			debug::fail(
				"(&v0!=this) && (&v1!=this)",
				"d:\\trunk\\src\\ucore\\libvector_pc.h",
				"_vector::cross3",
				(const char*)0x228,
				0);
		this->m = _mm_sub_ps(
			_mm_mul_ps(_mm_shuffle_ps(v0->m, v0->m, 201), _mm_shuffle_ps(v1->m, v1->m, 210)),
			_mm_mul_ps(_mm_shuffle_ps(v0->m, v0->m, 210), _mm_shuffle_ps(v1->m, v1->m, 201)));
		return this;
	}
	_vector* mad(const _vector* p, const _vector* d, float s)
	{
		__m128 v4; // xmm0

		v4.m128_f32[0] = s;
		this->m = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(v4, v4, 0), d->m), p->m);
		return this;
	}
	_vector* nrm3()
	{
		// 1. Квадраты компонент: [x*x, y*y, z*z, w*w]
		__m128 sq = _mm_mul_ps(this->m, this->m);

		// 2. Скалярное произведение с самим собой: x² + y² + z²
		// 85  = _MM_SHUFFLE(1,1,1,1) — broadcast y
		// 170 = _MM_SHUFFLE(2,2,2,2) — broadcast z
		float len_sq = sq.m128_f32[0]
			+ _mm_shuffle_ps(sq, sq, 85).m128_f32[0]
			+ _mm_shuffle_ps(sq, sq, 170).m128_f32[0];

		// 3. Приближенный 1/√(len_sq)
		float inv_len = 1.0f / sqrtf(len_sq);

		// 4. Уточнение Ньютона-Рафсона: inv * (3.0 - len_sq * inv²) * 0.5
		inv_len = inv_len * (3.0f - len_sq * inv_len * inv_len) * 0.5f;

		// 5. Умножаем весь вектор на 1/длину (broadcast через set1_ps)
		__m128 scale = _mm_set1_ps(inv_len);
		this->m = _mm_mul_ps(scale, this->m);

		return this;
	}
	_vector* set_w0()
	{
		this->m = _mm_and_ps(this->m, MM_fff0);
		return this;
	}
	_vector* set_w1()
	{
		this->m = _mm_blend_ps(this->m, _mm_setr_ps(0, 0, 0, 1.0f), 0x8); // w = 1.0
		return this;
	}
	_vector* set_zero()
	{
		*this = 0;
		return this;
	}
	_vector* lerp(const _vector* p1, const _vector* p2, float t)
	{
		__m128 v4; // xmm0

		v4.m128_f32[0] = t;
		this->m = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(p2->m, p1->m), _mm_shuffle_ps(v4, v4, 0)), p1->m);
		return this;
	}
	__m128* _hadd2_xz(__m128* result, const __m128* a)
	{
		*result = *a;
		result->m128_f32[0] += result->m128_f32[2];
		return result;
	}

	//MAYBE WRONG
	void get_angles_xy(float* _x, float* _y)
	{
		float y; // xmm1_4
		__m128 v6; // xmm1

		if (this->x == 0.0 && this->z == 0.0)
		{
			*_y = 0.0;
			y = this->y;
			if (y == 0.0)
			{
				*_x = 0.0;
			}
			else if (y <= 0.0)
			{
				*_x = std::numbers::pi / 2;
			}
			else
			{
				*_x = -(std::numbers::pi / 2);
			}
		}
		else
		{
			*_y = atan2f(this->x, this->z);

			__m128 sq = _mm_mul_ps(this->m, this->m);
			float xz_len = sqrtf(
				sq.m128_f32[0] +
				_mm_shuffle_ps(sq, sq, _MM_SHUFFLE(2, 2, 2, 2)).m128_f32[0]
			);

			float neg_y = -this->y;
			*_x = atan2f(neg_y, xz_len);
		}
	}
	float len2_xz()
	{
		__m128 v1; // xmm0

		v1 = _mm_mul_ps(this->m, this->m);
		return sqrtf(v1.m128_f32[0] + _mm_shuffle_ps(v1, v1, 170).m128_f32[0]);
	}
	_vector* mul(float s)
	{
		__m128 v2; // xmm0

		v2.m128_f32[0] = s;
		this->m = _mm_mul_ps(_mm_shuffle_ps(v2, v2, 0), this->m);
		return this;
	}
	float nrm3ms()
	{
		__m128 v1; // xmm3
		float result; // xmm0_4

		v1 = _mm_mul_ps(this->m, this->m);
		v1.m128_f32[0] = sqrtf(
			v1.m128_f32[0]
			+ (float)(_mm_shuffle_ps(v1, v1, 85).m128_f32[0] + _mm_shuffle_ps(v1, v1, 170).m128_f32[0]));
		result = v1.m128_f32[0];
		this->m = _mm_div_ps(this->m, _mm_shuffle_ps(v1, v1, 0));
		if (v1.m128_f32[0] == 0.0)
			*this = 0;
		return result;
	}
	_vector* set(const _vec4<float>* v)
	{
		this->m = _mm_setr_ps(v->x, v->y, v->z, v->w);
		return this;
	}
	_vector* set_angles_xy(float _x, float _y)
	{
		float v4; // xmm9_4
		float v5; // xmm8_4
		float v6; // xmm6_4
		float v7; // xmm0_4

		float sin_pitch = sinf(_x);
		float cos_pitch = cosf(_x);
		float sin_yaw = sinf(_y);
		float cos_yaw = cosf(_y);

		this->x = sin_yaw * cos_pitch;
		this->y = -sin_pitch;           // вместо LODWORD ^ _xmm
		this->z = cos_yaw * cos_pitch;

		return this;
	}
	_vector* vec4()
	{
		return this;
	}
	_vector* div(const _vector* v)
	{
		this->m = _mm_div_ps(this->m, v->m);
		return this;
	}
	// ... no comments
	void generate_orthonormal_basis_normalized(_vector* dir, _vector* up, _vector* right)
	{
		// ===== Нормализация dir (sqrt + 1x Newton-Raphson) =====
		float len_sq = dir->x * dir->x + dir->y * dir->y + dir->z * dir->z;
		float inv_len = 1.0f / sqrtf(len_sq);
		// Уточняем: x1 = x0 * (3.0 - a*x0²) * 0.5
		inv_len = inv_len * (3.0f - len_sq * inv_len * inv_len) * 0.5f;

		dir->x *= inv_len;
		dir->y *= inv_len;
		dir->z *= inv_len;
		dir->w *= inv_len;

		// ===== Выбор опорного вектора в зависимости от направления =====
		// Если dir смотрит почти строго вверх/вниз (|dir.y| ≈ 1), 
		// мировой UP (0,1,0) даст вырожденный базис — используем (0,0,1)
		const float epsilon = 0.00001f;

		if (fabsf(fabsf(dir->y) - 1.0f) >= epsilon)
		{
			// --- Опорный UP = (0, 1, 0) ---
			up->x = 0.0f;
			up->y = 1.0f;
			up->z = 0.0f;
			up->w = 0.0f;

			// Проекция на XZ: right = normalize(cross(up, dir))
			float len_xz_sq = dir->x * dir->x + dir->z * dir->z;
			if (len_xz_sq < 0.0f)
				debug::fatal("sqrt: x < 0 [%f]", len_xz_sq);

			float len_xz = sqrtf(len_xz_sq);
			float inv_len_xz = 1.0f / len_xz;

			right->x = dir->z * inv_len_xz;
			right->y = 0.0f;
			right->z = -(dir->x * inv_len_xz);  // XOR _xmm = смена знака
			right->w = 0.0f;

			// up = cross(dir, right) — гарантирует строгую ортогональность
			up->x = dir->y * right->z;
			up->y = dir->z * right->x - dir->x * right->z;
			up->z = -(dir->y * right->x);
			up->w = 0.0f;
		}
		else
		{
			// --- Опорный UP = (0, 0, 1) ---
			up->x = 0.0f;
			up->y = 0.0f;
			up->z = 1.0f;
			up->w = 0.0f;

			// Проекция на XY: right = normalize(cross(up, dir))
			float len_xy_sq = dir->x * dir->x + dir->y * dir->y;
			if (len_xy_sq < 0.0f)
				debug::fatal("sqrt: x < 0 [%f]", len_xy_sq);

			float len_xy = sqrtf(len_xy_sq);
			float inv_len_xy = 1.0f / len_xy;

			right->x = -(dir->y * inv_len_xy);  // XOR _xmm = смена знака
			right->y = dir->x * inv_len_xy;
			right->z = 0.0f;
			right->w = 0.0f;

			// up = cross(dir, right)
			up->x = -(dir->z * right->y);
			up->y = dir->z * right->x;
			up->z = dir->x * right->y - dir->y * right->x;
			up->w = 0.0f;
		}
	}
	_vector* mad(const _vector* d, float s)
	{
		__m128 v3; // xmm0

		v3.m128_f32[0] = s;
		this->m = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(v3, v3, 0), d->m), this->m);
		return this;
	}
	_vector* max(const _vector* v1, const _vector* v2)
	{
		this->m = _mm_max_ps(v1->m, v2->m);
		return this;
	}
	_vector* max_1(const _vector* v1, const _vector* v2)
	{
		_vector v3; // xmm0

		v3 = *v1;
		v3.x = fmaxf(*((float*)&v1), v2->x);
		*this = v3;
		return this;
	}
	_vector* min(const _vector* v1, const _vector* v2)
	{
		this->m = _mm_min_ps(v1->m, v2->m);
		return this;
	}
	_vector* min_1(const _vector* v1, const _vector* v2)
	{
		_vector v3; // xmm0

		v3 = *v1;
		v3.x = fminf(*((float*)&v1), v2->x);
		*this = v3;
		return this;
	}
	_vector* mul(const _vector* a, float s)
	{
		__m128 v3; // xmm0

		v3.m128_f32[0] = s;
		this->m = _mm_mul_ps(_mm_shuffle_ps(v3, v3, 0), a->m);
		return this;
	}
	_vector* set_all(float s)
	{
		__m128 v2; // xmm0

		v2.m128_f32[0] = s;
		this->m = _mm_shuffle_ps(v2, v2, 0);
		return this;
	}
	_vector* add(const _vector* v)
	{
		this->m = _mm_add_ps(v->m, this->m);
		return this;
	}
	// URANDOM::SIMPLE???
	_vector* random_dir(urandom::simple* R)
	{
		volatile int v3; // eax
		volatile int v4; // eax
		__m128 v5; // xmm6
		float v6; // xmm8_4
		float v7; // xmm7_4
		float v8; // xmm6_4
		float v9; // xmm0_4

		v5 = (__m128)(1.0f);
		v3 = 214013 * R->holdrand + 2531011;
		R->holdrand = v3;
		this->z = (float)((float)((float)((v3 >> 16) & 0x7FFF) * 0.000030518509) * 2.0) - 1.0;
		v4 = 214013 * R->holdrand + 2531011;
		R->holdrand = v4;
		v5.m128_f32[0] = 1.0 - (float)(this->z * this->z);
		v6 = (float)((float)((v4 >> 16) & 0x7FFF) * 0.000030518509) * 6.2831855;
		if (v5.m128_f32[0] < 0.0)
			debug::fatal("sqrt: x < 0 [%f]", (v5.m128_f32[0]));
		v7 = _mm_sqrt_ps(v5).m128_f32[0];
		v8 = sinf(v6);
		v9 = cosf(v6);
		this->y = v8 * v7;
		this->x = v9 * v7;
		return this;
	}
	_vector* random_dir(const _vector* cone_axis, float cone_angle, urandom::simple* R)
	{
		volatile int v7; // ebx
		_vector* v8; // rdi
		float v9; // xmm0_4
		__m128 v11; // xmm7
		__m128 v12; // xmm3
		__m128 v13; // xmm3
		__m128 v14; // xmm2
		float v15; // xmm1_4
		_vector rnd; // [rsp+20h] [rbp-48h] BYREF

		if (cone_angle >= 2.3561945)
			debug::fail(
				"cone_angle<M_PI*(3.f/4.f)",
				"d:\\trunk\\src\\ucore\\libvector.h",
				"_vector::random_dir",
				(const char*)0x35,
				rnd.m.m128_i32[0]);
		v7 = 214013 * R->holdrand + 2531011;
		R->holdrand = v7;
		v8 = _vector::random_dir(&rnd, R);
		v9 = tanf(cone_angle);
		v11 = (__m128)(3.0f);
		v12 = _mm_set_ss(static_cast<float>((v7 >> 16) & 0x7FFF));
		v12.m128_f32[0] = (float)(v12.m128_f32[0] * 0.000030518509) * v9;
		v13 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(v12, v12, 0), v8->m), cone_axis->m);
		v14 = _mm_mul_ps(v13, v13);
		v14.m128_f32[0] = v14.m128_f32[0]
			+ (float)(_mm_shuffle_ps(v14, v14, 85).m128_f32[0] + _mm_shuffle_ps(v14, v14, 170).m128_f32[0]);
		v15 = 1.0 / sqrtf(v14.m128_f32[0]);
		v11.m128_f32[0] = (float)((float)(3.0 - (float)(v15 * (float)(v14.m128_f32[0] * v15))) * v15) * 0.5;
		this->m = _mm_mul_ps(_mm_shuffle_ps(v11, v11, 0), v13);
		return this;
	}
	_vector* reflect3(const _vector* dir, const _vector* norm)
	{
		__m128 v4; // xmm2
		__m128 v5; // xmm0

		v4 = _mm_mul_ps(norm->m, dir->m);
		v5 = _mm_shuffle_ps(v4, v4, 85);
		v5.m128_f32[0] = (float)((float)(v5.m128_f32[0] + _mm_shuffle_ps(v4, v4, 170).m128_f32[0]) + v4.m128_f32[0]) * -2.0;
		this->m = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(v5, v5, 0), norm->m), dir->m);
		return this;
	}
	_vector* negate()
	{
		this->m = _mm_xor_ps(this->m, MM_FPSIGN_PS);
		return this;
	}
	_vector(_vector* v)
	{
		*this = *v;
	}
	_vector* set(_vector* v)
	{
		*this = *v;
		return this;
	}
	_vector* mul(const _vector* v)
	{
		this->m = _mm_mul_ps(v->m, this->m);
		return this;
	}
	_vector* add(const _vector* a, const _vector* v)
	{
		this->m = _mm_add_ps(a->m, v->m);
		return this;
	}
	_vector* negate3()
	{
		this->m = _mm_xor_ps(this->m, MM_FPSIGN_PS_3__0);
		return this;
	}
	_vector* negate3()
	{
		this->m = _mm_xor_ps(this->m, MM_FPSIGN_PS_3__0);
		return this;
	}
	_vector* nrm3(const _vector* v)
	{
		__m128 v2; // xmm3
		__m128 v4; // xmm4
		float v5; // xmm2_4

		v2 = (__m128)(3.0f);
		v4 = _mm_mul_ps(v->m, v->m);
		v4.m128_f32[0] = v4.m128_f32[0]
			+ (float)(_mm_shuffle_ps(v4, v4, 85).m128_f32[0] + _mm_shuffle_ps(v4, v4, 170).m128_f32[0]);
		v5 = 1.0 / sqrtf(v4.m128_f32[0]);
		v2.m128_f32[0] = (float)((float)(3.0 - (float)(v5 * (float)(v4.m128_f32[0] * v5))) * v5) * 0.5;
		this->m = _mm_mul_ps(_mm_shuffle_ps(v2, v2, 0), v->m);
		return this;
	}

	//REMAKE THIS ASAP
	void generate_orthonormal_basis(const _vector* dir, _vector* up, _vector* right)
	{
		// v8[0] = dir->z * dir->z
		float v8 = dir->z * dir->z;

		// if (fabsf(dir->x) < fabsf(dir->y))
		if (fabsf(dir->x) < fabsf(dir->y))
		{
			// v8 = z*z + y*y
			v8 = v8 + dir->y * dir->y;
			if (v8 < 0.0f)
				debug::fatal("sqrt: x < 0 [%f]", v8);

			float v11 = 1.0f / sqrtf(v8);

			up->x = 0.0f;
			up->y = v11 * dir->z;
			up->z = -(v11 * dir->y);   // LODWORD(...) ^ _xmm  → смена знака
		}
		else
		{
			// v8 = z*z + x*x
			v8 = v8 + dir->x * dir->x;
			if (v8 < 0.0f)
				debug::fatal("sqrt: x < 0 [%f]", v8);

			float v9 = 1.0f / sqrtf(v8);
			float v10 = v9 * dir->z;

			up->y = 0.0f;
			up->x = -(v10);              // LODWORD(v10) ^ _xmm  → смена знака
			up->z = v9 * dir->x;
		}

		// Оригинальный assert на aliasing
		if (up == right || dir == right)
			debug::fail(
				"(&v0!=this) && (&v1!=this)",
				"d:\\trunk\\src\\ucore\\libvector_pc.h",
				"_vector::cross3",
				(const char*)0x228,
				0);

		// right = up × dir  (точно как в оригинальном SSE: shuffle 210/201 даёт up×dir, а не dir×up)
		right->x = up->y * dir->z - up->z * dir->y;
		right->y = up->z * dir->x - up->x * dir->z;
		right->z = up->x * dir->y - up->y * dir->x;
		right->w = 0.0f;   // SSE cross даёт w = 0
	}
	// local variable allocation has failed, the output may be wrong!
	_vector* set3fw(const _vec3<float>* v, double _w)
	{
		this->m = _mm_unpacklo_ps(
			_mm_unpacklo_ps((__m128)LODWORD(v->x), (__m128)LODWORD(v->z)),
			_mm_unpacklo_ps((__m128)LODWORD(v->y), *(__m128*) & _w));
		return this;
	}
	_vector* random_point_box(const _vector* box_size, urandom::simple* R)
	{
		float z = box_size->z;

		int v5 = 214013 * R->holdrand + 2531011;
		R->holdrand = v5;

		float y = box_size->y;

		int v7 = 214013 * R->holdrand + 2531011;
		R->holdrand = v7;

		float x = box_size->x;

		int v9 = 214013 * R->holdrand + 2531011;
		R->holdrand = v9;

		int v10 = box_size->m.m128_i32[0];

		this->m.m128_i32[3] = box_size->m.m128_i32[3];

		// X: через SSE int2float
		__m128i v11 = _mm_cvtsi32_si128((v9 >> 16) & 0x7FFF);
		float neg_x = std::bit_cast<float>(static_cast<std::uint32_t>(v10) ^ 0x80000000u);
		this->x = (_mm_cvtepi32_ps(v11).m128_f32[0] * 0.000030518509f) * (x - neg_x) + neg_x;

		// Y: скалярный путь
		float neg_y = std::bit_cast<float>(std::bit_cast<std::uint32_t>(y) ^ 0x80000000u);
		this->y = (((float)((v7 >> 16) & 0x7FFF) * 0.000030518509f) * (y - neg_y)) + neg_y;

		// Z: скалярный путь
		float neg_z = std::bit_cast<float>(std::bit_cast<std::uint32_t>(z) ^ 0x80000000u);
		this->z = (((float)((v5 >> 16) & 0x7FFF) * 0.000030518509f) * (z - neg_z)) + neg_z;

		return this;
	}
	float distance2_xz(const _vector* v)
	{
		__m128 v2; // xmm3
		_vector t; // [rsp+0h] [rbp-18h]

		t.m = _mm_sub_ps(this->m, v->m);
		t.m.m128_i32[1] = 0;
		v2 = _mm_mul_ps(t.m, t.m);
		return sqrtf(v2.m128_f32[0] + (float)(_mm_shuffle_ps(v2, v2, 85).m128_f32[0] + _mm_shuffle_ps(v2, v2, 170).m128_f32[0]));
	}
	_vector* div(float s)
	{
		__m128 v2; // xmm0

		v2.m128_f32[0] = s;
		this->m = _mm_div_ps(this->m, _mm_shuffle_ps(v2, v2, 0));
		return this;
	}
	_vector* vec4()
	{
		return this;
	}
	_vector* nrm4()
	{
		__m128 v1; // xmm3
		__m128 v3; // xmm1
		__m128 v4; // xmm4
		float v5; // xmm2_4

		v1 = (__m128)(3.0f);
		v3 = _mm_mul_ps(this->m, this->m);
		v4 = _mm_add_ps(_mm_movehl_ps(v3, v3), v3);
		v4.m128_f32[0] = v4.m128_f32[0] + _mm_shuffle_ps(v4, v4, 1).m128_f32[0];
		v5 = 1.0 / sqrtf(v4.m128_f32[0]);
		v1.m128_f32[0] = (float)((float)(3.0 - (float)(v5 * (float)(v4.m128_f32[0] * v5))) * v5) * 0.5;
		this->m = _mm_mul_ps(_mm_shuffle_ps(v1, v1, 0), this->m);
		return this;
	}
	float distance2sq(const _vector* v)
	{
		__m128 v2; // xmm0
		__m128 v3; // xmm0

		v2 = _mm_sub_ps(this->m, v->m);
		v3 = _mm_mul_ps(v2, v2);
		return v3.m128_f32[0] + _mm_shuffle_ps(v3, v3, 85).m128_f32[0];
	}
	float dp2(const _vector* v, const _vector* d)
	{
		__m128 v3; // xmm0

		v3 = _mm_mul_ps(v->m, d->m);
		return v3.m128_f32[0] + _mm_shuffle_ps(v3, v3, 85).m128_f32[0];
	}
	float len2sq()
	{
		__m128 v1; // xmm0

		v1 = _mm_mul_ps(this->m, this->m);
		return v1.m128_f32[0] + _mm_shuffle_ps(v1, v1, 85).m128_f32[0];
	}
	void make_ortho_unsafe(_vector* dir, _vector* up, _vector* right)
	{
		__m128 v3; // xmm6
		__m128 v7; // xmm3
		__m128 v8; // xmm1
		float v9; // xmm2_4
		__m128 v10; // xmm3
		__m128 v11; // xmm2
		float v12; // xmm1_4
		int v13; // [rsp+20h] [rbp-28h]

		v3 = (__m128)(3.0f);
		v7 = _mm_mul_ps(dir->m, dir->m);
		v8 = (__m128)(3.0f);
		v7.m128_f32[0] = v7.m128_f32[0]
			+ (float)(_mm_shuffle_ps(v7, v7, 85).m128_f32[0] + _mm_shuffle_ps(v7, v7, 170).m128_f32[0]);
		v9 = 1.0 / sqrtf(v7.m128_f32[0]);
		v8.m128_f32[0] = (float)((float)(3.0 - (float)(v9 * (float)(v7.m128_f32[0] * v9))) * v9) * 0.5;
		dir->m = _mm_mul_ps(_mm_shuffle_ps(v8, v8, 0), dir->m);
		if (up == right || dir == right)
			debug::fail(
				"(&v0!=this) && (&v1!=this)",
				"d:\\trunk\\src\\ucore\\libvector_pc.h",
				"_vector::cross3",
				(const char*)0x228,
				v13);
		v10 = _mm_sub_ps(
			_mm_mul_ps(_mm_shuffle_ps(up->m, up->m, 201), _mm_shuffle_ps(dir->m, dir->m, 210)),
			_mm_mul_ps(_mm_shuffle_ps(up->m, up->m, 210), _mm_shuffle_ps(dir->m, dir->m, 201)));
		v11 = _mm_mul_ps(v10, v10);
		v11.m128_f32[0] = v11.m128_f32[0]
			+ (float)(_mm_shuffle_ps(v11, v11, 85).m128_f32[0] + _mm_shuffle_ps(v11, v11, 170).m128_f32[0]);
		v12 = 1.0 / sqrtf(v11.m128_f32[0]);
		v3.m128_f32[0] = (float)((float)(3.0 - (float)(v12 * (float)(v11.m128_f32[0] * v12))) * v12) * 0.5;
		right->m = _mm_mul_ps(_mm_shuffle_ps(v3, v3, 0), v10);
		if (dir == up || right == up)
			debug::fail(
				"(&v0!=this) && (&v1!=this)",
				"d:\\trunk\\src\\ucore\\libvector_pc.h",
				"_vector::cross3",
				(const char*)0x228,
				v13);
		up->m = _mm_sub_ps(
			_mm_mul_ps(_mm_shuffle_ps(right->m, right->m, 210), _mm_shuffle_ps(dir->m, dir->m, 201)),
			_mm_mul_ps(_mm_shuffle_ps(right->m, right->m, 201), _mm_shuffle_ps(dir->m, dir->m, 210)));
	}
	float nrm3m(const _vector* v)
	{
		__m128 v2; // xmm3

		v2 = _mm_mul_ps(v->m, v->m);
		v2.m128_f32[0] = sqrtf(
			v2.m128_f32[0]
			+ (float)(_mm_shuffle_ps(v2, v2, 85).m128_f32[0] + _mm_shuffle_ps(v2, v2, 170).m128_f32[0]));
		this->m = _mm_div_ps(v->m, _mm_shuffle_ps(v2, v2, 0));
		return v2.m128_f32[0];
	}
	_vector* set_angle_y(float y)
	{
		float v3; // xmm6_4
		float v4; // xmm0_4

		this->m.m128_i32[1] = 0;
		v3 = sinf(y);
		v4 = cosf(y);
		this->x = v3;
		this->z = v4;
		return this;
	}
	_vector* abs()
	{
		this->m = _mm_and_ps(
			this->m,
			_mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF))
		);
		return this;
	}
	float distance2sq_xz(const _vector* v)
	{
		__m128 v2; // xmm3
		_vector t; // [rsp+0h] [rbp-18h]

		t.m = _mm_sub_ps(this->m, v->m);
		t.m.m128_i32[1] = 0;
		v2 = _mm_mul_ps(t.m, t.m);
		return v2.m128_f32[0] + (float)(_mm_shuffle_ps(v2, v2, 85).m128_f32[0] + _mm_shuffle_ps(v2, v2, 170).m128_f32[0]);
	}
	float get_angle_x()
	{
		if (this->x == 0.0f && this->z == 0.0f)
		{
			float y = this->y;
			if (y == 0.0f)
				return 0.0f;
			else if (y <= 0.0f)
				return 1.5707964f;
			else
				return -1.5707964f;
		}
		else
		{
			__m128 sq = _mm_mul_ps(this->m, this->m);
			float hyp = sqrtf(
				sq.m128_f32[0] +
				_mm_shuffle_ps(sq, sq, _MM_SHUFFLE(2, 2, 2, 2)).m128_f32[0]
			);

			if (hyp == 0.0f)
			{
				if (this->y <= 0.0f)
					return 1.5707964f;
				else
					return -1.5707964f;
			}
			else
			{
				float neg_y = -this->y;  // COERCE_FLOAT(m.m128_i32[1] ^ _xmm)

				float v7 = atan2f(neg_y, hyp);
				if (v7 != -atan2f(this->y, hyp))
					debug::fail(
						"atan2(-y, hyp) == -atan2(y, hyp)",
						"d:\\trunk\\src\\ucore\\libvector.h",
						"_vector::get_angle_x",
						(const char*)0x52,
						0);  // v8 был неинициализированным мусором IDA

				return atan2f(neg_y, hyp);
			}
		}
	}
	float len2sq_xz()
	{
		__m128 v1; // xmm0

		v1 = _mm_mul_ps(this->m, this->m);
		return v1.m128_f32[0] + _mm_shuffle_ps(v1, v1, 170).m128_f32[0];
	}
	bool similar3(const _vector* v, float E)
	{
		__m128 v3; // xmm0

		v3.m128_f32[0] = E;

		return (_mm_movemask_ps(
			_mm_sub_ps(
				_mm_and_ps(_mm_sub_ps(this->m, v->m), MM_FPABS_PS),
				_mm_shuffle_ps(v3, v3, 0)
			)
		) & 7) == 7;
	}
	bool similar3(const _vector* v, float E)
	{
		__m128 v3; // xmm0

		v3.m128_f32[0] = E;
		return (_mm_movemask_ps(
			_mm_sub_ps(
				_mm_and_ps(
					_mm_sub_ps(this->m, v->m),
					_mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF))
				),
				_mm_shuffle_ps(v3, v3, 0)
			)
		) & 7) == 7;
	}
	float dp4(const _vector* v, const _vector* d)
	{
		__m128 v3; // xmm1
		__m128 v4; // xmm0

		v3 = _mm_mul_ps(v->m, d->m);
		v4 = _mm_add_ps(_mm_movehl_ps(v3, v3), v3);
		return v4.m128_f32[0] + _mm_shuffle_ps(v4, v4, 1).m128_f32[0];
	}
	float len4sq()
	{
		__m128 v1; // xmm1
		__m128 v2; // xmm0

		v1 = _mm_mul_ps(this->m, this->m);
		v2 = _mm_add_ps(_mm_movehl_ps(v1, v1), v1);
		return v2.m128_f32[0] + _mm_shuffle_ps(v2, v2, 1).m128_f32[0];
	}
};