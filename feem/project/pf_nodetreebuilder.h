#ifndef PF_NODETREEBUILDER_H
#define PF_NODETREEBUILDER_H

#include <memory>

class ProjectNode;
class PF_Project;

class PF_NodeTreeBuilder
{
public:
    static std::unique_ptr<ProjectNode> buildTree(PF_Project *project);
};

#endif // PF_NODETREEBUILDER_H
