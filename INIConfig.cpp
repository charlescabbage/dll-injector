// Copyright 2012 Fredy Wijaya
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "INIConfig.h"

namespace cppiniparser {

bool INIConfig::HasSection(const std::string& sectionName) const {
    if (config.find(sectionName) == config.end()) {
        return false;
    }
    return true;
}

bool INIConfig::HasOption(const std::string& sectionName, const std::string& optionName) const {
    if (!HasSection(sectionName)) {
        return false;
    }
    const std::map<std::string, std::string>& options = config.at(sectionName);
    if (options.find(optionName) == options.end()) {
        return false;
    }
    return true;
}

std::string INIConfig::Get(const std::string& sectionName, const std::string& optionName, std::string defaultValue) const {
	if (!HasOption(sectionName, optionName)) {
		return defaultValue;
	}

    const std::map<std::string, std::string>& options = config.at(sectionName);
    return options.at(optionName);
}

unsigned long INIConfig::GetInteger(const std::string& sectionName, const std::string& optionName, unsigned long defaultValue) {
	std::string valstr = Get(sectionName, optionName, "");
	const char* value = valstr.c_str();
	char* end;
	unsigned long n = strtoul(value, &end, 0);
	return end > value ? n : defaultValue;
}

double INIConfig::GetReal(const std::string& sectionName, const std::string& optionName, double defaultValue) {
	std::string valstr = Get(sectionName, optionName, "");
	const char* value = valstr.c_str();
	char* end;
	double n = strtod(value, &end);
	return end > value ? n : defaultValue;
}

bool INIConfig::GetBoolean(const std::string& sectionName, const std::string& optionName, bool defaultValue) {
	std::string valstr = Get(sectionName, optionName, "");

	// convert to lower case to make string comparisons case-insensitive
	std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
	
	if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
		return true;
	
	if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
		return false;

	return defaultValue;
}

std::vector<std::string> INIConfig::Sections() const {
    std::vector<std::string> sections;
    std::map<std::string, std::map<std::string, std::string> >::const_iterator i = config.begin();
    for (; i != config.end(); ++i) {
        sections.push_back(i->first);
    }
    return sections;
}

std::vector<std::string> INIConfig::Options(const std::string& sectionName) const {
	if (!HasSection(sectionName)) {
		return std::vector<std::string>();
	}

    const std::map<std::string, std::string>& opts = config.at(sectionName);
    std::vector<std::string> options;
    std::map<std::string, std::string>::const_iterator i = opts.begin();
    for (; i != opts.end(); ++i) {
        options.push_back(i->first);
    }
    return options;
}

bool INIConfig::SetOption(const std::string& sectionName, const std::string& optionName, const std::string& optionValue) {
	if (!HasOption(sectionName, optionName)) {
		return false;
	}

    std::map<std::string, std::string>& options = config[sectionName];
    options[optionName] = optionValue;
	return true;
}

bool INIConfig::RemoveSection(const std::string& sectionName) {
	if (!HasSection(sectionName)) {
		return false;
	}

    config.erase(sectionName);
	return true;
}

bool INIConfig::RemoveOption(const std::string& sectionName, const std::string& optionName) {
	if (!HasOption(sectionName, optionName)) {
		return false;
	}

    config[sectionName].erase(optionName);
	return true;
}

bool INIConfig::AddSection(const std::string& sectionName) {
    if (HasSection(sectionName)) {
		// duplicate section found
		return false;
    }
    std::map<std::string, std::string> emptyMap;
    config[sectionName] = emptyMap;
	return true;
}

bool INIConfig::AddOption(const std::string& sectionName, const std::string& optionName, const std::string& optionValue) {
	if (!HasSection(sectionName)) {
		return false;
	}

    std::map<std::string, std::string>& options = config[sectionName];
    if (HasOption(sectionName, optionName)) {
		// duplicate option found
		return false;
    }
    options[optionName] = optionValue;
	return true;
}

void INIConfig::Clear()
{
	config.clear();
}

}
