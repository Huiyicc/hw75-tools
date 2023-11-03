//
// Created by 19254 on 2023/10/22.
//
#include <http/httplib.h>
#include "httpsvr.hpp"
#include "SvrThread.hpp"

httplib::Server g_svr;
auto m_svrThread = new SvrThread;

void initHTTPSvr() {
    g_svr.Get("/api/test", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("Hello World!", "text/plain");
    });
    g_svr.Post("/api/ctrl/eink/push", [](const httplib::Request &req, httplib::Response &res) {
        // 推送墨水屏图片
        // 全刷
        //req.body;


    });
    m_svrThread->start();
}