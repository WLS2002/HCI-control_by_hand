#include <particle.h>
#include <utils.h>

RectangleParticle::RectangleParticle(){
    for(int i = 0; i < 4; i++){
        weight[i] = r() * (HIGH_LIMIT[i] - LOW_LIMIT[i]) + LOW_LIMIT[i];
    }
}

double RectangleParticle::get_fitness(std::vector<double*>& points) {
    double fitness = rectangle_error(points, weight[0], weight[1], weight[2], weight[3]);
    if(fitness > bestFitness) {
        for(int i = 0; i < 4; i++){
            personalBest[i] = weight[i];
        }
        bestFitness = fitness;
    }
    return fitness;
}

void RectangleParticle::update_weight(double *best_weight) {
//    double omega = 0.5 + r() / 2;
//
//    for(int i = 0; i < 4; i++){
//        velocity[i] = omega * velocity[i];
//    }
//
//    double r1 = 0.5 * 1 * r();
//    for(int i = 0; i < 4; i++){
//        velocity[i] += r1 * (best_weight[i] - weight[i]);
//    }
//
//    double r2 = 0.5 * 1 * r();
//    for(int i = 0; i < 4; i++){
//        velocity[i] += r2 * (personalBest[i] - weight[i]);
//    }
//
//    for(int i = 0; i < 4; i++){
//        weight[i] += velocity[i];
//        if(weight[i] < LOW_LIMIT[i]) weight[i] = LOW_LIMIT[i];
//        if(weight[i] > HIGH_LIMIT[i]) weight[i] = HIGH_LIMIT[i];
//    }

    for(int i = 0; i < 4; i++){
        velocity[i] = r() * velocity[i];
    }

    for(int i = 0; i < 4; i++){
        velocity[i] += r() * (best_weight[i] - weight[i]);
    }

    for(int i = 0; i < 4; i++){
        velocity[i] += r() * (personalBest[i] - weight[i]);
    }

    for(int i = 0; i < 4; i++){
        weight[i] += velocity[i];
        if(weight[i] < LOW_LIMIT[i]) weight[i] = LOW_LIMIT[i];
        if(weight[i] > HIGH_LIMIT[i]) weight[i] = HIGH_LIMIT[i];
    }

}

double* RectangleParticle::getWeight() {
    return weight;
}

TriangleParticle::TriangleParticle(){
    for(int i = 0; i < 6; i++){
        weight[i] = r() * (HIGH_LIMIT[i] - LOW_LIMIT[i]) + LOW_LIMIT[i];
    }
}

double TriangleParticle::get_fitness(std::vector<double*>& points) {
    double fitness = triangle_error(points, weight[0], weight[1], weight[2], weight[3], weight[4], weight[5]);
    if(fitness > bestFitness) {
        for(int i = 0; i < 6; i++){
            personalBest[i] = weight[i];
        }
        bestFitness = fitness;
    }
    return fitness;
}

void TriangleParticle::update_weight(double *best_weight) {
    double omega = 0.5 + r() / 2;

    for(int i = 0; i < 6; i++){
        velocity[i] = omega * velocity[i];
    }

    double r1 = 0.5 * 1 * r();
    for(int i = 0; i < 6; i++){
        velocity[i] += r1 * (best_weight[i] - weight[i]);
    }

    double r2 = 0.5 * 1 * r();
    for(int i = 0; i < 6; i++){
        velocity[i] += r2 * (personalBest[i] - weight[i]);
    }

    for(int i = 0; i < 6; i++){
        weight[i] += velocity[i];
        if(weight[i] < LOW_LIMIT[i]) weight[i] = LOW_LIMIT[i];
        if(weight[i] > HIGH_LIMIT[i]) weight[i] = HIGH_LIMIT[i];
    }
}

double* TriangleParticle::getWeight() {
    return weight;
}

CircleParticle::CircleParticle(){
    for(int i = 0; i < 3; i++){
        weight[i] = r() * (HIGH_LIMIT[i] - LOW_LIMIT[i]) + LOW_LIMIT[i];
    }
}

double CircleParticle::get_fitness(std::vector<double*>& points) {
    double fitness = circle_error(points, weight[0], weight[1], weight[2]);
    if(fitness > bestFitness) {
        for(int i = 0; i < 3; i++){
            personalBest[i] = weight[i];
        }
        bestFitness = fitness;
    }
    return fitness;
}

void CircleParticle::update_weight(double *best_weight) {
    double omega = 0.5 + r() / 2;

    for(int i = 0; i < 3; i++){
        velocity[i] = omega * velocity[i];
    }

    double r1 = 0.5 * 1 * r();
    for(int i = 0; i < 3; i++){
        velocity[i] += r1 * (best_weight[i] - weight[i]);
    }

    double r2 = 0.5 * 1 * r();
    for(int i = 0; i < 3; i++){
        velocity[i] += r2 * (personalBest[i] - weight[i]);
    }

    for(int i = 0; i < 3; i++){
        weight[i] += velocity[i];
        if(weight[i] < LOW_LIMIT[i]) weight[i] = LOW_LIMIT[i];
        if(weight[i] > HIGH_LIMIT[i]) weight[i] = HIGH_LIMIT[i];
    }
}

double* CircleParticle::getWeight() {
    return weight;
}