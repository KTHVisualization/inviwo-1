/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#include <modules/pypackages/pyscriptrunner.h>
#include <inviwo/core/util/clock.h>

namespace inviwo {

PyScriptRunner::PyScriptRunner() : script_() { init(this); }

PyScriptRunner::~PyScriptRunner() {}

void PyScriptRunner::onPyhonExecutionOutput(std::string msg,OutputType outputType) {
    if (outputType==PythonExecutionOutputObeserver::standard) {
        if (standard_.empty())
            standard_ += msg;
        else
            standard_ += ("\n" + msg);
    }
    else if (outputType==PythonExecutionOutputObeserver::error) {
        if (error_.empty())
            error_ += msg;
        else
            error_ += ("\n" + msg);
    }
}

void PyScriptRunner::setScript(std::string simpleScript) {
    script_.setSource(simpleScript);
}

void PyScriptRunner::run(std::string simpleScript, bool noLogging) {
    setScript(simpleScript);
    run(noLogging);
}

void PyScriptRunner::run(bool noLogging) {
    clear();
    Clock c;
    c.start();
    bool ok = script_.run();
    c.stop();

    if (ok) {
        LogInfo("Python Script Executed succesfully");
    }

    LogInfo("Execution time: " << c.getElapsedMiliseconds() << " ms");
}

std::string PyScriptRunner::getStandardOutput() { return standard_;}

std::string PyScriptRunner::getError() { return error_;}

void PyScriptRunner::clear() { error_=""; standard_="";}

//Pip module

bool PyScriptRunner::isPipInstalled() {
    return isModuleAvailable("pip");
}

bool PyScriptRunner::isModuleAvailable(std::string moduleName) {
    //module does not require pip
    std::stringstream ss;
    std::string moduleAvailable = moduleName + " is available";
    std::string moduleNotAvailable = moduleName + " is not available";
    ss << "import imp "<< std::endl; 
    ss << "try: "<< std::endl; 
    ss << "    imp.find_module('" + moduleName +"') "<< std::endl;
    ss << "    print '" + moduleAvailable << "'"<< std::endl;
    ss << "except ImportError:" << std::endl;
    ss << "    print '" + moduleNotAvailable << "'"<< std::endl;

    std::string queryPackage(ss.str());
    this->run(queryPackage);
    std::string retError = this->getError();

    bool found = false;
    if (retError!="") {
        //LogWarn("Module query failed");
        found = false;
    }
    else {
        std::string status =  this->getStandardOutput();
        if (status==moduleAvailable) {
            //LogInfo("Pacakage " + moduleName + " is available");
            found = true;
        }
        else {
            //LogInfo("Pacakage " + moduleName + " is not available");
            found = false;
        }
    }
    this->clear();
    return found;
}

//Package management
bool PyScriptRunner::isPackageAvailable(std::string packageName, bool forceAvailabilityCheck) {
    if (!forceAvailabilityCheck) {
        if (std::find(availablePacakges_.begin(), availablePacakges_.end(), packageName)!=availablePacakges_.end()) 
            return true;
    }

    std::stringstream ss;
    std::string packageFound = "success";
    std::string packageNotFound = "failed";
    std::string pipFuncs = parsePipUtil();
    ss << pipFuncs << std::endl;
    ss << "pipSearch('" << packageName << "')" << std::endl;

    std::string searchPackage(ss.str());
    this->run(searchPackage);
    std::string retError = this->getError();

    bool found = false;
    if (retError!="") {
        //LogWarn("Package install failed");
        found = false;
    }
    else {
        std::string status =  this->getStandardOutput();
        if (status==packageFound) {
            //LogInfo("Pacakage " + packageName + " is installed successfully");
            found = true;
        }
        else {
            //LogInfo("Pacakage " + packageName + " is not installed");
            found = false;
        }
    }
    this->clear();

    if (!found) {
        /*LogWarn("Unable to install " << packageName << std::endl
            << "Manually install from one of the following sites :" << std::endl
            << "https://pypi.python.org/pypi/pip" << std::endl
            << "http://www.lfd.uci.edu/~gohlke/pythonlibs/ (unofficial)" << std::endl
            << ". Manual install recommended.");*/
    }

    if (found && std::find(availablePacakges_.begin(), availablePacakges_.end(), packageName)==availablePacakges_.end()) {
        availablePacakges_.push_back(packageName);
    }

    return found;
}

bool PyScriptRunner::installPackage(std::string packageName) {
    if (isPackageAvailable(packageName)) {
        LogWarn("Package already installed");
        return true;
    }

    if (!isPipInstalled()) {
        std::stringstream ss; 
        ss << "To install packages pip is required " << std::endl;
        ss << "Manually install pip from one of the following sites :" << std::endl;
        ss << "https://pypi.python.org/pypi/pip" << std::endl;
        ss << "http://www.lfd.uci.edu/~gohlke/pythonlibs/ (unofficial C-Python binaries)" << std::endl;
        return false;
    }

    std::stringstream ss;
    std::string packageInstalled = "success";
    std::string packageNotInstalled = "failed";
    std::string pipFuncs = parsePipUtil();
    ss << pipFuncs << std::endl;
    ss << "pipInstall('" << packageName << "')" << std::endl;

    std::string installPackage(ss.str());
    this->run(installPackage);
    std::string retError = this->getError();

    bool installed = false;
    if (retError!="") {
        LogWarn("Package install failed");
        installed = false;
    }
    else {
        std::string status =  this->getStandardOutput();
        if (status==packageInstalled) {
            LogInfo("Pacakage " + packageName + " is installed successfully");
            installed = true;
        }
        else {
            LogInfo("Pacakage " + packageName + " is not installed");
            installed = false;
        }
    }
    this->clear();

    if (!installed) {
        LogInfo("Unable to install " << packageName 
                << "Manually install from one of the following sites :" << std::endl
                << "https://pypi.python.org/pypi/pip" << std::endl
                << "http://www.lfd.uci.edu/~gohlke/pythonlibs/ (unofficial)" << std::endl
                << ". Manual install recommended.");
        return installed;
    }
    
    return isPackageAvailable(packageName, true);
}

bool PyScriptRunner::uninstallPackage(std::string packageName) {
    if (!isPackageAvailable(packageName)) {
        LogWarn("Package is not installed");
        return true;
    }

    std::vector<std::string>::iterator it = std::find(availablePacakges_.begin()
                                                      , availablePacakges_.end(), packageName);
    if (it!=availablePacakges_.end()) {
        availablePacakges_.erase(it);
    }
    LogWarn("Not implemented");
    return false;
}

bool PyScriptRunner::upgradePackage(std::string packageName) {
    LogWarn("Not implemented");
    return false;
}

std::string PyScriptRunner::parsePipUtil() {
    std::string pipUtilFile = InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES) +
                                                                        "pypackages/scripts/pip/piputils.py";

    std::ifstream file(pipUtilFile.c_str());
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return text;
}

} // namespace
