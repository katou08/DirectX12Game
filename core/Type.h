#pragma once

constexpr int INVALID_ID = -1;
#if 1
using ID_TYPE = int;
#else
struct ID_TYPE // int w/ default value -1
{
	ID_TYPE() : _id(INVALID_ID) {}
	ID_TYPE(int id) : _id(id) {}
	operator int() { return _id; }
	ID_TYPE& operator++() { return _id; }
	int _id;
};
#endif

using RTV_ID = ID_TYPE;
using DSV_ID = ID_TYPE;
using CBV_ID = ID_TYPE;
using SRV_ID = ID_TYPE;