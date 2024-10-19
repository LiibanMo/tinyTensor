#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

template <typename Type>
class Tensor{
    private:
        const std::vector<Type> tensor;
        std::vector<int> shape;
    public:
        Tensor(const std::vector<Type>& input_data, const std::vector<int>& input_shape) : tensor(input_data), shape(input_shape) {}
        
        void print(){
            const int N = shape[0];
            const int M = shape[1];

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
            std::vector<Type> result;
            for (int i = 0; i < tensor.size(); i++){
                result.push_back(tensor[i] + other_tensor.tensor[i]);
            }
            return Tensor(result, shape);
        }

        Tensor operator- (const Tensor& other_tensor){
            std::vector<Type> result;
            for (int i = 0; i < tensor.size(); i++){
                result.push_back(tensor[i] - other_tensor.tensor[i]);
            }
            return Tensor(result, shape);
        }

        Tensor operator* (const double& multiplier){
            std::vector<Type> result;
            for (double component : tensor){
                result.push_back(component * multiplier);
            }
            return Tensor(result, shape);
        }

        Tensor operator* (const Tensor& other_tensor){
            const int N = shape[0];
            const int M = shape[1];
            const int P = other_tensor.shape[1];

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
            return Tensor(result, shape);
        }

        // Methods

        Tensor T(){
            std::reverse(shape.begin(), shape.end());
            return *this;
        }
        double sum(){
            double result = 0;
            for (double component : tensor){
                result += component;
            }
            return result;
        }

        double mean(){
            double result = 0;
            for (double component : tensor){
                result += component;
            }
            return result / tensor.size();
        }
        
        double var(){
            double result = 0;
            for (double component : tensor){
                result += std::pow((component - (*this).mean()), 2);
            }
            return result / (tensor.size() - 1);
        }

        double std(){
            return pow((*this).var(), 0.5);
        }
};

int main(){
    return 0;
}