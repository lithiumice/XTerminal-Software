#ifndef __AppList_MODEL_H
#define __AppList_MODEL_H

#include "App/Accounts/Account_Master.h"

namespace Page
{

class AppListModel
{
public:
    void Init();
    void Deinit();
    
private:
    Account* account;

private:

};

}

#endif
