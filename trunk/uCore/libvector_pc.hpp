#pragma once
#include <iostream>
#include <numbers> // Requires C++20
#include <xmmintrin.h>
#include <bit>
#include <cstdint>
#include <immintrin.h>
#include "../uCore/debug.h"
#include "libvec3.h"
#include "libvec4.h"
#define MM_FPSIGN_PS_3_ _mm_castsi128_ps(_mm_setr_epi32( \
    0x80000000, 0x80000000, 0x80000000, 0x00000000))
#define MM_fff0 _mm_castsi128_ps(_mm_setr_epi32(-1, -1, -1, 0))
#define LODWORD(x) (*((unsigned int*)&(x)))
#define MM_FPSIGN_PS _mm_castsi128_ps(_mm_set1_epi32(0x80000000))
#define MM_FPSIGN_PS_3__0 _mm_castsi128_ps(_mm_setr_epi32( \
    0x80000000, 0x80000000, 0x80000000, 0))
#define MM_FPABS_PS _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF))
namespace urandom {
	struct simple {
		volatile int holdrand;
	};
}

/*
* ToDo:
* Rewrite functions from unsafe c style to C++ safe style
* Rewrite all legacy, make it more readable
* Replace this single file into many files
* Re-check and re-write (if needed) all hard mathematical & SSE-using functions
*/
float deg2rad(float val);
float rad2deg(float val);

class implement {
	float deg2rad(float val);
	float rad2deg(float val);
	char* p_memmove(char* dst0, char* src0, unsigned int length);
};

class _vector {     //////////// recheck all this!!!
	float x, y, w, z;
	__m128 m;
	_vector();
	_vector(float _x, float _y, float _z, float _w);
	_vector(const _vector* v);
	_vector(const _vec3<float>* v);

	float dp3(const _vector* v, const _vector* d);
	float dp3(const _vector* v);
	_vector* set(float _x, float _y, float _z, float _w);
	_vector* sub(const _vector* v);
	_vector* sub(const _vector* a, const _vector* v);
	float distance3sq(const _vector* v);
	_vector* operator= (const _vector* v);
	_vector* max(const _vector* v);
	_vector* min(const _vector* v);
	_vector* mul(const _vector* a, const _vector* v);
	_vector* set(const _vector* v);
	_vector* splat_x(const _vector* v);
	_vector* splat_y(const _vector* v);
	_vector* splat_z(const _vector* v);
	float distance3(const _vector* v);
	float get_angle_y();
	float len3();
	float len3sq();
	_vector* negate3(const _vector* a);
	float nrm3m();
	_vector* set(const _vec3<float>* vec);
	_vector* vec3();
	bool equal(const _vector* vector);
	__m128* _nr1_ss(__m128* result, const __m128* f, const __m128* a);
	_vector* cross3(const _vector* v0, const _vector* v1);
	_vector* mad(const _vector* p, const _vector* d, float s);
	_vector* nrm3();
	_vector* set_w0();
	_vector* set_w1();
	_vector* set_zero();
	_vector* lerp(const _vector* p1, const _vector* p2, float t);
	__m128* _hadd2_xz(__m128* result, const __m128* a);

	//MAYBE WRONG
	void get_angles_xy(float* _x, float* _y);
	float len2_xz();
	_vector* mul(float s);
	float nrm3ms();
	_vector* set(const _vec4<float>* v);
	_vector* set_angles_xy(float _x, float _y);
	_vector* vec4();
	_vector* div(const _vector* v);
	// ... no comments
	void generate_orthonormal_basis_normalized(_vector* dir, _vector* up, _vector* right);
	_vector* mad(const _vector* d, float s);
	_vector* max(const _vector* v1, const _vector* v2);
	_vector* max_1(const _vector* v1, const _vector* v2);
	_vector* min(const _vector* v1, const _vector* v2);
	_vector* min_1(const _vector* v1, const _vector* v2);
	_vector* mul(const _vector* a, float s);
	_vector* set_all(float s);
	_vector* add(const _vector* v);
	// URANDOM::SIMPLE???
	_vector* random_dir(urandom::simple* R);
	_vector* random_dir(const _vector* cone_axis, float cone_angle, urandom::simple* R);
	_vector* reflect3(const _vector* dir, const _vector* norm);
	_vector* negate();
	_vector(_vector* v);
	_vector* set(_vector* v);
	_vector* mul(const _vector* v);
	_vector* add(const _vector* a, const _vector* v);
	_vector* negate3();
	_vector* negate3();
	_vector* nrm3(const _vector* v);

	//REMAKE THIS ASAP
	void generate_orthonormal_basis(const _vector* dir, _vector* up, _vector* right);
	// local variable allocation has failed, the output may be wrong!
	_vector* set3fw(const _vec3<float>* v, double _w);
	_vector* random_point_box(const _vector* box_size, urandom::simple* R);
	float distance2_xz(const _vector* v);
	_vector* div(float s);
	_vector* vec4();
	_vector* nrm4();
	float distance2sq(const _vector* v);
	float dp2(const _vector* v, const _vector* d);
	float len2sq();
	void make_ortho_unsafe(_vector* dir, _vector* up, _vector* right);
	float nrm3m(const _vector* v);
	_vector* set_angle_y(float y);
	_vector* abs();
	float distance2sq_xz(const _vector* v);
	float get_angle_x();
	float len2sq_xz();
	bool similar3(const _vector* v, float E);
	bool similar3(const _vector* v, float E);
	float dp4(const _vector* v, const _vector* d);
	float len4sq();
};