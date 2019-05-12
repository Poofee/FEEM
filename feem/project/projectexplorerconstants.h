#ifndef PROJECTEXPLORERCONSTANTS_H
#define PROJECTEXPLORERCONSTANTS_H

namespace Constants {

// Modes and their priorities
const char MODE_SESSION[]         = "Project";

// Actions
const char BUILD[]                = "ProjectExplorer.Build";
const char STOP[]                 = "ProjectExplorer.Stop";
const char ADDNEWFILE[]           = "ProjectExplorer.AddNewFile";
const char FILEPROPERTIES[]       = "ProjectExplorer.FileProperties";
const char RENAMEFILE[]           = "ProjectExplorer.RenameFile";
const char REMOVEFILE[]           = "ProjectExplorer.RemoveFile";

// Context
const char C_PROJECT_TREE[]       = "ProjectExplorer.ProjectTreeContext";

// Menus
const char M_BUILDPROJECT[]       = "ProjectExplorer.Menu.Build";
const char M_DEBUG[]              = "ProjectExplorer.Menu.Debug";
const char M_DEBUG_STARTDEBUGGING[] = "ProjectExplorer.Menu.Debug.StartDebugging";

/** 要添加到ribbon栏的group **/
const char G_BUILD_BUILD[]        = "ProjectExplorer.Group.Build";
const char G_BUILD_DEPLOY[]       = "ProjectExplorer.Group.Deploy";
const char G_BUILD_REBUILD[]      = "ProjectExplorer.Group.Rebuild";
const char G_BUILD_CLEAN[]        = "ProjectExplorer.Group.Clean";
const char G_BUILD_RUN[]          = "ProjectExplorer.Group.Run";
const char G_BUILD_CANCEL[]       = "ProjectExplorer.Group.BuildCancel";

/** 不同的上下文菜单以及子菜单 **/
const char M_PROJECTCONTEXT[]     = "Project.Menu.Project";/** 项目节点 **/
const char M_ADDMODELCONTEXT[] = "Project.Menu.Project.AddModel";
const char M_ADDSTUDYCONTEXT[] = "Project.Menu.Project.AddStudy";

const char M_GLOBALDEFSCONTEXT[]  = "Project.Menu.GlobalDefs";/** 全局定义节点 **/

const char M_COMPONENTCONTEXT[]      = "Project.Menu.Component";/** 模型节点 **/

const char M_MATERIALCONTEXT[] = "Project.Menu.Material";/** 材料节点 **/

const char M_GEOMETRYCONTEXT[] = "Project.Menu.Geometry";/** 几何节点 **/

const char M_MESHCONTEXT[] = "Project.Menu.Mesh";/** 分网节点 **/

/** 每个上下文菜单当中的所有group **/
const char G_PROJECT_ADD[]      = "Project.Group.Add";
const char G_PROJECT_SOLVE[]      = "Project.Group.Solve";

const char G_FOLDER_LOCATIONS[]   = "ProjectFolder.Group.Locations";
const char G_FOLDER_FILES[]       = "ProjectFolder.Group.Files";
const char G_FOLDER_OTHER[]       = "ProjectFolder.Group.Other";
const char G_FOLDER_CONFIG[]      = "ProjectFolder.Group.Config";

const char G_FILE_OPEN[]          = "ProjectFile.Group.Open";
const char G_FILE_OTHER[]         = "ProjectFile.Group.Other";
const char G_FILE_CONFIG[]        = "ProjectFile.Group.Config";

const char G_HELP[] = "Project.Help";

} // namespace Constants

#endif // PROJECTEXPLORERCONSTANTS_H
