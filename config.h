#ifndef __CONFIGH__
#define __CONFIGH__

#include <iostream>
#include <map>

#define CONFIG_COMMENT_PRE "#"
#define CONFIG_DELIMITER "="

#define CONFIG_FILE_DESCRIPTION "# Tibria advanced options configuration\n# DO NOT EDIT THIS FILE\n"

typedef std::map<std::string, std::string> valuesMap;

class Config {
	public:
		Config();
		
		bool load(std::string filename);
		bool save();
		void free();
		
		std::string getString(std::string key, std::string def = std::string());
		int getInteger(std::string key, int def = 0);
		bool getBoolean(std::string key, bool def = false);
		
		void setString(std::string key, std::string value);
		void setInteger(std::string key, int value);
		void setBoolean(std::string key, bool value);
		
	private:
		std::map<std::string, std::string> m_values;
		std::string m_file;
		bool m_loaded;
};

#endif
