#ifndef __FS_H
#define __FS_H

#include <string>

void initFS(const char *argv0);
std::string loadFile(const std::string &pName);
void saveFile(const std::string &pName,const std::string &pContent);

bool fileExists(const std::string &pName);

#endif
