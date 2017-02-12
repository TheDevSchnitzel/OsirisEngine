#ifndef CONVERTER_HPP_
#define CONVERTER_HPP_

namespace OsirisI {
	class Converter {
		private:
			template<class T>
			static T StringToX(const std::string& s);
			template<class T>
			static std::string XToString(T value);

		public:
			static double StringToDouble(std::string value);
			static int StringToInt(std::string value);
			static float StringToFloat(std::string value);	
			static std::wstring StringToWString(std::string value);
			static LPCWSTR StringToLPCWSTR(std::string value) ;
			static std::string WStringToString(std::wstring value);

			static std::string DoubleToString(double value);
			static std::string IntToString(int value);
			static std::string FloatToString(float value);

			//Math
			static float GradToRad(float value);
			static float RadToGrad(float value);
	};
}

#endif /* CONVERTER_HPP_ */
