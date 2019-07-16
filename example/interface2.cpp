/*
# include either -DUSE_OPENBLAS or -DUSE_INTELBLAS for fast execution
g++ -I../include/ -std=c++17 -Ofast interface2.cpp -o interface2 && ./interface2
*/

#include <tlib/ttv.h>

#include <vector>
#include <numeric>
#include <iostream>


int main()
{
  using value_t    = float;
  using tensor_t   = tlib::tensor<value_t>;     // or std::array<value_t,N>
  using iterator_t = std::ostream_iterator<value_t>;
  
  auto mode = 2ul;
  
  auto A = tensor_t( {4,3,2}/*, {3,2,1}*/ ); 
  auto B = tensor_t( {A.shape().at(mode-1),1} ); 
  // strides are automatically computed. shape and layout verified.
  
  std::iota(A.begin(),A.end(),1);
  std::fill(B.begin(),B.end(),1);
  
  std::cout << "A = [ "; std::copy(A.begin(), A.end(), iterator_t(std::cout, " ")); std::cout << " ];" << std::endl;
  std::cout << "B = [ "; std::copy(B.begin(), B.end(), iterator_t(std::cout, " ")); std::cout << " ];" << std::endl;

/*
  a = 
  { 1  5  9  | 13 17 21
    2  6 10  | 14 18 22
    3  7 11  | 15 19 23
    4  8 12  | 16 20 24 };

  b = { 1 1 1 } ;
*/

  // correct shape, layout and strides of the output tensors C1,C2 are automatically computed and returned by the functions.  
  auto C1 = tlib::tensor_times_vector(mode, A,B, tlib::execution::seq , tlib::slicing::large, tlib::loop_fusion::none );  
  auto C2 = tlib::tensor_times_vector(mode, A,B, tlib::execution::blas, tlib::slicing::small, tlib::loop_fusion::all  );
  	

  std::cout << "C1 = [ "; std::copy(C1.begin(), C1.end(), iterator_t(std::cout, " ")); std::cout << " ];" << std::endl;
  std::cout << "C2 = [ "; std::copy(C2.begin(), C2.end(), iterator_t(std::cout, " ")); std::cout << " ];" << std::endl;
  
/*
  c = 
  { 1+5+ 9 | 13+17+21
    2+6+10 | 14+18+22
    3+7+11 | 15+19+23
    4+8+12 | 16+20+24 };
*/
}
