//
// Created by zhushuo on 2023/10/2.
//

#include "io_util.h"

std::optional<std::string> GetEnvVar(const char* name)
{
    char* c_str = getenv(name);
    if(c_str == nullptr)
    {
        return {};
    }

    return  std::string(c_str);
}

Status SetEnvVar(const char* name, const char* value)
{
    std::string  env_val = name;
    env_val = env_val + "=" + value;
    //std::cout<<env_val<<std::endl;
    if(putenv(env_val.c_str()) == 0)
    {
        return Status::make_ok();
    }
    else
    {
        return Status::make_invalid("failed setting env var");
    }
}

Status SetEnvVar(const std::string& name, const std::string& value)
{
    return SetEnvVar(name.c_str(), value.c_str());
}
