//
// Created by 19254 on 2023/10/22.
//

#include "SvrThread.hpp"
#include <http/httplib.h>
extern httplib::Server g_svr;

void SvrThread::run()
{
    g_svr.listen("0.0.0.0", 28903);
}
