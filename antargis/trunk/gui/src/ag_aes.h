#ifndef AG_AES_H
#define AG_AES_H

#include <string>

std::string aes_enc(const std::string &key,const std::string &c);
std::string aes_dec(const std::string &key,const std::string &c);

#endif
