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

#include "mx_node.hpp"
#include <cassert>
#include <typeinfo> 

using namespace std;

namespace CasADi{

MXNode::MXNode(){
  nfdir_ = 1;
  nadir_ = 1;
}

MXNode::~MXNode(){
}

void MXNode::print(ostream &stream) const{
  stream << "<empty matrix expression>";
}
  
const string& MXNode::getName() const{
  throw CasadiException(string("MXNode::getName() not defined for class ") + typeid(*this).name());
}

bool MXNode::isSymbolic() const{
  return false;
}

bool MXNode::isConstant() const{
  return false;
}

MX& MXNode::dep(int ind){
  return dep_.at(ind);
}

const MX& MXNode::dep(int ind) const{
  return dep_.at(ind);
}
  
int MXNode::ndep() const{
  return dep_.size();
}

void MXNode::init(){
  val_.dataF.resize(nfdir_);
  val_.dataA.resize(nadir_);
  val_.init();

  input_.resize(ndep(),0);
  fwdSeed_.resize(ndep());
  adjSens_.resize(ndep());
  for(int i=0; i<ndep(); ++i){
    fwdSeed_[i].resize(nfdir_,0);
    adjSens_[i].resize(nadir_,0);
    if(!dep(i).isNull()){
      input_[i] = &dep(i)->val_.data[0];
      for(int d=0; d<nfdir_; ++d)
        fwdSeed_[i][d] = &dep(i)->val_.dataF[d][0];
      for(int d=0; d<nadir_; ++d)
        adjSens_[i][d] = &dep(i)->val_.dataA[d][0];
    }
  }

  output_ = &val_.data[0];
  fwdSens_.resize(nfdir_);
  for(int d=0; d<nfdir_; ++d)
    fwdSens_[d] = &val_.dataF[d][0];
  adjSeed_.resize(nadir_);
  for(int d=0; d<nadir_; ++d)
    adjSeed_[d] = &val_.dataA[d][0];

}


void MXNode::setSize(int nrow, int ncol){
  sparsity_ = CRSSparsity(nrow,ncol,true);
  val_.data = Matrix<double>(sparsity_);
  val_.dense = false;
}

void MXNode::setSparsity(const CRSSparsity& sparsity){
  sparsity_ = sparsity;
  val_.data = Matrix<double>(sparsity_);
  val_.dense = false;
}

void MXNode::setDependencies(const MX& dep){
  dep_.resize(1);
  dep_[0] = dep;
}
    
void MXNode::setDependencies(const MX& dep1, const MX& dep2){
  dep_.resize(2);
  dep_[0] = dep1;
  dep_[1] = dep2;
}
    
void MXNode::setDependencies(const MX& dep1, const MX& dep2, const MX& dep3){
  dep_.resize(3);
  dep_[0] = dep1;
  dep_[1] = dep2;
  dep_[2] = dep3;
}

void MXNode::setDependencies(const std::vector<MX>& dep){
  dep_ = dep;
}

int MXNode::size() const{
  return sparsity_.size();
}

int MXNode::size1() const{
  return sparsity_.size1();
}

int MXNode::size2() const{
  return sparsity_.size2();
}

const CRSSparsity& MXNode::sparsity() const{
  return sparsity_;
}


} // namespace CasADi
