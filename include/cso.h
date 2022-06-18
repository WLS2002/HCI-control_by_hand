#include <particle.h>
class CSO_REC{
public:
    int SIZE = 400;
    int ITERATION = 100;

    double* globalBest;

    double bestFitness = INT_MIN;
    std::vector<double*> points;

    RectangleParticle particles[400];

    CSO_REC(std::vector<double*>& points){
        this->points = points;
    }

    void search(){
        int iteration = 0;
        while(iteration < ITERATION){

            double avg[4] {0};
            for(int i = 0; i < SIZE; i++){
                for(int j = 0; j < 4; j++){
                    avg[j] += particles[i].getWeight()[j];
                }
            }

            for(int i = 0; i < 4; i++){
                avg[i] /= SIZE;
            }

            int* order = random_order(SIZE);

            for(int i = 0; i < SIZE; i+=2){
                int x = order[i], y = order[i + 1];
                double fit_x = particles[x].get_fitness(points);
                double fit_y = particles[y].get_fitness(points);

                if(fit_x > bestFitness){
                    bestFitness = fit_x;
                    globalBest = particles[x].getWeight();
                }

                if(fit_y > bestFitness){
                    bestFitness = fit_y;
                    globalBest = particles[y].getWeight();
                }

                if(fit_x > fit_y){
                    particles[y].update_weight(particles[x].getWeight(), avg);
                } else {
                    particles[x].update_weight(particles[y].getWeight(), avg);
                }
            }
            //std::cout << bestFitness << std::endl;
            iteration ++;
        }
    }
};

class CSO_TRI{
public:
    int SIZE = 400;
    int ITERATION = 100;

    double* globalBest;

    double bestFitness = INT_MIN;
    std::vector<double*> points;

    TriangleParticle particles[400];

    CSO_TRI(std::vector<double*>& points){
        this->points = points;
    }

    void search(){
        int iteration = 0;
        while(iteration < ITERATION){

            double avg[6] {0};
            for(int i = 0; i < SIZE; i++){
                for(int j = 0; j < 6; j++){
                    avg[j] += particles[i].getWeight()[j];
                }
            }

            for(int i = 0; i < 6; i++){
                avg[i] /= SIZE;
            }

            int* order = random_order(SIZE);

            for(int i = 0; i < SIZE; i+=2){
                int x = order[i], y = order[i + 1];
                double fit_x = particles[x].get_fitness(points);
                double fit_y = particles[y].get_fitness(points);

                if(fit_x > bestFitness){
                    bestFitness = fit_x;
                    globalBest = particles[x].getWeight();
                }

                if(fit_y > bestFitness){
                    bestFitness = fit_y;
                    globalBest = particles[y].getWeight();
                }

                if(fit_x > fit_y){
                    particles[y].update_weight(particles[x].getWeight(), avg);
                } else {
                    particles[x].update_weight(particles[y].getWeight(), avg);
                }
            }
            //std::cout << bestFitness << std::endl;
            iteration ++;
        }
    }
};

class CSO_CIR{
public:
    int SIZE = 400;
    int ITERATION = 100;

    double* globalBest;

    double bestFitness = INT_MIN;
    std::vector<double*> points;

    CircleParticle particles[400];

    CSO_CIR(std::vector<double*>& points){
        this->points = points;
    }

    void search(){
        int iteration = 0;
        while(iteration < ITERATION){

            double avg[3] {0};
            for(int i = 0; i < SIZE; i++){
                for(int j = 0; j < 3; j++){
                    avg[j] += particles[i].getWeight()[j];
                }
            }

            for(int i = 0; i < 3; i++){
                avg[i] /= SIZE;
            }

            int* order = random_order(SIZE);

            for(int i = 0; i < SIZE; i+=2){
                int x = order[i], y = order[i + 1];
                double fit_x = particles[x].get_fitness(points);
                double fit_y = particles[y].get_fitness(points);

                if(fit_x > bestFitness){
                    bestFitness = fit_x;
                    globalBest = particles[x].getWeight();
                }

                if(fit_y > bestFitness){
                    bestFitness = fit_y;
                    globalBest = particles[y].getWeight();
                }

                if(fit_x > fit_y){
                    particles[y].update_weight(particles[x].getWeight(), avg);
                } else {
                    particles[x].update_weight(particles[y].getWeight(), avg);
                }
            }
            //std::cout << bestFitness << std::endl;
            iteration ++;
        }
    }
};