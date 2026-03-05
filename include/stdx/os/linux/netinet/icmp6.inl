#pragma once

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::netinet {
    #ifdef __linux__
    using ICmp6Filter = ::icmp6_filter;
    using ICmp6Header = ::icmp6_hdr;
    using NdRouterSolicit = ::nd_router_solicit;
    using NdRouterAdvert = ::nd_router_advert;
    using NdNeighborSolicit = ::nd_neighbor_solicit;
    using NdNeighborAdvert = ::nd_neighbor_advert;
    using NdRedirect = ::nd_redirect;
    using NdOptHeader = ::nd_opt_hdr;
    using NdOptPrefixInfo = ::nd_opt_prefix_info;
    using NdOptRdHeader = ::nd_opt_rd_hdr;
    using NdOptMTU = ::nd_opt_mtu;
    using MldHeader = ::mld_hdr;
    using ICmp6RouterREnum = ::icmp6_router_renum;
    using RrPcoMatch = ::rr_pco_match;
    using RrPcoUse = ::rr_pco_use;
    using RrResult = ::rr_result;
    using NdOptAdvInterval = ::nd_opt_adv_interval;
    using NdOptHomeAgentInfo = ::nd_opt_home_agent_info;
    #endif
}
