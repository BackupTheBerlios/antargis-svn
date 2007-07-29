#include "path_data_v3.h"
#include "path_vector_sort.h"

#include <ag_debug.h>
#include <algorithm>

#include <math.h>

namespace PathFinding
{

  Field::Field(const AGVector2 &pMiddle,size_t pID):mID(pID),
						    mMiddle(pMiddle)
  {
  }

  void Field::insert(const AGVector2 &v,DistanceComputer &pDist)
  {
    mVectors.insert(v);
    mNeighbors.erase(v);
    
    std::list<AGVector2> neighbors=pDist.getNeighbors(v);

    for(std::list<AGVector2>::iterator i=neighbors.begin();i!=neighbors.end();i++)
      {
	if(mVectors.find(*i)==mVectors.end())
	  mNeighbors.insert(*i);
      }

  }

  bool Field::isNeighbor(const AGVector2 &v) const
  {
    return mNeighbors.find(v)!=mNeighbors.end();
  }

  bool Field::hasVector(const AGVector2 &v) const
  {
    return mVectors.find(v)!=mVectors.end();
  }

  AGVector2 Field::getMiddle() const
  {
    return mMiddle;
  }







  void FieldCollection::assign(const AGVector2 &v,size_t pField,DistanceComputer &pDist)
  {
    assert(pField<mFields.size());

    mFields[pField].insert(v,pDist);
    mVec2Field[v]=pField;
  }


  size_t FieldCollection::newField(const AGVector2 &pMiddle)
  {
    size_t id=mFields.size();
    mFields.push_back(Field(pMiddle,id));
    return id;
  }

Field *FieldCollection::getField(size_t pField)
{
  return &(mFields[pField]);
}

  size_t FieldCollection::getFieldId(const AGVector2 &pVector) const
  {
    std::map<AGVector2,size_t>::const_iterator i=mVec2Field.find(pVector);

    if(i!=mVec2Field.end())
      return i->second;

    return  -1;
  }

  size_t FieldCollection::getFieldCount() const
  {
    return mFields.size();
  }












  AGVector2 takeRandom(const std::set<AGVector2> &pSet)
  {
    assert(pSet.size()>0);
    int pos=(int)(rand()%pSet.size());
    std::set<AGVector2>::const_iterator i=pSet.begin();
    for(;i!=pSet.end() && pos>0;pos--,i++);
    return *i;
  }

  void assignFields(FieldCollection &pCollection,DistanceComputer &pDist,size_t pFieldCount)
  {
    std::list<AGVector2> allVectorList=pDist.getAllPassable();

    float maxFieldWidth=sqrt(pDist.width()*pDist.height()/pFieldCount);

    std::set<AGVector2> restOfVectors;

    std::copy(allVectorList.begin(),allVectorList.end(),std::inserter(restOfVectors,restOfVectors.begin()));

    while(restOfVectors.size()>0)
      {
	AGVector2 fieldMiddle=takeRandom(restOfVectors);
	DistanceOrder order(fieldMiddle);

	size_t newField=pCollection.newField(fieldMiddle);
	

	std::set<AGVector2,DistanceOrder> growField(order);
	std::copy(restOfVectors.begin(),restOfVectors.end(),std::inserter(growField,growField.begin()));

	cdebug("MIDDLE:"<<fieldMiddle);
	
	size_t k=0;
	for(std::set<AGVector2,DistanceOrder>::iterator i=growField.begin();i!=growField.end() && k<30;i++,k++)
	  {
	    cdebug(" "<<*i<<"  "<<(*i-fieldMiddle).length());
	  }

	do
	  {
	    AGVector2 current=*growField.begin();
	    
	    if((current-fieldMiddle).length()<maxFieldWidth)
	      {
		growField.erase(growField.begin());
		restOfVectors.erase(current);
		pCollection.assign(current,newField,pDist);
	      }
	    else
	      break;
	  }while(growField.size()>0);
      }
    


    cdebug("FIELDS:"<<pCollection.getFieldCount());
  }



}
