#include "pch.h"
#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	Identity();
}

Matrix4x4::Matrix4x4(float* matrix)
	: _11(*matrix)
	, _12(*matrix + 1)
	, _13(*matrix + 2)
	, _14(*matrix + 3)

	, _21(*matrix + 4)
	, _22(*matrix + 5)
	, _23(*matrix + 6)
	, _24(*matrix + 7)

	, _31(*matrix + 8)
	, _32(*matrix + 9)
	, _33(*matrix + 10)
	, _34(*matrix + 11)

	, _41(*matrix + 12)
	, _42(*matrix + 13)
	, _43(*matrix + 14)
	, _44(*matrix + 15)
{

}

void Matrix4x4::Identity()
{
	_11 = 1.0f;
	_12 = 0.0f;
	_13 = 0.0f;
	_14 = 0.0f;

	_21 = 0.0f;
	_22 = 1.0f;
	_23 = 0.0f;
	_24 = 0.0f;

	_31 = 0.0f;
	_32 = 0.0f;
	_33 = 1.0f;
	_34 = 0.0f;

	_41 = 0.0f;
	_42 = 0.0f;
	_43 = 0.0f;
	_44 = 1.0f;
}
