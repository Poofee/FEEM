#ifndef TYPES_H
#define TYPES_H

class Variable_t{

};

class ValueList_t{

};

class EquationArray_t{

};

class ComponentArray_t{

};

class BodyForceArray_t{

};

class InitialConditionArray_t{

};

class BoundaryConditionArray_t{

};

class MaterialArray_t{

};

class BodyArray_t{

};

class BoundaryArray_t{

};

class Solver_t{

};

class Nodes_t{
public:
    int NumberOfNodes;
    double* x;
    double* y;
    double* z;
};

class Element_t{

};

class Mesh_t{

};

class Matrix_t{

};

class Circuit_t{

};

/*!
 \brief 保存所有的模型信息，包括分网，材料，边界条件等

*/
class SolutionModel{
public:
    /** 坐标维度和类型 **/
    int CoordinateSystem;

    /** 模型维度信息 **/
    int NumberOfBulkElements;
    int NumberOfNodes;
    int NumberOfBoundaryElements;

    /** 仿真输入信息，作为一个整体 **/
    ValueList_t Simulation;
    /** 变量 **/
    Variable_t Variables;

    /** 一些物理常量，可以从数据库当中读取，或者在这里设置 **/
    ValueList_t Constants;

    /** 要求解的公式 **/
    EquationArray_t Equations;

    /** 物理组件 **/
    ComponentArray_t Components;

    /** 应力，例如热源，电流源 **/
    BodyForceArray_t Bodyforces;

    /** 场变量的初始条件 **/
    InitialConditionArray_t ICs;

    /** 边界条件 **/
    BoundaryConditionArray_t BCs;

    /** 材料参数 **/
    MaterialArray_t Materials;

    /** 体，或者域，每一个单元都有一个对应的body的指针，每一个body都
    有自己的材料，初始条件，应力和方程等等**/
    BodyArray_t Bodies;

    BoundaryArray_t Boundaries;

    /** 线性方程求解器 **/
    Solver_t Solvers;

    /** 有限元节点 **/
    Nodes_t Nodes;

    /** 有限元单元 **/
    Element_t Elements;

    /** 有限元分网 **/
    Mesh_t Meshes;

    /** 电路 **/
    int n_Circuits;
    Matrix_t Circuits_Matrix;
    Circuit_t Circuits;
};

#endif // TYPES_H
