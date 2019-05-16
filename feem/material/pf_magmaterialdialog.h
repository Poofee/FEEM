#ifndef PF_MAGMATERIALDIALOG_H
#define PF_MAGMATERIALDIALOG_H

#include <QDialog>

/*!
 \brief 创建一个电磁材料的材料设置窗口

*/
class PF_MagMaterialDialog : public QDialog
{
    Q_OBJECT
public:
    PF_MagMaterialDialog();

    QWidget* createMagneticPage();
};

#endif // PF_MAGMATERIALDIALOG_H
