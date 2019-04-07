#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {
/** 定义项目中的常量 **/

/** RibbonBar **/
const char RIBBONBAR[]                  = "FEEM.RibbonBar";
/** pages **/
const char P_HOME[]                     = "FEEM.Page.Home";
const char P_DEFINITION[]               = "FEEM.Page.Definition";
const char P_GEOMETRY[]                 = "FEEM.Page.Geometry";
const char P_MATERIAL[]                 = "FEEM.Page.Material";
const char P_MESH[]                     = "FEEM.Page.Mesh";
const char P_SOLVER[]                   = "FEEM.Page.Solver";
const char P_Result[]                   = "FEEM.Page.Result";

// Contexts
const char C_GLOBAL[]                   = "Global Context";

/**  actions **/

/** Default groups **/
const char G_DEFAULT_ONE[]              = "FEEM.Group.Default.One";
const char G_DEFAULT_TWO[]              = "FEEM.Group.Default.Two";
const char G_DEFAULT_THREE[]            = "FEEM.Group.Default.Three";

/** RibbonBar groups **/

/** Home RibbonBar page groups **/
const char G_HOME_PROJECT[]             = "FEEM.Group.Home.Project";
/** Definition RibbonBar page groups **/
const char G_DEFINITION_VARIABLE[]      = "FEEM.Group.Definition.Variable";
/** Geometry RibbonBar page groups **/
const char G_GEOMETRY_IMEXPORT[]        = "FEEM.Group.Geometry.ImExport";
const char G_GEOMETRY_BUILD[]           = "FEEM.Group.Geometry.Build";
const char G_GEOMETRY_SETTING[]         = "FEEM.Group.Geometry.Setting";
const char G_GEOMETRY_DRAW[]            = "FEEM.Group.Geometry.Draw";
const char G_GEOMETRY_OPERATION[]       = "FEEM.Group.Geometry.Operation";
const char G_GEOMETRY_VIEW[]            = "FEEM.Group.Geometry.View";
/** Material RibbonBar page groups **/
const char G_MATERIAL_LIBRARY[]         = "FEEM.Group.Material.Library";
/** Mesh RibbonBar page groups **/
const char G_MESH_BUILD[]               = "FEEM.Group.Mesh.Build";
const char G_MESH_GENERATOR[]           = "FEEM.Group.Mesh.Generator";
const char G_MESH_IMEXPORT[]            = "FEEM.Group.Mesh.ImExport";
const char G_MESH_STATICS[]             = "FEEM.Group.Mesh.Statics";
const char G_MESH_CLEAR[]               = "FEEM.Group.Mesh.Clear";
/** Solver RibbonBar page groups **/
const char G_SOLVE_SOLVE[]              = "FEEM.Group.Solve.Solve";
const char G_SOLVE_SETTING[]            = "FEEM.Group.Solve.Setting";
const char G_SOLVE_CLEAR[]              = "FEEM.Group.Solve.Clear";
/** Result RibbonBar page groups **/
const char G_RESULT_PLOT[]              = "FEEM.Group.Result.Plot";
}
#endif // CONSTANTS_H
