include $(top_srcdir)/test/Makefile.inc
TESTS_ENVIRONMENT += TEST_DIR=${top_builddir}/test/validation

ODP_MODULES = pktio

if test_vald
LOG_COMPILER = $(top_srcdir)/platform/linux-dpdk/test/wrapper-script.sh
TESTS = pktio/pktio_run \
	${top_builddir}/test/validation/buffer/buffer_main$(EXEEXT) \
##	${top_builddir}/test/validation/classification/classification_main$(EXEEXT) \
	${top_builddir}/test/validation/cpumask/cpumask_main$(EXEEXT) \
	${top_builddir}/test/validation/crypto/crypto_main$(EXEEXT) \
	${top_builddir}/test/validation/errno/errno_main$(EXEEXT) \
	${top_builddir}/test/validation/init/init_main_ok$(EXEEXT) \
	${top_builddir}/test/validation/init/init_main_abort$(EXEEXT) \
	${top_builddir}/test/validation/init/init_main_log$(EXEEXT) \
	${top_builddir}/test/validation/packet/packet_main$(EXEEXT) \
	${top_builddir}/test/validation/pool/pool_main$(EXEEXT) \
	${top_builddir}/test/validation/queue/queue_main$(EXEEXT) \
	${top_builddir}/test/validation/random/random_main$(EXEEXT) \
	${top_builddir}/test/validation/scheduler/scheduler_main$(EXEEXT) \
	${top_builddir}/test/validation/synchronizers/synchronizers_main$(EXEEXT) \
	${top_builddir}/test/validation/thread/thread_main$(EXEEXT) \
	${top_builddir}/test/validation/time/time_main$(EXEEXT) \
	${top_builddir}/test/validation/timer/timer_main$(EXEEXT) \
	${top_builddir}/test/validation/shmem/shmem_main$(EXEEXT) \
	${top_builddir}/test/validation/system/system_main$(EXEEXT)

SUBDIRS = $(ODP_MODULES)
endif

#performance tests refer to pktio_env
if test_perf
SUBDIRS = pktio
endif