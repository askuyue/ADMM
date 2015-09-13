#ifndef BLASWRAPPER_H
#define BLASWRAPPER_H

#include <Eigen/Core>

namespace Linalg {

extern "C"
{
    // Level 1
    double ddot_(const int *n, const double *dx, const int *incx,
	             const double *dy, const int *incy);
    void daxpy_(const int *n, const double *alpha,
                const double *dx, const int *incx,
                double *dy, const int *incy);
    // Level 2
    void dgemv_(const char* transA, const int* m, const int* n,
                const double* alpha, const double* A, const int* ldA,
                const double* x, const int* incx,
                const double* beta, double* y, const int* incy);
    void dsymv_(const char *uplo, const int *n, const double *alpha,
            	const double *a, const int *lda,
            	const double *x, const int *incx,
            	const double *beta, double *y, const int *incy);
    // Level 3
    void dgemm_(const char* transA, const char* transB, const int* m, const int* n, const int* k,
                const double* alpha, const double* A, const int* ldA, const double* B, const int* ldB,
                const double* beta, double* C, const int* ldC);
    void dsyrk_(const char* uplo, const char* transA, const int* n, const int* k,
                const double* alpha, const double* A, const int* ldA,
                const double* beta, double* C, const int* ldC);
    void ssyrk_(const char* uplo, const char* transA, const int* n, const int* k,
                const float* alpha, const float* A, const int* ldA,
                const float* beta, float* C, const int* ldC);
    void dtrsm_(const char *side, const char *uplo,
            	const char *transa, const char *diag,
            	const int *m, const int *n, const double *alpha,
            	const double *a, const int *lda,
            	double *b, const int *ldb);
}



typedef Eigen::Ref<Eigen::MatrixXd>             GenericMatrix;
typedef const Eigen::Ref<const Eigen::MatrixXd> ConstGenericMatrix;
typedef Eigen::Ref<Eigen::VectorXd>             GenericVector;
typedef const Eigen::Ref<const Eigen::VectorXd> ConstGenericVector;

// Wrappers
inline double inner_product(const double *x, const double *y, const int len)
{
    const int inc = 1;
    return ddot_(&len, x, &inc, y, &inc);
}

inline void mat_vec_prod(Eigen::VectorXd &res, const Eigen::MatrixXd &X, const Eigen::VectorXd &v,
                         const double &alpha = 1.0, const double &beta = 0.0)
{
    const int n = X.rows();
    const int p = X.cols();
    const int inc = 1;

    res.resize(n);
    dgemv_("N", &n, &p, &alpha, X.data(), &n, v.data(), &inc, &beta, res.data(), &inc);
}

inline void mat_vec_prod(double *res, const double *X, const double *v, int n, int p,
                         const double &alpha = 1.0, const double &beta = 0.0)
{
    const int inc = 1;

    dgemv_("N", &n, &p, &alpha, X, &n, v, &inc, &beta, res, &inc);
}

inline void mat_vec_tprod(Eigen::VectorXd &res, const Eigen::MatrixXd &X, const Eigen::VectorXd &v,
                          const double &alpha = 1.0, const double &beta = 0.0)
{
    const int n = X.rows();
    const int p = X.cols();
    const int inc = 1;

    res.resize(p);
    dgemv_("T", &n, &p, &alpha, X.data(), &n, v.data(), &inc, &beta, res.data(), &inc);
}

inline void mat_vec_tprod(double *res, const double *X, const double *v, int n, int p,
                          const double &alpha = 1.0, const double &beta = 0.0)
{
    const int inc = 1;

    dgemv_("T", &n, &p, &alpha, X, &n, v, &inc, &beta, res, &inc);
}

// Calculating X'X
inline void cross_prod(Eigen::MatrixXd &res, ConstGenericMatrix &X)
{
    const double one = 1.0;
    const double zero = 0.0;
    const char trans = 'T';
    const char no_trans = 'N';

    const int n = X.rows();
    const int p = X.cols();
    const double *x_ptr = X.data();

    res.resize(p, p);
    double *res_ptr = res.data();

    dgemm_(&trans, &no_trans, &p, &p, &n,
           &one, x_ptr, &n, x_ptr, &n,
           &zero, res_ptr, &p);
}

inline void cross_prod_lower(Eigen::MatrixXd &res, ConstGenericMatrix &X)
{
    const double one = 1.0;
    const double zero = 0.0;

    const int n = X.rows();
    const int p = X.cols();
    const double *x_ptr = X.data();

    res.resize(p, p);
    double *res_ptr = res.data();

    dsyrk_("L", "T", &p, &n,
           &one, x_ptr, &n,
           &zero, res_ptr, &p);
}

inline void tcross_prod_lower(Eigen::MatrixXd &res, ConstGenericMatrix &X)
{
    const double one = 1.0;
    const double zero = 0.0;

    const int n = X.rows();
    const int p = X.cols();
    const double *x_ptr = X.data();

    res.resize(n, n);
    double *res_ptr = res.data();

    dsyrk_("L", "N", &n, &p,
           &one, x_ptr, &n,
           &zero, res_ptr, &n);
}

inline void tcross_prod_lower(Eigen::MatrixXf &res, const Eigen::MatrixXf &X)
{
    const float one = 1.0;
    const float zero = 0.0;

    const int n = X.rows();
    const int p = X.cols();
    const float *x_ptr = X.data();

    res.resize(n, n);
    float *res_ptr = res.data();

    ssyrk_("L", "N", &n, &p,
           &one, x_ptr, &n,
           &zero, res_ptr, &n);
}



} // namespace Linalg

#endif // BLASWRAPPER_H
