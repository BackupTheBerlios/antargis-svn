#ifndef HEURISTIC_H
#define HEURISTIC_H

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#include <ag_geometry.h>

class HeuristicFunction
{
 public:
  typedef std::pair<AGVector2,AGVector2> Input;
  typedef float Output;

  virtual ~HeuristicFunction()
    {
    }

  virtual Output operator()(const Input &input);
};

class StoredHeuristicFunction:public HeuristicFunction
{
  // FIXME: maybe store measure-count, too - and average!!!

  std::vector<float> a;
  size_t r;
  float w;
 public:
  StoredHeuristicFunction(size_t res,float width);

  void store(Input in,Output out);

  void display();

  virtual Output operator()(const Input &input);
 private:
  size_t getIndex(const Input &in);
};


#endif

