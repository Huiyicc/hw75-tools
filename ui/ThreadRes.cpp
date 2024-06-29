//
// Created by 19254 on 24-4-10.
//
#include <thread>
#include <memory>


std::shared_ptr<std::thread> ctrlConnectTickThread = nullptr;
std::shared_ptr<std::thread> m_pluginCallBack = nullptr;