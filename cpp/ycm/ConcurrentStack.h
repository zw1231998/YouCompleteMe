// Copyright (C) 2011, 2012  Strahinja Val Markovic  <val@markovic.io>
//
// This file is part of YouCompleteMe.
//
// YouCompleteMe is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// YouCompleteMe is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with YouCompleteMe.  If not, see <http://www.gnu.org/licenses/>.

#ifndef CONCURRENTSTACK_H_SYF1JPPG
#define CONCURRENTSTACK_H_SYF1JPPG

#include <stack>
#include <boost/thread.hpp>
#include <boost/utility.hpp>

namespace YouCompleteMe
{

template <typename T>
class ConcurrentStack : boost::noncopyable
{
public:

  // TODO: rename this class, it's not a stack anymore
  ConcurrentStack() : empty_( true ) {}

  void Set( const T& data )
  {
    {
      boost::unique_lock< boost::mutex > lock( mutex_ );
      latest_ = data;
      empty_ = false;
    }

    condition_variable_.notify_one();
  }

  T Get()
  {
    boost::unique_lock< boost::mutex > lock( mutex_ );

    while ( empty_ )
    {
      condition_variable_.wait( lock );
    }

    empty_ = true;
    return latest_;
  }


private:
  // std::stack< T > stack_;
  T latest_;
  bool empty_;
  boost::mutex mutex_;
  boost::condition_variable condition_variable_;

};

} // namespace YouCompleteMe

#endif /* end of include guard: CONCURRENTSTACK_H_SYF1JPPG */