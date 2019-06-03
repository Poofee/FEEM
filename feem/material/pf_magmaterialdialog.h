#ifndef PF_MAGMATERIALDIALOG_H
#define PF_MAGMATERIALDIALOG_H

#include <QDialog>

class QLineEdit;
class QTabWidget;
class FolderNode;
class CMaterialProp;

/*!
 \brief 创建一个电磁材料的材料设置窗口

*/
class PF_MagMaterialDialog : public QDialog
{
    Q_OBJECT
public:
    PF_MagMaterialDialog();

    void initialization();

    QWidget* createBasicPage();
    QWidget* createMagneticPage();
    QWidget* createHeatPage();
private:
    QLineEdit* edit_name;
    QLineEdit* edit_comment;
    QTabWidget* tab_material;
    QLineEdit* edit_ux;
    QLineEdit* edit_uy;
    QLineEdit* edit_hx;
    QLineEdit* edit_hy;
    QLineEdit* edit_conduct;
    QLineEdit* edit_coer;
    QLineEdit* edit_current;
    CMaterialProp* m_material;
};

#endif // PF_MAGMATERIALDIALOG_H
