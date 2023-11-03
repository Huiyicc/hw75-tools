//
// Created by 19254 on 2023/10/22.
//

#ifndef HW_TOOLS_SVRTHREAD_HPP
#define HW_TOOLS_SVRTHREAD_HPP

#include "QThread"

class SvrThread
        : public QThread {
public:
    explicit SvrThread() {}

private:
    void run() override;

};


#endif //HW_TOOLS_SVRTHREAD_HPP
