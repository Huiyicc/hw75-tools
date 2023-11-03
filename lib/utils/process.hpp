//
// Created by 19254 on 2023/10/10.
//

#ifndef HW_TOOLS_PROCESS_HPP
#define HW_TOOLS_PROCESS_HPP

namespace Lib {

struct ProcessInfo {
    std::string Name;
    uint64_t ID;
};

size_t ProcessList(std::vector<ProcessInfo>& list);
size_t ProcessList(std::map<std::string,uint64_t>& list);

}


#endif //HW_TOOLS_PROCESS_HPP
