#pragma once

#include "Windows.h"

#include "Platform.h"

inline void call(const LPCWSTR library, const LPCSTR function) {
	HINSTANCE hInst = LoadLibrary(library);
	if (!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	typedef void(*_func)();
	_func func = _func(GetProcAddress(hInst, function));
	try {
		func();
	}
	catch (...) {
		std::cerr << "call: Unknown error\n";
		FreeLibrary(hInst);
	}
	FreeLibrary(hInst);
}

template <class  type>
void call(const LPCWSTR library, const LPCSTR function, type& pass) {
	HINSTANCE hInst = LoadLibrary(library);
	if (!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	typedef void(*_func)(type&);
	_func func = _func(GetProcAddress(hInst, function));
	try {
		func(pass);
		FreeLibrary(hInst);
	}
	catch (...) {
		std::cerr << "call: Unknown error\n";
		FreeLibrary(hInst);
	}
}


/////////////////////////////////////////////////////////
//��ֹlibrary�з���ֵ������Ϊ��extern"C",Ҳ��Ϊ�˱�����û���
/*template <class rtn, class  type>
rtn call(const LPCWSTR library, const LPCSTR function, type& pass) {
	HINSTANCE hInst = LoadLibrary(library);
	if (!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	typedef void(*_func)(type);
	_func func = _func(GetProcAddress(hInst, function));
	rtn rt;
	try {
		rt = func(pass);
		FreeLibrary(hInst);
	}
	catch (...) {
		std::cerr << "call: Unknown error\n";
		FreeLibrary(hInst);
	}
	return rt;
}*/

