#include "pf_nodetreebuilder.h"

#include "pf_node.h"
#include "pf_project.h"

static void createTree(const PF_Project *pro, ProjectNode *node)
{
    if(!pro)
        return;
    if(!node)
        return;

    node->setDisplayName(pro->displayName());
//    node->setIcon(pro->);

//    // .pro/.pri-file itself:
//    node->addNode(std::make_unique<FileNode>(pri->filePath(), FileType::Project, false));

//    // other normal files:
//    const QVector<QmakeStaticData::FileTypeData> &fileTypes = qmakeStaticData()->fileTypeData;
//    for (int i = 0; i < fileTypes.size(); ++i) {
//        FileType type = fileTypes.at(i).type;
//        const QSet<FileName> &newFilePaths = Utils::filtered(pri->files(type), [&toExclude](const Utils::FileName &fn) {
//            return !Utils::contains(toExclude, [&fn](const Utils::FileName &ex) { return fn.isChildOf(ex); });
//        });

//        if (!newFilePaths.isEmpty()) {
//            auto vfolder = std::make_unique<VirtualFolderNode>(pri->filePath().parentDir(), Node::DefaultVirtualFolderPriority - i);
//            vfolder->setIcon(fileTypes.at(i).icon);
//            vfolder->setDisplayName(fileTypes.at(i).typeName);
//            vfolder->setAddFileFilter(fileTypes.at(i).addFileFilter);

//            if (type == FileType::Resource) {
//                for (const FileName &file : newFilePaths) {
//                    auto vfs = pri->project()->qmakeVfs();
//                    QString contents;
//                    QString errorMessage;
//                    // Prefer the cumulative file if it's non-empty, based on the assumption
//                    // that it contains more "stuff".
//                    int cid = vfs->idForFileName(file.toString(), QMakeVfs::VfsCumulative);
//                    vfs->readFile(cid, &contents, &errorMessage);
//                    // If the cumulative evaluation botched the file too much, try the exact one.
//                    if (contents.isEmpty()) {
//                        int eid = vfs->idForFileName(file.toString(), QMakeVfs::VfsExact);
//                        vfs->readFile(eid, &contents, &errorMessage);
//                    }
//                    vfolder->addNode(std::make_unique<ResourceEditor::ResourceTopLevelNode>(file, false, contents, vfolder.get()));
//                }
//            } else {
//                for (const FileName &fn : newFilePaths) {
//                    // Qmake will flag everything in SOURCES as source, even when the
//                    // qt quick compiler moves qrc files into it:-/ Get better data based on
//                    // the filename.
//                    type = FileNode::fileTypeForFileName(fn);
//                    vfolder->addNestedNode(std::make_unique<FileNode>(fn, type, false));
//                }
//                for (FolderNode *fn : vfolder->folderNodes())
//                    fn->compress();
//            }
//            node->addNode(std::move(vfolder));
//        }
//    }

//    // Virtual folders:
//    for (QmakePriFile *c : pri->children()) {
//        std::unique_ptr<QmakePriFileNode> newNode;
//        if (auto pf = dynamic_cast<QmakeProFile *>(c))
//            newNode = std::make_unique<QmakeProFileNode>(c->project(), c->filePath(), pf);
//        else
//            newNode = std::make_unique<QmakePriFileNode>(c->project(), node->proFileNode(), c->filePath(), c);
//        createTree(c, newNode.get(), toExclude);
//        node->addNode(std::move(newNode));
//    }
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
