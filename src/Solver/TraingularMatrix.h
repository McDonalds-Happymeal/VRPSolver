#pragma once

#include <iomanip>
template <typename T>
class TraingularMatrix
{
private:
	std::vector<T> matrix;
	
public:
	const unsigned int m_size;
	const unsigned int m_depth;


	TraingularMatrix(const unsigned& depth);
	~TraingularMatrix();

	T& operator()(const unsigned& x, const unsigned& y);
	void print(int width = 8);

};

//size calculated in iniitliaser list.
template<typename T>
inline TraingularMatrix<T>::TraingularMatrix(const unsigned& depth) : m_depth(depth), m_size(depth* (depth - 1) / 2)
{
	matrix.resize(m_size);
}

template<typename T>
inline TraingularMatrix<T>::~TraingularMatrix()
{
}


template<typename T>
inline T& TraingularMatrix<T>::operator()(const unsigned& x, const unsigned& y)
{
	if (x > y) return matrix[(x * (x - 1) / 2 + y)];
	else return matrix[(y * (y - 1) / 2 + x)];
}

template<typename T>
inline void TraingularMatrix<T>::print(int width)
{
	int i = 0;
	int row = 0;
	for (T x : matrix) {
		std::cout << std::setw(width) << x << ",";
		i++;
		if (i > row) {
			i = 0;
			row++;
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

