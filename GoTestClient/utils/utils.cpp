#include "utils.h"
Utils* Utils::_instance = new  Utils;
void Utils::init()
{

}

Utils::Utils(QObject *parent) : QObject(parent)
{

}
