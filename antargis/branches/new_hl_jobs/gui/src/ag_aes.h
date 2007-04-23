#ifndef AG_AES_H
#define AG_AES_H

#include <string>
#include <ag_base.h>

std::string AGEXPORT aes_enc(const std::string &key,const std::string &c);
std::string AGEXPORT aes_dec(const std::string &key,const std::string &c);

#endif
