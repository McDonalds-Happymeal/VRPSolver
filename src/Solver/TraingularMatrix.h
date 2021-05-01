#pragma once
template <typename T>
class TraingularMatrix
{
private:
	std::vector<T> matrix;
	const unsigned int m_size;
	const unsigned int m_depth;
public:
	TraingularMatrix(unsigned int depth);
	~TraingularMatrix();

	T& operator()(const unsigned&, const unsigned&);
	void print();
};

//size calcualted in iniitliaser list.
template<typename T>
inline TraingularMatrix<T>::TraingularMatrix(unsigned int depth) : m_depth(depth), m_size(depth* (depth - 1) / 2 + depth)
{
	matrix.resize(M_size);
}

template<typename T>
inline TraingularMatrix<T>::~TraingularMatrix()
{
}

template<typename T>
inline T& TraingularMatrix<T>::operator[](size_type x, size_type y)
{
	if (x < y) return matrix[x * depth + y];
	else return matrix[y * depth + x];
}
