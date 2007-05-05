#ifndef MESH_SORT_H
#define MESH_SORT_H

/**
   The class is for sorting purpose only. It provides the operator() function, that's needed
   for the STL-sorting algorithms.
   Here SceneNodes are sorted by their middle distance the camera.
*/
class SortDistance
{
  AGVector3 cam;
public:
  SortDistance(AGVector3 c):cam(c){}

  bool operator()(const SceneNode *n1,const SceneNode *n2)
  {
    AGVector3 m1=const_cast<SceneNode*>(n1)->bbox().base+const_cast<SceneNode*>(n1)->bbox().dir*0.5;
    AGVector3 m2=const_cast<SceneNode*>(n2)->bbox().base+const_cast<SceneNode*>(n2)->bbox().dir*0.5;

    return (m1-cam).length2()<(m2-cam).length2();
  }
};

/**
   This is a sorting class, too. It sorts by the given "SortOrder" of the scene-nodes
*/
class SortOrder
{
public:
  SortOrder(){}

  bool operator()(const SceneNode *n1,const SceneNode *n2)
  {
    return n1->getOrder()<n2->getOrder();
  }
};


#endif
