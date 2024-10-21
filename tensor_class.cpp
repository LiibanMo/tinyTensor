#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <cassert>

template <typename Type>
class Tensor{
    private:
        const std::vector<Type> tensor;
        std::vector<int> tensor_shape;
    public:
        Tensor(const std::vector<Type>& input_data, const std::vector<int>& input_shape) : tensor(input_data), tensor_shape(input_shape) {}
        
        void print(){
            const int N = tensor_shape[tensor_shape.size() - 2];
            const int M = tensor_shape[tensor_shape.size() - 1];

            std::cout << "{";
            for (int i = 0; i < N; i++){
                std::cout << "{ ";
                for (int j = 0; j < M; j++){
                    std::cout << tensor[j + i * M] << " ";
                }
                if (i != N-1){
                    std::cout << "}" << "\n";
                } else {
                    std::cout << "}";
                }
            }
            std::cout << "}" << "\n";
        }

        // Defining operators

        Tensor operator+ (const Tensor& other_tensor){
            assert(tensor.size() ==  other_tensor.tensor.size());
            assert(tensor_shape == other_tensor.tensor_shape);

            std::vector<Type> result;
            for (int i = 0; i < tensor.size(); i++){
                result.push_back(tensor[i] + other_tensor.tensor[i]);
            }
            return Tensor(result, tensor_shape);
        }

        Tensor operator- (const Tensor& other_tensor){
            assert(tensor.size() ==  other_tensor.tensor.size());
            assert(tensor_shape == other_tensor.tensor_shape);

            std::vector<Type> result;
            for (int i = 0; i < tensor.size(); i++){
                result.push_back(tensor[i] - other_tensor.tensor[i]);
            }
            return Tensor(result, tensor_shape);
        }

        Tensor operator* (const double& multiplier){
            std::vector<Type> result;
            for (double component : tensor){
                result.push_back(component * multiplier);
            }
            return Tensor(result, tensor_shape);
        }

        Tensor operator* (const Tensor& other_tensor){
            const int N = tensor_shape[tensor_shape.size() - 2];
            const int M = tensor_shape[tensor_shape.size() - 1];
            const int P = other_tensor.tensor_shape[1];

            std::vector<Type> result_tensor;

            for (int i = 0; i < N; ++i){
                for (int j = 0; j < P; j++){
                    double result_component = 0;
                    for (int k = 0; k < M; k++){
                        result_component += tensor[k + M * i] * other_tensor.tensor[P * k + j];
                    }
                    result_tensor.push_back(result_component);
                }
            }
            const std::vector<int> result_shape = {N, P};
            return Tensor(result_tensor, result_shape);
        }

        Tensor operator/ (const double& divisor){
            std::vector<Type> result;
            for (double component : tensor){
                result.push_back(component / divisor);
            }
            return Tensor(result, tensor_shape);
        }

        // Methods

        void shape(){
            std::cout << "{ ";
            std::vector<int>::iterator iter;
            for (iter = tensor_shape.begin(); iter != tensor_shape.end()-1; iter++){
                std::cout << *iter << ", ";
            }
            std::cout << *(iter++) << " }" << "\n";
        }

        Tensor T(){ // Performs the transpose of the tensor.
            Tensor transposed_tensor = *this; // making a copy of the tensor

            const int temp = transposed_tensor.tensor_shape[transposed_tensor.tensor_shape.size() - 2];
            transposed_tensor.tensor_shape[transposed_tensor.tensor_shape.size() - 2] = transposed_tensor.tensor_shape[transposed_tensor.tensor_shape.size() - 1];
            transposed_tensor.tensor_shape[transposed_tensor.tensor_shape.size() - 1] = temp;

            return transposed_tensor;
        }

        Tensor pow(const double& exponent){
            std::vector<Type> result;
            for (Type component : tensor){
                result.push_back(std::pow(component, exponent));
            }
            return Tensor(result, tensor_shape);
        }

        Type sum(){
            Type result = 0;
            for (Type component : tensor){
                result += component;
            }
            return result;
        }

        Type mean(){
            Type result = (*this).sum();
            return result / tensor.size();
        }
        
        Tensor mean(const int& axis){
            const int& N = tensor_shape[tensor_shape.size() - 2];
            const int& M = tensor_shape[tensor_shape.size() - 1];
            if (axis == 0){
                std::vector<Type> mean_row_vector; 
                for (int j = 0; j < M; j++){
                    double sum_of_column = 0;
                    for (int i = 0; i < N; i++){
                        sum_of_column += tensor[j + M * i];
                    }
                    mean_row_vector.push_back(sum_of_column / N);
                }
                std::vector<int> new_shape = {1, M};
                return Tensor(mean_row_vector, new_shape);
            } 
            else if (axis == 1){
                std::vector<Type> mean_column_vector; 
                for (int i = 0; i < N; i++){
                    double sum_of_row = 0;
                    for (int j = 0; j < M; j++){
                        sum_of_row += tensor[j + M * i];
                    }
                    mean_column_vector.push_back(sum_of_row / M);
                }
                std::vector<int> new_shape = {1, N};
                return Tensor(mean_column_vector, new_shape);
            } 
            else {
                throw std::invalid_argument("Invalid axis: any input must be 0 or 1.");
            }
        }

        Type var(){
            assert(tensor.size() > 1); // condition for calculating sample variance.

            Type result = 0;
            for (Type component : tensor){
                result += std::pow((component - (*this).mean()), 2);
            }
            return result / (tensor.size() - 1);
        }

        Tensor var(const int& axis){
            const int N = tensor_shape[tensor_shape.size() - 2];
            const int M = tensor_shape[tensor_shape.size() - 1];

            assert(N > 1); // condition for calculating sample variance.

            if (axis == 0){
                std::vector<Type> vector_of_vars;
                for (int j = 0; j < M; j++){
                double total_var = 0;
                    for (int i = 0; i < N; i++){
                        total_var += std::pow((tensor[j + M * i]) - (*this).mean(0).tensor[j], 2);
                    }
                vector_of_vars.push_back(total_var / (N-1));
                }
                std::vector<int> new_shape = {1, M};
                return Tensor(vector_of_vars, new_shape);
            }
            else if (axis == 1){
                assert(M > 1); // condition for calculating sample variance.

                std::vector<Type> vector_of_vars;
                for (int i = 0; i < N; i++){
                double total_var = 0;
                    for (int j = 0; j < M; j++){
                        total_var += std::pow((tensor[j + M * i]) - (*this).mean(1).tensor[i], 2);
                    }
                vector_of_vars.push_back(total_var / (M-1));
                }
                std::vector<int> new_shape = {1, N};
                return Tensor(vector_of_vars, new_shape);
            }
            else {
                throw std::invalid_argument("Invalid axis: any input must be 0 or 1.");
            }
        }

        Type std(){
            return std::pow((*this).var(), 0.5);
        }

        Tensor std(const int& axis){
            if (axis == 0 or axis == 1){
                return (*this).var(axis).pow(0.5);
            }
            else {
                throw std::invalid_argument("Invalid axis: any input must be 0 or 1.");
            }
        }

};

int main(){
    return 0;
}