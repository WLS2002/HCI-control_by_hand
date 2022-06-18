#include <vector>
class RectangleParticle{
public:
    double weight[4]{0};
    double velocity[4]{0};
    double personalBest[4]{0};
    double bestFitness = INT_MIN;
    double LOW_LIMIT[4] {0, 0, 0, 0};
    double HIGH_LIMIT[4] {640, 480, 640, 480};

    RectangleParticle();

    double get_fitness(std::vector<double*>& points);
    void update_weight(double* best_weight);
    double* getWeight();
};

class TriangleParticle{
public:
    double weight[6]{0};
    double velocity[6]{0};
    double personalBest[6]{0};
    double bestFitness = INT_MIN;
    double LOW_LIMIT[6] {0, 0, 0, 0, 0, 0};
    double HIGH_LIMIT[6] {640, 480, 640, 480, 640, 480};

    TriangleParticle();

    double get_fitness(std::vector<double*>& points);
    void update_weight(double* best_weight);
    double* getWeight();
};

class CircleParticle{
public:
    double weight[3]{0};
    double velocity[3]{0};
    double personalBest[3]{0};
    double bestFitness = INT_MIN;
    double LOW_LIMIT[3] {0, 0, 0};
    double HIGH_LIMIT[3] {640, 480, 480};

    CircleParticle();

    double get_fitness(std::vector<double*>& points);
    void update_weight(double* best_weight);
    double* getWeight();
};