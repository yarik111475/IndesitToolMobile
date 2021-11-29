#include "EepModel.h"

EepModel::EepModel()
{
    Model="";
    Code="";
    Serial="";
    FileChecksum=0;
    RealChecksum=0;
    IsModelValid=false;
    IsChecksumValid=false;
}

void EepModel::reset()
{
    Model.clear();
    Code.clear();
    Serial.clear();
    RealChecksum=0;
    FileChecksum=0;
    Data.clear();
}

bool EepModel::operator==(const EepModel &Model) const
{
    bool result=this->RealChecksum==Model.RealChecksum &&
                this->FileChecksum==Model.FileChecksum;
    return result;
}

bool EepModel::operator!=(const EepModel &other) const
{
    bool result=this->RealChecksum!=other.RealChecksum &&
                this->FileChecksum!=other.FileChecksum;
    return result;
}

EepModel &EepModel::operator=(const EepModel &other)
{
    if(this!=&other){
        this->Model=other.Model;
        this->Code=other.Code;
        this->Serial=other.Serial;
        this->RealChecksum=other.RealChecksum;
        this->FileChecksum=other.FileChecksum;
        this->IsChecksumValid=other.IsChecksumValid;
        this->IsModelValid=other.IsModelValid;

        this->Data.clear();
        this->Data.append(other.Data);
    }
    return *this;
}
