#ifndef MAGNETODYNAMICS2D_H
#define MAGNETODYNAMICS2D_H

class FEMModel;
class Solver;

class MagnetoDynamics2D
{
public:
    MagnetoDynamics2D();
    ~MagnetoDynamics2D();

    void MagnetoDynamics2D_Init();
    bool run();
private:
    FEMModel* m_model;
    Solver* m_solver;
    bool m_transientSimulation;

};

#endif // MAGNETODYNAMICS2D_H
