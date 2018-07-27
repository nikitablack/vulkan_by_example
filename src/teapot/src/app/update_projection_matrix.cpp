#include "app/App.h"

#include "Dense"

#include <cmath>

using Matrix4 = Eigen::Matrix<float, 4, 4, Eigen::RowMajor>;

namespace
{

Matrix4 perspective(float const fovy, float const aspect, float const zNear, float const zFar)
{
	float const yScale{1.0f / std::tan(fovy / 2)};
	float const xScale{yScale / aspect};
	
	Matrix4 mat{};
	mat << xScale, 0, 0, 0,
			0, yScale, 0, 0,
			0, 0, zFar / (zFar - zNear), 1,
			0, 0, -zNear*zFar / (zFar - zNear), 0;
	
	return mat;
}

} // namespace

namespace app
{

void update_projection_matrix(float const aspectRatio, char * const memPtr)
{
	Matrix4 mat{perspective(45.0f *  3.14159265358979323846f / 180.0f, aspectRatio, 0.1f, 100.0f)};
	
	memcpy(memPtr, mat.data(), sizeof(float) * 16);
}

} // namespace app