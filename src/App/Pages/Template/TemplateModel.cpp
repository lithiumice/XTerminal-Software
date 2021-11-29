#include "TemplateModel.h"

#include "Template.h"
#include "App/Accounts/Account_Master.h"
using namespace Page;

static int onEvent(Account* account, Account::EventParam_t* param)
{
    auto instance = (TemplateModel*)(account->UserData);

    if(param->event==Account::EVENT_PUB_PUBLISH)
    {
        instance->data_reved_flag = 1;
        memcpy(&instance->info, param->data_p, sizeof(HAL::Weather_Info_t));
    }
    return 0;
}

void TemplateModel::Init()
{
    account = new Account(
        "TemplateModel",
        AccountSystem::Broker(), 
        0, 
        this
    );
    account->SetEventCallback(onEvent);
    account->Subscribe("Weather");

    GetWeatherInfo();
}

void TemplateModel::DeInit()
{
    if (account)
    {
        delete account;
        account = nullptr;
    }
}

void Page::TemplateModel::GetWeatherInfo()
{
    account->Pull("Weather", &info, sizeof(HAL::Weather_Info_t));
    data_reved_flag = 1;
}

void Page::TemplateModel::GetWeatherInfo(HAL::Weather_Info_t* info)
{
    account->Pull("Weather", info, sizeof(HAL::Weather_Info_t));
    data_reved_flag = 1;
}
