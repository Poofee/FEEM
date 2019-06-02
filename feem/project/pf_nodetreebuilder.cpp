#include "pf_nodetreebuilder.h"

#include "pf_node.h"
#include "pf_project.h"

#include <QObject>
#include <QString>

static void createTree(const PF_Project *pro, ProjectNode *node)
{
    if(!pro)
        return;
    if(!node)
        return;

    node->setDisplayName(pro->displayName());

    /** 添加变量 **/
    auto def_node = std::make_unique<FolderNode>(QString(QObject::tr("Definitions")),NodeType::Variable,QIcon(":/tree/definitions.png"));
    /** 添加材料 **/
    auto material_node = std::make_unique<FolderNode>(QString(QObject::tr("Materials: Materials")),NodeType::Material,QIcon(":/tree/material.png"));
    /** 添加几何 **/
    auto geo_node = std::make_unique<FolderNode>(QString(QObject::tr("Geometry1")),NodeType::Geometry,QIcon(":/tree/geometry.png"));
    /** 添加分网 **/
    auto mesh_node = std::make_unique<FolderNode>(QString(QObject::tr("Mesh1")),NodeType::Mesh,QIcon(":/tree/mesh.png"));

    auto globaldef_node = std::make_unique<FolderNode>(QString(QObject::tr("Global Definitions")),NodeType::Definition,QIcon(":/tree/global_branch.png"));

    auto comp_node = std::make_unique<FolderNode>(QString(QObject::tr("Component: Component1")),NodeType::Component,QIcon(":/tree/model_2d_axi.png"));

    globaldef_node->addNode(std::move(def_node));
    globaldef_node->addNode(std::move(material_node));

    comp_node->addNode(std::move(geo_node));
    comp_node->addNode(std::move(mesh_node));

    node->addNode(std::move(globaldef_node));
    node->addNode(std::move(comp_node));
}

/**
 * @brief 根据project的内容生成treenode
 *
 * @param project
 * @return std::unique_ptr<ProjectNode>
 */
std::unique_ptr<ProjectNode> PF_NodeTreeBuilder::buildTree(PF_Project *project)
{
    auto root = std::make_unique<ProjectNode>(project);

    createTree(project, root.get());

    return root;
}
