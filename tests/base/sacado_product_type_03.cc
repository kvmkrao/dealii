// ---------------------------------------------------------------------
//
// Copyright (C) 2015 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------


// test ProductType with sacado

#include <iomanip>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <typeinfo>

#include <deal.II/base/tensor.h>
#include <deal.II/base/sacado_product_type.h>

#include "../tests.h"

template <typename T, typename U, typename CompareType>
void check()
{
  AssertThrow (typeid(typename ProductType<T,U>::type) == typeid(CompareType),
               ExcInternalError());
}


int main()
{
  typedef Sacado::Fad::DFad<double> Sdouble;
  typedef Sacado::Fad::DFad<Sdouble> SSdouble;
  initlog();
  deallog.threshold_double(1.e-10);


  // check product of scalars
  check<Sdouble,SSdouble,SSdouble>();
  check<SSdouble,Sdouble,SSdouble>();

  // check product with Tensor<1,dim>
  check<Tensor<1,2,Sdouble>,SSdouble,Tensor<1,2,SSdouble> >();
  check<Tensor<1,2,SSdouble>,Sdouble,Tensor<1,2,SSdouble> >();

  check<SSdouble,Tensor<1,2,Sdouble>,Tensor<1,2,SSdouble> >();
  check<Sdouble,Tensor<1,2,SSdouble>,Tensor<1,2,SSdouble> >();

  // check product with Tensor<2,dim> (which is a different class)
  check<Tensor<2,2,Sdouble>,SSdouble,Tensor<2,2,SSdouble> >();
  check<Tensor<2,2,SSdouble>,Sdouble,Tensor<2,2,SSdouble> >();

  check<SSdouble,Tensor<2,2,Sdouble>,Tensor<2,2,SSdouble> >();
  check<Sdouble,Tensor<2,2,SSdouble>,Tensor<2,2,SSdouble> >();

  deallog << "OK" << std::endl;
}
