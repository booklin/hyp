// © 2021 Qualcomm Innovation Center, Inc. All rights reserved.
//
// SPDX-License-Identifier: BSD-3-Clause

#include <hyptypes.h>

#include <events/smccc.h>

#include "event_handlers.h"

bool
smccc_version(uint32_t *ret0)
{
	*ret0 = SMCCC_VERSION;
	return true;
}

bool
smccc_arch_features(uint32_t arg1, uint32_t *ret0)
{
	smccc_function_id_t fn_id    = smccc_function_id_cast(arg1);
	bool		    is_smc64 = smccc_function_id_get_is_smc64(&fn_id);
	smccc_function_t    fn	     = smccc_function_id_get_function(&fn_id);
	uint32_t	    ret;

	if ((smccc_function_id_get_interface_id(&fn_id) ==
	     SMCCC_INTERFACE_ID_ARCH) &&
	    smccc_function_id_get_is_fast(&fn_id) &&
	    (smccc_function_id_get_res0(&fn_id) == 0U)) {
		if (is_smc64) {
			ret = trigger_smccc_arch_features_fast64_event(fn);
		} else {
			ret = trigger_smccc_arch_features_fast32_event(fn);
		}
	} else if ((smccc_function_id_get_interface_id(&fn_id) ==
		    SMCCC_INTERFACE_ID_STANDARD_HYP) &&
		   smccc_function_id_get_is_fast(&fn_id) &&
		   (smccc_function_id_get_res0(&fn_id) == 0U)) {
		if (is_smc64) {
			ret = trigger_smccc_standard_hyp_features_fast64_event(
				fn);
		} else {
			ret = trigger_smccc_standard_hyp_features_fast32_event(
				fn);
		}
	} else {
		ret = SMCCC_UNKNOWN_FUNCTION32;
	}

	*ret0 = ret;
	return true;
}

bool
smccc_std_hyp_call_uid(uint32_t *ret0, uint32_t *ret1, uint32_t *ret2,
		       uint32_t *ret3)
{
	// uuidgen -s -n @url -N "qualcomm.com/gunyah/smccc/standard_hyp"
	// UUID: c1d58fcd-a453-5fdb-9265-ce36673d5f14
	*ret0 = 0x673d5f14U;
	*ret1 = 0x9265ce36U;
	*ret2 = 0xa4535fdbU;
	*ret3 = 0xc1d58fcdU;

	return true;
}

bool
smccc_std_hyp_revision(uint32_t *ret0, uint32_t *ret1)
{
	// From: ARM DEN 0028E
	// Incompatible argument changes cannot be made to an
	// existing SMC or HVC call. A new call is required.
	//
	// Major revision numbers must be incremented when:
	// - Any SMC or HVC call is removed.
	// Minor revision numbers must be incremented when:
	// - Any SMC or HVC call is added.
	// - Backwards compatible changes are made to existing
	//   function arguments.
	*ret0 = 1U; // Major Revision
	*ret1 = 0U; // Minor Revision

	return true;
}
