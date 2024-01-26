//
// Created by 19254 on 2023/10/30.
//
#include "Plugin.hpp"
#include "ui/mainwindow.h"
#include <map>
#include <iostream>

extern MainWindow *g_mainWindow;

namespace Lib::Plugin {

void CallPluginLogInfo(const char *name,const char *raw) {
	std::cout << "["<< name <<"]PluginLogInfo:"  << raw << std::endl;
}

void CallEinkFullUpdateImage(const char *data, int size) {
  g_mainWindow->ctrlEinkPushImage(data, size);
}


std::map<std::string, void *> g_mapPluginFunc = {
		{"PluginLogInfo",                (void *) CallPluginLogInfo},
		{"EinkFullUpdateImage", (void *) CallEinkFullUpdateImage},
};


} // Lib