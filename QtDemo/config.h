#pragma once
#include<tinyxml.h>
#include "glog/logging.h"
#include "glog/log_severity.h"
class Config
{
public:
	static Config * get_instance(void);
	int load_config(const char* path);
	std::string  get_app_name();
	std::string  get_thread_num();
	std::string  get_img_height();
	std::string  get_img_width();

private:
	Config();
	static Config * m_instance;
public:
	~Config();
private:
	std::string m_app_name;
	std::string m_thread_num;
	std::string m_height;
	std::string m_width;

};

std::string Config::get_app_name()
{
	return m_app_name;
}


std::string Config::get_thread_num()
{
	return m_thread_num;
}

std::string Config::get_img_height()
{
	return m_height;
}

std::string Config::get_img_width()
{
	return m_width;
}

Config::Config() {}
Config* Config::m_instance = new Config();
Config::~Config()
{ 
	if (m_instance)
	{
		delete m_instance;
	}
	
}
Config * Config::get_instance(void)
{
	if (m_instance == NULL)
		m_instance = new Config();
	return m_instance;
}

inline int Config::load_config(const char * path)
{
	TiXmlDocument doc(path);
	bool loadOkay = doc.LoadFile();
	if (!loadOkay)
	{
		LOG(ERROR) << "Could not load test file" << path << "Error =" << doc.ErrorDesc();
		return 1;
	}

	TiXmlElement* root = doc.RootElement();

	for (TiXmlNode* item = root->FirstChild("item"); item; item = item->NextSibling("item"))
	{
		LOG(INFO) << "reading config file ...";

		// read name.  
		TiXmlNode* child = item->FirstChild();
		const char* name = child->ToElement()->GetText();
		if (name)
		{
			m_app_name = name;
			LOG(INFO) << "config: app name = " << name;
		}
		else
		{
			LOG(ERROR) << "get name fail,please check config file";
		}

		// read address.  
		child = item->IterateChildren(child);
		const char* thread = child->ToElement()->GetText();
		if (thread) {
			m_thread_num = thread;
			LOG(INFO) << "config: threads = " << thread;
		}
		else {
			LOG(ERROR) << ("get threads fail,please check config file ");
		}

		// read telephone no.  
		child = item->IterateChildren(child);
		const char* height = child->ToElement()->GetText();
		if (height)
		{
			LOG(INFO) << "config: height = " << height;
		}
		else
		{
			LOG(ERROR) << ("get height fail,please check config file");
		}
		// read e-mail.  
		child = item->IterateChildren(child);
		const char* width = child->ToElement()->GetText();
		if (width)
		{
			LOG(INFO) << "config: width = " << width;
		}
		else
		{
			LOG(ERROR) << ("get width fail,please check config file");
		}
	}
	LOG(INFO) << "read config file succeed.";
	return 0;
}


