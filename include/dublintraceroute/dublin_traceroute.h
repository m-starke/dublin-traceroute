/**
 * \file   dublin_traceroute.h
 * \author Andrea Barberio <insomniac@slackware.it>
 * \copyright 2-clause BSD
 * \date   October 2015
 * \brief  Main class for the NAT-aware multipath traceroute
 *
 * This module contains the implementation of the NAT-aware multipath
 * traceroute known as dublin-traceroute.
 *
 * \sa dublin_traceroute.cc
 */

#ifndef _Dublin_TRACEROUTE_H
#define _Dublin_TRACEROUTE_H

#include <iostream>
#include <arpa/inet.h>
#include <thread>
#include <mutex>

#include "common.h"
#include "exceptions.h"
#include "traceroute_results.h"

using namespace Tins;

// TODO add copyright and author

/* TODO put everything into a namespace, e.g.
 *      namespace DublinTraceroute {  // or DublinTraceroute
 *          class Traceroute {
 *              TracerouteResults run();
 *              ...
 *          }
 *      }
 */


class  DublinTraceroute {

private:
	const uint16_t		 srcport_,
				 dstport_;
	const std::string	 dst_;
  const bool ipv6_;
	IPv4Address		 target4_;
  IPv6Address target6_;
	const uint8_t		 npaths_,
				 min_ttl_,
				 max_ttl_;
	const uint16_t		 delay_;
	const bool		 broken_nat_;
	std::mutex		 mutex_tracerouting,
				 mutex_sniffed_packets;
	// IPv4Address		 my_address; // unused
	std::vector<std::shared_ptr<Packet>>	 sniffed_packets;
	const void		 validate_arguments();

public:
	static const uint16_t	 default_srcport = 12345;
	static const uint16_t	 default_dstport = 33434;
	static const uint8_t	 default_npaths = 20;
	static const uint8_t	 default_min_ttl = 1;
	static const uint8_t	 default_max_ttl = 30;
	static const uint16_t	 default_delay = 10;
	static const bool	 default_broken_nat = false;
  static const bool default_ipv6 = false;

	DublinTraceroute(
			const std::string &dst,
      const bool is_ipv6 = default_ipv6,
			const uint16_t srcport = default_srcport,
			const uint16_t dstport = default_dstport,
			const uint8_t npaths = default_npaths,
			const uint8_t min_ttl = default_min_ttl,
			const uint8_t max_ttl = default_max_ttl,
			const uint16_t delay = default_delay,
			const bool broken_nat = default_broken_nat
			):
				srcport_(srcport),
				dstport_(dstport),
				dst_(dst),
				npaths_(npaths),
				min_ttl_(min_ttl),
				max_ttl_(max_ttl),
				delay_(delay),
				broken_nat_(broken_nat),
        ipv6_(is_ipv6)
	{ validate_arguments(); }

	DublinTraceroute(
			const char *dst,
      const bool is_ipv6 = default_ipv6,
			const uint16_t srcport = default_srcport,
			const uint16_t dstport = default_dstport,
			const uint8_t npaths = default_npaths,
			const uint8_t min_ttl = default_min_ttl,
			const uint8_t max_ttl = default_max_ttl,
			const uint16_t delay = default_delay,
			const bool broken_nat = default_broken_nat
		       ):
				srcport_(srcport),
				dstport_(dstport),
				dst_(std::string(dst)),
				npaths_(npaths),
				min_ttl_(min_ttl),
				max_ttl_(max_ttl),
				delay_(delay),
				broken_nat_(broken_nat),
        ipv6_(is_ipv6)
	{ validate_arguments(); }

	~DublinTraceroute() { std::lock_guard<std::mutex> lock(mutex_tracerouting); };

	DublinTraceroute(const DublinTraceroute& source):
		srcport_(source.srcport_),
		dstport_(source.dstport_),
		dst_(source.dst_),
		npaths_(source.npaths_),
		min_ttl_(source.min_ttl_),
		max_ttl_(source.max_ttl_),
		delay_(source.delay_),
		broken_nat_(source.broken_nat_),
    ipv6_(source.ipv6_)
	{ validate_arguments(); }

	inline const uint16_t srcport() const { return srcport_; }
	inline const uint16_t dstport() const { return dstport_; }
	inline const uint8_t npaths() const { return npaths_; }
	inline const uint8_t min_ttl() const { return min_ttl_; }
	inline const uint8_t max_ttl() const { return max_ttl_; }
	inline const uint16_t delay() const { return delay_; }
	inline const bool broken_nat() const { return broken_nat_; }
  inline const bool is_ipv6() const { return ipv6_; }
	inline const std::string &dst() const { return dst_; }

	inline const IPv4Address &target4() const { return target4_; }
	void target4(const IPv4Address &addr) { target4_ = addr; }

  inline const IPv6Address &target6() const { return target6_; }
	void target6(const IPv6Address &addr) { target6_ = addr; }

	std::shared_ptr<TracerouteResults> traceroute();

private:
	bool sniffer_callback(Packet& packet);
	void match_sniffed_packets(TracerouteResults &results);
	void match_hostnames(TracerouteResults &results, std::shared_ptr<flow_map_t> flows);
};

#endif /* _Dublin_TRACEROUTE_H */
