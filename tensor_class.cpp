#include <iostream>
#include <vector>

class Tensor{
    private:
        const std::vector<double> data;
        const std::vector<int> shape;
    public:
        Tensor(const std::vector<double>& input_data, const std::vector<int>& input_shape) : data(input_data), shape(input_shape) {}
        
        void print(){
            int N = this->shape[0];
            int M = this->shape[1];

            std::cout << "{";
            for (int i = 0; i < N; i++){
                std::cout << "{ ";
                for (int j = 0; j < M; j++){
                    std::cout << this->data[j + i * M] << " ";
                }
                if (i != N-1){
                    std::cout << "}" << "\n";
                } else {
                    std::cout << "}";
                }
            }
            std::cout << "}" << "\n";
        }

};

int main(){
    std::vector<double> data1 = {1, 2, 3, 4, 5, 6};
    std::vector<int> shape1 = {3, 2};

    Tensor tensor1(data1, shape1);

    tensor1.print();

    return 0;
}