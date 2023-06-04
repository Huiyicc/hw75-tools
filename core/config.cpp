//
// Created by HuiYi on 2023/6/3.
//


#include "config.hpp"
#include "utils/file.hpp"
#include "config_templet.hpp"

using namespace core;

config config::m_instance;
std::shared_ptr<nJson> config::m_config = nullptr;

config &config::Instance() {
	if (m_config == nullptr) {
		utils::FileIfSet(CONFIF_FILE, true,CONFIF_TEMPLET);
		auto data=utils::ReadFile(CONFIF_FILE);
		m_config = std::make_shared<nJson>(nJson::parse(data.toStdWString()));
	}
	return m_instance;
}

void config::Save() {
	utils::WriteFile(CONFIF_FILE,QString::fromStdString(m_config->dump()));
}