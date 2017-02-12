#ifndef HELPERFUNCTIONS_HPP_
#define HELPERFUNCTIONS_HPP_

namespace OsirisI
{
	namespace Utilities
	{
		class HelperFunctions
		{
		public:
			static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
			static std::vector<std::string> split(const std::string &s, char delim);
			static void replace(std::string& str, const std::string& from, const std::string& to);
			static bool EndsWith(std::string const &fullString, std::string const &ending);
			static bool StringContains(std::string str, std::string sequence);
			static int StringCharacterOccurrences(std::string str, char character);
			static std::string ToLower(std::string str);
			static void TrimLeft(std::string &s);
			static void TrimRight(std::string &s);
			static void Trim(std::string &s);

			static unsigned int GetFileSize(std::string filePath);

			template <class T, class R>
			static bool IsTypeOf(R ptr)
			{
				T conPTR = dynamic_cast<T>(ptr);

				if (conPTR == nullptr)
				{
					return false;
				}
				else
				{
					return true;
				}
			}

			template <class T, class R>
			static bool IsTypeOf_SharedPtr(R ptr)
			{
				std::shared_ptr<T> conPTR = std::dynamic_pointer_cast<T>(ptr);

				if (conPTR.get() == nullptr)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
		};
	}
}
#endif /* HELPERFUNCTIONS_HPP_ */
