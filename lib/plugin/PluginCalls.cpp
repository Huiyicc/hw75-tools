//
// Created by 19254 on 2023/10/30.
//
#include "Plugin.hpp"
#include "ui/mainwindow.h"
#include "utils/Log.hpp"
#include <iostream>
#include <map>



namespace Lib::Plugin {

void CallPluginLogInfo(const char *name, const char *raw) {
  PrintInfo("[{}]PluginLogInfo: {}", name, raw);
}

void CallEinkFullUpdateImage(const char *data, int size) {
  g_mainWindow->ctrlEinkPushImage(data, size);
}


std::map<std::string, void *> g_mapPluginFunc = {
    {"PluginLogInfo", (void *) CallPluginLogInfo},
    {"EinkFullUpdateImage", (void *) CallEinkFullUpdateImage},
};


}// namespace Lib::Plugin