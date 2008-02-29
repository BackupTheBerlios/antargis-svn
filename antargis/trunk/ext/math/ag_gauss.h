#ifndef AG_GAUSS_H
#define AG_GAUSS_H

#include <math.h>

/// input:a should be a unit-matrix, b contains the matrix to be inverted!
/// afterwards a contains the result 
template<class TMatrix>
bool gauss(TMatrix &a,TMatrix &b,int size)
  {
    //TRACE;
    // cdebug("A:\n"<<a.toString());
    // cdebug("B:\n"<<b.toString());

    // lower-left triangle
    for(int c=0;c<size-1;c++) // cols
      {
        //FIXME: check that we have a one in (c,c) - otherwise swap rows !
        if(fabs(b.get(c,c))<0.0001)
          {
            int k;
            // find a row where b.get(c,c+n)!=0 for n>1 
            for(k=1;k+c<size;k++)
              if(fabs(b.get(c,c+k))>0)
                {
                  // swap rows
                  b.swapRows(c,c+k);
                  a.swapRows(c,c+k);
                  break;
                }
            if(k+c==size)
              return false; // PROBLEM!!!!
          }

        for(int r=c+1;r<size;r++) // rows
          {
            if(fabs(b.get(c,r))>0.0001)
              {
                float f=-b.get(c,c)/b.get(c,r);
                for(int i=0;i<size;i++)
                  {
                    // modify row
                    a.set(i,r,a.get(i,c)+a.get(i,r)*f);
                    b.set(i,r,b.get(i,c)+b.get(i,r)*f);
                  }
                // cdebug("c:"<<c<<" r:"<<r);
                // cdebug("INTERMED A:\n"<<a.toString());
                // cdebug("INTERMED B:\n"<<b.toString());
              }
          }
      }
    // cdebug("A:\n"<<a.toString());
    // cdebug("B:\n"<<b.toString());

    // upper-right triangle
    for(int c=size-1;c>0;c--) // cols
      {
        for(int r=0;r<c;r++) // rows
          {
            if(fabs(b.get(c,r))>0.0001)
              {
                float f=-b.get(c,r+1)/b.get(c,r);
                for(int i=0;i<size;i++)
                  {
                    // modify row
                    a.set(i,r,a.get(i,r+1)+a.get(i,r)*f);
                    b.set(i,r,b.get(i,r+1)+b.get(i,r)*f);
                  }
              }
          }
      }
    // cdebug("A:\n"<<a.toString());
    // cdebug("B:\n"<<b.toString());

    // norming

    for(int r=0;r<size;r++)
      {
        float v=b.get(r,r);
        if(v!=0)
          for(int c=0;c<size;c++)
            {
              a.set(c,r,a.get(c,r)/v);
              b.set(c,r,b.get(c,r)/v);
            }
      }
    // cdebug("A:\n"<<a.toString());
    // cdebug("B:\n"<<b.toString());
    return true; // everythin ok
  }


#endif

