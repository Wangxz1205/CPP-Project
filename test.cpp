#include <iostream>
#include <cstdlib>
#include <immintrin.h>
#include <chrono>
#include <omp.h>
using namespace std;

#define TIME_START start=std::chrono::steady_clock::now();
#define TIME_END(NAME) end=std::chrono::steady_clock::now(); \
             duration=std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();\
             cout<<(NAME)<<": result="<<result \
             <<", duration = "<<duration<<"ms"<<endl;

float add(const float *p1, const float *p2, size_t n)
{

    float *p3 = static_cast<float *>(aligned_alloc(256, n * sizeof(float)));
    for (size_t i = 0; i < n; i++)
    {
        p3[i] = p1[i] + p2[i];
    }
    float result = *(p3+2);
    return result;
}

float add_avx2(const float *p1, const float *p2, size_t n)
{
    if (n % 8 != 0)
    {
        cerr << "The size n must be a multiple of 8." << endl;
        return NULL;
    }
    float *p3 = static_cast<float *>(aligned_alloc(256, n * sizeof(float)));
    __m256 a, b, c;

    for (size_t i = 0; i < n; i += 8)
    {
        a = _mm256_load_ps(p1 + i);
        b = _mm256_load_ps(p2 + i);
        c = _mm256_add_ps(a, b);
        _mm256_store_ps(p3 + i, c);
    }
        float result = *(p3+2);
    return result;
}

float add_avx2_omp(const float *p1, const float *p2, size_t n)
{
    if (n % 8 != 0)
    {
        cerr << "The size n must be a multiple of 8." << endl;
        return NULL;
    }
    float *p3 = static_cast<float *>(aligned_alloc(256, n * sizeof(float)));
    __m256 a, b, c;

#pragma omp parallel for
    for (size_t i = 0; i < n; i += 8)
    {
        a = _mm256_load_ps(p1 + i);
        b = _mm256_load_ps(p2 + i);
        c = _mm256_add_ps(a, b);
        _mm256_store_ps(p3 + i, c);
    }
    float result = *(p3+2);
    return result;
}

int main(int argc, char **argv)
{

    size_t nSize = 10000000;

    float *p1 = static_cast<float *>(aligned_alloc(256, nSize * sizeof(float)));
    float *p2 = static_cast<float *>(aligned_alloc(256, nSize * sizeof(float)));

    float result = 0.0f;
    p1[2] = 2.3f;
    p2[2] = 3.0f;
    p1[nSize - 1] = 2.0f;
    p2[nSize - 1] = 1.1f;

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    auto duration = 0L;

    TIME_START
    result = add(p1, p2, nSize);
    TIME_END("normal")

    TIME_START
    result = add_avx2(p1, p2, nSize);
    TIME_END("avx2")

    TIME_START
    result = add_avx2_omp(p1, p2, nSize);
    TIME_END("avx2+omp")
}