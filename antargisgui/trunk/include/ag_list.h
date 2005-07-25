/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_list.h
 * by David Kamphausen (david.kamphausen@web.de)
 *
 * The "Antargis" project, including all files needed to compile it,
 * is free software; you can redistribute it and/or use it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 */

#ifndef AG_LIST_H
#define AG_LIST_H

template<class T>
class AGListIterator
{
  
};

template<class T>
class AGList
{
 public:
  typedef AGListIterator<T> iterator;
  typedef AGListIterator<const T> const_iterator;

  AGList();
  AGList(const AGList<T> &l);
  
  AGList<T> &operator+=(const AGList<T> &l);
  AGList<T> operator+(const AGList<T> &l) const;

  T max() const;
  T min() const;

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

  
};

#endif
