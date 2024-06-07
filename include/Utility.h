#pragma once

#define VkCall(status, msg...) if(status != VK_SUCCESS)\
{\
    throw std::runtime_error(msg);\
}

namespace utils 
{
    
}