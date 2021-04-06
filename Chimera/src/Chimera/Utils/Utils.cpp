#include "cmpch.h"
#include "Utils.h"

namespace Chimera
{
    bool VectorUtils::VectorOfStringGetter(void* data, int n, const char** out_text)
    {
        auto& vector = *static_cast<std::vector<std::string>*>(data);
        if (n < 0 || n >= static_cast<int>(vector.size())) { return false; }
        *out_text = vector.at(n).c_str();
        return true;

    }
    bool StringUtils::TransformIntoRelativePath(const std::string& basePath, std::string& currentPath)
    {
        //CM_CORE_ERROR(basePath);
        if (!currentPath.compare(0, basePath.size(), basePath))
        {
            currentPath.erase(0, basePath.length());
            return true;
        }
        return false;
    }
}
