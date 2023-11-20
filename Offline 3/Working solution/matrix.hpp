#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>

class Matrix {
private:
    int row, col;
    double **mat;

public:
    Matrix();
    Matrix(int row, int col);
    Matrix(const Matrix &m);
    ~Matrix();

    int getRow() const;
    int getCol() const;
    double get(int i, int j) const;
    void set(int i, int j, double val);

    // Vector3D toVector3D() const;

    double determinant() const;
    double cofactor(int, int) const;
    Matrix adjoint() const;
    Matrix transpose() const;
    Matrix inverse() const;
    Matrix normalize() const;

    Matrix operator+(const Matrix &m) const;
    Matrix operator-(const Matrix &m) const;
    Matrix operator*(const Matrix &m) const;
    Matrix operator*(double val) const;
    Matrix operator/(double val) const;
    Matrix operator-() const;

    Matrix &operator=(const Matrix &m);

    bool operator==(const Matrix &m) const;
    bool operator!=(const Matrix &m) const;

    friend std::ostream &operator<<(std::ostream &out, const Matrix &m);
};

Matrix::Matrix()
{
    row = 0;
    col = 0;
    mat = nullptr;
}
/**
 * Creates a matrix of size row x col
 * @param row number of rows
 * @param col number of columns
 * @return void
 */
Matrix::Matrix(int row, int col)
{
    this->row = row;
    this->col = col;
    mat = new double *[row];

    for (int i = 0; i < row; i++)
    {
        mat[i] = new double[col];
        for (int j = 0; j < col; j++)
        {
            mat[i][j] = 0;
        }
    }
}

/**
 * Creates a copy of the given matrix m
 * @param m matrix of size row x col
 * @return void
 */
Matrix::Matrix(const Matrix &m)
{
    row = m.row;
    col = m.col;
    mat = new double *[row];
    for (int i = 0; i < row; i++)
    {
        mat[i] = new double[col];
        for (int j = 0; j < col; j++)
        {
            mat[i][j] = m.mat[i][j];
        }
    }
}

/**
 * destructor
 */
Matrix::~Matrix()
{
    for (int i = 0; i < row; i++)
    {
        delete[] mat[i];
    }
    delete[] mat;
}

/**
 * Returns the number of rows in the matrix
 * @return number of rows
 */
int Matrix::getRow() const
{
    return row;
}

/**
 * Returns the number of columns in the matrix
 * @return number of columns
 */
int Matrix::getCol() const
{
    return col;
}

/**
 * Returns the value at the given index
 * @param i row index
 * @param j column index
 * @return value at index (i, j)
 */
double Matrix::get(int i, int j) const
{
    return mat[i][j];
}

/**
 * Sets the value at the given index
 * @param i row index
 * @param j column index
 * @param val value to be set
 * @return void
 */
void Matrix::set(int i, int j, double val)
{
    mat[i][j] = val;
}

/**
 * Adds two matrices
 * @param m matrix to be added
 * @return sum of the two matrices
 */
Matrix Matrix::operator+(const Matrix &m) const
{
    if (row != m.row || col != m.col)
    {
        throw "Matrix dimensions do not match";
    }

    Matrix result(row, col);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            result.mat[i][j] = mat[i][j] + m.mat[i][j];
        }
    }
    return result;
}

/**
 * Subtracts two matrices
 * @param m matrix to be subtracted
 * @return difference of the two matrices
 */
Matrix Matrix::operator-(const Matrix &m) const
{
    if (row != m.row || col != m.col)
    {
        throw "Matrix dimensions do not match";
    }

    Matrix result(row, col);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            result.mat[i][j] = mat[i][j] - m.mat[i][j];
        }
    }
    return result;
}

/**
 * Multiplies two matrices
 * @param m matrix to be multiplied
 * @return product of the two matrices
 */
Matrix Matrix::operator*(const Matrix &m) const
{
    if (col != m.row)
    {
        throw "Matrices of dimensions " + std::to_string(row) + "x" + std::to_string(col) + " and " + std::to_string(m.row) + "x" + std::to_string(m.col) + " cannot be multiplied";
    }

    Matrix result(row, m.col);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < m.col; j++)
        {
            for (int k = 0; k < col; k++)
            {
                result.mat[i][j] += mat[i][k] * m.mat[k][j];
            }
        }
    }

    return result;
}

/**
 * Multiplies a matrix with a scalar
 * @param val scalar to be multiplied
 * @return product of the matrix and the scalar
 */
Matrix Matrix::operator*(double val) const
{
    Matrix result(row, col);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            result.mat[i][j] = mat[i][j] * val;
        }
    }
    return result;
}

/**
 * Divides a matrix with a scalar
 * @param val scalar to be divided
 * @return quotient of the matrix and the scalar
 */
Matrix Matrix::operator/(double val) const
{
    Matrix result(row, col);
    if (val == 0)
    {
        throw "Division by zero";
    }
    double inv = 1 / val;
    return *this * inv;
}

/**
 * Assigns a matrix to another matrix
 * @param m matrix to be assigned
 * @return void
 */
Matrix &Matrix::operator=(const Matrix &m)
{
    if (this == &m)
    {
        return *this;
    }

    if (row != m.row || col != m.col)
    {
        for (int i = 0; i < row; i++)
        {
            delete[] mat[i];
        }
        delete[] mat;

        row = m.row;
        col = m.col;
        mat = new double *[row];
    }

    for (int i = 0; i < row; i++)
    {
        mat[i] = new double[col];
        for (int j = 0; j < col; j++)
        {
            mat[i][j] = m.mat[i][j];
        }
    }

    return *this;
}

/**
 * Scalar multiplication with -1
 * @return Matrix
 */
Matrix Matrix::operator-() const
{
    Matrix result(row, col);
    return result - *this;
}

/**
 * Checks if two matrices are equal
 * @param m matrix to be compared
 * @return true if equal, false otherwise
 */
bool Matrix::operator==(const Matrix &m) const
{
    if (row != m.row || col != m.col)
    {
        return false;
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (mat[i][j] != m.mat[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * Checks if two matrices are not equal
 * @param m matrix to be compared
 * @return true if not equal, false otherwise
 */
bool Matrix::operator!=(const Matrix &m) const
{
    return !(*this == m);
}

/**
 * Outputs the matrix in the form of a string
 * @return string representation of the matrix
 */
std::ostream &operator<<(std::ostream &out, const Matrix &m)
{
    for (int i = 0; i < m.row; i++)
    {
        for (int j = 0; j < m.col; j++)
        {
            out << m.mat[i][j] << " ";
        }
        out << std::endl;
    }
    return out;
}

/**
 * Returns the cofactor of the matrix at the given index
 * @param i row index
 * @param j column index
 * @return cofactor of the matrix at index (i, j)
 */
double Matrix::cofactor(int i, int j) const
{
    if (row != col)
    {
        throw "Matrix is not square";
    }

    Matrix result(row - 1, col - 1);
    int x = 0, y = 0;
    for (int k = 0; k < row; k++)
    {
        if (k == i)
        {
            continue;
        }
        y = 0;
        for (int l = 0; l < col; l++)
        {
            if (l == j)
            {
                continue;
            }
            result.mat[x][y] = mat[k][l];
            y++;
        }
        x++;
    }

    double sign = (i + j) % 2 == 0 ? 1 : -1;
    return sign * result.determinant();
}

/**
 * Returns the transpose of the matrix
 * @return transpose of the matrix
 */
Matrix Matrix::transpose() const
{
    Matrix result(col, row);
    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < row; j++)
        {
            result.mat[i][j] = mat[j][i];
        }
    }

    return result;
}

/**
 * Returns the adjoint of the matrix
 * @return adjoint of the matrix
 */
Matrix Matrix::adjoint() const
{
    if (row != col)
    {
        throw "Matrix is not square";
    }

    Matrix result(row, col);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            result.mat[i][j] = cofactor(i, j);
        }
    }
    return result.transpose();
}

/**
 * Returns the determinant of the matrix
 * @return determinant of the matrix
 */
double Matrix::determinant() const
{
    if (row != col)
    {
        throw "Matrix is not square";
    }

    if (row == 1)
    {
        return mat[0][0];
    }

    double result = 0;
    for (int i = 0; i < row; i++)
    {
        result += mat[0][i] * cofactor(0, i);
    }
    return result;
}

/**
 * Returns the inverse of the matrix
 * @return inverse of the matrix
 */
Matrix Matrix::inverse() const
{
    if (row != col)
    {
        throw "Matrix is not square";
    }

    Matrix result(row, col);
    double det = determinant();
    if (det == 0)
    {
        throw "Matrix is not invertible";
    }

    Matrix adj = adjoint();
    result = adj / det;
    return result;
}

/**
 * Converts the matrix to its normal form
 * @return Matrix
 */
Matrix Matrix::normalize() const
{
    Matrix result(row, col);
    double det = determinant();
    if (det == 0)
    {
        throw "Matrix is not invertible";
    }

    result = *this / det;
    return result;
}

#endif