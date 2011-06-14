/*
 *    This file is part of CasADi.
 *
 *    CasADi -- A symbolic framework for dynamic optimization.
 *    Copyright (C) 2010 by Joel Andersson, Moritz Diehl, K.U.Leuven. All rights reserved.
 *
 *    CasADi is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    CasADi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with CasADi; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef SX_FUNCTION_INTERNAL_HPP
#define SX_FUNCTION_INTERNAL_HPP

#include "sx_function.hpp"
#include "x_function_internal.hpp"

namespace CasADi{

  template<int n>
  struct AlgElData{
    // Partial derivatives
    double d[n+1];
};

/** \brief  Internal node class for SXFunction
  A regular user should never work with any Node class. Use SXFunction directly.
  \author Joel Andersson 
  \date 2010
*/
class SXFunctionInternal : public XFunctionInternal{
  friend class SXFunction;
  
  protected:
    /** \brief  Constructor (only to be called from SXFunction, therefore protected) */
    SXFunctionInternal(const std::vector<Matrix<SX> >& inputv, const std::vector<Matrix<SX> >& outputv);

  public:

  /** \brief  Make a deep copy */
  virtual SXFunctionInternal* clone() const;
    
  /** \brief  Destructor */
  virtual ~SXFunctionInternal();

  /** \brief  Evaluate the function with partial derivatives up to order ord */
  virtual void evaluate(int nfdir, int nadir);

  /** \brief  evaluate symbolically */
  virtual void evaluateSX(const std::vector<Matrix<SX> >& input_s, std::vector<Matrix<SX> >& output_s, bool eliminate_constants=false);

  /** \brief  Check if smooth */
  bool isSmooth() const;

  /** \brief  Print the algorithm */
  virtual void print(std::ostream &stream) const;

  /** \brief Calculate the jacobian of a number of function outputs with respect to a number of function inputs, optionally include the function outputs */
  virtual FX jacobian(const std::vector<std::pair<int,int> >& jblocks);

  /** \brief Hessian of output oind with respect to input iind */
  virtual FX hessian(int iind=0, int oind=0);

  /** \brief Calculate the expression for the jacobian of a number of function outputs with respect to a number of function inputs, optionally include the function outputs */
  virtual std::vector<Matrix<SX> > jac(const std::vector<std::pair<int,int> >& jblocks);
  
  /** \brief Get the unidirectional or bidirectional partition */
  virtual void getPartition(const std::vector<std::pair<int,int> >& blocks, std::vector<CRSSparsity> &D1, std::vector<CRSSparsity> &D2);
  
  /** \brief Perform a unidirectional coloring */
  virtual CRSSparsity unidirectionalColoring(const CRSSparsity& A, const CRSSparsity& AT);
  
  /** \brief  DATA MEMBERS */
  
  /** \brief  Indices of the nodes corresponding to the inputs */
  std::vector<std::vector<int> > input_ind;
  
  /** \brief  Indices of the nodes corresponding the non-zeros of the outputs */
  std::vector<std::vector<int> > output_ind;

  /** \brief  An elemenent of the algorithm, namely a binary operation */
  typedef SXAlgEl AlgEl;
  
/** \brief  all binary nodes of the tree in the order of execution */
  std::vector<AlgEl> algorithm;
  std::vector<AlgElData<1> > pder1;
  std::vector<AlgElData<2> > pder2;
  
  /** \brief  All nodes */
  std::vector<SXNode*> nodes;

  /** \brief  Working vector for numeric calculation */
  std::vector<double> work;
  std::vector<double> dwork;
  int worksize;

  /// work vector for symbolic calculations (allocated first time)
  std::vector<SX> swork;
  std::vector<SX> free_vars;
  
  /** \brief  Initialize */
  virtual void init();

  /** \brief  Print to a c file */
  void generateCode(const std::string& filename) const;
    
  /** \brief  Print to a c file */
  static void printVector(std::ostream &cfile, const std::string& name, const std::vector<int>& v);
  
  /** \brief  Inputs of the function (needed for symbolic calculations) */
  std::vector<Matrix<SX> > inputv;

  /** \brief  Outputs of the function (needed for symbolic calculations) */
  std::vector<Matrix<SX> > outputv;
  
  /** \brief Clear the function from its symbolic representation, to free up memory, no symbolic evaluations are possible after this */
  void clearSymbolic();

  /// Generate the sparsity of a Jacobian block
  virtual CRSSparsity getJacSparsity(int iind, int oind);

};


} // namespace CasADi

#endif // SX_FUNCTION_INTERNAL_HPP
