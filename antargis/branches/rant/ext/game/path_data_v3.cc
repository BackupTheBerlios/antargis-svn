#include "path_data_v3.h"
#include "path_vector_sort.h"

#include <ag_profiler.h>

#include <ag_debug.h>
#include <algorithm>

#include <math.h>

//namespace PathFinding
//{

  Field::Field(const AGVector2 &pMiddle,size_t pID):mID(pID),
						    mMiddle(pMiddle)
  {
  }

  Field::~Field()
  {
  }

  void Field::insert(const AGVector2 &v,DistanceComputer &pDist)
  {
    mVectors.insert(v);
    mNeighbors.erase(v);
    
    std::vector<AGVector2> neighbors=pDist.getNeighbors(v);

    for(std::vector<AGVector2>::iterator i=neighbors.begin();i!=neighbors.end();i++)
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

  std::vector<AGVector2> Field::getVectors() const
  {
    std::vector<AGVector2> v;
    std::copy(mVectors.begin(),mVectors.end(),std::back_inserter(v));
    return v;
  }

  std::vector<AGVector2> Field::getNeighbors() const
  {
    std::vector<AGVector2> v;
    std::copy(mNeighbors.begin(),mNeighbors.end(),std::back_inserter(v));
    return v;
  }





////////////////////////////////////////////////////////
// FieldCollection
////////////////////////////////////////////////////////


  FieldCollection::~FieldCollection()
  {
  }

  Field *FieldCollection::createField(const AGVector2 &pMiddle,size_t pId)
  {
    return new FieldWithDistances(pMiddle,pId);
  }
  

  //
  void FieldCollection::assign(const AGVector2 &v,size_t pField,DistanceComputer &pDist)
  {
    assert(pField<mFields.size());

    mFields[pField]->insert(v,pDist);
    mVec2Field[v]=pField;
  }


  size_t FieldCollection::newField(const AGVector2 &pMiddle)
  {
    size_t id=mFields.size();
    mFields.push_back(createField(pMiddle,id));
    return id;
  }

Field *FieldCollection::getField(size_t pField)
{
  return mFields[pField];
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

  void FieldCollection::mark()
  {
    CTRACE;
    for(std::vector<Field*>::iterator i=mFields.begin();i!=mFields.end();i++)
      markObject(*i);
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
    std::vector<AGVector2> allVectorList=pDist.getAllPassable();

    float maxFieldWidth=sqrt(allVectorList.size()/pFieldCount)*0.7;


    //    maxFieldWidth=3;

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
	    //	    cdebug(" "<<*i<<"  "<<(*i-fieldMiddle).length());
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




  // FieldWithDistances


  FieldWithDistances::FieldWithDistances(const AGVector2 &pMiddle,size_t pID):
    Field(pMiddle,pID)
  {
  }

void FieldWithDistances::initLocalDistances(const DistanceComputer &pComputer,const std::vector<AGVector2> &pVectors)
  {
    STACKTRACE;
    AGVector2 m=getMiddle();
    std::set<AGVector2> baseVectors;
    std::copy(pVectors.begin(),pVectors.end(),std::inserter(baseVectors,baseVectors.begin()));

    std::vector<AGVector2> vectors=getVectors();

    for(std::vector<AGVector2>::iterator i=vectors.begin();i!=vectors.end();i++)
      {
	STACKTRACE;
	std::map<AGVector2,float> curDists;

	// compute distances for paths starting at *i
	DistanceOrder order(*i);
	std::set<AGVector2,DistanceOrder> vecSet(order);
	vecSet.insert(*i);
	//	cdebug("VEC:"<<*i);
	while(vecSet.size()>0)
	  {
	    STACKTRACE;
	    AGVector2 currentVec=*vecSet.begin();
	    vecSet.erase(vecSet.begin());
	    std::vector<std::pair<AGVector2,float> > newVecs;
	    {
	      STACKTRACE;
	      newVecs=pComputer.getAllReachableFrom(currentVec);
	    }
	    //	    cdebug("newVecs:"<<newVecs.size());
	    for(std::vector<std::pair<AGVector2,float> >::iterator ni=newVecs.begin();ni!=newVecs.end();ni++)
	      {
		STACKTRACE;
		// FIXME: check if in "my Field" or neighbor !!!
		if(ni->first!=*i) // don't store initial vector
		  {
		    STACKTRACE;
		    if(baseVectors.find(ni->first)!=baseVectors.end())
		      {
			STACKTRACE;
			std::map<AGVector2,float>::iterator found=curDists.find(ni->first);
			if(ni->second>0)
			  if(found==curDists.end() || (found->second>ni->second))
			    //			if((curDists[ni->first]>ni->second || curDists[ni->first]==0) && ni->second>0)
			    {
			      STACKTRACE;
			      curDists[ni->first]=ni->second;
			      vecSet.insert(ni->first);
			    }
		      }
		  }
	      }

	  }
	


	// store them into the distance-field

	for(std::map<AGVector2,float>::iterator di=curDists.begin();di!=curDists.end();di++)
	  {
	    STACKTRACE;
	    mDistances.insert(std::make_pair(std::make_pair(*i,di->first),di->second));
	  }
	
      }
  }

  // FIXME: make this const !
  float FieldWithDistances::getDistance(const AGVector2 &a,const AGVector2 &b)
  {
    return mDistances[std::make_pair(a,b)];
  }



////////////////////////////////////////////////////////
// FieldWithNeighborDistances
////////////////////////////////////////////////////////
FieldWithNeighborDistances::FieldWithNeighborDistances(const AGVector2 &pMiddle,size_t pID):
  FieldWithDistances(pMiddle,pID)
{
}

// from my a to b, that is in neighbor field
void FieldWithNeighborDistances::setNeighborDistance(const AGVector2 &a,const AGVector2 &b,float value)
{
  mNeighborDistances[std::make_pair(a,b)]=value;
}
float FieldWithNeighborDistances::getNeighborDistance(const AGVector2 &a,const AGVector2 &b)
{
  return mNeighborDistances[std::make_pair(a,b)];
}

void FieldWithNeighborDistances::setFieldDistance(const AGVector2 &a,size_t pField,float value)
{
  mFieldDistances[std::make_pair(a,pField)]=value;
}
float FieldWithNeighborDistances::getFieldDistance(const AGVector2 &a,size_t pField)
{
  return mFieldDistances[std::make_pair(a,pField)];
}







////////////////////////////////////////////////////////
// FieldCollectionWithDistances
////////////////////////////////////////////////////////



Field *FieldCollectionWithDistances::createField(const AGVector2 &pMiddle,size_t pId)
{
  return new FieldWithNeighborDistances(pMiddle,pId);
}


void FieldCollectionWithDistances::computeFieldNeighbors()
{
  if(mFieldNeighbors.size()>0)
    return;
  for(size_t i=0;i<getFieldCount();i++)
    {
      std::set<size_t> ns;
      std::vector<AGVector2> vs=getField(i)->getNeighbors();
      for(std::vector<AGVector2>::iterator j=vs.begin();j!=vs.end();j++)
	{
	  size_t id=getFieldId(*j);
	  if(id<getFieldCount())
	    ns.insert(id);
	}
      mFieldNeighbors[i]=ns;
    }
}

std::set<size_t> FieldCollectionWithDistances::getNeighborFields(size_t fieldNum)
{
  return mFieldNeighbors[fieldNum];
}


void FieldCollectionWithDistances::computeDistances(const DistanceComputer &pDist)
{

  computeFieldNeighbors();

  for(size_t fieldNum=0;fieldNum<getFieldCount();fieldNum++)
    {
      STACKTRACE;
      std::set<size_t> fields;

      if(true)
	fields=getNeighborFields(fieldNum);
      else
	fields.insert(fieldNum);

      std::vector<AGVector2> vectors;

      fields.insert(fieldNum);

      for(std::set<size_t>::iterator i=fields.begin();i!=fields.end();i++)
	{
	  assert(*i<getFieldCount());
	  std::vector<AGVector2> tmp=getField(*i)->getVectors();
	  std::copy(tmp.begin(),tmp.end(),std::back_inserter(vectors));
	}
      Field *f=getField(fieldNum);
      FieldWithDistances *df=dynamic_cast<FieldWithDistances*>(f);
      assert(df);
      cdebug("FIELD:"<<fieldNum<<" vecs:"<<vectors.size());
      df->initLocalDistances(pDist,vectors);
    }




  /*
  for(size_t fieldNum=0;fieldNum<getFieldCount();fieldNum++)
    {
      cdebug("fieldNum:"<<fieldNum<<" of "<<getFieldCount());
      FieldWithNeighborDistances *curField=dynamic_cast<FieldWithNeighborDistances*>(getField(fieldNum));
      assert(curField);
      if(curField)
	{
	  std::vector<AGVector2> curVectors=curField->getVectors();
	  std::vector<AGVector2> curNeighbors=curField->getNeighbors();
	  std::map<std::pair<AGVector2,AGVector2>,float> distances;
	  
	  cdebug("curVectors:"<<curVectors.size());
	  cdebug("curNeighbors:"<<curNeighbors.size());

	  for(std::vector<AGVector2>::iterator curVector=curVectors.begin();curVector!=curVectors.end();curVector++)
	    {
	      for(std::vector<AGVector2>::iterator curNeighbor=curNeighbors.begin();curNeighbor!=curNeighbors.end();curNeighbor++)
		{
		  size_t neighborField=getFieldId(*curNeighbor);

		  if(neighborField<getFieldCount()) // check if field is valid
		    {
		      FieldWithNeighborDistances *curField2=dynamic_cast<FieldWithNeighborDistances*>(getField(neighborField));
		      assert(curField2);
		      if(curField2)
			{
			  std::vector<AGVector2> field2Vecs=curField2->getVectors();
			  for(std::vector<AGVector2>::iterator field2Vec=field2Vecs.begin();field2Vec!=field2Vecs.end();field2Vec++)
			    {
			      float nDist=curField->getDistance(*curVector,*curNeighbor)+curField2->getDistance(*curNeighbor,*field2Vec);
			      std::pair<AGVector2,AGVector2> index(*curVector,*field2Vec);
			      float oldDist=distances[index];
			      if(oldDist==0 || oldDist>nDist)
				distances[index]=nDist;
			    }
			}
		    }
		}
	    }
	}
    }

  */
}





//}
