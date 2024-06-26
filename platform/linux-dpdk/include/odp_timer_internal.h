/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2014-2018 Linaro Limited
 * Copyright (c) 2021-2023 Nokia
 */

/**
 * @file
 *
 * ODP timeout descriptor - implementation internal
 */

#ifndef ODP_TIMER_INTERNAL_H_
#define ODP_TIMER_INTERNAL_H_

#include <odp/api/align.h>
#include <odp/api/debug.h>
#include <odp/api/timer.h>

#include <odp_event_internal.h>
#include <odp_global_data.h>
#include <odp_pool_internal.h>

#include <rte_config.h>

#include <stdint.h>

/*
 * Use as the argument to timer_run() to force a scan and to ignore rate
 * limit.
 */
#define TIMER_SCAN_FORCE INT32_MAX

/**
 * Internal Timeout header
 */
typedef struct ODP_ALIGNED_CACHE odp_timeout_hdr_t {
	/* Underlying DPDK rte_mbuf */
	struct rte_mbuf mb;

	/* Common internal header */
	_odp_event_hdr_int_t event_hdr;

	/* Requested expiration time */
	uint64_t expiration;

	/* User ptr inherited from parent timer */
	const void *user_ptr;

	/* User area pointer */
	void *uarea_addr;

	/* Parent timer */
	odp_timer_t timer;

} odp_timeout_hdr_t;

ODP_STATIC_ASSERT(sizeof(odp_timeout_hdr_t) <= 3 * RTE_CACHE_LINE_SIZE,
		  "Additional cache line required for odp_timeout_hdr_t");

/* A larger decrement value should be used after receiving events compared to
 * an 'empty' call. */
void _odp_timer_run_inline(int dec);

/* Static inline wrapper to minimize modification of schedulers. */
static inline uint64_t timer_run(int dec)
{
	if (odp_global_rw->inline_timers)
		_odp_timer_run_inline(dec);

	/* Time to the next timeout not available with DPDK timers */
	return UINT64_MAX;
}

#endif
