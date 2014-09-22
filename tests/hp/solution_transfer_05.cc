// ---------------------------------------------------------------------
//
// Copyright (C) 2011 - 2014 by the deal.II authors
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



// testcase by Minh Do-Quang: a case where SolutionTransfer got into trouble
// in a couple of places when using FE_Nothing and FESystem.

#include "../tests.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include <deal.II/base/logstream.h>

#include <deal.II/grid/tria.h>

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/tria.h>
#include <deal.II/hp/dof_handler.h>
#include <deal.II/grid/tria_iterator.h>

#include <deal.II/fe/fe_system.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_nothing.h>
#include <deal.II/dofs/dof_tools.h>

#include <deal.II/hp/fe_collection.h>

#include <deal.II/numerics/solution_transfer.h>

#include <deal.II/numerics/data_out.h>

using namespace dealii;

int main()
{
  std::ofstream logfile("output");
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  Triangulation<2> triangulation;
  GridGenerator::hyper_cube (triangulation);
  triangulation.refine_global (1);


  hp::FECollection<2>     fe_collection;

  fe_collection.push_back(FESystem<2>(FE_Q<2>(1), 1, FE_Q<2>(1), 1));
  fe_collection.push_back(FESystem<2>(FE_Nothing<2>(), 1, FE_Nothing<2>(), 1));

  hp::DoFHandler<2> dof_handler(triangulation);

  // Assign FE to cells
  hp::DoFHandler<2>::active_cell_iterator cell;
  hp::DoFHandler<2>::active_cell_iterator endc = dof_handler.end();      


  cell = dof_handler.begin_active();
  cell->set_active_fe_index(1);
  cell++;
  cell->set_active_fe_index(1);
  cell++;
  cell->set_active_fe_index(0);
  cell++;
  cell->set_active_fe_index(0);


  dof_handler.distribute_dofs (fe_collection);

  // Init solution
  Vector<double> solution(dof_handler.n_dofs());
  solution = 1.0;


  SolutionTransfer<2, Vector<double>, hp::DoFHandler<2> > solultion_trans(dof_handler);
  solultion_trans.prepare_for_coarsening_and_refinement(solution);

  triangulation.execute_coarsening_and_refinement ();
  dof_handler.distribute_dofs (fe_collection);
  
  Vector<double> new_solution(dof_handler.n_dofs());
  solultion_trans.interpolate(solution, new_solution);

  // a follow-up error to the one fixed with _04 was that DataOut also got
  // itself confused
  DataOut<2, hp::DoFHandler<2> > data_out2;
  data_out2.attach_dof_handler (dof_handler);
  data_out2.add_data_vector (new_solution, "Solution");
  data_out2.build_patches();
  data_out2.write_vtu(deallog.get_file_stream());
  
  // we are good if we made it to here
  deallog << "OK" << std::endl;
}



