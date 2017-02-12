#include "stdafx.h"

namespace OsirisI {
	template<class T>
	T Converter::StringToX(const std::string& s) {
		std::istringstream stream (s);
		T t;
		stream >> t;
		return t;
	}
	template<class T>
	std::string Converter::XToString(T value) {
		std::ostringstream sstream;
		sstream << value;
		return sstream.str();
	}

	std::string Converter::DoubleToString(double value) {
		return XToString(value);
	}
	std::string Converter::IntToString(int value) {
		return XToString(value);
	}
	std::string Converter::FloatToString(float value) {
		return XToString(value);
	}


	double Converter::StringToDouble(std::string value) {
		return StringToX<double>(value);
	}
	int Converter::StringToInt(std::string value) {
		return StringToX<int>(value);
	}
	float Converter::StringToFloat(std::string value) {
		return StringToX<float>(value);
	}	

	std::wstring Converter::StringToWString(std::string value) {	
		/*const std::ctype<wchar_t>& CType = std::use_facet<std::ctype<wchar_t> >(std::locale());
		std::vector<wchar_t> wideStringBuffer(value.length());
		CType.widen(value.data(), value.data() + value.length(), &wideStringBuffer[0]);
		std::wstring wstr = std::wstring(&wideStringBuffer[0], wideStringBuffer.size());
		return wstr;
		*/
		USES_CONVERSION;
		LPCSTR a = value.c_str();
		LPCWSTR w = A2W(a);
		return std::wstring(w);
	}
	LPCWSTR Converter::StringToLPCWSTR(std::string value) {	
		USES_CONVERSION;
		LPCSTR a = value.c_str();
		LPCWSTR w = A2W(a);
		return w;
	}
	std::string Converter::WStringToString(std::wstring value) {
		int slength = (int)value.length() + 1;
		int len = WideCharToMultiByte(CP_ACP, 0, value.c_str(), slength, 0, 0, 0, 0);
		char* buf = new char[len];
		WideCharToMultiByte(CP_ACP, 0, value.c_str(), slength, buf, len, 0, 0);
		std::string r(buf); // If this throws an exception, you'll leak memory. It will be slightly easier to use a std::vector instead, which does self checking.
		delete[] buf;
		return r;
	} 

	float Converter::GradToRad(float value) {
		return value * 0.0174532925f;
	}
	float Converter::RadToGrad(float value) {
		return value / 0.0174532925f;
	}
}
