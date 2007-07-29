#include "path_v2.h"
#include <ag_debug.h>
#include <ag_geometry.h>
#include <set>


#include "path_vector_sort.h"



///////////////////////////////////////////////////////////////////////////////////


/**
 * PathV2Data constructor - does nothing yet
 */
PathV2Data::PathV2Data(size_t pBaseSize):mBaseSize(pBaseSize)
{
}


void PathV2Data::compute(const DistanceComputer &pCalc)
{
  mMatrices.clear();

  float x,y;
  size_t ix,iy;

  for(x=pCalc.beginX();x<=pCalc.endX();x+=pCalc.stepX())
    {
      for(y=pCalc.beginY();y<=pCalc.endY();y+=pCalc.stepY())
	{
	  AGVector2 start(x,y);

	  traceFrom(pCalc,start);
	}
    }

}


void PathV2Data::traceFrom(const DistanceComputer &pCalc,const AGVector2 &p)
{
  // FIXME: maybe store directly into matrix ???

  std::map<AGVector2,float> distances;
  std::list<AGVector2> queue; // FIXME: use a distance-filter ????

  float cDist,nextDist,oldDist;
  AGVector2 curPos;

  queue.push_back(p);
  distances[p]=0;

  while(queue.size()>0)
    {
      curPos=queue.front();
      queue.pop_front();
      cDist=distances[curPos];
      std::map<AGVector2,float> neighbors=pCalc.getAllReachableFrom(curPos);

      for(std::map<AGVector2,float>::iterator i=neighbors.begin();i!=neighbors.end();i++)
	{
	  nextDist=cDist+i->second;
	  oldDist=distances[i->first];
	  if(oldDist==0 || nextDist<oldDist)
	    {
	      // store in map
	      distances[i->first]=nextDist;
	      // push into queue
	      queue.push_back(i->first);
	    }
	}

    }

  // ok, we're ready
  // store into matrix
  
  size_t matrixWidth=size_t((pCalc.endX()-pCalc.beginX())/pCalc.stepX()+1);
  size_t matrixHeight=size_t((pCalc.endY()-pCalc.beginY())/pCalc.stepY()+1);

  AGMatrixN matrix(matrixWidth,matrixHeight);

  for(std::map<AGVector2,float>::iterator i=distances.begin();i!=distances.end();i++)
    {
      size_t x=size_t((i->first[0]-pCalc.beginX())/pCalc.stepX());
      size_t y=size_t((i->first[1]-pCalc.beginY())/pCalc.stepY());
      matrix.set(x,y,i->second);
    }

  //FIXME  mMatrices.push_back(matrix);
  
}




///////////////////////////////////////////////////////////////////////////////////

/**
 * PathV2 constructor
 * \param pData the data that's used to compute distances - is owned by PathV2 later on!
 */
PathV2::PathV2(PathV2Data *pData):mData(pData)
{
}

PathV2::~PathV2()
{
  if(mData)
    saveDelete(mData);
}


float PathV2::weight(const AGVector2 &a,const AGVector2 &b)
{
}

void PathV2::mark()
{
  markObject(mData);
}









/**
 * PathV3Data constructor
 */

PathV3Data::PathV3Data(size_t pFieldCount):
  mFieldCount(pFieldCount)
{
}


void PathV3Data::compute(const DistanceComputer &pCalc)
{
  std::vector<AGVector2> allValidPoints;
  std::set<AGVector2> allValidPointsSet;
  AGVector2 currentPoint;
  float x,y;

  for(x=pCalc.beginX();x<=pCalc.endX();x+=pCalc.stepX())
    {
      for(y=pCalc.beginY();y<=pCalc.endY();y+=pCalc.stepY())
	{
	  currentPoint=AGVector2(x,y);
	  if(pCalc.isPassable(currentPoint))
	    {
	      allValidPoints.push_back(currentPoint);
	    }
	}
    }

  std::copy(allValidPoints.begin(),allValidPoints.end(),std::inserter(allValidPointsSet,allValidPointsSet.begin()));

  size_t estFieldSize=allValidPoints.size()/mFieldCount;
  // shuffle ???


  cdebug("estFieldSize:"<<estFieldSize);

  while(allValidPoints.size()>0)
    {
      // make new field
      size_t randomIndex=rand()%allValidPoints.size();
      std::set<AGVector2> currentField;
      AGVector2 fieldMiddle=allValidPoints[randomIndex];

      currentField.insert(fieldMiddle);
      allValidPointsSet.erase(fieldMiddle);

      // grow field
      
      DistanceOrder order(fieldMiddle);
      std::set<AGVector2,DistanceOrder> growField(order);//(DistanceOrder(fieldMiddle));
      //      std::set<AGVector2> growField;
      growField.insert(fieldMiddle);

      while(currentField.size()<estFieldSize && growField.size()>0)
	{
	  currentPoint=*growField.begin();
	  growField.erase(currentPoint);
	  mVec2Field[currentPoint]=mFields.size();

	  std::map<AGVector2,float> reachable=pCalc.getAllReachableFrom(currentPoint);

	  //	  cdebug("reachable:"<<reachable.size()<<" currentPoint:"<<currentPoint);

	  for(std::map<AGVector2,float>::iterator i=reachable.begin();i!=reachable.end();i++)
	    {
	      //	      cdebug("1");
	      if(growField.find(i->first)==growField.end())
		{
		  //		  cdebug("2");
		  if(currentField.find(i->first)==currentField.end())
		    {
		      //		      cdebug("3");
		      //		      cdebug("allValid:"<<allValidPointsSet.size());

		      //		      for(  std::set<AGVector2>::iterator k=allValidPointsSet.begin();k!=allValidPointsSet.end();k++)
		      //			{
		      //			  cdebug(i->first<<":"<<*k<<":"<<(i->first==*k));
		      //			}


		      if(allValidPointsSet.find(i->first)!=allValidPointsSet.end())
			{
			  //  cdebug("4");
			  growField.insert(i->first);
			  currentField.insert(i->first);
			  allValidPointsSet.erase(i->first);

			  mVec2Field[i->first]=mFields.size();
			}
		    }
		}
	    }

	}


      // copy back into vector
      allValidPoints.clear();
      std::copy(allValidPointsSet.begin(),allValidPointsSet.end(),std::back_inserter(allValidPoints));
      std::list<AGVector2> resultField;
      std::copy(currentField.begin(),currentField.end(),std::back_inserter(resultField));
      mFields.push_back(resultField);

      for(std::list<AGVector2>::iterator i=resultField.begin();i!=resultField.end();i++)
	{
	  cdebug(*i);
	}
      cdebug("----");
    }
  
}


std::list<AGVector2> PathV3Data::vecList(size_t field) const
{
  return mFields[field];
}
size_t PathV3Data::fieldCount() const
{
  return mFields.size();
}

float PathV3Data::getDistanceFromTo(const AGVector2 &pFrom,const AGVector2 &pTo) const
{
  //FIXME
  return 0;
}


bool PathV3Data::isInField(const AGVector2 &pPoint,size_t pField) const
{
  std::map<AGVector2,size_t>::const_iterator i=mVec2Field.find(pPoint);

  if(i==mVec2Field.end())
    return false; //FIXME: throw exception

  return i->second==pField;
}

std::map<size_t,float> PathV3Data::getNeighbors(size_t pField) const
{
  std::map<size_t,float> x;
  //FIXME: implement me
  return x;
}


size_t PathV3Data::getField(const AGVector2 &a) const
{
  std::map<AGVector2,size_t>::const_iterator i=mVec2Field.find(a);

  if(i==mVec2Field.end())
    return -1;

  return i->second;
}

float PathV3Data::directNeighbors(const AGVector2 &a,const AGVector2 &b) const
{
  std::map<AGVector2,std::map<AGVector2,float> >::const_iterator i=mDirectMaps.find(a);

  if(i==mDirectMaps.end())
    return -1;
  
  std::map<AGVector2,float>::const_iterator j=i->second.find(b);

  if(j==i->second.end())
    return -2;

  return j->second;

}
