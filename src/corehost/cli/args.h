// Copyright (c) .NET Foundation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef ARGS_H
#define ARGS_H

#include "utils.h"
#include "pal.h"
#include "trace.h"
#include "deps_format.h"
#include "libhost.h"

struct probe_config_t
{
    pal::string_t probe_dir;
    bool patch_roll_fwd;
    bool prerelease_roll_fwd;
    const deps_json_t* probe_deps_json;

    bool only_runtime_assets;
    bool only_serviceable_assets;

    bool probe_publish_dir;

    void print() const
    {
        trace::verbose(_X("probe_config_t: probe=[%s]  deps-json=[%p] deps-dir-probe=[%d]"),
            probe_dir.c_str(), probe_deps_json, probe_publish_dir);
    }

    probe_config_t(
        const pal::string_t& probe_dir,
        const deps_json_t* probe_deps_json,
        bool only_serviceable_assets,
        bool only_runtime_assets,
        bool probe_publish_dir)
        : probe_dir(probe_dir)
        , probe_deps_json(probe_deps_json)
        , only_serviceable_assets(only_serviceable_assets)
        , only_runtime_assets(only_runtime_assets)
        , probe_publish_dir(probe_publish_dir)
    {
    }

    static probe_config_t svc_ni(const pal::string_t& dir)
    {
        return probe_config_t(dir, nullptr, true, true, false);
    }

    static probe_config_t svc(const pal::string_t& dir)
    {
        return probe_config_t(dir, nullptr, true, false, false);
    }

    static probe_config_t fx(const pal::string_t& dir, const deps_json_t* deps)
    {
        return probe_config_t(dir, deps, false, false, false);
    }

    static probe_config_t lookup(const pal::string_t& dir)
    {
        return probe_config_t(dir, nullptr, false, false, false);
    }

    static probe_config_t published_deps_dir()
    {
        return probe_config_t(_X(""), nullptr, false, false, true);
    }
};

struct arguments_t
{
    pal::string_t own_path;
    pal::string_t app_dir;
    pal::string_t deps_path;
    pal::string_t core_servicing;
    std::vector<pal::string_t> probe_paths;
    pal::string_t managed_application;
    pal::string_t local_shared_store;
    pal::string_t global_shared_store;
    pal::string_t dotnet_shared_store;
    std::vector<pal::string_t> env_shared_store;
    int app_argc;
    const pal::char_t** app_argv;

    arguments_t();

    inline void print()
    {
        if (trace::is_enabled())
        {
            trace::verbose(_X("-- arguments_t: own_path='%s' app_dir='%s' deps='%s' core_svc='%s' mgd_app='%s'"),
                own_path.c_str(), app_dir.c_str(), deps_path.c_str(), core_servicing.c_str(), managed_application.c_str());
            for (const auto& probe : probe_paths)
            {
                trace::verbose(_X("-- arguments_t: probe dir: '%s'"), probe.c_str());
            }
            for (const auto& shared : env_shared_store)
            {
                trace::verbose(_X("-- arguments_t: env shared store: '%s'"), shared.c_str());
            }
            trace::verbose(_X("-- arguments_t: local shared store: '%s'"), local_shared_store.c_str());
            trace::verbose(_X("-- arguments_t: dotnet shared store: '%s'"), dotnet_shared_store.c_str());
            trace::verbose(_X("-- arguments_t: global shared store: '%s'"), global_shared_store.c_str());
        }
    }
};

bool parse_arguments(
    const hostpolicy_init_t& init,
    const int argc, const pal::char_t* argv[], arguments_t* arg_out);

#endif // ARGS_H
