#include "app/App.h"

#include "Dense"

#include <cmath>

using Matrix4 = Eigen::Matrix<float, 4, 4, Eigen::RowMajor>;

namespace
{

Matrix4 lookAt(Eigen::RowVector3f const & eye, Eigen::RowVector3f const & at, Eigen::RowVector3f const & up)
{
	Eigen::RowVector3f const zAxis{(at - eye).normalized()};
	Eigen::RowVector3f const xAxis{(up.cross(zAxis)).normalized()};
	Eigen::RowVector3f const yAxis{zAxis.cross(xAxis)};
	
	Matrix4 mat;
	mat << xAxis.x(), yAxis.x(), zAxis.x(), 0.0f,
			xAxis.y(), yAxis.y(), zAxis.y(), 0.0f,
			xAxis.z(), yAxis.z(), zAxis.z(), 0.0f,
			xAxis.dot(-eye), yAxis.dot(-eye), zAxis.dot(-eye), 1.0f;
	
	return mat;
}

} // namespace

namespace app
{

void update_view_matrix(char * const memPtr)
{
	Matrix4 mat{lookAt({0.0f, 10.0f, 5.5f}, {0.0f, 0.0f, 1.5f}, {0.0f, 0.0f, -1.0f})};
	
	memcpy(memPtr, mat.data(), sizeof(float) * 16);
}

} // namespace app