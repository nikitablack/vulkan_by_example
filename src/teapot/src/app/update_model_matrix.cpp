#include "app/App.h"

#include "Dense"

#include <cmath>

using Matrix4 = Eigen::Matrix<float, 4, 4, Eigen::RowMajor>;

namespace
{

Matrix4 model_matrix(uint32_t const n)
{
	Eigen::AngleAxis<float> const aa{n * 0.001f, Eigen::Vector3f{0.0f, 0.0f, 1.0f}};
	Eigen::Matrix3f const r{aa.toRotationMatrix()};
	
	Matrix4 mat{Matrix4::Identity()};
	mat.row(0) << r.row(0), 0;
	mat.row(1) << r.row(1), 0;
	mat.row(2) << r.row(2), 0;
	
	return mat;
}

} // namespace

namespace app
{

void update_model_matrix(uint32_t const n, char * const memPtr)
{
	Matrix4 mat{model_matrix(n)};
	
	memcpy(memPtr, mat.data(), sizeof(float) * 16);
}

} // namespace app