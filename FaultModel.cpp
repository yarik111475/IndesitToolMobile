#include "FaultModel.h"

FaultModel::FaultModel()
{

}

FaultModel FaultModel::operator=(const FaultModel &other)
{
    if(this!=&other){
        FaultCode=other.FaultCode;
        SubCode=other.SubCode;
        Occurence=other.Occurence;
        Description=other.Description;
    }
    return *this;
}
