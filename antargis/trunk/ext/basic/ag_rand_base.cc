#include <ag_rand_base.h>

float AGRandomizerBase::operator()(float f)
{
  return rand()/float(RAND_MAX);
}
int AGRandomizerBase::operator()(int i)
{
  return rand()%i;
}
