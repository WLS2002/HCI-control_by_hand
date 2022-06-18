#include <particle.h>
class CSO_REC{
public:
    int SIZE = 400;
    int ITERATION = 50;

    double* globalBest;
    double bestFitness = INT_MIN;
    std::vector<double*> points;

    RectangleParticle particles[400];

    CSO_REC(std::vector<double*>& points){
        this->points = points;
        globalBest = new double[4];
    }

    void search(){
        int iteration = 0;
        while(iteration < ITERATION){
            for(int i = 0; i < SIZE; i++){
                double fitness = particles[i].get_fitness(points);
                if(fitness > bestFitness){
                    bestFitness = fitness;
                    globalBest = particles[i].getWeight();
                }

            }

            for(int i = 0; i < SIZE; i++){
                particles[i].update_weight(globalBest);
            }
            //std::cout << bestFitness << std::endl;
            iteration++;
        }
    }
};

class CSO_TRI{
public:
    int SIZE = 400;
    int ITERATION = 50;

    double* globalBest;
    double bestFitness = INT_MIN;
    std::vector<double*> points;

    TriangleParticle particles[400];

    CSO_TRI(std::vector<double*>& points){
        this->points = points;
        globalBest = new double[6];
    }

    void search(){
        int iteration = 0;
        while(iteration < ITERATION){
            for(int i = 0; i < SIZE; i++){
                double fitness = particles[i].get_fitness(points);
                if(fitness > bestFitness){
                    bestFitness = fitness;
                    globalBest = particles[i].getWeight();
                }

            }

            for(int i = 0; i < SIZE; i++){
                particles[i].update_weight(globalBest);
            }
            //std::cout << bestFitness << std::endl;
            iteration++;
        }
    }
};

class CSO_CIR{
public:
    int SIZE = 400;
    int ITERATION = 50;

    double* globalBest;
    double bestFitness = INT_MIN;
    std::vector<double*> points;

    CircleParticle particles[400];

    CSO_CIR(std::vector<double*>& points){
        this->points = points;
        globalBest = new double[3];
    }

    void search(){
        int iteration = 0;
        while(iteration < ITERATION){
            for(int i = 0; i < SIZE; i++){
                double fitness = particles[i].get_fitness(points);
                if(fitness > bestFitness){
                    bestFitness = fitness;
                    globalBest = particles[i].getWeight();
                }

            }

            for(int i = 0; i < SIZE; i++){
                particles[i].update_weight(globalBest);
            }
            //std::cout << bestFitness << std::endl;
            iteration++;
        }
    }
};