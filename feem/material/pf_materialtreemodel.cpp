#include "pf_materialtreemodel.h"
#include "pf_material.h"

//#include <memory>
//#include <stdio.h>
#include <QDebug>

char* StripKey(char *c)
{
    char *d;
    int i,k;

    k=strlen(c);

    for(i=0;i<k;i++){
        if (c[i] == '='){
            d=c+i+1;
            return d;
        }
    }

    return c+k;
}
enum LABELTAGS{
    BEGINFOLDER = 1,
    ENDFOLDER,
    BEGINBLOCK,
    ENDBLOCK,
    NOTHING
};

int ParseLine(char *s, FILE *fp, CMaterialProp* MProp,std::vector<std::unique_ptr<FolderNode>> &nodes)
{
    char q[1024];
    char *v;
    int i,j,k;

    if (sscanf(s,"%s",q)==EOF) return false;

    // Library Hierarchy
    if( _strnicmp(q,"<beginfolder>",13)==0){
        q[0]=NULL;
        return BEGINFOLDER;
    }

    if( _strnicmp(q,"<foldername>",11)==0){
        v=StripKey(s);
        k=strlen(v);
        for(i=0;i<k;i++)
            if(v[i]=='\"'){
                v=v+i+1;
                i=k;
            }
        k=strlen(v);
        if(k>0) for(i=k-1;i>=0;i--){
            if(v[i]=='\"'){
                v[i]=0;
                i=-1;
            }
        }
        nodes.emplace_back(std::make_unique<FolderNode>(QString(v)));

//        FoldProps[FoldProps.GetUpperBound()].FolderName=v;
//        m_mytree.SetItemText(Parent,v);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<folderurl>",10)==0){
        v=StripKey(s);
        k=strlen(v);
        for(i=0;i<k;i++)
            if(v[i]=='\"'){
                v=v+i+1;
                i=k;
            }
        k=strlen(v);
        if(k>0) for(i=k-1;i>=0;i--){
            if(v[i]=='\"'){
                v[i]=0;
                i=-1;
            }
        }
//        FoldProps[FoldProps.GetUpperBound()].FolderURL=v;
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<foldervendor>",14)==0){
        v=StripKey(s);
        k=strlen(v);
        for(i=0;i<k;i++)
            if(v[i]=='\"'){
                v=v+i+1;
                i=k;
            }
        k=strlen(v);
        if(k>0) for(i=k-1;i>=0;i--){
            if(v[i]=='\"'){
                v[i]=0;
                i=-1;
            }
        }
//        FoldProps[FoldProps.GetUpperBound()].FolderVendor=v;
        q[0]=NULL;
        return NOTHING;
    }

    // Block Properties;
    if( _strnicmp(q,"<beginblock>",12)==0){

        q[0]=NULL;
        return BEGINBLOCK;
    }

    // Library Hierarchy
    if( _strnicmp(q,"<endfolder>",11)==0){

        q[0]=NULL;
        return ENDFOLDER;
    }

    if( _strnicmp(q,"<blockname>",10)==0){
        v=StripKey(s);
        k=strlen(v);
        for(i=0;i<k;i++)
            if(v[i]=='\"'){
                v=v+i+1;
                i=k;
            }
        k=strlen(v);
        if(k>0) for(i=k-1;i>=0;i--){
            if(v[i]=='\"'){
                v[i]=0;
                i=-1;
            }
        }
        MProp->BlockName=v;
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<mu_x>",6)==0){
        v=StripKey(s);
        sscanf_s(v,"%lf",&MProp->mu_x);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<mu_y>",6)==0){
        v=StripKey(s);
        sscanf_s(v,"%lf",&MProp->mu_y);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<H_c>",5)==0){
        v=StripKey(s);
        sscanf_s(v,"%lf",&MProp->H_c);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<J_re>",6)==0){
        v=StripKey(s);
        sscanf_s(v,"%lf",&MProp->Jsrc.re);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<J_im>",6)==0){
        v=StripKey(s);
        sscanf_s(v,"%lf",&MProp->Jsrc.im);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<sigma>",7)==0){
        v=StripKey(s);
        sscanf_s(v,"%lf",&MProp->Cduct);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<phi_h>",7)==0){
        v=StripKey(s);
        sscanf_s(v,"%lf",&MProp->Theta_hn);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<phi_hx>",8)==0){
        v=StripKey(s);
        sscanf_s(v,"%lf",&MProp->Theta_hx);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<phi_hy>",8)==0){
        v=StripKey(s);
        sscanf_s(v,"%lf",&MProp->Theta_hy);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<d_lam>",7)==0){
        v=StripKey(s);
        sscanf_s(v,"%lf",&MProp->Lam_d);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<LamFill>",8)==0){
        v=StripKey(s);
        sscanf_s(v,"%lf",&MProp->LamFill);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<LamType>",9)==0){
        v=StripKey(s);
        sscanf_s(v,"%i",&MProp->LamType);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<NStrands>",10)==0){
        v=StripKey(s);
        sscanf_s(v,"%i",&MProp->NStrands);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<WireD>",7)==0){
        v=StripKey(s);
        sscanf_s(v,"%lf",&MProp->WireD);
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<BHPoints>",10)==0){
        v=StripKey(s);
        sscanf_s(v,"%i",&MProp->BHpoints);
        if (MProp->BHpoints>0)
        {
            MProp->BHdata=(CComplex *)calloc(MProp->BHpoints,sizeof(CComplex));
            for(j=0;j<MProp->BHpoints;j++){
                fgets(s,1024,fp);
                sscanf_s(s,"%lf	%lf",&MProp->BHdata[j].re,&MProp->BHdata[j].im);
            }
        }
        q[0]=NULL;
        return NOTHING;
    }

    if( _strnicmp(q,"<endblock>",9)==0){
        q[0]=NULL;
        return ENDBLOCK;
    }

    return NOTHING;
}

PF_MaterialTreeModel::PF_MaterialTreeModel(QObject *parent)
    : BaseTreeModel(new WrapperNode(nullptr), parent)
{

}

PF_MaterialTreeModel::~PF_MaterialTreeModel()
{
    nodes.clear();
}

QVariant PF_MaterialTreeModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if (const Node *node = nodeForIndex(index)) {
        switch (role) {
        case Qt::DisplayRole: {/**要以文本形式呈现的关键数据**/
            result = node->displayName();
            break;
        }
        case Qt::EditRole: {/**适合在编辑器中编辑的形式的数据**/
            break;
        }
        case Qt::ToolTipRole: {/**项目工具提示中显示的数据**/
            QString tooltip = node->tooltip();
            break;
        }
        case Qt::DecorationRole: {/**要以图标形式呈现为装饰的数据**/
            if(node->asFolderNode())
                result = QIcon(":/tree/material_library.png");
            if(node->asLeafNode())
                result = QIcon(":/tree/material.png");
            break;
        }
        case Qt::FontRole: {/**用于使用默认委托呈现的项目的字体**/
            QFont font;
            //            if (project == SessionManager::startupProject())
            //                font.setBold(true);
            //            result = font;
            break;
        }
        case Qt::TextColorRole: {/**文本颜色**/
            break;
        }
        }
    }

    return result;
}

Qt::ItemFlags PF_MaterialTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;
    // We claim that everything is editable
    // That's slightly wrong
    // We control the only view, and that one does the checks
    Qt::ItemFlags f = Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsDragEnabled;
    if (Node *node = nodeForIndex(index)) {
        //        if (!node->asProjectNode()) {
        // either folder or file node
        //            if (node->supportsAction(Rename, node))
        //                f = f | Qt::ItemIsEditable;
        //        }
    }
    return f;
}

bool PF_MaterialTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (role != Qt::EditRole)
        return false;

    Node *node = nodeForIndex(index);
    if(!node) return false;

    return true;
}

Node *PF_MaterialTreeModel::nodeForIndex(const QModelIndex &index) const
{
    WrapperNode* flatNode = static_cast<WrapperNode* >(BaseTreeModel::itemForIndex(index));
    return flatNode ? flatNode->m_node : nullptr;
}

WrapperNode *PF_MaterialTreeModel::wrapperForNode(const Node *node) const
{
    auto pred = [node](WrapperNode *item) {
        return item->m_node == node;
    };
    const auto pred0 = [pred](TreeItem *treeItem) -> bool { return pred(static_cast<WrapperNode *>(treeItem)); };
    return static_cast<WrapperNode *>(m_root->findAnyChild(pred0));
}

QModelIndex PF_MaterialTreeModel::indexForNode(const Node *node) const
{
    WrapperNode *wrapper = wrapperForNode(node);
    return wrapper ? indexForItem(wrapper) : QModelIndex();
}

void PF_MaterialTreeModel::onExpanded(const QModelIndex &idx)
{

}

void PF_MaterialTreeModel::onCollapsed(const QModelIndex &idx)
{

}

/*!
 \brief 读入自带的材料库文件

 \return bool
*/
bool PF_MaterialTreeModel::loadBuiltinMaterials()
{
    char LibName[] = "matlib.dat";
    FILE *fp;
    CMaterialProp* MProp = nullptr;
//    QVector<FolderNode* > folderNodes;
//    CFolderProp FProp;
    char s[1024];
//    int i,k;

//    std::unique_ptr<FolderNode> root = std::make_unique<FolderNode>(QString(tr("Global Definitions")),NodeType::Definition,QIcon(":/tree/global_branch.png"));

    nodes.emplace_back(std::make_unique<FolderNode>(QString("root")));

    // read in materials library;
    if ((fp=fopen(LibName,"rt")) == NULL)
        return true;


    while (fgets(s,1024,fp) != NULL)
    {
        int result = ParseLine(s,fp,MProp,nodes);
        if(result == BEGINBLOCK){/**  **/
            MProp = new CMaterialProp();
        }else if(result == ENDBLOCK){/**  **/
            std::unique_ptr<CMaterialPropNode> node = std::make_unique<CMaterialPropNode>(MProp);
            nodes.back()->addNode(std::move(node));
        }else if(result == BEGINFOLDER){/**  **/

        }else if(result == ENDFOLDER){
            /** 将该节点的添加到上一个节点当中，由于用了moveto，所以该节点的
                指针指向会被清空**/
            int size = nodes.size();
            nodes.at(size-2)->addNode(std::move(nodes.at(size-1)));
            nodes.pop_back();
        }
    }
    fclose(fp);
    QSet<Node *> seen;
//    WrapperNode *container = new WrapperNode(nodes.at(0).get());
//    rootItem()->appendChild(container);
//    addFolderNode(container,nodes.at(0).get(),&seen);
    for(Node* n : nodes.at(0).get()->nodes()){
        WrapperNode *container = new WrapperNode(n);
        rootItem()->appendChild(container);
        if(FolderNode* f = n->asFolderNode())
            addFolderNode(container,f,&seen);
    }
//    nodes.pop_back();

    return true;
}

void PF_MaterialTreeModel::addFolderNode(WrapperNode *parent, FolderNode *folderNode, QSet<Node *> *seen)
{
    for (Node *node : folderNode->nodes()) {
        if (FolderNode *subFolderNode = node->asFolderNode()) {
            auto node = new WrapperNode(subFolderNode);
            parent->appendChild(node);
            addFolderNode(node, subFolderNode, seen);
        } else if (LeafNode *leafNode = node->asLeafNode()) {
            parent->appendChild(new WrapperNode(leafNode));
        }
    }
}

CMaterialPropNode::CMaterialPropNode(CMaterialProp *material)
    :LeafNode (material->BlockName,LeafType::CMaterialProp)
    ,m_material(material)
{
//    setIcon(QIcon(":/tree/material.png"));
}

CMaterialPropNode::~CMaterialPropNode()
{
    qDebug()<<Q_FUNC_INFO;
}
