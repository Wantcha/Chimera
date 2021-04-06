#pragma once
#include <vector>

namespace Chimera
{
	class VectorUtils
	{
	public:
		static bool VectorOfStringGetter(void* data, int n, const char** out_text);

		template <typename T>
		static bool IsElementInVector(const std::vector<T>& vector, const T& element)
		{
			std::vector<T>::const_iterator position = std::find(vector.begin(), vector.end(), element);
			return position != vector.end();
		}

		template <typename T>
		static typename std::vector<T>::const_iterator GetElementPosition(const std::vector<T>& vector, const T& element)
		{
			std::vector<T>::const_iterator position = std::find(vector.begin(), vector.end(), element);
			return position;
		}
	};

	class StringUtils
	{
	public:
		static bool TransformIntoRelativePath(const std::string& basePath, std::string& currentPath);
	};
}
