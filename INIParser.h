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

#ifndef INIPARSER_H_
#define INIPARSER_H_

#include <string>

#include "INIConfig.h"

namespace cppiniparser {

/**
 * This class is responsible for reading/writing an INI file.
 */
class INIParser {
public:
    /**
     * Reads INI file to create an INIConfig.
     *
     * If there is a problem in reading an INI file, false will be returned.
     */
    static bool Read(INIConfig& config, const std::string& filename);

    /**
     * Writes an INI file from an INIConfig.
     *
     * If there is a problem in writing an INI file, false will be returned.
     */
    static bool Write(INIConfig& iniConfig, const std::string& filename);

	/**
	* Modifies an INI file from an INIConfig.
	*
	* If there is a problem in writing an INI file, false will be returned.
	*/
	static bool Modify(INIConfig& iniConfig, const std::string& filename);

    virtual ~INIParser() {}
};

}

#endif /* INIPARSER_H_ */
