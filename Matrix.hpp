#ifndef EX3_MATRIX_H
#define EX3_MATRIX_H

#include <vector>
#include <iostream>
#include "Complex.h"

#define DEFAULT_ROWS 1
#define DEFAULT_COLS 1

class MatrixException;
class MulException;
class SumException;
class CreationException;
class TransException;
class IllegalSizeException;

/**
 * Template class that represents a matrix.
 * @tparam T a type that has a zero element and implements operators +, -, -=, +=, *, =, ==, <<.
 */
template <class T>
class Matrix
{

public:

    typedef typename std::vector<T>::const_iterator const_iterator;

    /**
     * Default constructor.
     */
    Matrix(): Matrix(DEFAULT_ROWS, DEFAULT_COLS)
    {
    }

    /**
     * Builds a matrix that has zeros in all its cells.
     * @param rows numbers of rows.
     * @param cols number of column.
     */
    Matrix(unsigned int rows, unsigned int cols): _rows(rows), _cols(cols),
                                                  _elements(rows * cols, T(0))
    {
        if ((rows == 0 && cols != 0) || (rows != 0 && cols == 0) || rows < 0 || cols < 0)
        {
            throw IllegalSizeException();
        }
    }

    /**
     * Destructor.
     */
    ~Matrix()
    {
    }

    /**
     * Copy-Constructor.
     * @param other the matrix to copy.
     */
    Matrix(const Matrix<T> &other): Matrix(other._rows, other._cols, other._elements)
    {
    }

    /**
     * Constructor that gets a vector and assign its value to the new matrix.
     * @param rows the number of rows.
     * @param cols the number of columns.
     * @param cells the vector.
     */
    Matrix(unsigned int rows, unsigned int cols, const std::vector<T> &cells):
            _rows(rows), _cols(cols), _elements(rows * cols)
    {
        if (this->_rows * this->_cols != this->_elements.size())
        {
            throw CreationException();
        }
        if ((rows == 0 && cols != 0) || (rows != 0 && cols == 0) || rows < 0 || cols < 0)
        {
            throw IllegalSizeException();
        }
        unsigned int i = 0;
        for (i = 0; i < cells.size(); i++)
        {
            _elements.at(i) = cells.at(i);
        }
    }

    /**
     * Move constructor.
     * @param other the matrix to move.
     */
    Matrix(const Matrix<T> && other): _rows(other._rows), _cols(other._cols),
                                     _elements(std::move(other._elements))
    {
    }

    /**
     * operator=, assigns new matrix.
     * @param other the other matrix to copy.
     * @return new matrix, same values as other.
     */
    Matrix<T>& operator=(Matrix<T> other)
    {
        if (this == &other)
        {
            return *this;
        }
        this->_rows = other._rows;
        this->_cols = other._cols;
        this->_elements = other._elements;
        return *this;
    }

    /**
     * throws exception when the size of this and other are not the same.
     * @param other the matrix to add to this.
     * @return new matrix which is the sum of this matrix and other.
     */
    Matrix<T> operator+(const Matrix<T> &other) const
    {
        if (this->_rows != other._rows || this->_cols != other._cols)
        {
            throw SumException();
        }
        Matrix<T> matrix(_rows, _cols);
        unsigned int i;
        for (i = 0; i < other._elements.size(); i++)
        {
            matrix._elements.at(i) = _elements.at(i) + other._elements.at(i);
        }
        return matrix;
    }

    /**
     * throws exception when the size of this and other are not the same.
     * @param other the matrix the subtract from this.
     * @return new matrix which is this minus other.
     */
    Matrix<T> operator-(const Matrix<T> &other) const
    {
        if (this->_rows != other._rows || this->_cols != other._cols)
        {
            throw SumException();
        }
        Matrix<T> matrix(_rows, _cols);
        int i  = 0;
        for (i = 0; i < other._elements.size(); i++)
        {
            matrix._elements.at(i) = _elements.at(i) - other._elements.at(i);
        }
        return matrix;
    }

    /**
     * throws exception when the multiplication is illegal.
     * @param other the matrix to multiply by this.
     * @return new matrix which is this multiply by other
     */
    Matrix<T> operator*(const Matrix<T> &other) const
    {
        if (this->_cols != other._rows)
        {
            throw MulException();
        }
        Matrix<T> matrix(_rows, other._cols);
        unsigned int i, j, k;
        T sum(0);
        for (i = 0; i < _rows; i++)
        {
            for (j = 0; j < other._cols; ++j)
            {
                sum = 0;
                for (k = 0; k < _cols; ++k)
                {
                    sum += this->operator()(i, k) * other(k, j);
                }
                matrix(i, j) = sum;
            }
        }
        return matrix;
    }

    /**
     * @param other to matrix to compare to this.
     * @return true iff the size and the values of the two matrixes are the same.
     */
    bool operator==(const Matrix<T> &other) const
    {
        if (_rows != other._rows || _cols != other._cols)
        {
            return false;
        }
        unsigned int i = 0;
        for (i = 0; i < _elements.size(); i++)
        {
            if (_elements.at(i) != other._elements.at(i))
            {
                return false;
            }
        }
        return true;
    }

    /**
     * @param other the matrix to compare to this.
     * @return true iff operator= returns false.
     */
    bool operator!=(const Matrix<T> &other) const
    {
        return !operator==(*this);
    }

    /**
     * @param row the rows.
     * @param cols the columns.
     * @return the value at the given location.
     */
    const T& operator()(unsigned int row, unsigned int cols) const
    {
        int cell = row * _cols + cols;
        return _elements.at(cell);
    }

    /**
     * @param row the rows.
     * @param cols the columns.
     * @return the value at the given location.
     */
    T& operator()(unsigned int row, unsigned int cols)
    {
        int cell = row * _cols + cols;
        return _elements.at(cell);
    }

    /**
     * @return true iff the matrix is squared.
     */
    bool isSquareMatrix() const
    {
        return _rows  == _cols;
    }

    /**
     * transpose function to the matrix.
     * @return the transpose matrix of the matrix. throws exception if the matrix is not squared.
     */
    Matrix<T> trans() const
    {
        if (_rows != _cols)
        {
            throw TransException();
        }
        Matrix<T> matrix(_rows, _cols);
        unsigned int i, j;
        for (i = 0; i < _rows; ++i)
        {
            for (j = 0; j < _cols; j++)
            {
                matrix(i, j) = this->operator()(j, i);
            }
        }
        return matrix;
    }

    /**
     * prints the matrix to the screen.
     * @param os the os you like to use.
     * @param matrix the matrix to print.
     * @return os with the values of the matrix.
     */
    friend std::ostream& operator<<(std::ostream& os, const Matrix<T> &matrix)
    {
        unsigned int i, j;
        for (i = 0; i < matrix._rows; ++i)
        {
            for (j = 0; j < matrix._cols; ++j)
            {
                os << matrix(i, j) << "\t";
            }
            os << std::endl;
        }
        return os;
    }

    /**
     * @return number of rows.
     */
    int rows() const
    {
        return _rows;
    }

    /**
     * @return number of columns.
     */
    int cols() const
    {
        return _cols;
    }

    /**
     * @return iterator the the beginning of the matrix.
     */
    const_iterator begin() const
    {
        return _elements.cbegin();
    }

    /**
     * @return iterator to the end of the matrix.
     */
    const_iterator end() const
    {
        return _elements.cend();
    }


private:

    unsigned int _rows;
    unsigned int _cols;
    std::vector<T> _elements;
};

/**
 * General Exception.
 */
class MatrixException : public std::exception
{

public:

    /**
     * @return Explanation of the exception.
     */
    virtual const char* what() const noexcept
    {
        return "General error";
    }
};

/**
 * Multiplication error.
 */
class MulException : public MatrixException
{

public:

    /**
     * @return Explanation of the exception.
     */
    virtual const char* what() const noexcept
    {
        return "Multiplication of those matrix is not allowed!";
    }
};

/**
 * Sum exception.
 */
class SumException : public MatrixException
{

public:

    /**
     * @return Explanation of the exception.
     */
    virtual const char* what() const noexcept
    {
        return "summing or subtracting those matrix is not allowed!";
    }
};

/**
 * Creation exception.
 */
class CreationException : public MatrixException
{

public:

    /**
     * @return Explanation of the exception.
     */
    virtual const char* what() const noexcept
    {
        return "rows * cols != number of cells";
    }
};

/**
 * Transpose exception.
 */
class TransException : public MatrixException
{

public:

    /**
     * @return Explanation of the exception.
     */
    virtual const char* what() const noexcept
    {
        return "trans function can be performed only on squared matrix";
    }
};

/**
 * Illegal size exception.
 */
class IllegalSizeException : public MatrixException
{

public:

    /**
     * @return Explanation of the exception.
     */
    virtual const char* what() const noexcept
    {
        return "trans function can be performed only on squared matrix";
    }
};

/**
 * transpose function in case T is Complex.
 * @return the transposed-harmitian matrix.
 */
template <>
Matrix<Complex> Matrix<Complex>::trans() const
{
    if (_rows != _cols)
    {
        throw TransException();
    }
    Matrix<Complex> matrix(_cols, _rows);
    unsigned int i, j;
    for (i = 0; i < matrix._rows; ++i)
    {
        for (j = 0; j < matrix._cols; j++)
        {
            Complex temp = this->operator()(j, i);
            matrix(i, j) = temp.conj();
        }
    }
    return matrix;
}

#endif //EX3_MATRIX_H
