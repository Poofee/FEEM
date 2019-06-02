#include "pf_document.h"

PF_Document::PF_Document(PF_EntityContainer *parent)
    :PF_EntityContainer(parent)
{

}

/*!
 \brief 将列表当中的形状元素导出到.geo文件

 \param filename
*/
void PF_Document::exportGmshGeo(const QString &filename)
{

}
