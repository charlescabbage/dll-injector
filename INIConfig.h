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

#ifndef INICONFIG_H_
#define INICONFIG_H_

#include <string>
#include <vector>
#include <map>
#include <algorithm>

namespace cppiniparser {

/**
 * This class stores INI config information.
 */
class INIConfig {
public:
    /**
     * Checks if a section exists.
     */
    bool HasSection(const std::string& sectionName) const;

    /**
     * Checks if an option exists.
     */
    bool HasOption(const std::string& sectionName, const std::string& optionName) const;

    /**
     * Gets a string option value.
     *
     * If the section name/option name does not exist, the defaultValue will be returned.
     */
    std::string Get(const std::string& sectionName, const std::string& optionName, std::string defaultValue = "") const;

	/**
	* Gets an integer (unsigned long) option value.
	*
	* If the section name/option name does not exist or value is not valid, the defaultValue will be returned.
	*
	* Valid values are decimal "1234", and hex "0xDEADBEEF".
	*/
	unsigned long GetInteger(const std::string& sectionName, const std::string& optionName, unsigned long defaultValue = 0);


	/**
	* Gets a real (floating point double) option value.
	*
	* If the section name/option name does not exist or value is not valid, the defaultValue will be returned.
	*/
	double GetReal(const std::string& sectionName, const std::string& optionName, double defaultValue = 0.0);

	/**
	* Gets a boolean option value.
	*
	* If the section name/option name does not exist or value is not valid, the defaultValue will be returned.
	*
	* Valid true values are "true", "yes", "on", "1", and valid false values are "false", "no", "off", "0" (case-insensitive).
	*/
	bool GetBoolean(const std::string& sectionName, const std::string& optionName, bool defaultValue = false);

    /**
     * Gets all the sections.
     */
    std::vector<std::string> Sections() const;

    /**
     * Gets list of options from a given section name.
     *
     * If the section name does not exist, an empty vector will be returned.
     */
    std::vector<std::string> Options(const std::string& sectionName) const;

    /**
     * Sets an option.
     *
     * If the section name/option name does not exist, function will return false.
     */
    bool SetOption(const std::string& sectionName, const std::string& optionName, const std::string& optionValue);

    /**
     * Removes a section.
     *
     * If the section name does not exist, function will return false.
     */
    bool RemoveSection(const std::string& sectionName);

    /**
     * Removes an option.
     *
     * If the section name/option name does not exist, function will return false.
     */
    bool RemoveOption(const std::string& sectionName, const std::string& optionName);

    /**
     * Adds a new section.
     *
     * If the section name exists, AddSection will return false.
     */
    bool AddSection(const std::string& sectionName);

    /**
     * Adds a new option.
     *
     * If the section name/option name does not exist, function will return false.
     */
	bool AddOption(const std::string& sectionName, const std::string& optionName, const std::string& optionValue);

	/**
	* Clears all sections and options.
	*/
	void Clear();

    virtual ~INIConfig() {}

private:
    // key --> section name
    // value --> map (key --> option name, value --> option value)
    std::map<std::string, std::map<std::string, std::string> > config;
};

}

#endif /* INICONFIG_H_ */
