#include "task.h"
//формулы
static double f(int k, int n, int i, int j) {
    switch (k) {
        case 1: { int mx = i > j ? i : j; return (double)(n - mx + 1); }
        case 2: { int mx = i > j ? i : j; return (double)mx; }
        case 3: return (double)abs(i - j);
        case 4: return 1.0 / (double)(i + j - 1);
        default: return 0.0;
    }
}
//матрица из файла
static int read_matrix_from_file(const char *filename, int n, double *A) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { fprintf(stderr, "error 1\n"); return -1; }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(fp, "%lf", &A[i * n + j]) != 1) {
                fprintf(stderr, "error 2\n");
                fclose(fp);
                return -1;
            }
        }
    }
    fclose(fp);
    return 0;
}

//создание матрицы по формуле
static void init_matrix_by_formula(int k, int n, double *A) {
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            A[(i - 1) * n + (j - 1)] = f(k, n, i, j);
}


static void build_rhs(int n, const double *A, double *b) {
    int kmax = (n + 1) / 2;
    for (int i = 0; i < n; i++) {
        double s = 0.0;
        for (int k = 0; k <= kmax; k++) {
            int col = 2 * k + 1;
            if (col >= 1 && col <= n)
                s += A[i * n + (col - 1)];
        }
        b[i] = s;
    }
}

//вывод
static void print_matrix(const char *title, int l, int n, const double *M, int m) {
    if (title) printf("%s\n", title);
    int rows = (l < m) ? l : m;
    int cols = (n < m) ? n : m;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            printf(" %10.3e", M[i * n + j]);
        printf("\n");
    }
}

//метод гаусса
static int gauss_invert(int n, const double *A, double *X,
                        double *work, int *perm) {
    memcpy(work, A, sizeof(double) * n * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            X[i * n + j] = (i == j) ? 1.0 : 0.0;
        perm[i] = i;
    }

    for (int col = 0; col < n; col++) {
        int piv = col;
        double maxv = fabs(work[col * n + col]);
        for (int r = col + 1; r < n; r++) {
            double v = fabs(work[r * n + col]);
            if (v > maxv) { maxv = v; piv = r; }
        }
        if (maxv < 1e-15) return -1;

        if (piv != col) {
            for (int j = 0; j < n; j++) {
                double t = work[col * n + j];
                work[col * n + j] = work[piv * n + j];
                work[piv * n + j] = t;
                t = X[col * n + j];
                X[col * n + j] = X[piv * n + j];
                X[piv * n + j] = t;
            }
            int t = perm[col]; perm[col] = perm[piv]; perm[piv] = t;
        }

        double d = work[col * n + col];
        for (int j = 0; j < n; j++) {
            work[col * n + j] /= d;
            X[col * n + j] /= d;
        }

        for (int r = 0; r < n; r++) {
            if (r == col) continue;
            double factor = work[r * n + col];
            if (fabs(factor) < 1e-15) continue;
            for (int j = 0; j < n; j++) {
                work[r * n + j] -= factor * work[col * n + j];
                X[r * n + j] -= factor * X[col * n + j];
            }
        }
    }
    return 0;
}

//норма неявки
static double residual_inverse(int n, const double *A, const double *Ainv) {
    double s = 0.0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            double v = 0.0;
            for (int k = 0; k < n; k++)
                v += A[i * n + k] * Ainv[k * n + j];
            if (i == j) v -= 1.0;
            s += v * v;
        }
    return sqrt(s);
}

//функция
void task104(int argc, char **argv) {
    if (argc != 4 && argc != 5) {
        fprintf(stderr,
            "Использование:\n"
            "  %s n m 0 filename\n"
            "  %s n m k   (k=1..4)\n",
            argv[0], argv[0]);
        return;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int k = atoi(argv[3]);
    const char *filename = (argc == 5) ? argv[4] : NULL;

    if (n <= 0) { fprintf(stderr, "n > 0 required\n"); return; }
    if (k < 0 || k > 4) { fprintf(stderr, "k in 0..4 required\n"); return; }
    if (k == 0 && !filename) {
        fprintf(stderr, "filename required when k = 0\n");
        return;
    }

    double *A    = malloc(sizeof(double) * n * n);
    double *X    = malloc(sizeof(double) * n * n);
    double *work = malloc(sizeof(double) * n * n);
    double *b    = malloc(sizeof(double) * n);
    double *x    = malloc(sizeof(double) * n);
    int    *perm = malloc(sizeof(int) * n);

    if (!A || !X || !work || !b || !x || !perm) {
        fprintf(stderr, "malloc fail\n");
        free(A); free(X); free(work); free(b); free(x); free(perm);
        return;
    }

    if (k == 0) {
        if (read_matrix_from_file(filename, n, A) != 0) {
            free(A); free(X); free(work); free(b); free(x); free(perm);
            return;
        }
    } else {
        init_matrix_by_formula(k, n, A);
    }

    print_matrix("Исходная матрица A:", n, n, A, m);

    clock_t t0 = clock();
    int rc = gauss_invert(n, A, X, work, perm);
    clock_t t1 = clock();

    if (rc != 0) {
        fprintf(stderr, "Матрица вырождена\n");
        free(A); free(X); free(work); free(b); free(x); free(perm);
        return;
    }

    print_matrix("Обратная матрица A^{-1}:", n, n, X, m);

    double res = residual_inverse(n, A, X);
    printf("Норма невязки ||A*A^{-1} - E|| = %e\n", res);

    double elapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;
    printf("Время обращения: %.2f с\n", elapsed);

    build_rhs(n, A, b);
    for (int i = 0; i < n; i++) {
        double s = 0.0;
        for (int j = 0; j < n; j++)
            s += X[i * n + j] * b[j];
        x[i] = s;
    }
    double err = 0.0;
    for (int i = 0; i < n; i++) {
        double exact = (i % 2 == 0) ? 1.0 : 0.0;
        double d = x[i] - exact;
        err += d * d;
    }
    printf("Норма погрешности решения: %e\n", sqrt(err));

    free(A); free(X); free(work); free(b); free(x); free(perm);
}
