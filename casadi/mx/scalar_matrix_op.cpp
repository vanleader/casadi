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

#include "scalar_matrix_op.hpp"
#include <vector>
#include <sstream>
#include "../stl_vector_tools.hpp"

using namespace std;

namespace CasADi{

ScalarMatrixOp::ScalarMatrixOp(OPERATION op_, const MX& x, const MX& y) : op(op_){
  setDependencies(x,y);
  setSparsity(y.sparsity());
}

ScalarMatrixOp* ScalarMatrixOp::clone() const{
  return new ScalarMatrixOp(*this);
}

void ScalarMatrixOp::print(std::ostream &stream) const{
  stringstream sx; sx << dep(0);
  stringstream sy; sy << dep(1);
  print_c[op](stream,sx.str(),sy.str());
}

void ScalarMatrixOp::evaluate(const VDptr& input, Dptr& output, const VVDptr& fwdSeed, VDptr& fwdSens, const VDptr& adjSeed, VVDptr& adjSens, int nfwd, int nadj){
  if(nfwd==0 && nadj==0){
    // No sensitivities
    for(int i=0; i<size(); ++i)
      nfun0[op](input[0][0],input[1][i],&output[i]);
    
  } else {
    // Sensitivities
    double tmp[3];  // temporary variable to hold value and partial derivatives of the function
    for(int i=0; i<size(); ++i){
      // Evaluate and get partial derivatives
      nfun1[op](input[0][0],input[1][i],tmp);
      output[i] = tmp[0];
      
      // Propagate forward seeds
      for(int d=0; d<nfwd; ++d){
        fwdSens[d][i] = tmp[1]*fwdSeed[0][d][0] + tmp[2]*fwdSeed[1][d][i];
      }

      // Propagate adjoint seeds
      for(int d=0; d<nadj; ++d){
        adjSens[0][d][0] += adjSeed[d][i]*tmp[1];
        adjSens[1][d][i] += adjSeed[d][i]*tmp[2];
      }
    }
  }
}

} // namespace CasADi

