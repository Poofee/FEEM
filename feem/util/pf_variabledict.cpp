#include "pf_variabledict.h"

#include <QDebug>

void PF_VariableDict::clear()
{
    variables.clear();
}

void PF_VariableDict::add(const QString& key, double var)
{
    if(key.isEmpty()){
        qDebug()<<Q_FUNC_INFO<<"Empty string";
    }
    variables.insert(key,var);
}

void PF_VariableDict::remove(const QString& key)
{
    variables.remove(key);
}

double PF_VariableDict::getValue(const QString& key)
{
    auto i = variables.find(key);

    if(variables.end() != i){
        return i.value();
    }
    return 0.0;
}

