#ifndef _PTR_H_
#define _PTR_H_

#include <memory>

template<typename T>
using Ptr = std::shared_ptr<T>; // 智能指针，简写

#endif // !_PTR_H_
