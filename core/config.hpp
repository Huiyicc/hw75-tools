//
// Created by HuiYi on 2023/6/3.
//

#ifndef HW_TOOLS_CONFIG_HPP
#define HW_TOOLS_CONFIG_HPP
#include <memory>
#include <optional>
#include <QString>
#include "nlohmann/json.hpp"
using nJson = nlohmann::json;

namespace core {
	class config {
	public:
		config() = default;

		~config() = default;

		/** 获取实例  */
		static config &Instance();

		/** 获取值 */
		template<typename T>
		T GetValue(const std::string &path, const T &default_value) {
			try {
				return m_config->at(path).get<T>();
			} catch (...) {
				return default_value;
			}
		};

		template<typename T>
		void SetValue(const std::string &path, const T &value, bool autoSave = true) {
			m_config->operator[](path) = value;
			if (autoSave) {
				Save();
			}
		};

		void Save();

		static std::shared_ptr<nJson> GetConfig() {
			return m_config;
		}

	public:
		static constexpr const char *CONFIF_FILE = "config.cfg";

		static constexpr const char *HTTPAPI_ENABLE = "httpapi.enable";
		static constexpr const char *HTTPAPI_PORT = "httpapi.port";

	private:

		static config m_instance;
		static std::shared_ptr<nJson> m_config;
	};

}


#endif //HW_TOOLS_CONFIG_HPP
