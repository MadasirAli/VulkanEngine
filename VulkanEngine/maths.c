#include "maths.h"

matrix4x1 MultiplyMatrix4x4ByMatrix4x1(matrix4x4 matrix1, matrix4x1 matrix2)
{
	matrix4x1 result = { 0 };

	result._00 = (matrix1._00 * matrix2._00) + (matrix1._01 * matrix2._10) + (matrix1._02 * matrix2._20) + (matrix1._03 * matrix2._30);
	result._10 = (matrix1._10 * matrix2._00) + (matrix1._11 * matrix2._10) + (matrix1._12 * matrix2._20) + (matrix1._13 * matrix2._30);
	result._20 = (matrix1._20 * matrix2._00) + (matrix1._21 * matrix2._10) + (matrix1._22 * matrix2._20) + (matrix1._23 * matrix2._30);
	result._30 = (matrix1._30 * matrix2._00) + (matrix1._31 * matrix2._10) + (matrix1._32 * matrix2._20) + (matrix1._33 * matrix2._30);

	return result;
}

matrix4x1 MultiplyMatrix4x4ByVector4(matrix4x4 matrix, vector4 vector)
{
	matrix4x1 result = { 0 };

	result._00 = (matrix._00 * vector.x) + (matrix._01 * vector.y) + (matrix._02 * vector.z) + (matrix._03 * vector.w);
	result._10 = (matrix._10 * vector.x) + (matrix._11 * vector.y) + (matrix._12 * vector.z) + (matrix._13 * vector.w);
	result._20 = (matrix._20 * vector.x) + (matrix._21 * vector.y) + (matrix._22 * vector.z) + (matrix._23 * vector.w);
	result._30 = (matrix._30 * vector.x) + (matrix._31 * vector.y) + (matrix._32 * vector.z) + (matrix._33 * vector.w);

	return result;
}