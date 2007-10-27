#ifndef TEST_PLUGIN_H
#define TEST_PLUGIN_H

#include "ag_plugin.h"

class TestPlugin:public AGPlugin
{
 public:
  std::string getName();
};

DECLARE_PLUGIN(TestPlugin)

#endif
