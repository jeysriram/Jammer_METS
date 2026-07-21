#ifndef DP_EMITTERRELPARAM_H
#define DP_EMITTERRELPARAM_H

#include <stdio.h>
#include <string.h>
#include "dp_aspj_comm.h"


S32BIT ReadWayPointLatLong(PS8BIT in_ps8FileName, S_DP_ASPJ_WAYPOINTS_LAT_LON *out_pSdp_LatLong);

S32BIT ReadEmitterLatLong(PS8BIT in_ps8FileName, S_DP_ASPJ_EMITTERS_LAT_LON *out_pSdp_EmitLatLong);

S32BIT GetEmitterRelativeParameters(S_DP_ASPJ_WAYPOINTS_LAT_LON *in_pSdp_LatLong, S_DP_ASPJ_EMITTERS_LAT_LON *in_pSdp_EmitLatLong, \
                                    S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitDetRange);

S32BIT WriteWayPointEmitterLatLong(U32BIT in_u32NoOfSamples, U32BIT in_u32EmitterSamples, S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *in_pSWaypntEmtLoc);

S32BIT Calculate_TotalDistance_And_RelParam(S_DP_ASPJ_WAYPOINTS_LAT_LON *in_pSdp_LatLong,S_DP_ASPJ_EMITTERS_LAT_LON *in_pSdp_EmitLatLong,\
                                            FDOUBLE in_pfdTimeTaken, S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitDetRange);

S32BIT Calculate_WayPoints_TotalDistance(const S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *in_pSWayPntlatLong, S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter);

S32BIT Calculate_WayPoints_HeadingAngle(const S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *in_pSWayPntlatLong, S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter);

S32BIT Calculate_Velocity_for_Each_WayPoint(FDOUBLE in_pfdTimeTaken_ms,const S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *in_pSWayPntlatLong,S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter);

S32BIT Calculate_AOA_for_EachEmitter(const S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *in_pSWayPntlatLong, S_DP_ASPJ_WAYPOINT_EMITTER_LOCATIONS *out_pSWayPointEmitter);



#endif // DP_EMITTERRELPARAM_H
